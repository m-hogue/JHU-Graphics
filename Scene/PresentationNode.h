//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:	David W. Nesbitt
//	File:    PresentationNode.h
//	Purpose: Scene graph presentation node.
//
//============================================================================

#ifndef __PRESENTATIONNODE_H
#define __PRESENTATIONNODE_H

#include <IL/il.h>

/**
 * Presentation node. Applies material and texture
 */
class PresentationNode : public SceneNode
{
public:
	/**
	 * Constructor
	 */
	PresentationNode() 
	{
		m_nodeType          = SCENE_PRESENTATION;
		m_materialShininess = 1.0f;
		m_texture = 0;
		m_textureUnit = 0;
		// Note: color constructors default rgb to 0 and alpha to 1
	}
	
	/**
	 * Destructor
	 */
	~PresentationNode() { }

	/**
	 * Set material ambient reflection coefficient.
	 */
	void SetMaterialAmbient(const Color4& c)
	{
		m_materialAmbient = c;
	}
	
	void SetMaterialDiffuse(const Color4& c)
	{
		m_materialDiffuse = c;
	}
	
	void SetMaterialAmbientAndDiffuse(const Color4& c)
	{
		m_materialAmbient = c;
		m_materialDiffuse = c;
	}
	
	void SetMaterialSpecular(const Color4& c)
	{
		m_materialSpecular = c;
	}
	
	void SetMaterialEmission(const Color4& c)
	{
		m_materialEmission = c;
	}
	
	void SetMaterialShininess(const float s)
	{
		m_materialShininess = s;
	}

	void SetTexture(const char* fname, GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint magFilter, GLenum textureUnit)
	{
		m_textureUnit = textureUnit;
		// Bind a DevIL image
		ILuint id;
		ilGenImages(1, &id);
		ilBindImage(id);
		ILuint err = ilGetError();
		if (err)
			printf("Error binding image. %s %d\n", fname, err);

		// Load image using lower left origin. Convert to RGBA
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		ilEnable(IL_ORIGIN_SET);
		ilLoadImage(fname);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		err = ilGetError();
		if (err)
			printf("Error loading texture. %s %d\n", fname, err);

		// Get image dimensions and data
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);
		unsigned char* data = ilGetData();
		if (ilGetError() != IL_NO_ERROR)
			printf("Error getting image data\n");

		// allocate texture
		glGenTextures(1, &m_texture);
		glActiveTexture(m_textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// when texture area is small, bilinear filter the closest mipmap
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		//	GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

		// when texture area is large, bilinear filter the first mipmap
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

		// build our texture mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
	}


	/**
	 * Draw. Simply sets the material properties.
	 */
	void Draw(SceneState& sceneState)
	{
      // Set the material uniform values
      glUniform4fv(sceneState.m_materialAmbientLoc, 1,  &m_materialAmbient.r);
      glUniform4fv(sceneState.m_materialDiffuseLoc, 1,  &m_materialDiffuse.r);
      glUniform4fv(sceneState.m_materialSpecularLoc, 1, &m_materialSpecular.r);
      glUniform4fv(sceneState.m_materialEmissionLoc, 1, &m_materialEmission.r);
      glUniform1f(sceneState.m_materialShininessLoc, m_materialShininess);
	  glUniform1i(sceneState.m_textureLoc, m_texture);
	  //glActiveTexture(m_textureUnit);
	  //glBindTexture(GL_TEXTURE_2D, m_texture);
  
		// Draw children of this node
		SceneNode::Draw(sceneState);

		// unbind texture
		//glActiveTexture(GL_TEXTURE0);
	}
	
protected:
	Color4       m_materialAmbient;
	Color4       m_materialDiffuse;
	Color4       m_materialSpecular;
	Color4       m_materialEmission;
	GLfloat      m_materialShininess;
	GLuint		 m_texture;
	GLenum		 m_textureUnit;
};

#endif