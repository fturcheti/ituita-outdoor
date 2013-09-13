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
//  ParticlesPath.cpp
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#include "ParticlesPath.h"

ParticlesPath::ParticlesPath(float radius, ofVec2f start, ofVec2f end) {
    this->radius = radius;
    this->start.set(start);
    this->end.set(end);
}

void ParticlesPath::display() {
    // draw radius
    ofSetLineWidth(radius*2);
    ofSetColor(255, 20);
    ofLine(start.x, start.y, end.x, end.y);
    
    // draw path line
    ofSetLineWidth(1);
    ofSetColor(255);
    ofLine(start.x, start.y, end.x, end.y);
}
