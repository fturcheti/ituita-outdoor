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
#define LOADING_URL "http://fronte.co/dev/ituita/resultados.php"

class XMLThread : public ofThread {
    public:
        XMLThread();
        void threadedFunction();
        bool isAvailable();
        string getXML();
    
    private:
        string sXML;
        bool loaded;
};

#endif