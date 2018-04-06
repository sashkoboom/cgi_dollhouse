
#include <cmath>
#include <random>;
#include <iostream>;
#include <math.h>;
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "Data.h"
#include "GameState.h"
#include "Model.h"
#include "Picture.h"
#include "pgr.h"
#include "Star.h"
#include "Spline.h"
#include "SkyBox.h"
#include "PointLight.h"
#include "Reflector.h"
#include "Banner.h"


#define KEY_LEFT_ARROW 0
#define KEY_RIGHT_ARROW 1
#define KEY_UP_ARROW 2
#define KEY_DOWN_ARROW 3

//application starts with an inicial state
GameState* gameState = new InitialGameState();

//vector of models to be load
std::vector<Model*> models;


Model *plane = new Model();
Model *room = new Model();
Model *flor = new Model();
Model *dollhouse = new Model();
Model *horse = new Model();
Model *train = new Model();
Model *rabbit = new Model();
Model *chest = new Model();
Model *bike = new Model();
Model *carpet = new Model();
Model *car = new Model();
Model *plant = new Model();
Model *bed = new Model();
HardcodedModel *rubikCube = new HardcodedModel();


//special models

SkyboxObject * skybox = new SkyboxObject;
Reflector* reflector = new Reflector();
PictureAnimated* picture = new PictureAnimated();
BannerObject* banner = new BannerObject();
PointLight* duckPointLight = new PointLight(trainPosition + glm::vec3(-0.2f, 0.0f, -1.0f));
StarrySky *starrySky = new StarrySky();




bool pictureIsSet = false;
bool bannerIsSet = false;


/*
Renders the model and grants it given id to be passed through th stencil test
*/
void renderWithStencilTest(Model* model, int index){
	
    glStencilFunc(GL_ALWAYS, index + 1, 0xFF);
	model->render(elapsedTime, view, projection, false);
}

/*
Renders all the models
*/
void handleRenderingModels(){
	//RENDER MODELS
	skybox->render(view, projection);
	
	if (gameState->showPicture)picture->render(view, projection);
	CHECK_GL_ERROR();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	CHECK_GL_ERROR();


	for (int i = 0; i < models.size(); i++){
		if (!(gameState->starry   &&  i == 0)){
			renderWithStencilTest(models[i], i);
		}
	}
	CHECK_GL_ERROR();

	// enable stencil test
	if (gameState->isInitial){
		int id = 42;
		// set the stencil test function
		// -> stencil test always passes and reference value for stencil test is set to be object ID (id+1)
		glStencilFunc(GL_ALWAYS, id, 0xFF);
		//render banner;
		banner->render(view, projection);
	}
	CHECK_GL_ERROR();

	if (!gameState->starry){
		glStencilFunc(GL_ALWAYS, 24, 0xFF);
		plane->render(elapsedTime, view, projection, true);
		CHECK_GL_ERROR();
	}

	glDisable(GL_STENCIL_TEST);
	CHECK_GL_ERROR();

	if (gameState->starry)starrySky->render(view, projection);
	CHECK_GL_ERROR();
	// set params to reflector


	reflector->render();

	duckPointLight->render();


}

/*
Handels displaying
*/
void displayCallback() {
	CHECK_GL_ERROR();
	GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	mask |= GL_STENCIL_BUFFER_BIT;
	glClear(mask);
	elapsedTime = 0.001 * (double)glutGet(GLUT_ELAPSED_TIME);
	picture->currentTime += deltaTime;
	
	handleRenderingModels();

	CHECK_GL_ERROR();
	glutSwapBuffers();
	CHECK_GL_ERROR();
	
}

/*
Sets picture 
*/
void setPicture(void){

	
	picture->startTime = elapsedTime;
	picture->currentTime = 1;
	picture->frameDuration = 0.15f;
	picture->textureFrames = 20;
	picture->position = glm::vec3(10.0f, -6.0f, -10.0f);
	//glm::vec3(-10.0f, -1.0f, -10.0f);
	pictureIsSet = true;
}

/*
Sets moving banner
*/
void setBanner(void) {
	
	banner->speed = 0.0f;
	banner->position = glm::vec3(3.0f, 4.0f, -18.0f);
	banner->startTime = elapsedTime;
	banner->currentTime = 5;
	banner->direction = glm::vec3(0.0f, 1.0f, 0.0f);
	bannerIsSet = true;

}

