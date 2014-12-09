//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    LightingViewing.cpp
//	Purpose: OpenGL and GLUT program to draw a simple scene with
//          viewing controls and lighting and shading.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include <fcntl.h>
#include <io.h>

#include "geometry/geometry.h"
#include "ShaderSupport/GLSLShader.h"
#include "Scene/scene.h"

#include "LightingShaderNode.h"

// While mouse button is down, the view will be updated
bool  Animate = false;
bool  Forward = true;
float Velocity = 1.0f;
int   MouseX;
int   MouseY;

// Initial window dimensions
int RenderWidth  = 640;
int RenderHeight = 480;

const float FrameRate = 72.0f;

// Scene root
SceneNode* SceneRoot;

// Global camera node. Use this as the root of the scene graph.
CameraNode* MyCamera;

// Keep the spotlight global so we can update its poisition
LightNode* Spotlight;

// Global scene state
SceneState MySceneState;

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("PhongShading.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Convencience method to add a material, then a transform, then a geometry
 * node as a child to a specified node.
 */
void AddSubTree(SceneNode* parent, SceneNode* material, 
                SceneNode* transform, SceneNode* geometry)
{
   parent->AddChild(material);
   material->AddChild(transform);
   transform->AddChild(geometry);
}

/**
 * Construct a unit box.
 * @param  unitSquare  Geometry node to use
 */
SceneNode* ConstructUnitBox(UnitSquareSurface* unitSquare)
{
   // Contruct transform nodes for the sides of the box.
   // Perform rotations so the sides face outwards

   // Bottom is rotated 180 degrees so it faces outwards
   TransformNode* bottomTransform = new TransformNode;
   bottomTransform->Translate(0.0f, 0.0f, -0.5f);
   bottomTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);   

   // Back is rotated -90 degrees about x: (z -> y)
   TransformNode* backTransform = new TransformNode;
   backTransform->Translate(0.0f, 0.5f, 0.0f);
   backTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);   

   // Front wall is rotated 90 degrees about x: (y -> z)
   TransformNode* frontTransform = new TransformNode;
   frontTransform->Translate(0.0f, -0.5f, 0.0f);
   frontTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   

   // Left wall is rotated -90 about y: (z -> -x)
   TransformNode* leftTransform = new TransformNode;
   leftTransform->Translate(-0.5f, 0.0f, 00.0f);
   leftTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);

   // Right wall is rotated 90 degrees about y: (z -> x)
   TransformNode* rightTransform = new TransformNode;
   rightTransform->Translate(0.5f, 0.0f, 0.0f);
   rightTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);

   // Top 
   TransformNode* topTransform = new TransformNode;
   topTransform->Translate(0.0f, 0.0f, 0.50f);

   // Create a SceneNode and add the 6 sides of the box.
   SceneNode* box = new SceneNode;
   box->AddChild(backTransform);
   backTransform->AddChild(unitSquare);
   box->AddChild(leftTransform);
   leftTransform->AddChild(unitSquare);
   box->AddChild(rightTransform);
   rightTransform->AddChild(unitSquare);
   box->AddChild(frontTransform);
   frontTransform->AddChild(unitSquare);
   box->AddChild(bottomTransform);
   bottomTransform->AddChild(unitSquare);
   box->AddChild(topTransform);
   topTransform->AddChild(unitSquare);

   return box;
}

/**
 * Construct room as a child of the specified node
 * @param  parent      Parent node
 * @param  unitSquare  Geometry node to use
 */
