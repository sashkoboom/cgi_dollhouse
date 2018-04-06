#include "Reflector.h"
#include <iostream>
#include "Data.h"

Reflector::Reflector()
{
}


Reflector::~Reflector()
{

}

void Reflector::render()
{

	glUseProgram(ShaderProgram);
	//passing params using iniform
	glUniform4fv(glGetUniformLocation(ShaderProgram, "spotPosition"), 1, glm::value_ptr(position));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "spotDirection"), 1, glm::value_ptr(direction));

	
}