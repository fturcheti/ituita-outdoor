
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