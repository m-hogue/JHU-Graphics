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

#include <iostream>
#include <vector>

// Include OpenGL support
#include <GL/gl3w.h>
#include <GL/freeglut.h>

// Include local libraries (geometry first)
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
int   RenderWidth  = 640;
int   RenderHeight = 480;
const float FrameRate = 72.0f;

// Root of the scene graph
SceneNode* SceneRoot;

// Global scene state
SceneState MySceneState;

// Global camera node (so we can change view easily)
CameraNode* MyCamera;

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("LightingViewing.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Updates the view given the mouse position and whether to move 
 * forward or backward
 */
void UpdateView(const int x, const int y, bool forward)
{
   // Find relative dx and dy relative to center of the window
   float dx = 4.0f * ((x - ((float)RenderWidth * 0.5f))  / (float)RenderWidth);
   float dy = 4.0f * ((((float)RenderHeight * 0.5f) - y) / (float)RenderHeight);
   float dz = (forward) ? Velocity : -Velocity;
   MyCamera->MoveAndTurn(dx * Velocity, dy * Velocity, dz);
   glutPostRedisplay();
}

void ConstructLighting(LightingShaderNode* lighting)
{
   // Set the global light ambient
   Color4 globalAmbient(0.4f, 0.4f, 0.4f, 1.0f);
   lighting->SetGlobalAmbient(globalAmbient);
 
   // Light 0 - a point light source located at the back right corner. Note the w component is 1
   // This light is somewhat dim. No ambient - let the global ambient control the ambient lighting
   HPoint3 position0(90.0f, 90.0f, 30.0f, 1.0f);
   Color4 ambient0(0.0f, 0.0f, 0.0f, 1.0f);
   Color4 diffuse0(0.5f, 0.5f, 0.5f, 1.0f);
   Color4 specular0(0.5f, 0.5f, 0.5f, 1.0f);
   lighting->SetLight(0, position0, ambient0, diffuse0, specular0);

   // Light 1 - a directional light from above. Note the w component is 0
   // This light is somewhat bright. No ambient - let the global ambient control the ambient lighting
   HPoint3 position1(0.0f, 0.0f, 1.0f, 0.0f);
   Color4 ambient1(0.0f, 0.0f, 0.0f, 1.0f);
   Color4 diffuse1(0.7f, 0.7f, 0.7f, 1.0f);
   Color4 specular1(0.7f, 0.7f, 0.7f, 1.0f);
   lighting->SetLight(1, position1, ambient1, diffuse1, specular1);

   // Not supporting attenuation or spotlights yet...
}

/**
 * Convenience method to add a material, then a transform, then a geometry
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
   wallMaterial->SetMaterialSpecular(Color4(0.4f, 0.4f, 0.4f));
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
   // Construct the scene root node
   SceneRoot = new SceneNode;

   // Construct the lighting shader node
   LightingShaderNode* lightingShader = new LightingShaderNode();
   if (!lightingShader->Create("VertexLighting.vert", "VertexLighting.frag") ||
       !lightingShader->GetLocations())
      exit(-1);

   int positionLoc = lightingShader->GetPositionLoc();
   int normalLoc = lightingShader->GetNormalLoc();

   // Add the lighting node to the scene
   SceneRoot->AddChild(lightingShader);
   ConstructLighting(lightingShader);

   // Add the camera to the scene
   // Initialize the view and set a perspective projection
   MyCamera = new CameraNode();
   MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
   MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
   MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
   MyCamera->SetPerspective(50.0, 1.0, 1.0, 300.0);
   lightingShader->AddChild(MyCamera);

   // Construct the geometry nodes (use vertex buffer objects)
   UnitSquareSurface* unitSquare = new UnitSquareSurface(10, positionLoc, normalLoc);

   // Construct a unit box
   SceneNode* box = ConstructUnitBox(unitSquare);

   // Construct a unit cylinder surface
   ConicSurface* cylinder = new ConicSurface(1.0f, 1.0f, 18, 4, positionLoc, normalLoc);
  
   // Construct the room as a child of the root node
   ConstructRoom(MyCamera, unitSquare);

   // Construct the table
   SceneNode* table = ConstructTable(box, cylinder); 

   // Wood
   PresentationNode* wood = new PresentationNode;
   wood->SetMaterialAmbientAndDiffuse(Color4(0.55f, 0.45f, 0.15f));
   wood->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
   wood->SetMaterialShininess(64.0f);

   // Position the table in the room
   TransformNode* tableTransform = new TransformNode;
   tableTransform->Translate(-50.0f, 50.0f, 0.0f);
   tableTransform->Rotate(30.0f, 0.0f, 0.0f, 1.0f);

   MyCamera->AddChild(wood);
   wood->AddChild(tableTransform);
   tableTransform->AddChild(table);

   // Silver material
   PresentationNode* silver = new PresentationNode;
   silver->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
   silver->SetMaterialDiffuse(Color4(0.50754f, 0.50754f, 0.50754f));
   silver->SetMaterialSpecular(Color4(0.508273f, 0.508273f, 0.508273f));
   silver->SetMaterialShininess(51.2f);

   // Teapot transform. It is tough to place this - if you make too small then if you look
   // from above the table intersects the bottom, but if you move higher when you look from
   // outside it looks like the teapot is above the table. This is because we don't know the
   // exact dimensions of the teapot.
   TransformNode* teapotTransform = new TransformNode;
   teapotTransform->Translate(0.0f, 0.0f, 26.0f);
   teapotTransform->Scale(2.5f, 2.5f, 2.5f);

   // Teapot
   MeshTeapot* teapot = new MeshTeapot(4, positionLoc, normalLoc);

   tableTransform->AddChild(teapotTransform);
   teapotTransform->AddChild(silver);
   silver->AddChild(teapot);
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
      UpdateView(MouseX, MouseY, Forward);
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

   // Initialize the scene state
   MySceneState.Init();

   // Draw the scene graph
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
      exit(0);
	   break;

    // Reset the view
    case 'i':
      MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
      MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
      MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
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
      glutPostRedisplay();
	   break;

    // Change the pitch of the camera by 5 degrees (clockwise)
    case 'P':
      MyCamera->Pitch(-5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees
    case 'h':
      MyCamera->Heading(5);
      glutPostRedisplay();
	   break;

    // Change the heading of the camera by 5 degrees (clockwise)
    case 'H':
      MyCamera->Heading(-5);
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

    // Slide right
    case 'X':
      MyCamera->Slide(5.0f, 0.0f, 0.0f);
      glutPostRedisplay();
	   break;

    // Slide left
    case 'x':
      MyCamera->Slide(-5.0f, 0.0f, 0.0f);
      glutPostRedisplay();
	   break;

    // Slide up
    case 'Y':
      MyCamera->Slide(0.0f, 5.0f, 0.0f);
      glutPostRedisplay();
	   break;

    // Slide down
    case 'y':
      MyCamera->Slide(0.0f, -5.0f, 0.0f);
      glutPostRedisplay();
	   break;

    // Move forward
    case 'F':
      MyCamera->Slide(0.0f, 0.0f, -5.0f);
      glutPostRedisplay();
	   break;

    // Move backward
    case 'f':
      MyCamera->Slide(0.0f, 0.0f, 5.0f);
      glutPostRedisplay();
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
         UpdateView(x, y, true);
         
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
         UpdateView(x, y, true);

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
   UpdateView(x, y, true);
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

   // Reset the perspective projection to reflect the change of aspect ratio 
   MyCamera->ChangeAspectRatio((float)width / (float)height);
}

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
   glutCreateWindow("Lighting and Viewing");
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

   // Construct scene
   ConstructScene();

   glutMainLoop();
   return 0;
}