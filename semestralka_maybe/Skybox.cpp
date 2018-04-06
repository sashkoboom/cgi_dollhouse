#include "SkyBox.h"

SkyboxObject::SkyboxObject(){}
SkyboxObject::~SkyboxObject()
{
	//clean up the object
	glDeleteVertexArrays(1, &(vertexArrayObject));
	glDeleteBuffers(1, &(elementBufferObject));
	glDeleteBuffers(1, &(vertexBufferObject));
}
void SkyboxObject::loadModel(){
	GLint screenCoordLoc = glGetAttribLocation(SkyboxShaderProgram, "screenCoord");
	//set screen coordinates
	static const float screenCoords[] = {
		1.0f, 1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// buffer for far plane rendering
	glGenBuffers(1, &(vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_DYNAMIC_DRAW);


	
	glEnableVertexAttribArray(screenCoordLoc);
	glVertexAttribPointer(screenCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();

	numTriangles = 2;
	//set texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	CHECK_GL_ERROR();
	//load cube map texture
	for (int i = 0; i < 6; i++) {
		std::string texName = std::string("skybox/skybox") + "_" + suffixes[i] + ".jpg";
		std::cout << "Loading cube map texture: " << texName << std::endl;
		if (!pgr::loadTexImage2D(texName, targets[i])) {
			pgr::dieWithError("Skybox cube map loading failed!");
		}
	}

	CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR();
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();


}

void SkyboxObject::render(const glm::mat4 & view, const glm::mat4 & projection){
	glUseProgram(SkyboxShaderProgram);
	//calculate matrix
	glm::mat4 matrix = projection * view;
	glm::mat4 viewRotation = view;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 inversePVmatrix = glm::inverse(projection * view);
	//pass params using uniform
	
	glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram, "inversePVmatrix"), 1, GL_FALSE, glm::value_ptr(inversePVmatrix));
	glUniform1i(glGetAttribLocation(SkyboxShaderProgram, "skyboxSampler"), 0);
	if (fogIsOn){
		glUniform1f(glGetUniformLocation(SkyboxShaderProgram, "fogIsOn"), 1);
	}
	else{
		glUniform1f(glGetUniformLocation(SkyboxShaderProgram, "fogIsOn"), 0);
	}
	glBindVertexArray(vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, numTriangles + 2);

	glBindVertexArray(0);
	glUseProgram(0);

}