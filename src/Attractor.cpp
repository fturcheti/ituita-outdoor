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
