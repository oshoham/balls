#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTimeline.h"
#include "ofxTLAudioTrack.h"
#include "ofxBLEHeartRate.h"

#define TARGET_FPS 60
#define LOCKED_AND_MUTED false
#define QLAB_OSC_PORT 6000
#define MAX_MSP_OSC_PORT 9000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(ofKeyEventArgs & keyArgs);
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
        void receivedSwitchEvent(ofxTLSwitchEventArgs& switchEvent);
		void printOscMessage(ofxOscMessage message);
    
        void onScanEvent(ofxBLEHeartRateEventArgs& args);
        void onHRMEvent(ofxBLEHeartRateEventArgs& args);
        void onConnectEvent(ofxBLEHeartRateEventArgs& args);
        void onDisconnectEvent(ofxBLEHeartRateEventArgs& args);

		ofxTimeline timeline;
		ofxOscSender oscSender;
		ofxOscReceiver oscReceiver;
    
        ofxBLEHeartRate bleHeartRate1;
        ofxBLEHeartRate bleHeartRate2;
        string bleDeviceId1;
        string bleDeviceId2;
};
