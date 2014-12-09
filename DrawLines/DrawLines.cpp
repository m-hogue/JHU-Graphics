//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.461 Principles of Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    DrawLines.cpp
//	Purpose: OpenGL and GLUT program to draw line segments entered with 
//			   the mouse.  Line intersection calculation is also demonstrated.
//
//============================================================================

#include <iostream>
#include <vector>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include "geometry/geometry.h"
#include "Scene/Color3.h"
#include "ShaderSupport/GLSLShader.h"
#include "Scene/Scene.h"
#include "PointShaderNode.h"
#include "PointNode.h"
#include "LineShaderNode.h"
#include "LineNode.h"

char PointVShader[] = "\
   #version 150\n\
   in vec2 position;\n\
   uniform float point_size;\n\
   uniform mat4 ortho;\n\
   void main() {\n\
      gl_PointSize = point_size;\n\
      gl_Position  = ortho * vec4(position, 0.0, 1.0);\n\
   }";

char PointFShader[] = "\
   #version 150\n\
   out vec4 fragColor;\n\
   void main() {\n\
      if (dot(gl_PointCoord-0.5, gl_PointCoord-0.5) > 0.25)\n\
        discard;\n\
      else\n\
		  fragColor = vec4(gl_PointCoord.st, 0, 1);\n\
   }";

char LineVShader[] = "#version 150\n\
in vec2 position;			// Vertex position\n\
in vec3 vertexColor;		// Vertex color\n\
uniform mat4 ortho;			// Orthographic projection matrix\n\
smooth out vec4 color;		// Smooth shaded output to fragment shader\n\
void main()\n\
{\n\
	// Set the output color and transform the position\n\
    color = vec4(vertexColor, 1.0);\n\
    gl_Position = ortho * vec4(position, 0.0, 1.0);\n\
}";

char LineFShader[] = "#version 150\n\
smooth in vec4 color;		// Smooth shaded input color\n\
out vec4 fragColor;			// Output color to framebuffer\n\
void main()\n\
{\n\
    fragColor = color;\n\
}";

// Maximum number of line segments (for capacity of the VBO)
const unsigned int MAX_LINE_SEGMENTS = 1000;

// Root of the scene graph
SceneNode* SceneRoot;

// Scene nodes for storing and interacting with lines and points
LineNode*  CurrentLineNode;
LineNode*  PriorLinesNode;
PointNode* PriorIntersections;
PointNode* CurrentIntersections;

// Scene state
SceneState MySceneState;

// Are we drawing a line with the mouse? This global flag allows us to 
// follow mouse motion events and draw if the left button is down.
bool DrawLine = false;

// Current line being drawn (while dragging the mouse)
LineSegment2 CurrentLine;
Color3 CurrentLineColor0(0.8f, 0.1f, 0.1f);     // Color at the start of the line
Color3 CurrentLineColor1(0.1f, 0.8f, 0.1f);      // Color at the end of the line

// Seed the random number generator for colors
void SeedRandomColors()
{
   srand(111000);
}

// Get a random color. Make sure it isn't too bright (no component > 0.75)
Color3 GetRandomColor()
{
   float r = 1.0f;
   float g = 1.0f;
   float b = 1.0f;
   while(r >= 0.75 && g >= 0.75 && b >= 0.75)
   {
      r = rand01();
      g = rand01();
      b = rand01();
   }
   return Color3(r, g, b);
}

/**
 * Display callback function
 */
void display(void)
{
   // Clear the framebuffer
   glClear(GL_COLOR_BUFFER_BIT);

   if (DrawLine)
   {
      // Calculate the intersection of the current line with all others and update the 
      // current intersections geometry node with the new intersections.
      std::vector<Point2> currentIntersectionPts;
      Point2 intersectPt;
      std::vector<LineSegment2>& segments = PriorLinesNode->GetLineSegments();
      std::vector<LineSegment2>::iterator line;
      for (line = segments.begin(); line != segments.end(); line++)
         if (line->Intersect(CurrentLine, intersectPt))
            currentIntersectionPts.push_back(intersectPt);
      CurrentIntersections->Replace(currentIntersectionPts);
   }

   // Draw the scene
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

    // Clear the list of PreviousLines, intersection points, and the current line
    // See random colors again so hopefully colors appear in same order
    case 'c':
      CurrentLineNode->Clear();
      PriorLinesNode->Clear();
      PriorIntersections->Clear();
      CurrentLine.A.Set(0.0f, 0.0f);
      CurrentLine.B.Set(0.0f, 0.0f);
      SeedRandomColors();
      glutPostRedisplay();
	   break;

    // Enable anti-aliasing
    case 'A':
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glutPostRedisplay();
      break;

    // Disable anti-aliasing
    case 'a':
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_BLEND);
      glutPostRedisplay();
      break;

    case 'M':
       glEnable(GL_MULTISAMPLE);
       glutPostRedisplay();
       break;

    case 'm':
       glDisable(GL_MULTISAMPLE);
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
      // Set the current line endpoints (both) to the mouse position and
      // set flag indicating we are dragging a line
      DrawLine = true;
      CurrentLine.A.Set((float)x, (float)y);
      CurrentLine.B.Set((float)x, (float)y);

	   // Force a redisplay
      glutPostRedisplay();
   }

   // On a button up event add the CurrentLine to the list of PreviousLines
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
      // Use the same color at each end of the line
      Color3 c = GetRandomColor();
      PriorLinesNode->AddLineSegment(CurrentLine, c, c);
   
      // Get all the intersections of the current line with all other PreviousLines
      // and add them to the Prior. This avoids having to calculate these 
      // intersections again.
      Point2 intersectPt;
      std::vector<Point2> intersectionPts;
      std::vector<LineSegment2>& segments = PriorLinesNode->GetLineSegments();
      std::vector<LineSegment2>::iterator line;
      for (line = segments.begin(); line != segments.end(); line++)
         if (line->Intersect(CurrentLine, intersectPt))
            intersectionPts.push_back(intersectPt);
      PriorIntersections->Add(intersectionPts);

	   // Clear the current intersection points
	   CurrentIntersections->Clear();

      // No longer dragging a line - clear it
      DrawLine = false;
      CurrentLineNode->Clear();

	   // Force a redisplay
      glutPostRedisplay();
   }
}

