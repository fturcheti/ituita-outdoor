
#ifndef ituita_particles_Particle_h
#define ituita_particles_Particle_h

#include "ofMain.h"
#include "ParticlesPath.h"

class Particle {

    public:
        
        Particle(ParticlesPath&, float, float, int);
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
    
        int color;
        vector<int> colors;
    
};

#endif