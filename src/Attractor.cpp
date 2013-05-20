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
    lifeTime = 1.2;
}

Attractor::Attractor(ofVec2f location) {
    this->location = location;
    
    bornTime = time(0);
    lifeTime = 1.2;
}

Attractor::Attractor(ofVec2f location, float intensity) {
    this->location = location;
    this->intensity = intensity;
    
    bornTime = time(0);
    lifeTime = floor(intensity/1000.0) * 2;
}

void Attractor::setLocation(ofVec2f location) {
    this->location = location;
}
