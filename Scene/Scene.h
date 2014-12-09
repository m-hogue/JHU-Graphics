//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    Scene.h
//	Purpose:	Scene graph support.
//
//============================================================================

#ifndef __SCENE_H
#define __SCENE_H

#include <vector>

enum SceneNodeType { SCENE_BASE, SCENE_PRESENTATION, 
                     SCENE_TRANSFORM, SCENE_GEOMETRY,
                     SCENE_SHADER, SCENE_CAMERA, SCENE_LIGHT  };

// Include other scene files
#include "Scene/Color3.h"
#include "Scene/Color4.h"
#include "Scene/SceneState.h"
#include "Scene/SceneNode.h"
#include "Scene/TransformNode.h"
#include "Scene/PresentationNode.h"
#include "Scene/GeometryNode.h"
#include "Scene/ShaderNode.h"
#include "Scene/CameraNode.h"
#include "Scene/TriSurface.h"
#include "Scene/MeshTeapot.h"
#include "Scene/UnitSquare.h"
#include "Scene/ConicSurface.h"
#include "Scene/LightNode.h"
#include "Scene/SphereSection.h"
#include "Scene/Torus.h"

inline void checkError(const char* str) 
{
   GLenum err = glGetError();
   if (err != GL_NO_ERROR)
      printf("%s: OpenGL Error: %d\n", str, err);
}

#endif