//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Animation3D.cpp
//	Purpose: OpenGL and GLUT program to draw a simple 3D animation with 
//          collision detection. The scene contains moving spheres inside 
//          a fixed, cube enclosure. A simple scene graph is used to define the 
//          scene. Transform nodes, geometry nodes, and presentation nodes
//          are used.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include "geometry/geometry.h"
#include "ShaderSupport/GLSLShader.h"   // Need to include before scene.h
#include "Scene/scene.h"

#include "ColorNode.h"
#include "UnitSquareSurface.h"
#include "UnitSphere.h"
#include "BallTransform.h"
#include "LightingShaderNode.h"

const unsigned int NUMBER_OF_BALLS       = 9;
const unsigned int NUMBER_OF_RED_BALLS   = 3;
const unsigned int NUMBER_OF_GREEN_BALLS = 3;
const unsigned int NUMBER_OF_BLUE_BALLS  = 3;

const float FRAMES_PER_SEC = 72.0f;

// Root of the scene graph
SceneNode* SceneRoot;

SceneState MySceneState;

// Bounding planes of the enclosure. Used for intersection testing.
std::vector<Plane> BoundingPlanes;

// Ball transforms. We have this global so we can more easily do intersection testing
BallTransform* BallTransforms[NUMBER_OF_BALLS];

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("AnimatedBalls.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Use a timer method to try to do a consistent update rate.
 * Without using a timer, the speed of movement will depend on how fast
 * the program runs (fast movement on a fast PC and slow movement on a
 * slower PC)
 */
void timerFunction(int value)
{
   // Initialize all balls to have no intersection
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i]->SetIntersectTime(0.0f);

   // Go through all balls and test for intersection with subsequent ball
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
   {
      // If intersection with a prior ball is not found, test for intersection with successive balls
      if (BallTransforms[i]->GetIntersectTime() == 0.0f)
      {
         for (unsigned int j = i+1; j < NUMBER_OF_BALLS; j++)
         {
            // If an intersection occurs, break out of loop. We will only worry about a ball intersecting
            // one other in a single frame and won't care much if it is the closest
            if (BallTransforms[i]->IntersectBall(BallTransforms[j]))
               break;
         }
      }
   }

   // Go through all ball and test for plane intersection on those that do not intersect with another ball
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
   {
      // Check for collision with any planes
      if (BallTransforms[i]->GetIntersectTime() == 0.0f)
      {
         float t = 0.0f;
         float smallestT = 1.0f;
         std::vector<Plane>::iterator plane = BoundingPlanes.begin();
         Plane intersectPlane;
         for ( ; plane != BoundingPlanes.end(); plane++)
         {
            t = BallTransforms[i]->IntersectWithPlane(*plane);
            if (t < smallestT)
            {
               // Copy the nearest intersection and the plane of intersection
               smallestT = t;
               intersectPlane = *plane;
            }
         }
         if (smallestT != 1.0f)
         {
            BallTransforms[i]->SetIntersectTime(smallestT);
            BallTransforms[i]->SetIntersectPlane(intersectPlane);
         }
      }
   }

   // Update the scene graph
   SceneRoot->Update(MySceneState);

   // Set update to specified frames per second
   glutTimerFunc((int)(1000.0f / FRAMES_PER_SEC), timerFunction, 0);

   glutPostRedisplay();
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Init scene state and draw the scene graph
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
      exit(0);
	   break;

    default:
       break;
   }
}

/**
 * Reshape callback. Alter the viewport to maintain a fixed, square viewport
 * in the middle of the screen window.
 */
void reshape(int width, int height)
{
   // Center the world window in the viewport so it does not
   // stretch world window
   float worldAspect = 1.0f; 
   float windowAspect = (float)width / (float)height;
   if (windowAspect > worldAspect)
   {
      // Window is wider than world
      int w = (int)(height * worldAspect);
      int startX = (int)((width - w) * 0.5f);
      glViewport(startX, 0, w, height);
   }
   else
   {
      // Window is taller than world
      int h = (int)(width / worldAspect);
      int startY = (int)((height - h) * 0.5f);
      glViewport(0, startY, width, h);
   }
}

/**
 * Construct the scene
 */
