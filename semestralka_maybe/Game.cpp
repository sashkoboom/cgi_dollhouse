/**
* Autor Martin Vondrak
*/

#include "game.h"
#include "pgr.h"

#include <iostream>

Game * Game::instance = NULL;

Game * Game::getInstance(void)
{
	if (!instance) {
		instance = new Game();
	}

	return instance;
}

Game::Game(void)
{
	projection = glm::mat4(1.0f);
	
	for (int i = 0; i < KEY_COUNT; i++) {
		keyMap[i] = false;
	}
}

void Game::reshapeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	projection = glm::perspective(60.0f, float(windowWidth) / float(windowHeight), 1.0f, 1000.0f);
	return;
}

void Game::draw(void)
{
	
}

GLuint Game::createShaderProgram(const char * vertexShaderFile, const char * fragmentShaderFile)
{
	std::vector<GLuint> shaders;
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderFile));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFile));
	return pgr::createProgram(shaders);
}

int Game::getWindowWidth(void)
{
	return windowWidth;
}

int Game::getWindowHeight(void)
{
	return windowHeight;
}

void Game::initShaders(void)
{
	
}

void Game::initStaticCars(void)
{
	
}




void Game::initObjects(void)
{

}

void Game::init(void)
{
	// OpenGL setup
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0f);

	// Initialize shaders
	initShaders();
	// Initialize objects
	initObjects();
	

	/** BASE */
	// Vao
	glGenVertexArrays(1, &trianglesBaseVao);
	glBindVertexArray(trianglesBaseVao);
	// Vbo
	glGenBuffers(1, &baseVbo);
	glBindBuffer(GL_ARRAY_BUFFER, baseVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
	// Eao
	glGenBuffers(1, &trianglesBaseEao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglesBaseEao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseTriangleIdx), baseTriangleIdx, GL_STATIC_DRAW);
	
	
	glBindVertexArray(0);
	CHECK_GL_ERROR();
	return;
}

void Game::cleanup()
{
	// TODO delete game objects
	pgr::deleteProgramAndShaders(lightingShader.program);
}

void Game::drawDynamicMeshGeometry(DynamicMeshGeometry * geometry, glm::vec3 position, glm::mat4 & projection)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

	glUseProgram(lightingShader.program);
	lightingShader.setTransformUniforms(projection, Camera::getInstance()->getViewMatrix(), model);
	lightingShader.setUniforms(time, fogEnabled);
	lightingShader.setMaterialUniforms(geometry->getMaterial());
	glBindVertexArray(geometry->getVao());
	glDrawElements(GL_TRIANGLES, geometry->getNumIndices(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	return;
}

void Game::drawLoadedObject(LoadedObject * object, glm::mat4 & projection)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), object->position);

	if (object->rotate) {
		model = glm::rotate(model, object->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, object->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, object->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if (object->scale) {
		model = glm::scale(model, glm::vec3(object->ratio));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(lightingShader.program);
	lightingShader.setTransformUniforms(projection, Camera::getInstance()->getViewMatrix(), model);
	lightingShader.setUniforms(time, fogEnabled);
	glBindVertexArray(object->geometry->getVao());

	for (unsigned int mat = 0; mat < object->geometry->getNumSubMeshes(); mat++) {
		const SubMesh * subMesh = object->geometry->getSubMesh(mat);
		Material * material = object->geometry->getSubMesh(mat)->createMaterial();
		lightingShader.setMaterialUniforms(material);
		glDrawElementsBaseVertex(GL_TRIANGLES, subMesh->nIndices, GL_UNSIGNED_INT, (void *)(subMesh->startIndex * sizeof(unsigned int)), subMesh->baseVertex);
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	return;
}

void Game::drawSkyboxMeshGeometry(SkyboxMeshGeometry * geometry, glm::mat4 & projection)
{
	glUseProgram(skyboxShader.program);

	// compose transformations
	glm::mat4 view = Camera::getInstance()->getViewMatrix();
	glm::mat4 matrix = projection * view;

	// create view rotation matrix by using view matrix with cleared translation
	glm::mat4 viewRotation = view;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// vertex shader will translate screen space coordinates (NDC) using inverse PV matrix
	glm::mat4 inversePvMatrix = glm::inverse(projection * viewRotation);

	glUniformMatrix4fv(skyboxShader.inversePvMatrixLoc, 1, GL_FALSE, glm::value_ptr(inversePvMatrix));
	glUniform1i(skyboxShader.skyboxSamplerLoc, 0);
	glUniform1i(skyboxShader.fogEnabledLoc, fogEnabled);

	// draw "skybox" rendering 2 triangles covering the far plane
	glBindVertexArray(geometry->getVao());
	glBindTexture(GL_TEXTURE_CUBE_MAP, geometry->getTexture());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->getNumIndices());

	glBindVertexArray(0);
	glUseProgram(0);
}

void Game::drawBase(glm::vec3 position, glm::mat4 & projection)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 12.0f, 0.5f));
	Material * mat = new Material();
	mat->color = glm::vec3(1.0f);
	mat->ambient = glm::vec3(1.0f);
	mat->specular = glm::vec3(1.0f);
	mat->diffuse = glm::vec3(1.0f);
	mat->shininess = 20.0f;

	glUseProgram(lightingShader.program);
	lightingShader.setTransformUniforms(projection, Camera::getInstance()->getViewMatrix(), model);
	lightingShader.setMaterialUniforms(mat);
	glBindVertexArray(trianglesBaseVao);
	glDrawElements(GL_TRIANGLES, baseNTriangles * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	delete mat;
	return;
}