/*
Sets model 
*/
void setModel(Model* object, std::string fileName, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, GLfloat angle){
	object->load(fileName);
	object->setPosition(position );
	object->setScale(scale);
	object->setRotation(rotation, angle);
	models.push_back(object);
}

/*
Handels loading all the models
*/
void loadModels(){
	
	
	skybox->loadModel();
	
	starrySky->loadStars();
	starrySky->step(elapsedTime);
	
	banner->loadModel("data/banner.png");
	
	picture->load("data/moon3.png");
	picture->setPosition(glm::vec3(30.0f, -1.0f, -10.0f));
	
	//loading and setting custom models 
	
	setModel(room, "data/room/room.obj", roomPosition, roomScale, roomRotationAxis, roomRotationAngle);
	setModel(flor, "data/room/room.obj", roomPosition + glm::vec3(0.0f, -16.35f, 0.0f), roomScale, roomRotationAxis, roomRotationAngle);	
	setModel(dollhouse, "data/house_obj.obj", dollhousePosition, dollhouseScale, dollhouseRotationAxis, dollhouseRotationAngle);
	setModel(horse, "data/gitara.obj", horsePosition, horseScale, horseRotationAxis, horseRotationAngle);
	setModel(train, "data/rubberduck.obj", trainPosition, trainScale, axisY, trainRotationAngle);
	setModel(rabbit, "data/Rabbit.obj", robotPosition, robotScale, axisY, robotRotationAngle);
	setModel(chest, "data/treasure_chest.obj", chestPosition, chestScale, axisY, chestRotationAngle);
	setModel(bike, "data/smallBicycle.obj", bikePosition, bikeScale, axisY, bikeRotationAngle);
	setModel(carpet, "data/carpet.obj", carpetPosition, carpetScale, axisY, carpetRotationAngle);
	setModel(car, "data/ct_obj.obj", carPosition, carScale, axisY, carRotationAngle);
	setModel(plant, "data/Plant_02.obj", plantPosition, plantScale, axisY, plantRotationAngle);
	setModel(bed, "data/bed.obj", bedPosition, bedScale, axisY, bedRotationAngle);
	
	//rubiks cube
	
	rubikCube->load("");
	rubikCube->setPosition(glm::vec3(12.0f, 0.7f, -17.0f));
	reflector->position = glm::vec4(glm::vec3(13.0f, 0.9f, -17.0f), 1.0f);

	rubikCube->setScale(glm::vec3(0.3f, 0.3f, 0.3f));
	models.push_back(rubikCube);

	
	

	//plane object
	
	plane->load("data/spt_obj.obj");
	plane->setPosition(planePosition);
	plane->setScale(glm::vec3(5.0f));
	plane->setRotation(planeRotationAxis, planeRotationAngle);
	plane->set(elapsedTime);

}

/*
Creates shaders
*/
void loadShders(){
	std::vector<GLuint> shaders; //main shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "vertexShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "fragmentShader.frag"));
	ShaderProgram = pgr::createProgram(shaders);


	std::vector<GLuint> skyboxShaders; //shaders for skybox
	skyboxShaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "skyboxVS.vert"));
	skyboxShaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "skyboxFS.frag"));
	SkyboxShaderProgram = pgr::createProgram(skyboxShaders);

	std::vector<GLuint> bannerShaders; //shaders for banner(moving)
	bannerShaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "bannerVS.vert"));
	bannerShaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "bannerFS.frag"));
	BannerShaderProgram = pgr::createProgram(bannerShaders);

	std::vector<GLuint> pictureShaders; //shaders for the picture
	pictureShaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "pictureVS.vert"));
	pictureShaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "pictureFS.frag"));
	PictureAnimatedShaderProgram = pgr::createProgram(pictureShaders);
}