void ConstructScene()
{
   // Shader node
   LightingShaderNode* shader = new LightingShaderNode;
   if (!shader->Create("SimpleLight.vert", "SimpleLight.frag") ||
       !shader->GetLocations())
      exit(-1);

   // Form bounding planes (for intersection testing)
   Plane p0(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
   BoundingPlanes.push_back(p0);
   Plane p1(Point3(0.0f, 0.0f, 100.0f), Vector3(0.0f, 0.0f, -1.0f));
   BoundingPlanes.push_back(p1);
   Plane p2(Point3(-50.0f, 0.0f, 50.0f), Vector3(1.0f, 0.0f, 0.0f));
   BoundingPlanes.push_back(p2);
   Plane p3(Point3(50.0f, 0.0f, 50.0f), Vector3(-1.0f, 0.0f, 0.0f));
   BoundingPlanes.push_back(p3);
   Plane p4(Point3(0.0f, 50.0f, 50.0f), Vector3(0.0f, -1.0f, 0.0f));
   BoundingPlanes.push_back(p4);
   Plane p5(Point3(0.0f, -50.0f, 50.0f), Vector3(0.0f, 1.0f, 0.0f));
   BoundingPlanes.push_back(p5);

   // Construct the geometry objects
   UnitSquareSurface1* unitSquare = new UnitSquareSurface1(shader->GetPositionLoc(), shader->GetNormalLoc());
   UnitSphere* unitSphere = new UnitSphere(18, 36, shader->GetPositionLoc(), shader->GetNormalLoc());

   // Contruct transform nodes for the walls. Perform rotations so the 
   // walls face inwards
   TransformNode* floorTransform = new TransformNode;
   floorTransform->Scale(100.0f, 100.0f, 1.0f);

   // Back wall is rotated +90 degrees about x: (y -> z)
   TransformNode* backWallTransform = new TransformNode;
   backWallTransform->Translate(0.0f, 50.0f, 50.0f);
   backWallTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);   
   backWallTransform->Scale(100.0f, 100.0f, 1.0f);

   // Left wall is rotated 90 degrees about y: (z -> x)
   TransformNode* leftWallTransform = new TransformNode;
   leftWallTransform->Translate(-50.0f, 0.0f, 50.0f);
   leftWallTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);
   leftWallTransform->Scale(100.0f, 100.0f, 1.0f);

   // Right wall is rotated -90 about y: (z -> -x)
   TransformNode* rightWallTransform = new TransformNode;
   rightWallTransform->Translate(50.0f, 0.0f, 50.0f);
   rightWallTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);
   rightWallTransform->Scale(100.0f, 100.0f, 1.0f);

   TransformNode* ceilingTransform = new TransformNode;
   ceilingTransform->Translate(0.0f, 0.0f, 100.0f);
   ceilingTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);
   ceilingTransform->Scale(100.0f, 100.0f, 1.0f); 

   // Construct transform nodes for the balls. Each one has a 
   // random initial position with x,y values between -40 and 40
   // and z values between 25 and 75. A random radius between 3 and 
   // 7 units, a random initial direction vectors (unit length) and 
   // random velocity between 5 and 15 units per second is used.
   for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i] = new BallTransform(FRAMES_PER_SEC);

   // Construct color nodes for the walls, floor, and ceiling
   ColorNode* floorColor    = new ColorNode(0.6f, 0.5f, 0.2f);
   ColorNode* backWallColor = new ColorNode(0.9f, 0.7f, 0.5f);
   ColorNode* wallColor     = new ColorNode(1.0f, 1.0f, 1.0f);
   ColorNode* ceilingColor  = new ColorNode(0.1f, 0.4f, 1.0f);

   // Construct color nodes for the balls
   ColorNode* redBall   = new ColorNode(1.0f, 0.0f, 0.0f);
   ColorNode* greenBall = new ColorNode(0.0f, 1.0f, 0.0f);
   ColorNode* blueBall  = new ColorNode(0.0f, 0.0f, 1.0f);

   // Construct the scene layout
   SceneRoot = new SceneNode;

   SceneRoot->AddChild(shader);

   // Walls
   shader->AddChild(backWallColor);
   backWallColor->AddChild(backWallTransform);
   backWallTransform->AddChild(unitSquare);
   shader->AddChild(wallColor);
   wallColor->AddChild(leftWallTransform);
   leftWallTransform->AddChild(unitSquare);
   wallColor->AddChild(rightWallTransform);
   rightWallTransform->AddChild(unitSquare);
   shader->AddChild(floorColor);
   floorColor->AddChild(floorTransform);
   floorTransform->AddChild(unitSquare);
   shader->AddChild(ceilingColor);
   ceilingColor->AddChild(ceilingTransform);
   ceilingTransform->AddChild(unitSquare);

   // Add a unit sphere child node to each ball transform node 
  for (unsigned int i = 0; i < NUMBER_OF_BALLS; i++)
      BallTransforms[i]->AddChild(unitSphere);

   // Add ball transforms under each ball presentation node
   unsigned int j = 0;
   shader->AddChild(redBall);
   for (unsigned int i = 0; i < NUMBER_OF_RED_BALLS; i++)
      redBall->AddChild(BallTransforms[j++]);
   shader->AddChild(greenBall);
   for (unsigned int i = 0; i < NUMBER_OF_GREEN_BALLS; i++)
      greenBall->AddChild(BallTransforms[j++]);
   shader->AddChild(blueBall);
   for (unsigned int i = 0; i < NUMBER_OF_BLUE_BALLS; i++)
      blueBall->AddChild(BallTransforms[j++]);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   printf("HERE\n");

   // Initialize free GLUT
   glutInit(&argc, argv);
   glutInitContextVersion(3, 2);
   //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);  // Using this causes LineWidth to error
   glutInitContextProfile(GLUT_CORE_PROFILE);

   // Set up depth buffer and double buffering
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 640);

   // Set callback methods
   glutCreateWindow("Animated Balls");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
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

   // Set the clear color to black. Any part of the window outside the
   // viewport should appear black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Construct scene. Seed the random number generator
   srand(929233);
   ConstructScene();

   // Enable depth testing
   glEnable(GL_DEPTH_TEST);

   // Enable back face polygon removal
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   // Set a fixed perspective projection. fov = 70, aspect = 1.0, near = 1.0 far = 200.
   // We are hard-coding viewing and projection matrices
   Matrix4x4 projection;
   projection.m00() = 1.428f;
   projection.m11() = 1.428f;
   projection.m22() = -1.010f;
   projection.m23() = -2.010f;
   projection.m32() = -1.0f;
   projection.m33() =  0.0f;

   // Set a fixed camera outside the center of the front wall (imagine
   // it being a window) looking parallel to the floor
   Matrix4x4 view;
   view.m00() = 1.0f;
   view.m11() = 0.0f;
   view.m12() = 1.0f;
   view.m13() = -50.0f;
   view.m21() = -1.0f;
   view.m22() =  0.0f;
   view.m23() = -90.0f;
   view.m33() = 1.0f;
   MySceneState.m_view = view;

   // Set the composite projection and viewing matrix
   MySceneState.m_pvMatrix = projection * view;

   // Set update rate
   glutTimerFunc((int)(1000.0f / FRAMES_PER_SEC), timerFunction, 0);

   glutMainLoop();
   return 0;
}