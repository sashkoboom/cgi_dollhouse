#pragma once
#include <iostream>
#include "data.h"
#include "pgr.h"

class BannerObject
{
public:
	BannerObject();
	~BannerObject();

	void loadModel(std::string fileName);
	void render(const glm::mat4 & view, const glm::mat4 & projection);
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 direction;

	int directionInt = 1;
	void changeDirection();

	GLuint        vertexBufferObject;
	GLuint        elementBufferObject;
	GLuint        vertexArrayObject;
	unsigned int  numTriangles;
	GLuint        texture;
	float startTime;
	float speed;
	float currentTime;
};

