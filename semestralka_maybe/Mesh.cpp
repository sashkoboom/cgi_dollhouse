#include "Mesh.h"
#include "Data.h"
#include "hardcodedObject.h"

MeshObject::MeshObject(){
}

MeshObject::MeshObject(const MeshObject &obj) {
}
MeshObject::~MeshObject(){
	//clean up
	glDeleteVertexArrays(1, &(vertexArrayObject));
	glDeleteBuffers(1, &(elementBufferObject));
	glDeleteBuffers(1, &(vertexBufferObject));
}
#include <iostream>
void  MeshObject::loadFromScene(const aiScene *scn, const int ind, std::string &fileName){
	CHECK_GL_ERROR();
	//one mesh from the model
	const aiMesh * mesh = scn->mMeshes[ind];
	// vertex buffer object, store all vertex positions and normals
	glGenBuffers(1, &(vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	
	// allocate memory for vertices, normals, and texture coordinates
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float)*mesh->mNumVertices, 0, GL_STATIC_DRAW);
	// first store all vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float)*mesh->mNumVertices, mesh->mVertices);
	// first store all normals
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float)*mesh->mNumVertices, 3 * sizeof(float)*mesh->mNumVertices, mesh->mNormals);

	float *textureCoords = new float[2 * mesh->mNumVertices];// 2 floats per vertex
	float *currentTextureCoord = textureCoords;
	// copy texture coordinates
	aiVector3D vect;
	
	if (mesh->HasTextureCoords(0)) {
		// we use 2D textures with 2 coordinates and ignore the third coordinate
		for (unsigned int idx = 0; idx < mesh->mNumVertices; idx++) {
			vect = (mesh->mTextureCoords[0])[idx];
			*currentTextureCoord++ = vect.x;
			*currentTextureCoord++ = vect.y;
		}
	}
	//store all texture coordinates
	glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float)*mesh->mNumVertices, 2 * sizeof(float)*mesh->mNumVertices, textureCoords);
	// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
	unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
	for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
		indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
		indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
		indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
	}
	// copy our temporary index array to OpenGL and free the array
	glGenBuffers(1, &(elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned)* mesh->mNumFaces, indices, GL_STATIC_DRAW);
	delete[] indices;

	// copy the material info to MeshGeometry structure
	const aiMaterial *mat = scn->mMaterials[mesh->mMaterialIndex];
	aiColor3D color;
	aiString name;
	//set material
	
	mat->Get(AI_MATKEY_NAME, name);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE, color);
	diffuse = glm::vec3(color.r, color.g, color.b);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_AMBIENT, color);
	ambient = glm::vec3(color.r, color.g, color.b);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_SPECULAR, color);
	specular = glm::vec3(color.r, color.g, color.b);
	CHECK_GL_ERROR();
	mat->Get<float>(AI_MATKEY_SHININESS, shininess);
	shininess = shininess / 4.0f;
	texture = 0;
	CHECK_GL_ERROR();
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		mat->Get<aiString>(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
		//load texture from file
		std::string textureName = name.data;
	
		std::cout << "tex: " << textureName << std::endl;
		
		bool mipmap = false;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		// set linear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		CHECK_GL_ERROR();
		// upload our image data to OpenGL
		if (!pgr::loadTexImage2D(textureName, GL_TEXTURE_2D)) {
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &texture);
		
		}
	
		// unbind the texture (just in case someone will mess up with texture calls later)
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR();
	}

	numTriangles = mesh->mNumFaces;


	CHECK_GL_ERROR();

	glGenVertexArrays(1, &(vertexArrayObject));
	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	CHECK_GL_ERROR();
	
	GLint vertexLoc = glGetAttribLocation(ShaderProgram, "vertex");
	GLint normalLoc = glGetAttribLocation(ShaderProgram, "normal");
	
	CHECK_GL_ERROR();
	GLint colorLoc = glGetAttribLocation(ShaderProgram, "color");
	GLint texCoordLoc = glGetAttribLocation(ShaderProgram, "texCoord");
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(normalLoc);
	CHECK_GL_ERROR();
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)* mesh->mNumVertices));
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(texCoordLoc);
	CHECK_GL_ERROR();
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float)* mesh->mNumVertices));
	CHECK_GL_ERROR();

	glBindVertexArray(0);



}


void MeshObject::render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &model){
	
	glm::mat4 matrix = projection * view * model;
	glUseProgram(ShaderProgram);
	//passing params using uniform
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Vmatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "Mmatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(ShaderProgram, "shininess"), shininess);

	glUniform3fv(glGetUniformLocation(ShaderProgram, "globalDiffuse"), 1, glm::value_ptr(globalDiffuse));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "globalAmbient"), 1, glm::value_ptr(globalAmbient));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "globalSpecular"), 1, glm::value_ptr(globalSpecular));
	
	if (fogIsOn){
	
		glUniform1f(glGetUniformLocation(ShaderProgram, "fogIsOn"), 1);
	}
	else{
		glUniform1f(glGetUniformLocation(ShaderProgram, "fogIsOn"), 0);
	}
	
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, (numTriangles)* 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}


void MeshObject::load(){}

void HardcodedMesh::load(){

	GLint vertexLoc = glGetAttribLocation(ShaderProgram, "vertex");
	GLint colorLoc = glGetAttribLocation(ShaderProgram, "color");
	GLint normalLoc = glGetAttribLocation(ShaderProgram, "normal");
	CHECK_GL_ERROR();
	GLint texCoordLoc = glGetAttribLocation(ShaderProgram, "texCoord");
	glGenVertexArrays(1, &(vertexArrayObject));
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &(vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(nVertices + 80)* nAttribsPerVertex, &vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &(elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int)* nIndices, &indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vertexLoc);

	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), 0);
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(normalLoc);
	CHECK_GL_ERROR();
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), (void*)(3 * sizeof(float)));
	CHECK_GL_ERROR();
	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, (nAttribsPerVertex *  sizeof(float)), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	//set materials
	ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	diffuse = glm::vec3(0.5f, 0.10f, 0.32f);
	specular = glm::vec3(0.5f, 0.5f, 0.5f);
	shininess = 92.156863 / 4.0f;
	numTriangles = nIndices;
	std::string textureName = "data/grave1.png";


	texture = pgr::createTexture(textureName);

}