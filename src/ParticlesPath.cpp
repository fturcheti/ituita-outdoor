
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
