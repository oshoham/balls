#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255 * 0.15);
	ofSetFrameRate(60);

	// setup OSC

	oscSender.setup("127.0.0.1", MAX_MSP_OSC_PORT);
	oscReceiver.setup(QLAB_OSC_PORT);

	// setup ofxTimeline

	ofxTimeline::removeCocoaMenusFromGlut("Empty Templates");
	timeline.setup();
	timeline.setFrameRate(60);
	timeline.setFrameBased(false);
	ofAddListener(timeline.events().bangFired, this, &ofApp::receivedBang);

	startShow();
}

//--------------------------------------------------------------
void ofApp::update(){
	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage message;
		oscReceiver.getNextMessage(message);
		string address = message.getAddress();

		if (address == "/start_show") {
			startShow();
		}
		else if (address == "/end_show") {
			endShow();
		}
		else {
			printOscMessage(message);
		}
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
	ofxOscMessage message;
	message.setAddress("/start_show");
	oscSender.sendMessage(message, false);
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
