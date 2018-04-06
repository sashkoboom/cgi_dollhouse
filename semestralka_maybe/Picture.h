#pragma once
#include "Model.h"
#include "Data.h"
#include <iostream>
class PictureAnimated
{
public:

	~PictureAnimated();
	
	void load(std::string fileName);
	
	void setPosition(const glm::vec3 position);
	
	void render(const glm::mat4 & view, const glm::mat4 & projection);
	
	glm::mat4 model;
	glm::vec3 position;
	
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	GLuint        texture;
	
	unsigned int  numTriangles;         // number of triangles in the mesh


	glm::vec3	  color;
	int    textureFrames;
	float  frameDuration;
	float startTime;
	float currentTime;

};