/**
 * Mouse motion callback (called when mouse button is depressed)
 */
void mouseMotion(int x, int y)
{
   // Set the current line endpoints to the mouse position
   if (DrawLine)
   {
      // Update the current line VBO
      CurrentLine.B.Set((float)x, (float)y);
      CurrentLineNode->Replace(CurrentLine, CurrentLineColor0, CurrentLineColor1);
      glutPostRedisplay();
   }
}

/**
 * Reshape callback. Load a 2-D orthographic projection matrix using the 
 * window width and height so we can directly take window coordinates and 
 * send to OpenGL
 * @param  width  Window width
 * @param  height Window height
 */
void reshape(int width, int height)
{
   // Set a 2-D orthographic projection matrix
   MySceneState.m_projection.m00() = 2.0f / (float)width;
   MySceneState.m_projection.m03() = -1.0f;
   MySceneState.m_projection.m11() = -2.0f / (float)height;
   MySceneState.m_projection.m13() = 1.0f;

   // Update the viewport
   glViewport(0, 0, width, height);
}

/**
 * Create the scene
 */
void CreateScene() 
{
      // Create the point shader node
   PointShaderNode* pointShader = new PointShaderNode();
   if (!pointShader->CreateFromSource(PointVShader, PointFShader) ||
//   if (!pointShader->Create("points.vert", "points.frag") ||
       !pointShader->GetLocations())
      exit(-1);

   LineShaderNode* lineShader = new LineShaderNode();
//   if (!lineShader->Create("lines.vert", "lines.frag") ||
   if (!lineShader->CreateFromSource(LineVShader, LineFShader) ||
       !lineShader->GetLocations())
      exit(-1);

   // Create a node for managing current line. Use a width = 4 for current lines
   CurrentLineNode = new LineNode(2, 4.0f, lineShader->GetPositionLoc(), lineShader->GetColorLoc());

   // Create a node for managing prior lines. Use a width = 2 for prior lines.
   PriorLinesNode = new LineNode(MAX_LINE_SEGMENTS, 2.0f, lineShader->GetPositionLoc(), lineShader->GetColorLoc());

   // Create the node that manages the prior intersection points
   PriorIntersections = new PointNode(MAX_LINE_SEGMENTS * MAX_LINE_SEGMENTS, 8.0f, pointShader->GetPositionLoc());

   // Create the node for intersections with the current line
   CurrentIntersections = new PointNode(MAX_LINE_SEGMENTS, 8.0f, pointShader->GetPositionLoc());

   // Create scene graph
   SceneRoot = new SceneNode;
   SceneRoot->AddChild(lineShader);
   lineShader->AddChild(PriorLinesNode);
   lineShader->AddChild(CurrentLineNode);
   SceneRoot->AddChild(pointShader);
   pointShader->AddChild(PriorIntersections);
   pointShader->AddChild(CurrentIntersections);
}

/**
 * Main 
 */
int main(int argc, char** argv)
{
   std::cout << "Keyboard Controls:" << std::endl;
   std::cout << "  A - Enable line anti-aliasing  a - Disable line anti-aliasing" 
             << std::endl;
   std::cout << "  M - Enable MSAA                m - Disable MSAA" 
             << std::endl;
   std::cout << "  c - Clear all points" << std::endl;
   std::cout << "ESC - Exit program" << std::endl;

   // Initialize free GLUT
   glutInit(&argc, argv);
   glutInitContextVersion(3, 2);
   //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);  // Using this causes LineWidth to error
   glutInitContextProfile(GLUT_CORE_PROFILE);
 
   // Double buffer and multisample anti-aliasing
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
   glutInitWindowPosition(100, 100); 
   glutInitWindowSize(640, 480);
   if (glutCreateWindow("DrawLines by David Nesbitt") < 0) 
   {
      printf("Could not create Window with glutCreateWindow\n");
      return -1;
   }

   // Set the callback methods
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

   // Create the scene
   SeedRandomColors();
   CreateScene();
   checkError("CreateScene");

   // Set the clear color to white
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   
   // Set the global point parameters
   glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
   glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

   // Setup blend function for line anti-aliasing
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);            // Best quality line AA

   glutMainLoop();
   return 0;
}