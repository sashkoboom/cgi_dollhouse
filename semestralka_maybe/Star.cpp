#include "Star.h" 
#include "pgr.h"
#include "Data.h"
#include <random>;
#include <iostream>;
#include <math.h>;
#include <cstdlib>
#include <ctime>
#include "Spline.h";


const int nAttribsPerVertex = 8;
// number of vertices
const int nVertices = 10;
//number of indices
const int nIndices = 9;


float paramH = 0.1f;
float paramW = 0.1f;
float paramB = -0.1f;


//indices
const int indicies[] = {
	//o b d
	0, 1, 2,
	//f o a
	3, 0, 6,
	//e o j
	5, 0, 7,
	//g h c
	8, 9, 4
	
};

Star::Star()
{
	model = glm::mat4(1.0f);
	radius = 18.5f;
	//translate
	position = this->randomizePosition();
	initPosition = position;
	
	model = glm::translate(model, position);
	model = glm::rotate(model, -90.0f - angle, axisY);
	//model = glm::translate(model, glm::vec3(37.0f, 3.0f, -40.0f));
}

void Star::step(const double elapsedTime)
{
	startTime = elapsedTime;
	currentTime = startTime;
	position = initPosition;
	
}


glm::vec3 calculateOrbit(float angle, float radius, float y){

	glm::vec3 pos = glm::vec3(0.0f);

	pos.x = cos(M_PI / 180.0f*angle) * radius;
	pos.z = sin(M_PI / 180.0f*angle) * radius;
	pos.y = y;
	

	pos += glm::vec3(10.1f, 0.0f, -24.5f);

	return pos;

}

glm::vec3 Star::randomizePosition(){

	glm::vec3 position = glm::vec3(0.0f);
	
	
	int height = 20;
		 angle = 0.0;	
		
	this->angle = rand() % 360 - 0; //Gives a number between 360 and 0;
	
	position.y = rand() % height - 0;
	position = calculateOrbit(angle, radius, position.y);
	
	
	
	
	

	return position;
}

void Star::loadFromSrc(int i){
	GLint vertexLoc = glGetAttribLocation(ShaderProgram, "vertex");
	GLint colorLoc = glGetAttribLocation(ShaderProgram, "color");
	GLint normalLoc = glGetAttribLocation(ShaderProgram, "normal");
	GLint texCoordLoc = glGetAttribLocation(ShaderProgram, "texCoord");
	glGenVertexArrays(1, &(vertexArrayObject));
	glBindVertexArray(vertexArrayObject);

	float w, b, h;

	switch (i % 4){
	
	case 0:
		w = paramW * 4;
		b = paramB * 5;
		h = paramH * 4;
		break;
	case 1:
		w = paramW * 5;
		b = paramB * 5;
		h = paramH * 3;
		break;
	case 2:
		w = paramW * 4;
		b = paramB * 5;
		h = paramH * 5;
		break;
	case 3:
		w = paramW * 6;
		b = paramB * 6;
		h = paramH * 3;
		break;
	
	}
	
	float vert[] = {
		//x		//y		//z
		0.125f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// o 0
		-1 * w, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,//b 1
		0.18f, b, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,//d 2
		-0.1f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//f 3
		-0.18f, b, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,//c 4
		w, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//e 5
		0.0f, h, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// a 6
		0.145f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// j 7
		0.0f, -0.3f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// g 8
		-0.145f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//9 h
	};
	 
	glGenBuffers(1, &(vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(nVertices + 80)* nAttribsPerVertex, &vert, GL_STATIC_DRAW);

	glGenBuffers(1, &(elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int)* nIndices, &indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), 0);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	//set materials
	ambient = glm::vec3(1.0f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);
	shininess = 1.0f;
	numTriangles = nIndices;
	
	CHECK_GL_ERROR();

	

}

glm::mat4 adjustStarPosition(glm::vec3& position, float angle) {


	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
	matrix = glm::rotate(matrix, -90.0f - angle, axisY);
	
	return matrix;
}


void Star::render(const glm::mat4 &view, const glm::mat4 &projection, int i)
{
	
	

	model = adjustStarPosition(position, angle);
	glm::mat4 matrix = projection * view * model;

	
	
	glUseProgram(ShaderProgram);
	//passing params using uniform
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Vmatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Mmatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	CHECK_GL_ERROR();
	glm::vec3 color = glm::vec3(1.0f);
	glUniform3fv(glGetUniformLocation(ShaderProgram, "color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(ShaderProgram, "shininess"), shininess);
	//glUniform1f(glGetUniformLocation(ShaderProgram, "isStar"), 1);
	CHECK_GL_ERROR();


;

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, (numTriangles)* 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	
}




void Star::updatePosition(float time){
	angle += 0.1;

	position = calculateOrbit(angle, radius, position.y);
}



Star::~Star()
{
}


StarrySky::StarrySky(){
	startTime = 0;
	currentTime = 0;

	for (int i = 0; i < 300; i++)
	{
	
		stars[i] = new Star();
	
	}

}
StarrySky::~StarrySky(){
	for (int i = 0; i < 300; i++)
	{

		delete stars[i];

	}
}

void StarrySky::loadStars(){

	for (int i = 0; i < 300; i++)
	{

		stars[i]->loadFromSrc(i);

	}

}

void StarrySky::step(float elapsedTime){

	speed = 0.4f;

	for (int i = 0; i < 300; i++)
	{

		stars[i]->step(elapsedTime);

	}


}


void StarrySky::render(const glm::mat4 &view, const glm::mat4 &projection){

	for (int i = 0; i < 300; i++)
	{

		stars[i]->render(view, projection, i);

	}

}
void StarrySky::updateStars(float elapsedTime){

	float time = speed * (currentTime - startTime);

	for (int i = 0; i < 300; i++)
	{

		stars[i]->updatePosition(time);

	}

}
