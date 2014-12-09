//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.461 Principles of Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    GetStarted.cpp
//	Purpose: OpenGL shader program to draw a set of "wide lines" and interactively
//          draw points based on mouse clicks.
//
//============================================================================

#include <iostream>
#include <vector>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include "geometry/geometry.h"
#include "ShaderSupport/GLSLShader.h"
#include "Scene/Scene.h"

#include "PointShaderNode.h"
#include "PointNode.h"
#include "LineShaderNode.h"
#include "LineNode.h"

// Root of the scene graph
SceneNode* SceneRoot;

// Scene state
SceneState MySceneState;

// PointNode - global so we can add points dynamically
PointNode* MyPoints;

// PointShaderNode - global so we can get the point location
PointShaderNode* PointShader;

// LineNode - global so we can change width
LineNode* OffsetLines;

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("GetStarted.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

/**
 * Reshape callback. Load a 2-D orthographic projection matrix using the 
 * window width and height so we can directly take window coordinates and 
 * send to OpenGL. Note that this callback will be called when the window
 * is opened.
 * @param  width  Window width
 * @param  height Window height
 */
void reshape(int width, int height)
{
   // Set a simple 4x4 matrix (use an array until we develop the Matrix4x4 class)
   MySceneState.m_ortho[0]  = 2.0f / (float)width;
   MySceneState.m_ortho[1]  = 0.0f;
   MySceneState.m_ortho[2]  = 0.0f;
   MySceneState.m_ortho[3]  = 0.0f;
   MySceneState.m_ortho[4]  = 0.0f;
   MySceneState.m_ortho[5]  = -2.0f / (float)height;
   MySceneState.m_ortho[6]  = 0.0f;
   MySceneState.m_ortho[7]  = 0.0f;
   MySceneState.m_ortho[8]  = 0.0f;
   MySceneState.m_ortho[9]  = 0.0f;
   MySceneState.m_ortho[10] = 1.0f;
   MySceneState.m_ortho[11] = 0.0f;
   MySceneState.m_ortho[12] = -1.0f;
   MySceneState.m_ortho[13] = 1.0f;
   MySceneState.m_ortho[14] = 0.0f;
   MySceneState.m_ortho[15] = 1.0f;

   // Update the viewport
   glViewport(0, 0, width, height);
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer
   glClear(GL_COLOR_BUFFER_BIT);
   
   SceneRoot->Draw(MySceneState);
   checkError("After Draw");  

   // Swap buffers
   glutSwapBuffers();
}

/**
 * Keyboard callback.
 */
void keyboard(unsigned char key, int x, int y)
{
   // Force a display update with glutPostRedisplay on a valid key input
   switch (key) 
   {
    // Escape key
    case 27:            
      exit(0);
	   break;

    // Multisample anti-aliasing - not part of HW - just a test
    case 'm':
       glDisable(GL_MULTISAMPLE);
       glutPostRedisplay();
       break;
    case 'M':
       glEnable(GL_MULTISAMPLE);
       glutPostRedisplay();
       break;

    // Alter point size and line width if a numeric key is pressed
    case '1':
       OffsetLines->SetWidth(1.0f);
       glPointSize(1.0f);
       break;
    case '2':
       OffsetLines->SetWidth(2.0f);
       glPointSize(2.0f);
       glutPostRedisplay();
       break;
    case '3':
       OffsetLines->SetWidth(3.0f);
       glPointSize(3.0f);
       glutPostRedisplay();
       break;
    case '4':
       OffsetLines->SetWidth(4.0f);
       glPointSize(4.0f);
       glutPostRedisplay();
       break;
    case '5':
       OffsetLines->SetWidth(5.0f);
       glPointSize(5.0f);
       glutPostRedisplay();
       break;
    case '6':
       OffsetLines->SetWidth(6.0f);
       glPointSize(6.0f);
       glutPostRedisplay();
       break;
    case '7':
       OffsetLines->SetWidth(7.0f);
       glPointSize(7.0f);
       glutPostRedisplay();
       break;
    case '8':
       OffsetLines->SetWidth(8.0f);
       glPointSize(8.0f);
       glutPostRedisplay();
       break;
    case '9':
       OffsetLines->SetWidth(9.0f);
       glPointSize(9.0f);
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
   // On a button down event add a point as the start of a line
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      // Add a point to display
      MyPoints->Add((float)x, (float)y, PointShader->GetPositionLoc());

	   // Force a redisplay
      glutPostRedisplay();
   }
}

/**
 * Create the scene.
 */
void createScene()
{
   SceneRoot = new SceneNode;

   LineShaderNode* offsetLineShader = new LineShaderNode();
   if (!offsetLineShader->Create("lines.vert", "simple.frag") ||
       !offsetLineShader->GetLocations())
      exit(-1);

   std::vector<Point2> pts;
   pts.push_back(Point2(20.0f, 50.0f));
   pts.push_back(Point2(100.0f, 120.0f));
   pts.push_back(Point2(170.0f, 160.0f));
   pts.push_back(Point2(220.0f, 180.0f));
   pts.push_back(Point2(380.0f, 250.0f));
   pts.push_back(Point2(400.0f, 350.0f));
   pts.push_back(Point2(320.0f, 450.0f));
   pts.push_back(Point2(120.0f, 400.0f));
   OffsetLines = new LineNode(pts, Color4(0.1f, 0.7f, 0.1f, 1.0f), offsetLineShader->GetPositionLoc(),
                     offsetLineShader->GetOffsetLoc());

   // Create the point shader node
   PointShader = new PointShaderNode();
   if (!PointShader->Create("points.vert", "simple.frag") ||
       !PointShader->GetLocations())
      exit(-1);

   // Create the node that manages the points
   Color4 pointColor(0.7f, 0.2f, 0.2f, 1.0f);
   MyPoints = new PointNode(pointColor);

   // Create scene graph
   SceneRoot->AddChild(offsetLineShader);
   offsetLineShader->AddChild(OffsetLines);
   SceneRoot->AddChild(PointShader);
   PointShader->AddChild(MyPoints);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   std::cout << "Keyboard Controls:" << std::endl;
   std::cout << "1-9 : Alter line width and point size" << std::endl;
   std::cout << "ESC - Exit program" << std::endl;

   // Double buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowPosition(250, 100);
   glutInitWindowSize(500, 500);
   glutInit(&argc, argv);

   glutCreateWindow("GetStarted by David Nesbitt");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
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

   // Set the clear color to white
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

   // Set a default 
   glPointSize(2.0f);

   // Create the scene
   createScene();

   checkError("CreateScene");

   // Main GLUT loop
   glutMainLoop();

   return 0;
}