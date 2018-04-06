#include "Picture.h"

PictureAnimated::~PictureAnimated()
{
	//clean up
	glDeleteVertexArrays(1, &(vertexArrayObject));
	glDeleteBuffers(1, &(elementBufferObject));
	glDeleteBuffers(1, &(vertexBufferObject));
}
void PictureAnimated::setPosition(const glm::vec3 position){
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
}
void PictureAnimated::load(std::string fileName)
{
	//create texture
	texture = pgr::createTexture(fileName);
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	CHECK_GL_ERROR();
	glGenBuffers(1, &vertexBufferObject);
	CHECK_GL_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	CHECK_GL_ERROR();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * pictureNumQuadVertices, pictureVertexData, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
	//pass params 
	glEnableVertexAttribArray(glGetAttribLocation(PictureAnimatedShaderProgram, "position"));
	CHECK_GL_ERROR();
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(glGetAttribLocation(PictureAnimatedShaderProgram, "position"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(glGetAttribLocation(PictureAnimatedShaderProgram, "texCoord"));
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(glGetAttribLocation(PictureAnimatedShaderProgram, "texCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	numTriangles = pictureNumQuadVertices;
	CHECK_GL_ERROR();
}
void PictureAnimated::render(const glm::mat4 & view, const glm::mat4 & projection)
{

	
	//set blending properties
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);
	glUseProgram(PictureAnimatedShaderProgram);

	

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
	
	glm::mat4 PVMmatrix = projection * view * matrix;
	//pass params using uniform
	glUniformMatrix4fv(glGetUniformLocation(PictureAnimatedShaderProgram, "PVMmatrix"), 1, GL_FALSE, glm::value_ptr(PVMmatrix));  // model-view-projection
	glUniform1f(glGetUniformLocation(PictureAnimatedShaderProgram, "time"), currentTime);
	glUniform1i(glGetUniformLocation(PictureAnimatedShaderProgram, "texSampler"), 0);
	glUniform1f(glGetUniformLocation(PictureAnimatedShaderProgram, "frameDuration"), frameDuration);
	
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vertexArrayObject);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTriangles);
	
	glBindVertexArray(0);
	glUseProgram(0);
	//disable blending function
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	return;

}