void ConstructRoom(SceneNode* parent, UnitSquareSurface* unitSquare)
{
   // Contruct transform nodes for the walls. Perform rotations so the 
   // walls face inwards
   TransformNode* floorTransform = new TransformNode;
   floorTransform->Scale(200.0f, 200.0f, 1.0f);

   // Back wall is rotated +90 degrees about x: (y -> z)
   TransformNode* backWallTransform = new TransformNode;
   backWallTransform->Translate(0.0f, 100.0f, 40.0f);
   backWallTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   
   backWallTransform->Scale(200.0f, 80.0f, 1.0f);

   // Front wall is rotated -90 degrees about x: (z -> y)
   TransformNode* frontWallTransform = new TransformNode;
   frontWallTransform->Translate(0.0f, -100.0f, 40.0f);
   frontWallTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);   
   frontWallTransform->Scale(200.0f, 80.0f, 1.0f);

   // Left wall is rotated 90 degrees about y: (z -> x)
   TransformNode* leftWallTransform = new TransformNode;
   leftWallTransform->Translate(-100.0f, 0.0f, 40.0f);
   leftWallTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);
   leftWallTransform->Scale(80.0f, 200.0f, 1.0f);

   // Right wall is rotated -90 about y: (z -> -x)
   TransformNode* rightWallTransform = new TransformNode;
   rightWallTransform->Translate(100.0f, 0.0f, 40.0f);
   rightWallTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);
   rightWallTransform->Scale(80.0f, 200.0f, 1.0f);

   // Ceiling is rotated 180 about x so it faces inwards
   TransformNode* ceilingTransform = new TransformNode;
   ceilingTransform->Translate(0.0f, 0.0f, 80.0f);
   ceilingTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);
   ceilingTransform->Scale(200.0f, 200.0f, 1.0f); 

   // Floor should be tan, mostly dull
   PresentationNode* floorMaterial = new PresentationNode;
   floorMaterial->SetMaterialAmbientAndDiffuse(Color4(0.3f, 0.45f, 0.1f));
   floorMaterial->SetMaterialSpecular(Color4(0.1f, 0.1f, 0.1f));
   floorMaterial->SetMaterialShininess(2.0f);

   // Make the walls reddish, slightly shiny
   PresentationNode* wallMaterial = new PresentationNode;
   wallMaterial->SetMaterialAmbientAndDiffuse(Color4(0.7f, 0.55f, 0.55f));
   wallMaterial->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
   wallMaterial->SetMaterialShininess(16.0f);

   // Ceiling should be white, moderately shiny
   PresentationNode* ceilingMaterial = new PresentationNode;
   ceilingMaterial->SetMaterialAmbientAndDiffuse(Color4(1.0f, 1.0f, 1.0f));
   ceilingMaterial->SetMaterialSpecular(Color4(0.9f, 0.9f, 0.9f));
   ceilingMaterial->SetMaterialShininess(64.0f);

   // Add floor and ceiling to the parent. Use convenience method to add material,
   // then presentation, then geometry.
   AddSubTree(parent, floorMaterial,   floorTransform,   unitSquare);
   AddSubTree(parent, ceilingMaterial, ceilingTransform, unitSquare);

   // Walls. We can group these all under a single presentation node.
   parent->AddChild(wallMaterial);
   wallMaterial->AddChild(backWallTransform);
   backWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(leftWallTransform);
   leftWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(rightWallTransform);
   rightWallTransform->AddChild(unitSquare);
   wallMaterial->AddChild(frontWallTransform);
   frontWallTransform->AddChild(unitSquare);
}

/**
 * Construct table
 * @param  unitSquare  Geometry node to use for table top
 * @param  legs        Geometry node to use for legs
 * @return  Returns a scene node representing the table
 */
SceneNode* ConstructTable(SceneNode* box, ConicSurface* leg)
{
   // Table legs (relative to center of table)
   TransformNode* lfLegTransform = new TransformNode;
   lfLegTransform->Translate(-20.0f, -10.0f, 10.0f);
   lfLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* lrLegTransform = new TransformNode;
   lrLegTransform->Translate(-20.0f, 10.0f, 10.0f);
   lrLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* rfLegTransform = new TransformNode;
   rfLegTransform->Translate(20.0f, -10.0f, 10.0f);
   rfLegTransform->Scale(3.0f, 3.0f, 20.0f);
   TransformNode* rrLegTransform = new TransformNode;
   rrLegTransform->Translate(20.0f, 10.0f, 10.0f);
   rrLegTransform->Scale(3.0f, 3.0f, 20.0f);
   
   // Construct dimensions for the table top
   TransformNode* topTransform = new TransformNode;
   topTransform->Translate(0.0f, 0.0f, 23.0f);
   topTransform->Scale(60.0f, 30.0f, 6.0f);

   // Create the tree
   SceneNode* table = new SceneNode;
   table->AddChild(topTransform);
   topTransform->AddChild(box);
   table->AddChild(lfLegTransform);
   lfLegTransform->AddChild(leg);
   table->AddChild(rfLegTransform);
   rfLegTransform->AddChild(leg);
   table->AddChild(lrLegTransform);
   lrLegTransform->AddChild(leg);
   table->AddChild(rrLegTransform);
   rrLegTransform->AddChild(leg);

   return table;
}

