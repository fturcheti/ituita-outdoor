//
//  Attractor.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 12/7/12.
//

#include "ofMain.h"

class Attractor {

    public:
        Attractor();
        Attractor(ofVec2f);
        Attractor(ofVec2f, float);
    
        void set(ofVec2f);
        
        ofVec2f location;
        float intensity;
        int bornTime;
        int lifeTime;
    
};
