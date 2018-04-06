#pragma once
#include "pgr.h"
#include <vector>
#include <memory>
#include "Mesh.h"



class Model {
public:
	Model();
	~Model();
	
	virtual void load(std::string fileName);
	
	void render(const double elapsedTime, const glm::mat4 & view, const glm::mat4 & projection, bool animation);
	
	void setPosition(const glm::vec3 position);
	
	void setRotation(const glm::vec3 rotation, GLfloat angle);
	
	void setScale(const glm::vec3 scale);
	
	bool isBanner;
	
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 position;
	glm::vec3 rot;
	glm::vec3 scale;
	
	
	std::vector<std::shared_ptr<MeshObject>> meshes;
	
	void set(const double elapsedTime);
	float speed;				//the speed of moving
	float startTime;			//start time
	float currentTime;			//current time
	
	glm::vec3 initPosition;		//position on the start
	glm::vec3 direction;		//direction of the zero

};


class HardcodedModel : public Model{


public:
	void load(std::string fileName);
};


