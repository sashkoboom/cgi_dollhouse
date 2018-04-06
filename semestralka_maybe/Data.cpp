#include "Data.h"
#include "GameState.h"


const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 760;
const int KEYS_COUNT = 4;

const char * WIN_TITLE = "Okay lemme do it";
const unsigned int REFRESH_INTERVAL = 33;
double elapsedTime = 0.0;
bool keyMap[KEYS_COUNT];

int windowWidth = WIN_WIDTH;
int windowHeight = WIN_HEIGHT;
bool freeCameraMode = true;
bool mouseMotionMode = false;


bool fogIsOn = false;

GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIN_WIDTH / 2.0;
GLfloat lastY = WIN_HEIGHT / 2.0;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

glm::vec3 cameraPos = glm::vec3(10.0f, 5.0f, -1.5f);
const glm::vec3 cameraPos_INITIAL = glm::vec3(10.0f, 5.0f, -1.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraCenter;
glm::vec3 cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 cameraViewDirection_INITIAL = glm::vec3(0.0f, 0.0f, -1.0f);
float cameraViewAngle = -90.0f;
const float cameraViewAngle_INITIAL = -90.0f;
const float cameraViewAngleDELTA = 2.0f;
glm::vec3 cameraPosPrev = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 projection = glm::perspective(50.0f, (GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT, 1.0f, 300.0f);
glm::mat4 projectionUi = glm::ortho(0.0f, (float)WIN_WIDTH, 0.0f, (float)WIN_HEIGHT, -1.0f, 1.0f);
glm::mat4 model = glm::mat4(0.01f);
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraViewDirection, cameraUp);
//shaders
GLuint ShaderProgram = 0;
GLuint SkyboxShaderProgram = 0;
GLuint PictureAnimatedShaderProgram =0;
GLuint BannerShaderProgram = 0;
GLuint StarShedrProgram = 0;

//axis Y
glm::vec3 axisY = glm::vec3(0.0f, 1.0f, 0.0f);

// Placing settings

glm::vec3 roomPosition = glm::vec3(11.0f, 0.0f, -23.0f);
glm::vec3 roomScale = glm::vec3(1.0f);
glm::vec3 roomRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat roomRotationAngle = 0.0f;

glm::vec3 horsePosition = glm::vec3(2.0f, 0.0f, -24.5f);
glm::vec3 horseScale = glm::vec3(0.02f, 0.02f, 0.02f);
glm::vec3 horseRotationAxis = glm::vec3(1.0f, 0.0f, -0.23f);
GLfloat horseRotationAngle = -120.0f;

glm::vec3 dollhousePosition = glm::vec3(3.0f, 0.0f, -28.0f);
glm::vec3 dollhouseScale = glm::vec3(0.006f, 0.006f, 0.006f);
glm::vec3 dollhouseRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat dollhouseRotationAngle = 30.0f;

glm::vec3 trainPosition = glm::vec3(6.5f, 0.0f, -16.0f);
glm::vec3 trainScale = glm::vec3(0.015f, 0.015f, 0.015f);
glm::vec3 trainRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
GLfloat trainRotationAngle = 1.0f;

glm::vec3 robotPosition = glm::vec3(0.8f, 0.0f, -22.5f);
glm::vec3 robotScale = glm::vec3(2.5f, 2.5f, 2.5f);
glm::vec3 robotRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
GLfloat robotRotationAngle = 45.0f;

glm::vec3 chestPosition = glm::vec3(16.5f, 0.0f, -19.0f);
glm::vec3 chestScale = glm::vec3(2.0f);
glm::vec3 chestRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat chestRotationAngle = 0.0f;

glm::vec3 bikePosition = glm::vec3(7.8f, 0.0f, -26.5f);
glm::vec3 bikeScale = glm::vec3(0.7f);
glm::vec3 bikeRotationAxis;
GLfloat bikeRotationAngle = -45.0f;

glm::vec3 carpetPosition = glm::vec3(7.8f, 0.0f, -23.5f);
glm::vec3 carpetScale = glm::vec3(0.04f);
glm::vec3 carpetRotationAxis;
GLfloat carpetRotationAngle = 20.0f;

glm::vec3 carPosition = glm::vec3(9.8f, 0.0f, -30.5f);
glm::vec3 carScale = glm::vec3(0.01f);
glm::vec3 carRotationAxis;
GLfloat carRotationAngle = 0.0f;

glm::vec3 plantPosition = glm::vec3(13.8f, -1.0f, -29.5f);
glm::vec3 plantScale = glm::vec3(0.7f);
glm::vec3 plantRotationAxis;
GLfloat plantRotationAngle = 0.0f;

glm::vec3 bedPosition = glm::vec3(15.0f, 0.0f, -21.0f);
glm::vec3 bedScale = glm::vec3(1.6f);
glm::vec3 bedRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
GLfloat bedRotationAngle = 0.0f;


glm::vec3 planePosition = glm::vec3(15.0f, 7.0f, -21.0f);
glm::vec3 planeDirection = glm::vec3(0.0f);
glm::vec3 planeScale = glm::vec3(0.17f);
glm::vec3 planeRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
GLfloat planeRotationAngle = 0.0f;

const int pictureNumQuadVertices = 4;

float x = 18.0f;
float z = -40.0f;
float paddingX = 10.0f;
float paddingY = 0.0f;

const float pictureVertexData[pictureNumQuadVertices * 5] = {
	// x      y     z     u     v
	-1*x, 0.0f + paddingY, z, 0.0f, 0.0f,
	x, 0.0f + paddingY, z, 1.0f, 0.0f,
	-1 * x, 2 * x + paddingY, z, 0.0f, 1.0f,
	x, 2 * x + paddingY, z, 1.0f, 1.0f,

};

const size_t  pointsSize = 4;
glm::vec3 curvePoints[] = {
	glm::vec3(-19.0, 1.0, -35.0),
	glm::vec3(20.0, 14.0, -35.0),
	glm::vec3(26.0, -8.0, -3.0),
	glm::vec3(-29.0, -4.0, 0.0),
	glm::vec3(16.0, 5.0, -3.0)
};

glm::vec3 globalAmbient = glm::vec3(0.0f);
glm::vec3 globalDiffuse = glm::vec3(0.7f);
glm::vec3 globalSpecular = glm::vec3(0.9f);

//banner's data
const int bannerNumQuadVertices = 4;
const float bannerVertexData[bannerNumQuadVertices * 5] = {

	// x      y      z     u     v
	0.0f, 1.25f, 2.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 2.0f, 0.0f, 0.0f,
	10.0f, 1.25f, 2.0f, 3.0f, 1.0f,
	10.0f, 0.0f, 2.0f, 3.0f, 0.0f
};


