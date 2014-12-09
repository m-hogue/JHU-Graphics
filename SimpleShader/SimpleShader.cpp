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

#include "LineShaderNode.h"
#include "LineNode.h"

// Root of the scene graph
SceneNode* SceneRoot;

// Scene state
SceneState MySceneState;

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("SimpleShader.log", "w");

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
   switch (key) 
   {
    // Escape key
    case 27:            
      exit(0);
	   break;

    case '1':
       glLineWidth(1.0f);
       break;
    case '2':
       glLineWidth(2.0f);
       break;
    case '3':
       glLineWidth(3.0f);
       break;
    case '4':
       glLineWidth(4.0f);
       break;
    case '5':
       glLineWidth(5.0f);
       break;
    case '6':
       glLineWidth(6.0f);
       break;
    case '7':
       glLineWidth(7.0f);
       break;
    case '8':
       glLineWidth(8.0f);
       break;
    case '9':
       glLineWidth(9.0f);
       break;
    default:
       break;
   }

   // Force display update
   glutPostRedisplay();
}

/**
 * Create the scene.
 */
void createScene()
{
   SceneRoot = new SceneNode;

   // Create shaders
   LineShaderNode* lineShader = new LineShaderNode();
   if (!lineShader->Create("lines.vert", "lines.frag") || !lineShader->GetLocations())
      exit(-1);

   // Create a line node using the following set of vertices
   std::vector<Point2> pts;
   pts.push_back(Point2(20.0f, 50.0f));
   pts.push_back(Point2(100.0f, 120.0f));
   pts.push_back(Point2(170.0f, 160.0f));
   pts.push_back(Point2(220.0f, 180.0f));
   pts.push_back(Point2(380.0f, 250.0f));
   pts.push_back(Point2(400.0f, 350.0f));
   pts.push_back(Point2(320.0f, 450.0f));
   pts.push_back(Point2(120.0f, 400.0f));
   LineNode* lines = new LineNode(pts, Color4(0.1f, 0.7f, 0.1f, 1.0f), MySceneState.m_positionLoc);

   // Create scene graph
   SceneRoot->AddChild(lineShader);
   lineShader->AddChild(lines);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   std::cout << "Keyboard Controls:" << std::endl;
   std::cout << "ESC - Exit program" << std::endl;

   // Initialize GLUT
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(640, 480);
   glutInit(&argc, argv);

   // Create the window and callback methods
   glutCreateWindow("SimpleShader by David Nesbitt");
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

   // Set OpenGL defaults
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glLineWidth(2.0f);

   // Create the scene
   createScene();
   checkError("CreateScene");

   // Main GLUT loop
   glutMainLoop();

   return 0;
}