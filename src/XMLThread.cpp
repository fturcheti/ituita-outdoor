//
//  XMLThread.cpp
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 8/15/12.
//

#include "XMLThread.h"

XMLThread::XMLThread() {
    loaded = false;    
}

void XMLThread::threadedFunction() {
    while(isThreadRunning()) {
        lock();
        
        ofLogVerbose() << "Loading XML... ";
        ofHttpResponse response = ofLoadURL(LOADING_URL);
        sXML = response.data;
        ofLogVerbose() << "loaded." << endl;
        ofLogVerbose() << sXML << endl;
        
        unlock();
        loaded = true;

        ofSleepMillis(LOADING_INTERVAL);
    }
}

string XMLThread::getXML() {
    return sXML;
}

bool XMLThread::isAvailable() {
    return loaded;
}