/**
 * Construct the scene
 */
void ConstructScene()
{
   // Construct the lighting shader node
   LightingShaderNode* lightingShader = new LightingShaderNode();
   if (!lightingShader->Create("phong.vert", "phong.frag") ||
       !lightingShader->GetLocations())
      exit(-1);

   int positionLoc = lightingShader->GetPositionLoc();
   int normalLoc = lightingShader->GetNormalLoc();

   // -------------------- Geometry -------------------- //

   // Construct a unit square - use less subdivisions to see how
   // phong shading improves the lighting
   UnitSquareSurface* unitSquare = new UnitSquareSurface(2, positionLoc, normalLoc);

   // Construct a unit box
   SceneNode* box = ConstructUnitBox(unitSquare);

   // Construct a unit cylinder surface
   ConicSurface* cylinder = new ConicSurface(1.0f, 1.0f, 18, 4, positionLoc, normalLoc);

   // Construct a torus
   TorusSurface* torus = new TorusSurface(20.0f, 5.0f, 18, 18, positionLoc, normalLoc);

   // Teapot
   MeshTeapot* teapot = new MeshTeapot(3, positionLoc, normalLoc);

   // Sphere
   SphereSection* sphere = new SphereSection(-90.0f, 90.0f, 18, 
               -180.0f, 180.0f, 36, 1.0f, positionLoc, normalLoc);

   //-------------------- Materials ------------------------- //

   // Wood
   PresentationNode* wood = new PresentationNode;
   wood->SetMaterialAmbientAndDiffuse(Color4(0.55f, 0.45f, 0.15f));
   wood->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
   wood->SetMaterialShininess(64.0f);

   // Silver
   PresentationNode* silver = new PresentationNode;
   silver->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
   silver->SetMaterialDiffuse(Color4(0.50754f, 0.50754f, 0.50754f));
   silver->SetMaterialSpecular(Color4(0.508273f, 0.508273f, 0.508273f));
   silver->SetMaterialShininess(51.2f);

   // Black, shiny
   PresentationNode* shinyBlack = new PresentationNode;
   shinyBlack->SetMaterialAmbient(Color4(0.0f, 0.0f, 0.0f));
   shinyBlack->SetMaterialDiffuse(Color4(0.01f, 0.01f, 0.01f));
   shinyBlack->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
   shinyBlack->SetMaterialShininess(32.0f);

   // Shiny blue
   PresentationNode* shinyBlue = new PresentationNode;
   shinyBlue->SetMaterialAmbient(Color4(0.05f, 0.05f, 0.2f));
   shinyBlue->SetMaterialDiffuse(Color4(0.2f, 0.2f, 0.6f));
   shinyBlue->SetMaterialSpecular(Color4(0.75f, 0.75, 0.75f));
   shinyBlue->SetMaterialShininess(76.8f);

   // ------------------ Transformations ------------------- //

   // Position the table in the room
   TransformNode* tableTransform = new TransformNode;
   tableTransform->Translate(-50.0f, 50.0f, 0.0f);
   tableTransform->Rotate(30.0f, 0.0f, 0.0f, 1.0f);

   // Teapot transform
   TransformNode* teapotTransform = new TransformNode;
   teapotTransform->Translate(0.0f, 0.0f, 26.0f);
   teapotTransform->Scale(2.5f, 2.5f, 2.5f);

   // Torus
   TransformNode* torusTransform = new TransformNode;
   torusTransform->Translate(0.0f, 90.0f, 20.0f);
   torusTransform->Rotate(60.0f, 1.0f, 0.0f, 0.0f);

   // Sphere
   TransformNode* sphereTransform = new TransformNode;
   sphereTransform->Translate(80.0f, 20.0f, 10.0f);
   sphereTransform->Scale(10.0f, 10.0f, 10.0f);

   // --------------------------- Camera ----------------------- //
   MyCamera = new CameraNode;
   MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
   MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
   MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
   MyCamera->SetPerspective(50.0, 1.0, 1.0, 300.0);

   // -------------------- Lighting --------------------------/

   // Set the global light ambient
   Color4 globalAmbient(0.4f, 0.4f, 0.4f, 1.0f);
   lightingShader->SetGlobalAmbient(globalAmbient);

   // Light 0 - point light source in back right corner
	LightNode* light0 = new LightNode(0);
	light0->SetDiffuse(Color4(0.5f, 0.5f, 0.5f, 1.0f));
	light0->SetSpecular(Color4(0.5f, 0.5f, 0.5f, 1.0f));
   light0->SetPosition(HPoint3(90.0f, 90.0f, 30.f, 1.0f));	
	light0->Enable();

   // Light1 - directional light from the ceiling
   LightNode* light1 = new LightNode(1);
	light1->SetDiffuse(Color4(0.7f, 0.7f, 0.7f, 1.0f ));
	light1->SetSpecular(Color4(0.7f, 0.7f, 0.7f, 1.0f));
   light1->SetPosition(HPoint3(0.0f, 0.0f, 1.0f, 0.0f));	
	light1->Enable();

   // Spotlight - reddish spotlight - we will place at the camera location
   // shining along -VPN
   Spotlight = new LightNode(2);
	Spotlight->SetDiffuse(Color4(0.5f, 0.1f, 0.1f, 1.0f ));
	Spotlight->SetSpecular(Color4(0.5f, 0.1f, 0.1f, 1.0f));
   Point3 pos = MyCamera->GetPosition();
   Spotlight->SetPosition(HPoint3(pos.x, pos.y, pos.z, 1.0f));
   Vector3 dir = MyCamera->GetViewPlaneNormal() * -1.0f;
   Spotlight->SetSpotlight(dir, 32.0f, 30.0f);
	Spotlight->Enable();

   // --------------------- Scene construction ----------------- //

   // Construct the scene root node
   SceneRoot = new SceneNode;
   SceneRoot->AddChild(lightingShader);
   lightingShader->AddChild(MyCamera);

   // Add the lights as the children of the camera
	MyCamera->AddChild(light0);
   light0->AddChild(light1);
   light1->AddChild(Spotlight);

   // Create a scene node to hold all scene objects (other than camera
   // and lights)
   SceneNode* myScene = new SceneNode;
   
   // Add the scene under the last light
   Spotlight->AddChild(myScene);

   // Construct the room (walls, floor, ceiling)
   ConstructRoom(myScene, unitSquare);

   // Construct the table
   SceneNode* table = ConstructTable(box, cylinder); 
   myScene->AddChild(wood);
   wood->AddChild(tableTransform);
   tableTransform->AddChild(table);

   // Place a teapot on the table
   tableTransform->AddChild(teapotTransform);
   teapotTransform->AddChild(silver);
   silver->AddChild(teapot);

   // Place a torus
   myScene->AddChild(shinyBlack);
   shinyBlack->AddChild(torusTransform);
   torusTransform->AddChild(torus);

    // Place a sphere
   myScene->AddChild(shinyBlue);
   shinyBlue->AddChild(sphereTransform);
   sphereTransform->AddChild(sphere);
}

