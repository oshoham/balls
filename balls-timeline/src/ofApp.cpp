#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255 * 0.15);
	ofSetFrameRate(60);

	// setup OSC

	oscSender.setup("127.0.0.1", MAX_MSP_OSC_PORT);
	oscReceiver.setup(QLAB_OSC_PORT);

	// setup ofxTimeline

	ofxTimeline::removeCocoaMenusFromGlut("Balls Timeline");
	timeline.setup();
//    timeline.setFrameRate(60);
//    timeline.setFrameBased(true);
	timeline.setLoopType(OF_LOOP_NONE);
    timeline.addAudioTrack("Audio", "_BALLS_script_mix3.1.mp3");
    timeline.setTimecontrolTrack("Audio");
    timeline.setDurationInSeconds(timeline.getAudioTrack("Audio")->getDuration());
    timeline.addCurves("Fade", ofRange(0.0, 1.0));
	timeline.addCurves("Left Ball Position", ofRange(0.0, 1.0));
	timeline.addCurves("Right Ball Position", ofRange(0.0, 1.0));
	ofAddListener(timeline.events().bangFired, this, &ofApp::receivedBang);
    
#if LOCKED_AND_MUTED
    ofxTLAudioTrack* track = timeline.getAudioTrack("Audio");
    track->setVolume(0);
    timeline.setSpacebarTogglePlay(false);
    timeline.disableEvents();
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage message;
		oscReceiver.getNextMessage(message);
		string address = message.getAddress();

		if (address == "/start_show" && !timeline.getIsPlaying()) {
            timeline.setCurrentTimeSeconds(0);
			timeline.play();
		}
		else if (address == "/end_show") {
			endShow();
		}
		else {
			printOscMessage(message);
		}
	}

	if (timeline.getIsPlaying()) {
        ofxOscMessage fadeMessage;
        fadeMessage.setAddress("/fade");
        fadeMessage.addFloatArg(timeline.getValue("Fade"));
        oscSender.sendMessage(fadeMessage, false);
        
		ofxOscMessage leftBallPositionMessage;
		leftBallPositionMessage.setAddress("/left_ball/position");
		leftBallPositionMessage.addFloatArg(timeline.getValue("Left Ball Position"));
		oscSender.sendMessage(leftBallPositionMessage, false);

		ofxOscMessage rightBallPositionMessage;
		rightBallPositionMessage.setAddress("/right_ball/position");
		rightBallPositionMessage.addFloatArg(timeline.getValue("Right Ball Position"));
		oscSender.sendMessage(rightBallPositionMessage, false);
        
        ofxTLAudioTrack* track = timeline.getAudioTrack("Audio");
        ofxOscMessage audioFFTMessage;
        audioFFTMessage.setAddress("/fft");        vector<float> fft = track->getFFT();
        for (int i = 0; i < track->getFFTSize(); i++) {
            audioFFTMessage.addFloatArg(fft[i]);
        }
        oscSender.sendMessage(audioFFTMessage, false);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	timeline.draw();
}

//--------------------------------------------------------------
void ofApp::receivedBang(ofxTLBangEventArgs& bang) {
	// cout << "Bang with flag " + bang.flag + " fired from track " + bang.track->getName() + ", current seconds: " + ofToString(bang.currentTime) << endl;
	string trackName = bang.track->getName();
	ofxOscMessage message;
	message.setAddress("/" + trackName);
	vector<string> args = ofSplitString(bang.flag, " ");
	if (trackName == "billboard") {
	}
	else if (trackName == "loading") {
	}
	else {
	}
	oscSender.sendMessage(message, false);
}

//--------------------------------------------------------------
void ofApp::startShow() {
    
}

//--------------------------------------------------------------
void ofApp::endShow() {
	ofxOscMessage message;
	message.setAddress("/end_show");
	oscSender.sendMessage(message, false);
}

//--------------------------------------------------------------
void ofApp::printOscMessage(ofxOscMessage message) {
	string msgString = message.getAddress();
	msgString += ":";
	for (size_t i = 0; i < message.getNumArgs(); i++) {
		msgString += " ";
		msgString += message.getArgTypeName(i);
		msgString += ":";

		// display the argument - make sure we get the right type
		if (message.getArgType(i) == OFXOSC_TYPE_INT32) {
			msgString += ofToString(message.getArgAsInt32(i));
		}
		else if (message.getArgType(i) == OFXOSC_TYPE_FLOAT) {
			msgString += ofToString(message.getArgAsFloat(i));
		}
		else if (message.getArgType(i) == OFXOSC_TYPE_STRING) {
			msgString += message.getArgAsString(i);
		}
		else {
			msgString += "unhandled argument type " + message.getArgTypeName(i);
		}
	}
	cout << msgString << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
