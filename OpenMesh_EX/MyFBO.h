#pragma once
#ifndef  MyFBO_h
#define MyFBO_h
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <iostream>
float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};
class MyFBO {
public:
	unsigned int fbo;
	unsigned int fboColor;
	unsigned int fboDepth;
private:
	unsigned int VAO, VBO;
public:
	MyFBO() {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &fboColor);
		glBindTexture(GL_TEXTURE_2D, fboColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColor, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &fboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 600, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}
	~MyFBO() {
		glDeleteFramebuffers(1,&fbo);
	}
	void use() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	void draw() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, fboColor);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};
#endif // ! MyFBO_h
