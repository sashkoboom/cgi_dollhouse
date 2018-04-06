#pragma once
#include "pgr.h"
#include <string>


class MeshObject
{
public:
	MeshObject();
	MeshObject(const MeshObject &obj);
	~MeshObject();

	virtual void load();
	
	void  loadFromScene(const aiScene *scn, const int ind, std::string &fileName);

		void render(const glm::mat4 &view,
		const glm::mat4 &projection,
		const glm::mat4 &model);

	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	GLuint        texture;
	unsigned int  numTriangles;         // number of triangles in the mesh

	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	glm::vec3	  color;
};


class HardcodedMesh : public MeshObject{

public:
	void load();

};