/*
 * ofAppEmscriptenWindow.h
 *
 *  Created on: May 8, 2014
 *      Author: arturo
 * ofxAppEmscriptenWindow.h
 * Reworked to use emscripten-glfw (contrib.glfw3)
 */

#ifndef OFAPPEMSCRIPTENWINDOW_H_
#define OFAPPEMSCRIPTENWINDOW_H_

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"

#ifdef TARGET_EMSCRIPTEN
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include <GLFW/emscripten_glfw3.h>
#endif

class ofxAppEmscriptenWindow: public ofAppBaseGLESWindow {
public:
	ofxAppEmscriptenWindow();
	~ofxAppEmscriptenWindow();

	static bool allowsMultiWindow(){ return false; }
	static bool doesLoop(){ return true; }
	static bool needsPolling(){ return false; }
	static void pollEvents(){}
	static void loop();

	void setup(const ofGLESWindowSettings & settings);

	void hideCursor();
	void showCursor();                     // now implemented

	void setWindowShape(int w, int h);
	glm::vec2 getWindowPosition();
	glm::vec2 getWindowSize();
	glm::vec2 getScreenSize();

	ofOrientation getOrientation();
	bool doesHWOrientation();

	int getWidth();
	int getHeight();

	ofWindowMode getWindowMode();

	void setFullscreen(bool fullscreen);
	void toggleFullscreen();

	void enableSetupScreen();
	void disableSetupScreen();

	void setVerticalSync(bool enabled);

	void setTouchSimulatesMouse(bool abEnable);
	void preventDefaultBrowserTouchMoveBehavior();
	void setFullscreenScaleStrategy(int aStrategy);

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

	void update();
	void draw();

	virtual void makeCurrent();
	virtual void startRender();
	virtual void finishRender();

	bool bIsSetup = false;

private:
	static ofxAppEmscriptenWindow * instance;

	static void display_cb();

	// GLFW callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void cursorEnterCallback(GLFWwindow* window, int entered);

	GLFWwindow* glfwWindow = nullptr;

	bool bEnableSetupScreen = true;
	ofCoreEvents _events;
	std::shared_ptr<ofBaseRenderer> _renderer;

	ofWindowMode mCurrentWindowMode = OF_WINDOW;
	ofWindowMode mTargetWindowMode = OF_WINDOW;

	bool mBTouchSimulatesMouse = true;
	int mFullscreenScaleStrategy = -1;

	int mCachedWidth = -1;
	int mCachedHeight = -1;
};

#endif /* OFAPPEMSCRIPTENWINDOW_H_ */
