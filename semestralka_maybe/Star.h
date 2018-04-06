#pragma once
#include "pgr.h"
#include "PointLight.h";

class Star{
public:
	Star();
	~Star();

	GLuint vertexBufferObject;   // identifier for the vertex buffer object
	GLuint  elementBufferObject;  // identifier for the element buffer object
	GLuint  vertexArrayObject;    // identifier for the vertex array object
	GLuint  texture;
	
	
	unsigned int  numTriangles;         // number of triangles in the mesh
	glm::mat4 model;
	
	glm::vec3 position;
	
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	glm::vec3	  color;
	
	void loadFromSrc(int i);
	glm::vec3 randomizePosition();
	
	void render(const glm::mat4 &view, const glm::mat4 &projection, int i);

	void step(const double elapsedTime);

	void updatePosition(float elapsedTime);


	float speed;				//the speed of moving
	float startTime;			//start time
	float currentTime;			//current time
	glm::vec3 initPosition;		//position on the start
	
	
	glm::vec3 direction;		//direction of the zero
	float angle;
	float radius;

	
	PointLight* pointLight;

};


class StarrySky{

private:
	Star* stars[300] ;

public:
	StarrySky();
	~StarrySky();
	float currentTime;
	float startTime = 0;
	float speed = 0.4f;
	void step(float elapsedTime);
	

	void render(const glm::mat4 &view, const glm::mat4 &projection);

	void loadStars();
	void updateStars(float elapsedTime);

};

