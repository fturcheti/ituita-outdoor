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
//  Particle.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#ifndef ituita_particles_Particle_h
#define ituita_particles_Particle_h

#include "ofMain.h"
#include "ParticlesPath.h"

class Particle {

    public:
        
        Particle(ParticlesPath&, float, float, ofColor);
        void run();
        void update();
        void applyForce(ofVec2f);
        void borders();
        void display();
        void follow(ParticlesPath);
        void seek(ofVec2f);
        float initLocX();
        ofVec2f getNormalPoint(ofVec2f, ofVec2f, ofVec2f);
    
        
        ParticlesPath* path;
        float          widthLimit;
        float          heightLimit;
    
        ofVec2f location;
        ofVec2f velocity;
        ofVec2f acceleration;
    
        float r;
        float maxForce;
        float maxSpeed;
        float originalMaxSpeed;
    
        ofColor originalColor;
        ofColor highlightColor;
    
};

#endif