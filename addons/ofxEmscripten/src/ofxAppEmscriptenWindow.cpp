/*
 * ofAppEmscriptenWindow.cpp
 *
 *  Created on: May 8, 2014
 *      Author: arturo
 * ofxAppEmscriptenWindow.cpp
 * Reworked to use emscripten-glfw (contrib.glfw3) - April 2026
 */


#include "ofxAppEmscriptenWindow.h"
#include "ofLog.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

using namespace std;

ofxAppEmscriptenWindow * ofxAppEmscriptenWindow::instance = nullptr;

void consoleErrorHandler(int iErrorCode, char const *iErrorMessage)
{
  printf("glfwError: %d | %s\n", iErrorCode, iErrorMessage);
}

EM_JS(void, forceDarkTheme, (), {
    document.body.style.setProperty('background', 'rgb(20,20,20)', 'important');
    document.getElementById('canvas-container').style.setProperty('background', '#111', 'important');
    // you can also style #output, canvas, etc. here
});



//------------------------------------------------------------
ofxAppEmscriptenWindow::ofxAppEmscriptenWindow() {
	instance = this;
}

//------------------------------------------------------------
ofxAppEmscriptenWindow::~ofxAppEmscriptenWindow() {
	if (glfwWindow) {
		glfwDestroyWindow(glfwWindow);
		glfwTerminate();
	}
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setup(const ofGLESWindowSettings & settings) {
	if (!glfwInit()) {
		ofLogFatalError("ofxAppEmscriptenWindow") << "glfwInit() failed";
		return;
	}

	glfwSetErrorCallback(consoleErrorHandler);

	// which html canvas to use
	emscripten::glfw3::SetNextWindowCanvasSelector("#canvas");

	// Hi-DPI support
	glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ES_API);

	glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE);

	glfwWindow = glfwCreateWindow(settings.getWidth(), settings.getHeight(),
	                              "openFrameworks", nullptr, nullptr);

	if (!glfwWindow) {
		ofLogFatalError("ofxAppEmscriptenWindow") << "glfwCreateWindow() failed";
		return;
	}

	glfwMakeContextCurrent(glfwWindow);

	_renderer = std::make_shared<ofGLProgrammableRenderer>(this);
	((ofGLProgrammableRenderer*)_renderer.get())->setup(2, 0);

	// Register GLFW callbacks
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
	glfwSetCursorPosCallback(glfwWindow, cursorPositionCallback);
	glfwSetScrollCallback(glfwWindow, scrollCallback);
	glfwSetFramebufferSizeCallback(glfwWindow, resizeCallback);
	glfwSetCursorEnterCallback(glfwWindow, cursorEnterCallback);

	emscripten::glfw3::MakeCanvasResizable(window, "#canvas-container");

	mCurrentWindowMode = settings.windowMode;
	mTargetWindowMode = settings.windowMode;

	bIsSetup = true;
	ofLogNotice("ofxAppEmscriptenWindow") << "Setup complete using emscripten-glfw";

	emscripten_set_main_loop_arg(main_loop, glfwWindow, 0, GLFW_FALSE);

}

void main_loop(void *user_data)
{
  auto window = reinterpret_cast<GLFWwindow *>(user_data);
  if(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    display_cb();
  }
  else
  {
    // done => terminating
    glfwTerminate();
    emscripten_cancel_main_loop();
  }
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::update() {
	glfwPollEvents();

	events().notifyUpdate();
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::draw() {
	renderer()->startRender();
	if (bEnableSetupScreen) renderer()->setupScreen();

	events().notifyDraw();

	renderer()->finishRender();

	glfwSwapBuffers(glfwWindow);
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::display_cb() {
	if (instance) {
		instance->update();
		instance->draw();
	}
}

void ofxAppEmscriptenWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		instance->events().notifyKeyPressed(key);
	} else if (action == GLFW_RELEASE) {
		instance->events().notifyKeyReleased(key);
	}
}

void ofxAppEmscriptenWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (action == GLFW_PRESS) {
		instance->events().notifyMousePressed(xpos, ypos, button);
	} else if (action == GLFW_RELEASE) {
		instance->events().notifyMouseReleased(xpos, ypos, button);
	}
}

