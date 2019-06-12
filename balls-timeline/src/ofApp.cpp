#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255 * 0.15);
    ofSetFrameRate(TARGET_FPS);
    ofSetEscapeQuitsApp(false);

	// setup OSC

	oscSender.setup("127.0.0.1", MAX_MSP_OSC_PORT);
	oscReceiver.setup(QLAB_OSC_PORT);

	// setup ofxTimeline

	ofxTimeline::removeCocoaMenusFromGlut("Balls Timeline");
	timeline.setup();
    timeline.setFrameRate(TARGET_FPS);
//    timeline.setFrameBased(true);
	timeline.setLoopType(OF_LOOP_NONE);
    timeline.addAudioTrack("Audio", "MixDown.mp3");
    timeline.setTimecontrolTrack("Audio");
    ofxTLAudioTrack* audioTrack = timeline.getAudioTrack("Audio");
    timeline.setDurationInSeconds(audioTrack->getDuration());
    
    timeline.setPageName("Master");
    timeline.addCurves("Master Fade", ofRange(0.0, 1.0));
    timeline.addCurves("Balls Fade", ofRange(0.0, 1.0));
    timeline.addCurves("Stars Fade", ofRange(0.0, 1.0));
    timeline.addCurves("Chaos Fade", ofRange(0.0, 1.0));
    
    timeline.addPage("Balls");
    timeline.getPage("Balls")->addTrack("Audio", audioTrack);
    timeline.addLFO("Left Ball Position", ofRange(0.0, 1.0), 0.0);
    timeline.addLFO("Right Ball Position", ofRange(0.0, 1.0), 0.0);
    timeline.addCurves("Left Ball Jitter", ofRange(0.0, 0.2));
    timeline.addCurves("Right Ball Jitter", ofRange(0.0, 0.2));
    timeline.addCurves("Left Ball Size", ofRange(0.0, 1.0));
    timeline.addCurves("Right Ball Size", ofRange(0.0, 1.0));
    
    timeline.addPage("Stars");
    timeline.getPage("Stars")->addTrack("Audio", audioTrack);
    timeline.addSwitches("Twinkle");
    
    timeline.addPage("Chaos");
    timeline.getPage("Chaos")->addTrack("Audio", audioTrack);
    timeline.addSwitches("Lots of Balls");
    
    timeline.setCurrentPage(0);
    
	ofAddListener(timeline.events().bangFired, this, &ofApp::receivedBang);
    ofAddListener(timeline.events().switched, this, &ofApp::receivedSwitchEvent);
    
#if LOCKED_AND_MUTED
    ofxTLAudioTrack* track = timeline.getAudioTrack("Audio");
    track->setVolume(0);
    timeline.setSpacebarTogglePlay(false);
    timeline.disableEvents();
#endif
    
    timeline.enableSnapToOtherKeyframes(false);
    timeline.enableSnapToBPM(false);
    timeline.moveToThread();
    
    bleDeviceId1 = "";
    bleDeviceId2 = "";
    bleHeartRate1.setup();
    bleHeartRate2.setup();
    ofAddListener(bleHeartRate1.hrmEvent, this, &ofApp::onHRMEvent);
    ofAddListener(bleHeartRate1.scanEvent, this, &ofApp::onScanEvent);
    ofAddListener(bleHeartRate1.connectEvent, this, &ofApp::onConnectEvent);
    ofAddListener(bleHeartRate1.disconnectEvent, this, &ofApp::onDisconnectEvent);
    ofAddListener(bleHeartRate2.hrmEvent, this, &ofApp::onHRMEvent);
    ofAddListener(bleHeartRate2.scanEvent, this, &ofApp::onScanEvent);
    ofAddListener(bleHeartRate2.connectEvent, this, &ofApp::onConnectEvent);
    ofAddListener(bleHeartRate2.disconnectEvent, this, &ofApp::onDisconnectEvent);
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
		else {
			printOscMessage(message);
		}
	}

	if (timeline.getIsPlaying()) {
        ofxOscMessage fadeMessage;
        fadeMessage.setAddress("/fade");
        fadeMessage.addFloatArg(timeline.getValue("Master Fade"));
        oscSender.sendMessage(fadeMessage, false);
        
        ofxOscMessage starsFadeMessage;
        starsFadeMessage.setAddress("/stars/fade");
        starsFadeMessage.addFloatArg(timeline.getValue("Stars Fade"));
        oscSender.sendMessage(starsFadeMessage, false);
        
        ofxOscMessage ballsFadeMessage;
        ballsFadeMessage.setAddress("/balls/fade");
        ballsFadeMessage.addFloatArg(timeline.getValue("Balls Fade"));
        oscSender.sendMessage(ballsFadeMessage, false);
        
        ofxOscMessage chaosFadeMessage;
        chaosFadeMessage.setAddress("/lots_of_balls/fade");
        chaosFadeMessage.addFloatArg(timeline.getValue("Chaos Fade"));
        oscSender.sendMessage(chaosFadeMessage, false);
        
        ofxOscMessage leftBallPositionMessage;
        leftBallPositionMessage.setAddress("/left_ball/position");
        leftBallPositionMessage.addFloatArg(timeline.getValue("Left Ball Position"));
        oscSender.sendMessage(leftBallPositionMessage, false);
        
        ofxOscMessage rightBallPositionMessage;
        rightBallPositionMessage.setAddress("/right_ball/position");
        rightBallPositionMessage.addFloatArg(timeline.getValue("Right Ball Position"));
        oscSender.sendMessage(rightBallPositionMessage, false);
        
        ofxOscMessage leftBallJitterMessage;
        leftBallJitterMessage.setAddress("/left_ball/jitter");
        leftBallJitterMessage.addFloatArg(timeline.getValue("Left Ball Jitter"));
        oscSender.sendMessage(leftBallJitterMessage, false);
        
        ofxOscMessage rightBallJitterMessage;
        rightBallJitterMessage.setAddress("/right_ball/jitter");
        rightBallJitterMessage.addFloatArg(timeline.getValue("Right Ball Jitter"));
        oscSender.sendMessage(rightBallJitterMessage, false);
        
        ofxOscMessage leftBallSizeMessage;
        leftBallSizeMessage.setAddress("/left_ball/size");
        leftBallSizeMessage.addFloatArg(timeline.getValue("Left Ball Size"));
        oscSender.sendMessage(leftBallSizeMessage, false);
        
        ofxOscMessage rightBallSizeMessage;
        rightBallSizeMessage.setAddress("/right_ball/size");
        rightBallSizeMessage.addFloatArg(timeline.getValue("Right Ball Size"));
        oscSender.sendMessage(rightBallSizeMessage, false);
        
        ofxTLAudioTrack* track = timeline.getAudioTrack("Audio");
        ofxOscMessage audioFFTMessage;
        audioFFTMessage.setAddress("/fft");
        vector<float> fft = track->getFFT();
        for (int i = 0; i < track->getFFTSize(); i++) {
            audioFFTMessage.addFloatArg(fft[i]);
        }
        oscSender.sendMessage(audioFFTMessage, false);
        
        ofxOscMessage rendererMessage;
        rendererMessage.setAddress("/enable_renderer");
        rendererMessage.addIntArg(1);
        oscSender.sendMessage(rendererMessage, false);
	}
    
