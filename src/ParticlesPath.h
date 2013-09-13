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
//  ParticlesPath.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#ifndef ituita_particles_ParticlesPath_h
#define ituita_particles_ParticlesPath_h

#include "ofMain.h"

class ParticlesPath {
    
    public:
        ParticlesPath(float, ofVec2f, ofVec2f);
    
        void display();
    
        // a path is a line between two points
        ofVec2f start;
        ofVec2f end;
        // the radius defines how far is it ok for the boid to wander off
        float radius;    
    
};

#endif