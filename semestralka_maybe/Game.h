/**
* Autor Martin Vondrak
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "pgr.h"
#include "ShaderProgram.h"
#include "Object.h"

enum { KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_W, KEY_A, KEY_S, KEY_D, KEY_COUNT };

class Game {
private:
	static Game * instance;

	/** General info */
	int windowWidth;
	int windowHeight;
	bool keyMap[KEY_COUNT];
	float time;
	bool fogEnabled = false;

	/** Shader programs */
	

	/** Projection matrix */
	glm::mat4 projection;

	// VBO, VAO, etc.
	GLuint baseVbo;
	GLuint trianglesBaseEao;
	GLuint trianglesBaseVao;

	/** Hide constructor, copy constructor and operator= because of singleton */
	Game(void);
	Game(Game const & game);
	Game & operator=(Game const & game);
	~Game(void);

public:
	/** Return game instance */
	static Game * getInstance(void);

	/** Reshape window after changing size */
	void reshapeWindow(int width, int height);

	/** Draw all objects */
	void draw(void);

	/** Create shader program from vertex and fragment shader files */
	GLuint createShaderProgram(const char * vertexShaderFile, const char * fragmentShaderFile);

	/** Return window width */
	int getWindowWidth(void);

	/** Return window height */
	int getWindowHeight(void);

	/** Initialize shaders */
	void initShaders(void);

	/** Initialize static cars */
	void initStaticCars(void);

	/** Initialize moving cars */
	void initMovingCars(void);

	/** Initialize building objects */
	void initBuildings(void);

	/** Initialize game objects */
	void initObjects(void);

	/** Initialize whole game */
	void init(void);

	/** Cleanup game objects */
	void cleanup(void);

	/** Draw base under VW */
	void drawBase(glm::vec3 position, glm::mat4 & projection);

	/** Enable key */
	void enableKey(int key);

	/** Disable key */
	void disableKey(int key);

	/** Checks whether key is enabled */
	bool isKeyEnabled(int key) const;

	/** Update game objects */
	void update(void);

	

	/** Set elapsed time */
	void setTime(float time);

};

#endif // __GAME_H__