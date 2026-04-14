/*
 * ofEmscriptenGrabber.cpp
 *
 *  Created on: May 12, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenVideoGrabber.h"
#include "html5video.h"

using namespace std;

enum ReadyState{
	HAVE_NOTHING = 0,
	HAVE_METADATA,
	HAVE_CURRENT_DATA,
	HAVE_FUTURE_DATA,
	HAVE_ENOUGH_DATA
};

/*
 * ofEmscriptenVideoGrabber.cpp
 * Updated April 2026: html5video library calls DEFINED OUT (deprecated)
 */

#include "ofxEmscriptenVideoGrabber.h"
#include "ofLog.h"

ofxEmscriptenVideoGrabber::ofxEmscriptenVideoGrabber() {
	ofLogWarning("ofxEmscriptenVideoGrabber") << "html5video grabber library is deprecated and has been defined out.";
	id = -1;
	desiredFramerate = -1;
	usePixels = true;
}

ofxEmscriptenVideoGrabber::~ofxEmscriptenVideoGrabber() { /* no-op */ }

vector<ofVideoDevice> ofxEmscriptenVideoGrabber::listDevices() const {
	ofLogWarning("ofxEmscriptenVideoGrabber") << "listDevices() stubbed";
	return {};
}

bool ofxEmscriptenVideoGrabber::setup(int w, int h) {
	ofLogWarning("ofxEmscriptenVideoGrabber") << "setup() stubbed";
	return false;
}

bool ofxEmscriptenVideoGrabber::isInitialized() const { return false; }
void ofxEmscriptenVideoGrabber::update() { /* no-op */ }
bool ofxEmscriptenVideoGrabber::isFrameNew() const { return false; }

ofPixels & ofxEmscriptenVideoGrabber::getPixels() { return pixels; }
const ofPixels & ofxEmscriptenVideoGrabber::getPixels() const { return pixels; }
void ofxEmscriptenVideoGrabber::close() {}

float ofxEmscriptenVideoGrabber::getHeight() const { return 0; }
float ofxEmscriptenVideoGrabber::getWidth() const { return 0; }

bool ofxEmscriptenVideoGrabber::setPixelFormat(ofPixelFormat) { return false; }
ofPixelFormat ofxEmscriptenVideoGrabber::getPixelFormat() const { return OF_PIXELS_UNKNOWN; }
ofTexture * ofxEmscriptenVideoGrabber::getTexture() { return &texture; }

void ofxEmscriptenVideoGrabber::setDeviceID(int) {}
void ofxEmscriptenVideoGrabber::setDesiredFrameRate(int fr) { desiredFramerate = fr; }
void ofxEmscriptenVideoGrabber::videoSettings() {}
void ofxEmscriptenVideoGrabber::setUsePixels(bool use) { usePixels = use; }
