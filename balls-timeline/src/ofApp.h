#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"

#define LOCKED_AND_MUTED false
#define QLAB_OSC_PORT 53001
#define MAX_MSP_OSC_PORT 9000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void receivedBang(ofxTLBangEventArgs& bang);
		void printOscMessage(ofxOscMessage message);
		void startShow();
		void endShow();

		ofxTimeline timeline;
		ofxOscSender oscSender;
		ofxOscReceiver oscReceiver;
};
