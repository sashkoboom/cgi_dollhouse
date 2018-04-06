#pragma once
#include "Data.h"
#include "pgr.h"
#include <iostream>

class SkyboxObject{
public:
	SkyboxObject();
	~SkyboxObject();

	//**************************************************************************************************
	/// load skybox 

	void loadModel();
	//**************************************************************************************************
	/// render skybox
	/**
	\param[in]  view			view matrix
	\param[in]  projection      projection matrix
	\param[in]  fog				fog Object
	*/
	void render(const glm::mat4 & view, const glm::mat4 & projection);
private:
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	GLuint        texture;				// texture of the banner
	unsigned int  numTriangles;         // number of triangles in the banner
};
