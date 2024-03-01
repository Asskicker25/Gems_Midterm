#include "RenderQuad.h"
#include "Debugger.h"

unsigned int RenderQuad::quadVAO = 0;
unsigned int RenderQuad::quadVBO = 0;

void RenderQuad::Render()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		GLCALL(glGenVertexArrays(1, &quadVAO));
		GLCALL(glGenBuffers(1, &quadVBO));
		GLCALL(glBindVertexArray(quadVAO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));
		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
		GLCALL(glEnableVertexAttribArray(1));
		GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	}
	GLCALL(glBindVertexArray(quadVAO));
	GLCALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	GLCALL(glBindVertexArray(0));
}