void ofxAppEmscriptenWindow::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		instance->events().notifyMouseDragged(xpos, ypos, 0);
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		instance->events().notifyMouseDragged(xpos, ypos, 1);
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		instance->events().notifyMouseDragged(xpos, ypos, 2);
	} else {
		instance->events().notifyMouseMoved(xpos, ypos);
	}
}

void ofxAppEmscriptenWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	instance->events().notifyMouseScrolled(ofGetMouseX(), ofGetMouseY(), xoffset, yoffset);
}

void ofxAppEmscriptenWindow::resizeCallback(GLFWwindow* window, int width, int height) {
	instance->mCachedWidth = width;
	instance->mCachedHeight = height;
	instance->events().notifyWindowResized(width, height);
}

void ofxAppEmscriptenWindow::cursorEnterCallback(GLFWwindow* window, int entered) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (entered) {
		instance->events().notifyMouseEntered(xpos, ypos);
	} else {
		instance->events().notifyMouseExited(xpos, ypos);
	}
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::hideCursor() {
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void ofxAppEmscriptenWindow::showCursor() {
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void ofxAppEmscriptenWindow::setWindowShape(int w, int h) {
	glfwSetWindowSize(glfwWindow, w, h);
}

glm::vec2 ofxAppEmscriptenWindow::getWindowPosition() {
	return glm::vec2(0, 0);
}

glm::vec2 ofxAppEmscriptenWindow::getWindowSize() {
	int w, h;
	glfwGetFramebufferSize(glfwWindow, &w, &h);
	mCachedWidth = w; mCachedHeight = h;
	return glm::vec2(w, h);
}

glm::vec2 ofxAppEmscriptenWindow::getScreenSize() {
	return getWindowSize();
}

ofOrientation ofxAppEmscriptenWindow::getOrientation() {
	return OF_ORIENTATION_DEFAULT;
}

bool ofxAppEmscriptenWindow::doesHWOrientation() {
	return false;
}

int ofxAppEmscriptenWindow::getWidth()  {
	return getWindowSize().x;
}

int ofxAppEmscriptenWindow::getHeight() {
	return getWindowSize().y;
}

ofWindowMode ofxAppEmscriptenWindow::getWindowMode() {
	return mCurrentWindowMode;
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::loop() {
	instance->events().notifySetup();
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::setFullscreen(bool fullscreen) {
	if (fullscreen) {
		// GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		// const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		// glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		emscripten::glfw3::RequestFullscreen(window, false, true);
		mCurrentWindowMode = OF_FULLSCREEN;
	} else {
		glfwSetWindowMonitor(glfwWindow, nullptr, 0, 0, mCachedWidth, mCachedHeight, 0);
		if (mTargetWindowMode == OF_GAME_MODE || mTargetWindowMode == OF_FULLSCREEN) {
			mCurrentWindowMode = OF_GAME_MODE;
		} else {
			mCurrentWindowMode = OF_WINDOW;
		}
	}
}

//------------------------------------------------------------
void ofxAppEmscriptenWindow::toggleFullscreen() {
	if (glfwGetWindowMonitor(glfwWindow) != nullptr) {
		setFullscreen(false);
	} else {
		setFullscreen(true);
	}
}

void ofxAppEmscriptenWindow::enableSetupScreen()  {
	bEnableSetupScreen = true;
}

void ofxAppEmscriptenWindow::disableSetupScreen() {
	bEnableSetupScreen = false;
}

void ofxAppEmscriptenWindow::setVerticalSync(bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
}

void ofxAppEmscriptenWindow::setTouchSimulatesMouse(bool abEnable) {
	mBTouchSimulatesMouse = abEnable;
}

void ofxAppEmscriptenWindow::preventDefaultBrowserTouchMoveBehavior() {
	// emscripten-glfw already handles this cleanly
}

void ofxAppEmscriptenWindow::setFullscreenScaleStrategy(int aStrategy) {
	mFullscreenScaleStrategy = aStrategy; // not needed with new port
}

ofCoreEvents & ofxAppEmscriptenWindow::events() {
	return _events;
}

std::shared_ptr<ofBaseRenderer> & ofxAppEmscriptenWindow::renderer() {
	return _renderer;
}

void ofxAppEmscriptenWindow::makeCurrent() {
	glfwMakeContextCurrent(glfwWindow);
}

void ofxAppEmscriptenWindow::startRender() {
	renderer()->startRender();
}

void ofxAppEmscriptenWindow::finishRender() {
	renderer()->finishRender();
}