//    cout << ofGetFrameRate() << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	timeline.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
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
void ofApp::receivedSwitchEvent(ofxTLSwitchEventArgs& switchEvent) {
    if (switchEvent.track->getName() == "Twinkle") {
        ofxOscMessage twinkleMessage;
        twinkleMessage.setAddress("/stars/enable_twinkle");
        twinkleMessage.addBoolArg(switchEvent.on);
        oscSender.sendMessage(twinkleMessage, false);
    } else if (switchEvent.track->getName() == "Lots of Balls") {
        ofxOscMessage lotsOfBallsMessage;
        lotsOfBallsMessage.setAddress("/lots_of_balls/enable");
        lotsOfBallsMessage.addBoolArg(switchEvent.on);
        oscSender.sendMessage(lotsOfBallsMessage, false);
    }
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
void ofApp::keyPressed(ofKeyEventArgs & keyArgs){
    if (keyArgs.key == 't' && keyArgs.hasModifier(OF_KEY_COMMAND)) {
        timeline.enableSnapToOtherKeyframes(!timeline.getSnapToOtherElements());
    } else if (keyArgs.key == OF_KEY_RIGHT && keyArgs.hasModifier(OF_KEY_COMMAND)) {
        timeline.setCurrentPage(ofWrap(timeline.getCurrentPageIndex() + 1, 0, timeline.getPages().size()));
    } else if (keyArgs.key == OF_KEY_LEFT && keyArgs.hasModifier(OF_KEY_COMMAND)) {
        timeline.setCurrentPage(ofWrap(timeline.getCurrentPageIndex() - 1, 0, timeline.getPages().size()));
    }
}

//--------------------------------------------------------------
void ofApp::onHRMEvent(ofxBLEHeartRateEventArgs& args) {
    if (args.peripheralId != bleDeviceId1 && args.peripheralId != bleDeviceId2) {
        return;
    }
    
    ofxOscMessage msg;
    if (args.peripheralId == bleDeviceId1) {
        msg.setAddress("/heartrate1");
    } else {
        msg.setAddress("/heartrate2");
    }
    msg.addIntArg(args.heartRate);
    oscSender.sendMessage(msg, false);
}

//--------------------------------------------------------------
void ofApp::onScanEvent(ofxBLEHeartRateEventArgs& args) {
    if (args.peripheralId != bleDeviceId1 && args.peripheralId != bleDeviceId2) {
        return;
    }
    
    cout << "Connecting to device ID " + args.peripheralId + "..." << endl;
    if (args.peripheralId == bleDeviceId1) {
        bleHeartRate1.connectDevice(args.peripheralId);
    } else {
        bleHeartRate2.connectDevice(args.peripheralId);
    }
}

//--------------------------------------------------------------
void ofApp::onConnectEvent(ofxBLEHeartRateEventArgs& args) {
    cout << "Sucessfully connected to device ID " + args.peripheralId + "." << endl;
}

void ofApp::onDisconnectEvent(ofxBLEHeartRateEventArgs& args) {
    cout << "Disconnected device ID " + args.peripheralId + "." << endl;
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
