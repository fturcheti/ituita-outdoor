//
//  XMLThread.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 8/15/12.
//

#ifndef ituita_outdoor_xmlthread_h
#define ituita_outdoor_xmlthread_h

#include "ofMain.h"
#include "ofThread.h"

#define LOADING_INTERVAL 3000

#define FINAL_URL 0
#define TEST_URL 1

class XMLThread : public ofThread {
    public:
        XMLThread();
        void threadedFunction();
        bool isAvailable();
        string getXML();
    
        void setLoadingInterval(int);
        void setTestURL(string);
        void setFinalURL(string);
        void setActiveURL(int);
    
    private:
        string sXML;
        bool loaded;
        int loadingInterval;
    
        int activeURL;
        string testURL;
        string finalURL;
};

#endif