// Update the spotlight based on camera position change
void UpdateSpotlight()
{
   Point3 pos = MyCamera->GetPosition();
   Spotlight->SetPosition(HPoint3(pos.x, pos.y, pos.z, 1.0f));
   Vector3 dir = MyCamera->GetViewPlaneNormal() * -1.0f;
   Spotlight->SetSpotlightDirection(dir);
}

/**
 * Use a timer method to try to do a consistent update rate or 72Hz.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void timerFunction(int value)
{
   // If mouse button is down, generate another view
   if (Animate)
   {
      // Find relative dx and dy relative to center of the window
      float dx = 4.0f * ((MouseX - ((float)RenderWidth * 0.5f))  / (float)RenderWidth);
      float dy = 4.0f * ((((float)RenderHeight * 0.5f) - MouseY) / (float)RenderHeight);
      float dz = (Forward) ? Velocity : -Velocity;
      MyCamera->MoveAndTurn(dx * Velocity, dy * Velocity, dz);

      MyCamera->Update(MySceneState);
      UpdateSpotlight();

      glutPostRedisplay();

      glutTimerFunc((int)(1000.0f / FrameRate), timerFunction, 0);
   }
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Initialize the scene state and draw the scene graph
   MySceneState.Init();
   SceneRoot->Draw(MySceneState);
  
   // Swap buffers
   glutSwapBuffers();
}

/**
 * Keyboard callback.
 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
    // Escape key
    case 27:  
      delete MyCamera;
      exit(0);
	   break;

    // Reset the view
    case 'i':
      MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
      MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
      MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
      UpdateSpotlight();
      glutPostRedisplay();
      break;

    // Roll the camera by 5 degrees
    case 'r':
      MyCamera->Roll(5);
      glutPostRedisplay();
	   break;

    // Roll the camera by 5 degrees (clockwise)
    case 'R':
      MyCamera->Roll(-5);
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees
    case 'p':
      MyCamera->Pitch(5);
      UpdateSpotlight();
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees (clockwise)
    case 'P':
      MyCamera->Pitch(-5);
      UpdateSpotlight();
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees
    case 'h':
      MyCamera->Heading(5);
      UpdateSpotlight();
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees (clockwise)
    case 'H':
      MyCamera->Heading(-5);
      UpdateSpotlight();
      glutPostRedisplay();
	   break;

	   // slide camera right
	case 'X':
		MyCamera->moveHorizontal(5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

		// slide camera left
	case 'x':
		MyCamera->moveHorizontal(-5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

		// slide camera up
	case 'Y':
		MyCamera->moveVertical(5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

		// slide camera down
	case 'y':
		MyCamera->moveVertical(-5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

		// move camera forward
	case 'F':
		MyCamera->moveForwardOrBackward(-5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

		// move camera backward
	case 'f':
		MyCamera->moveForwardOrBackward(5);
		spotlight->setSpotlightDirection(Vector3(MyCamera->getPosition(), MyCamera->getLookAtPoint()));
		spotlight->setPosition(HPoint3(MyCamera->getPosition().x, MyCamera->getPosition().y, MyCamera->getPosition().z, 1.0f));
		glutPostRedisplay();
		break;

    // Go faster
    case 'V':
      Velocity += 0.2f;
      break;

    // Go slower
    case 'v':
      Velocity -= 0.2f;
      if (Velocity < 0.2f)
         Velocity = 0.1f;
      break;

    default:
       break;
   }
}

/**
 * Mouse callback (called when a mouse button state changes)
 */
