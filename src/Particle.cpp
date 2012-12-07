
#include "Particle.h"

Particle::Particle(ParticlesPath &path, float widthLimit, float heightLimit, int color) {
    this->path = &path;
    this->widthLimit = widthLimit;
    this->heightLimit = heightLimit;
    this->color = color;
    
    location.set( initLocX(), ofRandom(0, heightLimit) );
    r = ofRandom(2, 4);
    maxForce = ofRandom(0.05, 0.2);
    maxSpeed = ofRandom(2, 5);
    acceleration.set(0, 0);
    velocity.set(1, 0);    
}

// set a random location.x on particle height reset
float Particle::initLocX() {
    int dir = (ofRandom(0, 1) > 0.5) ? 1 : -1;
    return path->start.x + dir*ofRandom(2, 10) * (widthLimit/20);
}

void Particle::run() {
    update();
    borders();
    display();
}

// update location
void Particle::update(){
    // update velocity
    velocity += acceleration;
    // limit speed
    velocity.limit(maxSpeed);
    // set new location based on current velocity
    location += velocity;
    // reset acceleration
    acceleration *= 0;
}

void Particle::applyForce(ofVec2f force) {
    acceleration += force;
}

// if the particle goes off the screen, put it back in
void Particle::borders() {
    if(location.y < -r) {
        location.y = heightLimit+r;
        location.x = initLocX();
    } else if(location.y > heightLimit+r) {
        location.y = -r;
        location.x = initLocX();
    }
}

// render the particle
void Particle::display() {
    ofPushMatrix();
    ofTranslate(location);
    ofSetHexColor(color);
    ofFill();
    ofCircle(0, 0, r);
    ofPopMatrix();
}

// implementation of Craig Reynolds' path following algorithm
void Particle::follow(ParticlesPath path) {
    // velocity.y shouldn't be positive or so slow,
    // but seeking a target outside the path can make it so
    if(velocity.y > -0.1) velocity.y = -0.1;
    
    // predict location "n" frames ahead
    int n = 20;
    ofVec2f predict = velocity;
    predict.normalize();
    predict *= n;
    ofVec2f predictLoc = location + predict;
    
    // get the normal point to that line
    ofVec2f normalPoint = getNormalPoint(predictLoc, path.start, path.end);
    
    // find target point a little further ahead of normal
    ofVec2f dir = path.end - path.start;
    dir.normalize();
    dir *= n;
    ofVec2f target = normalPoint + dir;
    
    // how far away are we from the path?
    float distance = predictLoc.distance(normalPoint);
    // only if the distance is greater than the path's radius do we steer
    if(distance > path.radius) seek(target);
}

// A function to get the normal point from a point (p) to a line segment (a-b)
ofVec2f Particle::getNormalPoint(ofVec2f p, ofVec2f a, ofVec2f b) {
    ofVec2f ap = p - a;
    ofVec2f ab = b-a;

    ab.normalize();
    ab *= ap.dot(ab);
    
    return a + ab;
}

// calculate and apply a steering force towards a target
void Particle::seek(ofVec2f target) {
    // A vector pointing from the location to the target
    ofVec2f desired = target - location;      
// if desired length equal 0, skip out of here
//if(desired.length() == 0) return;
    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired *= maxSpeed;
    // steering = desired - velocity
    ofVec2f steer = desired - velocity;
    // limit to maximum steering force
    steer.limit(maxForce);  
    // apply steering force
    applyForce(steer);
}


