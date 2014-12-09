#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Scene/Scene.h"
#include "GL/gl3w.h"
#include "GL/freeglut.h"

#include <io.h>

class Texture : public GeometryNode {
public:
	Texture(const char* filename, int wrap, int size) {
		int width, height;
		void* data;
		FILE* file;

		file = fopen(filename, "rb");
		if (file == NULL) return;

		width = size;
		height = size;
		data = malloc(width*height * 3);

		// read texture
		fread(data, width*height * 3, 1, file);
		fclose(file);

		// allocate texture
		glGenTextures(1, &m_texture);

		glBindTexture(GL_TEXTURE_2D, m_texture);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first mipmap
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP_TO_EDGE);

		glGenerateMipmap(GL_TEXTURE_2D);

		// build our texture mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, data);

		// free buffer
		free(data);
	}

	/**
	 * method to retrieve texture
	 */
	GLuint getTexture() {
		return m_texture;
	}

	/**
	* Draw this geometry node. Geometry nodes are leaf nodes and have no children.
	* @param  sceneState  Current scene state
	*/
	virtual void Draw(SceneState& sceneState) {
		// set uniform and bind texture for drawing
		glUniform1i(sceneState.m_textureLoc, m_texture);
		glActiveTexture(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		
		// Draw all children
		SceneNode::Draw(sceneState);

		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

protected:
	GLuint m_texture;
};


#endif
