#include "Banner.h"



BannerObject::BannerObject()
{
}
BannerObject::~BannerObject()
{
	glDeleteVertexArrays(1, &(vertexArrayObject));
	glDeleteBuffers(1, &(elementBufferObject));
	glDeleteBuffers(1, &(vertexBufferObject));
}
void BannerObject::loadModel(std::string fileName)
{
	CHECK_GL_ERROR();
	// get vertex attributes locations
	GLint posLocation = glGetAttribLocation(BannerShaderProgram, "position");
	CHECK_GL_ERROR();
	GLint texCoordLocation = glGetAttribLocation(BannerShaderProgram, "texCoord");
	CHECK_GL_ERROR();
	GLint matLoc = glGetUniformLocation(BannerShaderProgram, "PVMmatrix");
	GLint timeLocation = glGetUniformLocation(BannerShaderProgram, "time");
	GLint  texSamplerLocation = glGetUniformLocation(BannerShaderProgram, "texSampler");
	// create texture
	std::cout << "tex: " << fileName << std::endl;
	CHECK_GL_ERROR();
	texture = pgr::createTexture(fileName);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	// vertices and triangles
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bannerNumQuadVertices * 5, bannerVertexData, GL_STATIC_DRAW);
	//vertices
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	//textures
	glEnableVertexAttribArray(texCoordLocation);
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	numTriangles = bannerNumQuadVertices;
	CHECK_GL_ERROR();
	
}
void BannerObject::render(const glm::mat4 & view, const glm::mat4 & projection)
{
	
	// turn on set blending options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(BannerShaderProgram);
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
	matrix = glm::scale(matrix, glm::vec3(1.5f));
	glm::mat4 PVMmatrix = projection * view * matrix;
	//set the uniforms
	glUniformMatrix4fv(glGetUniformLocation(BannerShaderProgram, "PVMmatrix"), 1, GL_FALSE, glm::value_ptr(PVMmatrix));
	glUniform1f(glGetUniformLocation(BannerShaderProgram, "time"), currentTime);
	glUniform1i(glGetUniformLocation(BannerShaderProgram, "texSampler"), 0);
	
	

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vertexArrayObject);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTriangles);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	//turn of blending
	glDisable(GL_BLEND);

	return;
}

void BannerObject::changeDirection(){
	switch (directionInt){
	case 1:
		directionInt = -1; break;
	case -1:
		directionInt = 1; break;
	}
}