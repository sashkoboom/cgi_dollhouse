#ifndef __DATA_H_
#define __DATA_H_

#include "pgr.h"
#include "Model.h"

//parameters of window
extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;
// count of keys
extern const int KEYS_COUNT;
// title of the window
extern const char * WIN_TITLE;
// interval for refresh
extern const unsigned int REFRESH_INTERVAL;
// time used to callculate movement
extern double elapsedTime;
// keys used in the key map
extern bool keyMap[];
//window params for resize
extern int windowWidth;
extern int windowHeight;
// flags for check camera mode
extern bool freeCameraMode;
extern bool mouseMotionMode;


extern bool fogIsOn;


// Camera
extern GLfloat yaw;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
extern GLfloat pitch;
extern GLfloat lastX;
extern GLfloat lastY;
extern bool firstMouse;

extern GLfloat deltaTime;	// Time between current frame and last frame
extern GLfloat lastFrame;	// Time of last frame

extern glm::vec3 cameraPos;
extern const glm::vec3 cameraPos_INITIAL;
extern glm::vec3 cameraFront;
extern::glm::vec3 cameraPosPrev;
extern glm::vec3 cameraUp;
extern glm::vec3 cameraCenter;
extern glm::vec3 cameraViewDirection;
extern const glm::vec3 cameraViewDirection_INITIAL;
extern float cameraViewAngle;
extern const float cameraViewAngle_INITIAL;
extern const float cameraViewAngleDELTA;



extern glm::mat4 projection;
extern glm::mat4 projectionUi;
extern glm::mat4 model;
extern glm::mat4 view;

// Shaders

extern GLuint ShaderProgram;
extern GLuint SkyboxShaderProgram;
extern GLuint PictureAnimatedShaderProgram;
extern GLuint BannerShaderProgram;
extern GLuint StarShedrProgram;

//axis
extern glm::vec3 axisY;

// Horse placing settings
extern glm::vec3 roomPosition;
extern glm::vec3 roomScale;
extern glm::vec3 roomRotationAxis;
extern GLfloat roomRotationAngle;

// Horse placing settings
extern glm::vec3 horsePosition;
extern glm::vec3 horseScale;
extern glm::vec3 horseRotationAxis;
extern GLfloat horseRotationAngle;

// dollhouse placing settings
extern glm::vec3 dollhousePosition;
extern glm::vec3 dollhouseScale;
extern glm::vec3 dollhouseRotationAxis;
extern GLfloat dollhouseRotationAngle;

//fox placing
extern glm::vec3 foxPosition;
extern glm::vec3 foxScale;
extern glm::vec3 foxRotationAxis;
extern GLfloat foxRotationAngle;
//train placing
extern glm::vec3 trainPosition;
extern glm::vec3 trainScale;
extern glm::vec3 trainRotationAxis;
extern GLfloat trainRotationAngle;
//robot
extern glm::vec3 robotPosition;
extern glm::vec3 robotScale;
extern glm::vec3 robotRotationAxis;
extern GLfloat robotRotationAngle;
//chest
extern glm::vec3 chestPosition;
extern glm::vec3 chestScale;
extern glm::vec3 chestRotationAxis;
extern GLfloat chestRotationAngle;

extern glm::vec3 bikePosition;
extern glm::vec3 bikeScale;
extern glm::vec3 bikeRotationAxis;
extern GLfloat bikeRotationAngle;

extern glm::vec3 carpetPosition;
extern glm::vec3 carpetScale;
extern glm::vec3 carpetRotationAxis;
extern GLfloat carpetRotationAngle;

extern glm::vec3 carPosition;
extern glm::vec3 carScale;
extern glm::vec3 carRotationAxis;
extern GLfloat carRotationAngle;

extern glm::vec3 plantPosition;
extern glm::vec3 plantScale;
extern glm::vec3 plantRotationAxis;
extern GLfloat plantRotationAngle;

//bed
extern glm::vec3 bedPosition;
extern glm::vec3 bedScale;
extern glm::vec3 bedRotationAxis;
extern GLfloat bedRotationAngle;

extern glm::vec3 planePosition ;
extern glm::vec3 planeDirection;
extern glm::vec3 planeScale ;
extern glm::vec3 planeRotationAxis ;
extern GLfloat planeRotationAngle ;


extern const int pictureNumQuadVertices;
extern const float pictureVertexData[];

extern const size_t  pointsSize;
extern glm::vec3 curvePoints[];

extern glm::vec3 globalAmbient;
extern glm::vec3 globalDiffuse;
extern glm::vec3 globalSpecular;

extern const int bannerNumQuadVertices;
extern const float bannerVertexData[];

#endif __DATA_H_