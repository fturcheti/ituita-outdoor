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
        // LOCK the thread
        lock();
        
        ofLogVerbose() << "Loading XML... ";
        
        // CHECK from which url the data should be loaded
        string urlToLoad;
        if(activeURL == TEST_URL) {
            urlToLoad = testURL;
        } else {
            urlToLoad = finalURL;
        }
        ofLogVerbose() << "from " << urlToLoad << "...";

        // LOAD the data
        ofHttpResponse response = ofLoadURL(urlToLoad);
        sXML = response.data;
        ofLogVerbose() << "XML loaded.";
        ofLogVerbose() << sXML;
        
        // UNLOCK the thread
        unlock();
        loaded = true;

        // SET the loading interval
        if(loadingInterval == 0) loadingInterval = LOADING_INTERVAL;
        ofSleepMillis(loadingInterval);
        ofLogVerbose() << "Loading interval: " << loadingInterval << " miliseconds";
    }
}

string XMLThread::getXML() {
    return sXML;
}

bool XMLThread::isAvailable() {
    return loaded;
}

void XMLThread::setLoadingInterval(int _loadingInterval) {
    loadingInterval = _loadingInterval;
}

void XMLThread::setTestURL(string _testURL) {
    testURL = _testURL;
}

void XMLThread::setFinalURL(string _finalURL) {
    finalURL = _finalURL;
}

void XMLThread::setActiveURL(int _activeURL) {
    activeURL = _activeURL;
}
