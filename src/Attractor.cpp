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
//  Attractor.cpp
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#include "Attractor.h"

Attractor::Attractor() {
    location.set(0, 0);
    intensity = 0.0;
    
    bornTime = time(0);
    lifeTime = 0.5;
}

Attractor::Attractor(ofVec2f location) {
    this->location = location;
    
    bornTime = time(0);
    lifeTime = 0.5;
}

Attractor::Attractor(ofVec2f location, float lifetime) {
    this->location = location;
    this->lifeTime = lifetime;
    
    bornTime = time(0);
}

void Attractor::setLocation(ofVec2f location) {
    this->location = location;
}
