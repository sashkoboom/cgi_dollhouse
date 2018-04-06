#include "PointLight.h"
#include "Data.h"

PointLight::PointLight(glm::vec3 pos)
{
	//initialize point light
	isOn = true;
	position = pos;
	ambient = glm::vec3(1.0f);
	diffuse = glm::vec3(0.1f);
	specular = glm::vec3(0.1f);
	color = glm::vec3(1.0f);
	//point light 
	pointConstantAtt = 1.0f;
	pointLinearAtt = 2.0f;
	pointQuadraticAtt = 0.6f;
}


PointLight::~PointLight()
{
}

#include <sstream>
#include <iostream>
void PointLight::render(){
	
	glUseProgram(ShaderProgram);
	std::stringstream strS;
	strS << id;
	std::string idStr = strS.str();
	GLint colorInt = glGetUniformLocation(ShaderProgram, std::string("points[" + idStr + "].color").c_str());
	glUniform3fv(colorInt, 1, glm::value_ptr(color));
	
	//passing structure of point lights using unifrom
	glUniform3fv(glGetUniformLocation(ShaderProgram, std::string("point.position").c_str()), 1, glm::value_ptr(position));
	glUniform3fv(glGetUniformLocation(ShaderProgram, std::string("point.diffuse").c_str()), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(ShaderProgram, std::string("point.ambient").c_str()), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(ShaderProgram, std::string("point.specular").c_str()), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(ShaderProgram, std::string("point.pointConstantAtt").c_str()), pointConstantAtt);
	glUniform1f(glGetUniformLocation(ShaderProgram, std::string("point.isOn").c_str()), 1);
	glUniform1f(glGetUniformLocation(ShaderProgram, std::string("point.pointLinearAtt").c_str()), pointLinearAtt);
	glUniform1f(glGetUniformLocation(ShaderProgram, std::string("point.pointQuadraticAtt").c_str()), pointQuadraticAtt);
	
	CHECK_GL_ERROR();

	
}
