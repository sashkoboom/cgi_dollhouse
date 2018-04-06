#pragma once
#include "pgr.h"



class PointLight
{
public:
	PointLight(glm::vec3 pos);
	~PointLight();
	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;
	glm::vec3  position;
	glm::vec3 color;
	int id;
	bool isOn;
	 
	float	pointConstantAtt;
	float	pointLinearAtt;
	float	pointQuadraticAtt;
	
	void render();

};

