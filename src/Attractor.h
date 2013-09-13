////////////////////////////////////////////////////////////
//                                                        //
// This code is part of "ituita-outdoor", an interactive  //
// software developed by André Mintz and Felipe Turcheti, //
// in 2012-2013, for the Ituita Project.                  //
// http://ituita.com.br                                   //
//                                                        //
// It was built with, and depends on, open-source tools:  //
// - openFrameworks v007                                  //
// - ofxBlob                                              //
// - ofxCountourFinder                                    //
// - ofxKinect                                            //
// - ofxMSAInteractiveObject                              //
// - ofxOpenCv                                            //
// - ofxSimpleGuiToo                                      //
// - ofxXmlSettings                                       //
//                                                        //
////////////////////////////////////////////////////////////

//
//  Attractor.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#include "ofMain.h"

class Attractor {

    public:
        Attractor();
        Attractor(ofVec2f);
        Attractor(ofVec2f, float);
    
        void setLocation(ofVec2f);
        
        ofVec2f location;
        float intensity;
        int bornTime;
        int lifeTime;
    
};