void Game::drawExplosion(ExplosionObject * object, glm::mat4 & projection)
{
	if (object == NULL) {
		return;
	}

	glm::mat4 view = Camera::getInstance()->getViewMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(explosionShader.program);

	// just take rotation part of the view transform
	glm::mat4 rotationMatrix = glm::mat4(
		view[0],
		view[1],
		view[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
	// inverse view rotation
	rotationMatrix = glm::transpose(rotationMatrix);

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), explosion->position);
	matrix = glm::scale(matrix, glm::vec3(explosion->size));
	matrix = matrix * rotationMatrix; // make billboard to face the camera

	glm::mat4 pvmMatrix = projection * view * matrix;
	glUniformMatrix4fv(explosionShader.pvmMatrixLoc, 1, GL_FALSE, glm::value_ptr(pvmMatrix));  // model-view-projection
	glUniform1f(explosionShader.timeLoc, explosion->currentTime - explosion->startTime);
	glUniform1i(explosionShader.explosionSamplerLoc, 0);
	glUniform1f(explosionShader.frameDurationLoc, explosion->frameDuration);

	glBindVertexArray(explosion->geometry->getVao());
	glBindTexture(GL_TEXTURE_2D, explosion->geometry->getTexture());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, explosion->geometry->getNumIndices());

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
}

void Game::drawBmw(void)
{
	glUseProgram(lightingShader.program);
	lightingShader.setBmwLightUniforms(bmw->position + glm::vec3(-0.7f, 4.0f, -4.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.9f, 1.5f, bmw->interactivityEnabled);
	glUseProgram(0);
	drawLoadedObject(bmw, projection);
}

void Game::drawBillboard(DynamicMeshGeometry * geometry, glm::vec3 position, glm::mat4 & projection)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 view = Camera::getInstance()->getViewMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(billboardShader.program);

	model = glm::rotate(model, -90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 pvmMatrix = projection * view * model;
	glUniformMatrix4fv(billboardShader.pvmMatrixLoc, 1, GL_FALSE, glm::value_ptr(pvmMatrix));
	glUniform1i(billboardShader.billboardSamplerLoc, 0);
	glUniform1f(billboardShader.timeLoc, time);
	glBindVertexArray(geometry->getVao());
	glBindTexture(GL_TEXTURE_2D, billboardMesh->getMaterial()->texture);
	glDrawElements(GL_TRIANGLES, geometry->getNumIndices(), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	return;
}

void Game::enableKey(int key)
{
	keyMap[key] = true;
}

void Game::disableKey(int key)
{
	keyMap[key] = false;
}

bool Game::isKeyEnabled(int key) const
{
	return keyMap[key];
}
