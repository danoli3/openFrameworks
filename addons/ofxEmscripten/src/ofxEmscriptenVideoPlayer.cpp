/*
 * ofEmscriptenVideoPlayer.cpp
 *
 *  Created on: May 10, 2014
 *      Author: arturo
 */

#include "ofxEmscriptenVideoPlayer.h"
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
 * ofEmscriptenVideoPlayer.cpp
 * Updated April 2026: html5video library calls DEFINED OUT (deprecated)
 * → will be replaced with modern WebCodecs + MediaSource in CMake port
 */

#include "ofxEmscriptenVideoPlayer.h"
#include "ofLog.h"

ofxEmscriptenVideoPlayer::ofxEmscriptenVideoPlayer() {
	ofLogWarning("ofxEmscriptenVideoPlayer") << "html5video library is deprecated and has been defined out.";
	player_id = -1;
}

ofxEmscriptenVideoPlayer::~ofxEmscriptenVideoPlayer() {
	// no-op
}

bool ofxEmscriptenVideoPlayer::load(const of::filesystem::path & fileName) {
	ofLogWarning("ofxEmscriptenVideoPlayer") << "load() - html5video stubbed out";
	return false;
}

void ofxEmscriptenVideoPlayer::close() { /* no-op */ }
void ofxEmscriptenVideoPlayer::update() { /* no-op */ }

void ofxEmscriptenVideoPlayer::play() { ofLogWarning("ofxEmscriptenVideoPlayer") << "play() stubbed"; }
void ofxEmscriptenVideoPlayer::stop() { /* no-op */ }

bool ofxEmscriptenVideoPlayer::isFrameNew() const { return false; }
ofPixels & ofxEmscriptenVideoPlayer::getPixels() { return pixels; }
const ofPixels & ofxEmscriptenVideoPlayer::getPixels() const { return pixels; }
ofTexture * ofxEmscriptenVideoPlayer::getTexture() { return &texture; }

float ofxEmscriptenVideoPlayer::getWidth() const { return 0; }
float ofxEmscriptenVideoPlayer::getHeight() const { return 0; }

bool ofxEmscriptenVideoPlayer::isPaused() const { return true; }
bool ofxEmscriptenVideoPlayer::isLoaded() const { return false; }
bool ofxEmscriptenVideoPlayer::isPlaying() const { return false; }

bool ofxEmscriptenVideoPlayer::setPixelFormat(ofPixelFormat) { return false; }
ofPixelFormat ofxEmscriptenVideoPlayer::getPixelFormat() const { return OF_PIXELS_UNKNOWN; }

float ofxEmscriptenVideoPlayer::getPosition() const { return 0; }
float ofxEmscriptenVideoPlayer::getSpeed() const { return 1.0f; }
float ofxEmscriptenVideoPlayer::getDuration() const { return 0; }
bool ofxEmscriptenVideoPlayer::getIsMovieDone() const { return true; }
float ofxEmscriptenVideoPlayer::getPan() const { return 0; }

void ofxEmscriptenVideoPlayer::setPaused(bool) {}
void ofxEmscriptenVideoPlayer::setPosition(float) {}
void ofxEmscriptenVideoPlayer::setVolume(float) {}
void ofxEmscriptenVideoPlayer::setLoopState(ofLoopType) {}
void ofxEmscriptenVideoPlayer::setSpeed(float) {}
void ofxEmscriptenVideoPlayer::setFrame(int) {}
void ofxEmscriptenVideoPlayer::setPan(float) {}

int ofxEmscriptenVideoPlayer::getCurrentFrame() const { return 0; }
int ofxEmscriptenVideoPlayer::getTotalNumFrames() const { return 0; }
ofLoopType ofxEmscriptenVideoPlayer::getLoopState() const { return OF_LOOP_NONE; }

void ofxEmscriptenVideoPlayer::firstFrame() {}
void ofxEmscriptenVideoPlayer::nextFrame() {}
void ofxEmscriptenVideoPlayer::previousFrame() {}

void ofxEmscriptenVideoPlayer::setUsePixels(bool use) { usePixels = use; }
