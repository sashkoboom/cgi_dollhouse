#include "GameState.h"
#include "Data.h"


GameState::GameState(){
	
}

GameState::~GameState(){}

InitialGameState::InitialGameState(){
	ambientLight = glm::vec3(0.6f, 0.0f, 0.07f);
	diffuseLight = glm::vec3(0.7f);
	specularLight = glm::vec3(0.9f);

	
	isInitial = true;
	freeCamera = false;
}

glm::vec3 InitialGameState::getPointOfView(){

	
	return cameraPos_INITIAL;
}

glm::vec3 InitialGameState::getViewDirection(){
	return cameraViewDirection_INITIAL;
}

BasicGameState::BasicGameState(){
	ambientLight = glm::vec3(0.0f);
	diffuseLight = glm::vec3(0.7f);
	specularLight = glm::vec3(0.9f);
}

PictureGameState::PictureGameState(){

	showPicture = true;
	freeCamera = false;


	ambientLight = glm::vec3(0.3f, 0.25f, 0.0f);
	diffuseLight = glm::vec3(0.7f);
	specularLight = glm::vec3(0.9f);

	pointOfView = glm::vec3(11.0f, 0.5f, -12.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

}

glm::vec3 PictureGameState::getPointOfView(){
	return pointOfView;
}

glm::vec3 PictureGameState::getViewDirection(){
	return viewDirection;
}


MovingPlaneGameState::MovingPlaneGameState(){
	freeCamera = false;
	isMoving = true;
}

glm::vec3 MovingPlaneGameState::getPointOfView(){
	
	std::cout << "hm" << std::endl;
	return planePosition + glm::vec3(0.0f, 7.0f, 0.0f);
}

glm::vec3 MovingPlaneGameState::getViewDirection(){
	return planeDirection;
}

StarrySkyGameState::StarrySkyGameState(){

	starry = true;

}