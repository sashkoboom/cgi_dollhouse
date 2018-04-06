#include "Model.h"
#include <map>
#include <iostream>
#include "Spline.h"


Model::Model(){
	
	
}
Model::~Model(){
}
void Model::render(const double elapsedTime, const glm::mat4 & view, const glm::mat4 & projection, bool animation){
	
	if (animation){
		planePosition = position;
		planeDirection = direction;
		
		model = adjustObjectPosition(position, direction, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	for (unsigned int i = 0; i < meshes.size(); i++){
		//render model
		
		meshes[i]->render(view, projection, model);
	}
}
Assimp::Importer importer; 
std::map<std::string, Model *> sceneMap;

void Model::load(std::string fileName){
	//load file
	
	std::cout << std::endl << "LOADING: " << fileName << std::endl << std::endl << std::endl << std::endl;
	CHECK_GL_ERROR();
	//check if model has already loaded
	if (sceneMap.find(fileName) != sceneMap.end())
	{
	
		Model *obj = sceneMap[fileName];
		for (int i = 0; i < obj->meshes.size(); i++)
			meshes.push_back(std::shared_ptr<MeshObject>(obj->meshes[i]));
	}
	else
	{
	
	
		// Load asset from the file 
		const aiScene * scn = importer.ReadFile(fileName.c_str(), 0
			| aiProcess_Triangulate
			| aiProcess_PreTransformVertices
			| aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices);
		   CHECK_GL_ERROR();
		for (int i = 0; i < scn->mNumMeshes; i++){
			CHECK_GL_ERROR();
		
			
			
			// create meshObject for each mesh in the model
			MeshObject*meshObj = new MeshObject;
			
			meshObj->loadFromScene(scn, i, fileName);
			CHECK_GL_ERROR();
			meshes.push_back(std::shared_ptr<MeshObject>(meshObj));
		}
		
		//add to exists models
		sceneMap[fileName] = this;
	
	}
}

void Model::setPosition(const glm::vec3 position){
	model = glm::mat4(1.0f);
	//translate
	model = glm::translate(model, position);
}
void Model::setRotation(const glm::vec3 rotation, GLfloat angle){
	//rotate
	model = glm::rotate(model, angle, rotation);

}
void Model::setScale(const glm::vec3 scale){
	//scale
	model = glm::scale(model, glm::vec3(scale));
	
}


void Model::set(const double elapsedTime){
	//use to callcilate step
	startTime = elapsedTime;
	currentTime = startTime;
	initPosition = glm::vec3(13.0f, 5.0f, -4.0f);
	position = initPosition;
	speed = 0.4f;
	direction = glm::normalize(glm::vec3(0.5f, 0.0f, 0.5f));
}


void HardcodedModel::load(std::string fileName){
	HardcodedMesh* meshObj = new HardcodedMesh;
	//load from .cpp .h 
	meshObj->load();

	// add to meshes
	meshes.push_back(std::shared_ptr<MeshObject>(meshObj));
}