void mouse(int button, int state, int x, int y)
{
   // On a left button up event MoveAndTurn the view with forward motion
   if (button == GLUT_LEFT_BUTTON)
   {
      if (state == GLUT_DOWN)
      {
         MouseX = x;
         MouseY = y;
         Forward = true;
         Animate = true;
         
         // Set update
         glutTimerFunc((int)(1000.0f / FrameRate), timerFunction, 0);
      }
      else
         Animate = false;  // Disable animation when the button is released
   }

   // On a right button up event MoveAndTurn the view with reverse motion
   if (button == GLUT_RIGHT_BUTTON)
   {
      if (state == GLUT_DOWN)
      { 
         MouseX = x;
         MouseY = y;
         Forward = false;
         Animate = true;

         // Set update
         glutTimerFunc((int)(1000.0f / FrameRate), timerFunction, 0);
      }
      else
         Animate = false;  // Disable animation when the button is released
   }
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y)
{
   // Update position used for changing the view and force a new view
   MouseX = x;
   MouseY = y;
}

/**
 * Reshape callback.  Reset the perpective projection so the field of
 * view matches the window's aspect ratio.
 */
void reshape(int width, int height)
{
   RenderWidth  = width;
   RenderHeight = height;

   // Reset the viewport
   glViewport(0, 0, width, height);

   // Reset the perspective projection to reflect the change of 
   // the aspect ratio 
   MyCamera->ChangeAspectRatio((float)width / (float)height);
}

