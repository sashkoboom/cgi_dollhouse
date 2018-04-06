#pragma once
#include "pgr.h"
#include <iostream>
class GameState
{
public:
	GameState();

	~GameState();
	
	bool freeCamera = true;
	


	//lights
	glm::vec3 ambientLight ;
	glm::vec3 diffuseLight ;
	glm::vec3 specularLight ;

	//whether the doll picture is shown
	bool showPicture = false;

	bool isInitial = false;
	bool isMoving = false;
	bool starry = false;
	
	glm::vec3 pointOfView;
	glm::vec3 viewDirection;

	virtual glm::vec3 getPointOfView(){
	  
		return glm::vec3(0.0f);
	};
	virtual glm::vec3 getViewDirection(){
		return glm::vec3(0.0f);
	};
private: 
	int i = 0;

};

class InitialGameState : public GameState{
   public:
	   InitialGameState();
	   glm::vec3 getPointOfView();
	   glm::vec3 getViewDirection();
};

class BasicGameState : public GameState{
public:
	BasicGameState();
};


class PictureGameState : public GameState{
public:
	PictureGameState();
	glm::vec3 getPointOfView();
	glm::vec3 getViewDirection();
};

class MovingPlaneGameState : public GameState{
public:
	MovingPlaneGameState();
	glm::vec3 getPointOfView();
	glm::vec3 getViewDirection();
};

class StarrySkyGameState : public GameState{
public:
	StarrySkyGameState();
};