void initializeApplication() {
	//initialize openGL
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	glClearStencil(0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	elapsedTime = 0.001 * (double)glutGet(GLUT_ELAPSED_TIME);
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	
	loadShders();
	
	CHECK_GL_ERROR();
	
	// load models
	loadModels();
}

void cleanupModels() {
	delete skybox;
	delete picture;
	delete reflector;
	delete banner;
	delete starrySky;
	delete plane;
	
	for (int i = 0; i < models.size(); i++){
		delete models[i];
	}

}

void finalizeApplication() {
	cleanupModels();

	delete gameState;
	
	pgr::deleteProgramAndShaders(ShaderProgram);
	pgr::deleteProgramAndShaders(SkyboxShaderProgram);
	pgr::deleteProgramAndShaders(PictureAnimatedShaderProgram);
	pgr::deleteProgramAndShaders(BannerShaderProgram);
	pgr::deleteProgramAndShaders(StarShedrProgram);
}


void reshapeCallback(int w, int h) {
	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}



/*
Makes camera go back
*/
void goBackward(GLfloat cameraSpeed){
	cameraPos -= cameraSpeed * cameraViewDirection;
	cameraPosPrev = cameraPos;
}
/*
Makes camera go forward
*/
void goForward(GLfloat cameraSpeed){
	cameraPos += cameraSpeed * cameraViewDirection;
	cameraPosPrev = cameraPos;
}

/*
Handles camera movements
*/
void moveCamera()
{
	if (!gameState->freeCamera)return;

	GLfloat cameraSpeed = 5.0f * deltaTime;

	if (keyMap[KEY_UP_ARROW])	{ goForward(cameraSpeed); }
	if (keyMap[KEY_DOWN_ARROW]){ goBackward(cameraSpeed); }
	if (keyMap[KEY_LEFT_ARROW])
	{
		//turn 5 degrees left
		cameraViewAngle -= cameraViewAngleDELTA;

		if (cameraViewAngle > 360.0f)cameraViewAngle -= 360.0f;
		float angle = glm::radians(cameraViewAngle);

		cameraViewDirection.x = cos(angle);
		cameraViewDirection.z = sin(angle);

	}
	if (keyMap[KEY_RIGHT_ARROW])
	{
		//turn 5 degrees right
		cameraViewAngle += cameraViewAngleDELTA;

		if (cameraViewAngle > 360.0f)cameraViewAngle -= 360.0f;
		float angle = glm::radians(cameraViewAngle);

		cameraViewDirection.x = cos(angle);
		cameraViewDirection.z = sin(angle);

	}
}

void updatePlane(float elapsedTime)
{
	plane->currentTime = elapsedTime;
	float curveTime = plane->speed * (plane->currentTime - plane->startTime);
	//update position and direction
	plane->position = plane->initPosition + evaluateClosedCurve(curvePoints, pointsSize, curveTime);
	plane->direction = glm::normalize(evaluateClosedCurveDerivative(curvePoints, pointsSize, curveTime));

	
}

void updateStarrySky()
{
	//update position
	starrySky->updateStars(elapsedTime);
}


//**************************************************************************************************
/// Detects collision with the borders of the room
void detectCollision(){
	if (cameraPos.x > 20.0f) cameraPos.x = 20.0f;
	if (cameraPos.x < 0.0f) cameraPos.x = 0.0f;

	if (cameraPos.z > -12.0f) {
		if (cameraPos.x > 12.0f) cameraPos.x = 12.0f;
		if (cameraPos.x < 9.0f) cameraPos.x = 9.0f;
	}

	if (cameraPos.x > 12.0f || cameraPos.x < 9.0f) {
		if (cameraPos.z > -14.0f) cameraPos.z = -14.0f;

	}


	if (cameraPos.z > -1.5f) cameraPos.z = -1.5f;
	if (cameraPos.z < -30.0f) cameraPos.z = -30.0f;
}


int bannerDirection = 1;

/*
Updates the scene and redisplays
*/
void refreshCallback(int) {
	CHECK_GL_ERROR();
	//update scene time
	elapsedTime = 0.001 * (double)glutGet(GLUT_ELAPSED_TIME);
	reflector->direction = cameraViewDirection;
	globalAmbient = gameState->ambientLight;

	// time of current frame
	GLfloat currentFrame = elapsedTime;
	// delta time
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	banner->currentTime += deltaTime * (banner->directionInt);
	//change direction
	if (banner->currentTime >= 7 || banner->currentTime <= 0) {
		banner->changeDirection();
		banner->currentTime += deltaTime * (banner->directionInt);
		
	}

	if (!pictureIsSet)setPicture();
	if (!bannerIsSet)setBanner();

	//change camera position by arrows
	moveCamera();
	
	if (gameState->freeCamera)	{
		detectCollision();
		view = glm::lookAt(cameraPos, cameraViewDirection + cameraPos, cameraUp);
	} else{
		// static view 1
		view = glm::lookAt(
			gameState->getPointOfView(),
			gameState->getViewDirection() + gameState->getPointOfView(),
			axisY
			);
	}
	

    updatePlane(elapsedTime);
	updateStarrySky();
	
	glutTimerFunc(REFRESH_INTERVAL, refreshCallback, 0);
	glutPostRedisplay();
	CHECK_GL_ERROR();
}
//**************************************************************************************************




void specialKeyboardCallback(int key, int x, int y){

	switch (key) {
	case GLUT_KEY_RIGHT:
		keyMap[KEY_RIGHT_ARROW] = true;
		break;
	case GLUT_KEY_LEFT:
		keyMap[KEY_LEFT_ARROW] = true;
		break;
	case GLUT_KEY_UP:
		keyMap[KEY_UP_ARROW] = true;
		break;
	case GLUT_KEY_DOWN:
		keyMap[KEY_DOWN_ARROW] = true;
		break;
	default:
		printf("Unrecognized special key pressed\n");
	}

	glutPostRedisplay();
}



void specialKeyboardUpCallback(int key, int x, int y){

	switch (key) {
	case GLUT_KEY_RIGHT:
		keyMap[KEY_RIGHT_ARROW] = false;
		break;
	case GLUT_KEY_LEFT:
		keyMap[KEY_LEFT_ARROW] = false;
		break;
	case GLUT_KEY_UP:
		keyMap[KEY_UP_ARROW] = false;
		break;
	case GLUT_KEY_DOWN:
		keyMap[KEY_DOWN_ARROW] = false;
		break;
	default:
		printf("Unrecognized special key pressed\n");
	}

	glutPostRedisplay();
}

void passiveMouseMotionCallback(int xpos, int ypos)
{
	

	if (!freeCameraMode) return;
	if (!mouseMotionMode)return;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 1.0f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraViewDirection = glm::normalize(front);
	reflector->position = glm::vec4(cameraViewDirection + cameraPos, 1.0f);


	glutPostRedisplay();
}

void switchState(int id){

	switch (id){
	case 42:
		gameState = new BasicGameState();
		break;
	case 3:
		gameState = new PictureGameState();
		break;
	case 24:
		gameState = new MovingPlaneGameState();
		break;
	case 52:
		gameState = new StarrySkyGameState();
		break;
	default:
		gameState = new BasicGameState();

	}
}


void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {

	switch (keyPressed) {
	case 27:
		glutLeaveMainLoop();
		break;


	case 'm':
		if (mouseMotionMode){
			cameraViewDirection = cameraViewDirection_INITIAL;
			mouseMotionMode = false;
		}
		else{
			mouseMotionMode = true;
		}
		break;
	case 'f': 
		fogIsOn = !fogIsOn;
		break;
	case 's':
		switchState(52);
		break;
	default:
		if (gameState->isMoving || gameState->showPicture){
			switchState(42);
		}
		printf("Unrecognized key pressed\n");
		}
	}



void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY) {

	if ((buttonPressed == GLUT_LEFT_BUTTON) && (buttonState == GLUT_DOWN)) {
		
		if (gameState->isMoving){
			switchState(42);
			return;
		}
		
		unsigned char id = 0;
		glReadPixels(mouseX, windowHeight - 1 - mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);
		printf("X: %d, Y:%d\n", mouseX, windowHeight - mouseY);
		std::cout << " ; id : " << id << std::endl;
	
		switchState(id);
	
	}
		
}

int main(int argc, char** argv) {
	srand(time(NULL));
	// initialize windowing system
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// initial window size
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(WIN_TITLE);
	// register callback for updating the display
	glutDisplayFunc(displayCallback);
	// register callback for change of window size
	glutReshapeFunc(reshapeCallback);

	// register callbacks for keyboard
	glutKeyboardFunc(keyboardCallback); //key pressed
	glutSpecialFunc(specialKeyboardCallback); //special key pressed
	glutSpecialUpFunc(specialKeyboardUpCallback); //special key released
	//register callback for mouse in free camera
	glutPassiveMotionFunc(passiveMouseMotionCallback);
	// register callback for mouse
	glutMouseFunc(mouseCallback);

	// update the scene every 33 ms
	glutTimerFunc(REFRESH_INTERVAL, refreshCallback, 0);
	// initialize GL, devil etc.
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");
	// initialize application
	initializeApplication();
	// register callback when window closed
	glutCloseFunc(finalizeApplication);
	//enters the GLUT event processing loop
	glutMainLoop();
	
	return 0;
}