/*


   // Populate matrix uniform locations in scene state
   MyShaderProgram.GetUniformLocation(MySceneState.m_pvmLoc, "pvm");
   MyShaderProgram.GetUniformLocation(MySceneState.m_modelMatrixLoc, "modelMatrix");
   MyShaderProgram.GetUniformLocation(MySceneState.m_normalMatrixLoc, "normalMatrix");
  
   // Populate material uniform locations in scene state 
   MyShaderProgram.GetUniformLocation(MySceneState.m_materialAmbientLoc, "materialAmbient");
   MyShaderProgram.GetUniformLocation(MySceneState.m_materialDiffuseLoc, "materialDiffuse");
   MyShaderProgram.GetUniformLocation(MySceneState.m_materialSpecularLoc, "materialSpecular");
   MyShaderProgram.GetUniformLocation(MySceneState.m_materialEmissionLoc, "materialEmission");
   MyShaderProgram.GetUniformLocation(MySceneState.m_materialShininessLoc, "materialShininess");

   // Populate camera position uniform location in scene state
   MyShaderProgram.GetUniformLocation(MySceneState.m_cameraPositionLoc, "cameraPosition");

   // Populte light uniforms
   char name[128];
   for (int i = 0; i < 3; i++)
   {
      sprintf(name, "lights[%d].enabled", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].enabled, name);
      sprintf(name, "lights[%d].spotlight", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].spotlight, name);
      sprintf(name, "lights[%d].position", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].position, name);
      sprintf(name, "lights[%d].ambient", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].ambient, name);
      sprintf(name, "lights[%d].diffuse", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].diffuse, name);
      sprintf(name, "lights[%d].specular", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].specular, name);
      sprintf(name, "lights[%d].constantAttenuation", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].constantAttenuation, name);
      sprintf(name, "lights[%d].linearAttenuation", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].linearAttenuation, name);
      sprintf(name, "lights[%d].quadraticAttenuation", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].quadraticAttenuation, name);
      sprintf(name, "lights[%d].spotCosCutoff", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].spotCosCutoff, name);
      sprintf(name, "lights[%d].spotExponent", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].spotExponent, name);
      sprintf(name, "lights[%d].spotDirection", i);
      MyShaderProgram.GetUniformLocation(MySceneState.lights[i].spotDirection, name);
   }

   // Get the vertex position attribute from the vertex shader
   MyShaderProgram.GetAttributeLocation(MySceneState.m_positionLoc, "vertexPosition");
   glEnableVertexAttribArray(MySceneState.m_positionLoc);

   // Get the vertex normal attribute from the shader
   MyShaderProgram.GetAttributeLocation(MySceneState.m_normalLoc, "vertexNormal");
   glEnableVertexAttribArray(MySceneState.m_normalLoc);

   // Set the number of lights used
   GLint nl = -1;
   MyShaderProgram.GetUniformLocation(nl, "numLights");
   glUniform1i(nl, 3);

   // Set the global ambient light
   Color4 globalAmbient(0.4f, 0.4f, 0.4f, 1.0f);
   GLint globalLightAmbientLoc = -1;
   MyShaderProgram.GetUniformLocation(globalLightAmbientLoc, "globalLightAmbient");
   glUniform4fv(globalLightAmbientLoc, 1, &globalAmbient.r);
}
*/

/**
 * Main 
 */
int main(int argc, char** argv)
{
   // Print the keyboard commands
   printf("i - Reset to initial view\n");
   printf("R - Roll    5 degrees clockwise   r - Counter-clockwise\n");
   printf("P - Pitch   5 degrees clockwise   p - Counter-clockwise\n");
   printf("H - Heading 5 degrees clockwise   h - Counter-clockwise\n");
   printf("X - Slide camera right            x - Slide camera left\n");
   printf("Y - Slide camera up               y - Slide camera down\n");
   printf("F - Move camera forward           f - Move camera backwards\n");
   printf("V - Faster mouse movement         v - Slower mouse movement\n");

   // Initialize free GLUT
   glutInit(&argc, argv);
   glutInitContextVersion(3, 2);
   //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);  // Using this causes LineWidth to error
   glutInitContextProfile(GLUT_CORE_PROFILE);

   // Set up depth buffer and double buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);

   // Set callback methods
   glutCreateWindow("Phong Shading");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboard);

  // Initialize Open 3.2 core profile
   if (gl3wInit()) {
      fprintf(stderr, "gl3wInit: failed to initialize OpenGL\n");
      return -1;
   }
   if (!gl3wIsSupported(3, 2)) {
      fprintf(stderr, "OpenGL 3.2 not supported\n");
      return -1;
   }
   printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

   // Set the clear color to black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Enable the depth buffer
   glEnable(GL_DEPTH_TEST);

   // Enable back face polygon removal
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   // Enable mutlisample anti-aliasing
   glEnable(GL_MULTISAMPLE);

   // Construct scene
   ConstructScene();

   glutMainLoop();
   return 0;
}
