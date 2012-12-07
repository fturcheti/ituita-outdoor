//--------------------------------------------------------------
// [fturcheti] TODO list
//--------------------------------------------------------------
//
//
//--------------------------------------------------------------


#include "testApp.h"


//--------------------------------------------------------------
// MARK: SETUP
//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofBackground(30);

    // --------------------------------------------
    // MARK: CONTROL VARIABLES SETUP
    
    iNearThreshold  = 0;
    iFarThreshold   = 255;
    iMinBlobSize    = 1000;
    iMaxBlobSize    = 300000;
    iMaxNumBlobs    = 10;
    iLeftKinectId   = 0;
    iRightKinectId  = 1;
    

    iFboAlpha   = 60;
    fPathRadius = 20.0;

    bResetData            = true;
    iMaxRandomParticles   = 100;
    iDeltaRandomParticles = 60;

    fProx = 50.0;
    fMinParticleSize = 4.0;
    fMaxParticleSize = 20.0;
    
    // --------------------------------------------
    // MARK: INTERFACE SETUP
    
    iDrawWidth  = 400;
    iDrawHeight = 300;
    
    iTopMargin  = 115;
    iLeftMargin = 250;
    
    bDrawDepthMap   = true;
    bDrawThreshold  = false;
    bDrawBlobs      = false;
    
    iMode           = 0;
    
    bLockKinTilt = true;
    fKin1TiltAngle = 0;
    fKin2TiltAngle = 0;
    
    gui.addSlider("Display Modes", iMode, 0, 2);
    gui.addToggle("Lock Tilt Angle", bLockKinTilt);
    gui.addSlider("Kin 1 Tilt Angle", fKin1TiltAngle, -30, 30);
    gui.addSlider("Kin 2 Tilt Angle", fKin2TiltAngle, -30, 30);
    gui.addSlider("Near Threshold", iNearThreshold, 0, 255);
    gui.addSlider("Far Threshold", iFarThreshold, 255, 0);
    gui.addSlider("Min Blob Size", iMinBlobSize, 0, 40000);
    gui.addSlider("Max Blob Size", iMaxBlobSize, 1, 307200);
    gui.addSlider("Max Num Blobs", iMaxNumBlobs, 1, 30);
    
    gui.addPage("Particles");
    gui.addSlider("FBO Alpha (real-time)", iFboAlpha, 0, 255);
    gui.addSlider("Path Radius", fPathRadius, 2.0f, 60.0f);
    gui.addSlider("Random Max", iMaxRandomParticles, 50, 500);
    gui.addSlider("Random Delta", iDeltaRandomParticles, 0, 100);
    gui.addSlider("Min Particle Size", fMinParticleSize, 1.0f, 10.0f);
    gui.addSlider("Max Particle Size", fMaxParticleSize, 1.0f, 100.0f);
    gui.addButton("Reset particles", bResetData);
    gui.addSlider("Prox (real-time)", fProx, 0.0f, 100.0f);
    
    gui.addPage("Kinect IDs");
    gui.addSlider("Left Kinect ID", iLeftKinectId, 0, 1);
    gui.addSlider("Right Kinect ID", iRightKinectId, 0, 1);
    
    gui.loadFromXML();
    gui.show();
    
    isGUIActive = true;
    
    // --------------------------------------------
    // MARK: KINECT SETUP
    
    #ifdef USE_TWO_KINECTS
        kinect.setup(true, iLeftKinectId, iRightKinectId);
    #endif
        
    #ifndef USE_TWO_KINECTS
        kinect.setup(false);        
    #endif
    
    
    // --------------------------------------------
    // MARK: FBO SETUP
    
    fbo.allocate(FBO_W, FBO_H);
    
    
    // --------------------------------------------
    // MARK: DATA SETUP
    
    setupData();
    
    
    // --------------------------------------------
    // MARK: PARTICLES SETUP
    
    initPaths();
    initParticles();
        
}


//--------------------------------------------------------------
// MARK: DATA
//--------------------------------------------------------------
void testApp::setupData() {
    
        // generating random values
        int delta = iMaxRandomParticles * ((100.0-iDeltaRandomParticles)/100.0);
        data.generateRandomValues(delta, iMaxRandomParticles);
        
        // getting the values
        personalData[POSITIVE] = data.getPersonalPositives();
        personalData[NEUTRAL]  = data.getPersonalNeutrals();
        personalData[NEGATIVE] = data.getPersonalNegatives();
        
        neighborhoodData[POSITIVE] = data.getNeighborhoodPositives();
        neighborhoodData[NEUTRAL]  = data.getNeighborhoodNeutrals();
        neighborhoodData[NEGATIVE] = data.getNeighborhoodNegatives();
        
        cityData[POSITIVE] = data.getCityPositives();
        cityData[NEUTRAL]  = data.getCityNeutrals();
        cityData[NEGATIVE] = data.getCityNegatives();
        
        // logging the data
        string datalog = "- DATA --------------------------------------------- \n";    
        datalog += "personal     (";
        datalog += ofToString(personalData[POSITIVE]+personalData[NEUTRAL]+personalData[NEGATIVE]) + "): ";
        datalog += ofToString(personalData[POSITIVE]) + " / "; 
        datalog += ofToString(personalData[NEUTRAL]) + " / ";
        datalog += ofToString(personalData[NEGATIVE]) + "\n";
        datalog += "neighborhood (";
        datalog += ofToString(neighborhoodData[POSITIVE]+neighborhoodData[NEUTRAL]+neighborhoodData[NEGATIVE]) + "): ";
        datalog += ofToString(neighborhoodData[POSITIVE]) + " / "; 
        datalog += ofToString(neighborhoodData[NEUTRAL]) + " / ";
        datalog += ofToString(neighborhoodData[NEGATIVE]) + "\n";
        datalog += "city         (";
        datalog += ofToString(cityData[POSITIVE]+cityData[NEUTRAL]+cityData[NEGATIVE]) + "): ";
        datalog += ofToString(cityData[POSITIVE]) + " / "; 
        datalog += ofToString(cityData[NEUTRAL]) + " / ";
        datalog += ofToString(cityData[NEGATIVE]) + "\n";
        datalog += "---------------------------------------------------- \n";
        ofLog(OF_LOG_NOTICE, datalog);
        
}


//--------------------------------------------------------------
// MARK: PARTICLES
//--------------------------------------------------------------
void testApp::initParticles() {
    
    isMousePressed = false;
    
    personalParticles.clear();    
    neighborhoodParticles.clear();
    cityParticles.clear();
    
    addParticles(personalParticles, personalData[NEUTRAL],  GRAY,   *personalPath);
    addParticles(personalParticles, personalData[NEGATIVE], RED,    *personalPath);
    addParticles(personalParticles, personalData[NEUTRAL],  YELLOW, *personalPath);
    addParticles(personalParticles, personalData[POSITIVE], GREEN,  *personalPath);
    
    addParticles(neighborhoodParticles, neighborhoodData[NEUTRAL],  GRAY,   *neighborhoodPath);
    addParticles(neighborhoodParticles, neighborhoodData[NEGATIVE], RED,    *neighborhoodPath);
    addParticles(neighborhoodParticles, neighborhoodData[NEUTRAL],  YELLOW, *neighborhoodPath);
    addParticles(neighborhoodParticles, neighborhoodData[POSITIVE], GREEN,  *neighborhoodPath);
    
    addParticles(cityParticles, cityData[NEUTRAL],  GRAY,   *cityPath);
    addParticles(cityParticles, cityData[NEGATIVE], RED,    *cityPath);
    addParticles(cityParticles, cityData[NEUTRAL],  YELLOW, *cityPath);
    addParticles(cityParticles, cityData[POSITIVE], GREEN,  *cityPath);
    
}

//--------------------------------------------------------------
void testApp::initPaths() {
    
    personalPath     = new ParticlesPath( fPathRadius, 
                                         ofVec2f(FBO_W/10.0, FBO_W), 
                                         ofVec2f(FBO_W/10.0, 0) );
    neighborhoodPath = new ParticlesPath( fPathRadius, 
                                         ofVec2f(FBO_W/10.0 * 5, FBO_H), 
                                         ofVec2f(FBO_W/10.0 * 5, 0) );
    cityPath         = new ParticlesPath( fPathRadius, 
                                         ofVec2f(FBO_W/10.0 * 9, FBO_H), 
                                         ofVec2f(FBO_W/10.0 * 9, 0) );
    
}

//--------------------------------------------------------------
void testApp::addParticles(vector<Particle> &particles, int number, int color, ParticlesPath &path) {
    
    for(int i = 0; i < number; i++) {
        Particle p(path, FBO_W/5.0, FBO_H, color);
        particles.push_back(p);
    }
    
}

//--------------------------------------------------------------
void testApp::runParticles(vector<Particle> &particles, ParticlesPath &path) {
    for(int i = 0; i < particles.size(); i++) {
        if(isMousePressed) { 
            particles[i].seek(ofVec2f(mouseX, mouseY));
        } else {
            particles[i].follow(path);
        }
        particles[i].run();
    }    
}


//--------------------------------------------------------------
// MARK: UPDATE
//--------------------------------------------------------------
void testApp::update(){
    
    kinect.updateThreshPar(iFarThreshold, iNearThreshold);
    kinect.updateBlobPar(iMinBlobSize, iMaxBlobSize, iMaxNumBlobs);
    
    if(!bLockKinTilt) {
        kinect.setKinTiltAngle(false, fKin1TiltAngle);
        kinect.setKinTiltAngle(true, fKin2TiltAngle);
    }
    
    kinect.update();
    
    if(bResetData) {
        bResetData = false;
        setupData();
        initPaths();
        initParticles();
    }

    
//    // --------------------------------------------
//    // MARK: BLOBS BOXES    
//    // destroy dead blobs/boxes
//    for(map<int, ofxBox2dRect>::iterator it = b2dBlobs.begin(); it != b2dBlobs.end(); it++) {
//        if( kinect.foundBlobsMap.find((*it).first) == kinect.foundBlobsMap.end() ) {
//            ((*it).second).destroy();
//            b2dBlobs.erase(it);
//        }
//    }
//    
//    // update and add blobs/boxes
//    for(int i = 0; i < kinect.activeBlobsIds.size(); i++) {
//        
//        // search for the key on the map
//        int theKey = kinect.activeBlobsIds[i];
//        ofxBlob b = kinect.foundBlobsMap[theKey];
//        map<int, ofxBox2dRect>::iterator it = b2dBlobs.find(theKey);
//
//        // if it's already on it, update box
//        if( it != b2dBlobs.end() ) {
//            // inverting blob x
//            float x = ofMap(b.centroid.x, 1.0f, 0.0f, 0.0f, 1.0f);
//            b2dBlobs[theKey].setPosition(x * FBO_W, b.centroid.y * FBO_H);
//        }
//        
//        // else, add box to the map and the world 
//        else {
//            ofxBox2dRect r;
//            r.setPhysics(fDensity, fBounce, fFriction);
//            // inverting blob x
//            float x = ofMap(b.centroid.x, 1.0f, 0.0f, 0.0f, 1.0f);
//            r.setup(box2d.getWorld(), x * FBO_W, b.centroid.y * FBO_H, 
//                    b.boundingRect.width * FBO_W, b.boundingRect.height * FBO_H);
//            
//            b2dBlobs[theKey] = r;
//        }
//    }
    
}


//--------------------------------------------------------------
// MARK: DRAW
//--------------------------------------------------------------
void testApp::draw(){

    // MARK: DRAW PARTICLES TO FBO
    fbo.begin();
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);  
    glEnable(GL_BLEND);  
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);  
    
    //ofClear(0, 0, 0, 40);
    ofSetColor(0, iFboAlpha);
    ofRect(0, 0, FBO_W, FBO_H);

    
//        for(int i = 0; i < b2dParticles.size(); i++) {
//            CustomParticle p = b2dParticles[i];
//            Data * customData = (Data*)p.getData();        
//            
//            
//            if(kinect.pointCloud.size() > 0) {
//                int relativeX = ofMap(p.getPosition().x, 0, FBO_W, kinect.getOutputWidth(), 0);            
//                int relativeY = ofMap(p.getPosition().y, 0, FBO_H, 0, kinect.getOutputHeight());
//                
//                int relativeKinectIndex = relativeX + (kinect.getOutputWidth() * relativeY);
//                
//                if(relativeKinectIndex < kinect.pointCloud.size()) {
//                    ofPoint kinectPoint = kinect.pointCloud[relativeKinectIndex];
//                    float z = (kinectPoint.z < 0.001) ? 1 : kinectPoint.z;
//                    float prox = 1.2 - z;
//                    
//                    float sz = 0;
//                    switch(customData->scope) {
//                        case PERSONAL:
//                            sz += personalMinParticleSize;
//                            break;
//                        case NEIGHBORHOOD:
//                            sz += neighborhoodMinParticleSize;
//                            break;
//                        case CITY:
//                            sz += cityMinParticleSize;
//                            break;
//                    }                
//                    sz += prox * fProx;
//
//                    float r = p.getRadius();
//                    
//                    // property = (target - property) / speed
//                    r += (sz - r) / 4.0f;
//                    
//                    p.setRadius(r);
//                }
//            }    
//
//            p.draw();
//        }
    

    runParticles(personalParticles, *personalPath);
    runParticles(neighborhoodParticles, *neighborhoodPath);
    runParticles(cityParticles, *cityPath);
    
    glDisable(GL_BLEND);  
    glPopAttrib(); 
        
    fbo.end();
    
    ofEnableAlphaBlending();
//    ofSetColor(255);
//    fbo.draw(0, 0);
    drawPanels(fbo);
    ofDisableAlphaBlending();
    
    if(isGUIActive) {
        drawGUI();
    }
    
}


//--------------------------------------------------------------
void testApp::drawKinectPointCloud() {
    int inc = 20;
    for (int i = 0; i < kinect.pointCloud.size(); i+=inc) {
        
        float z = (kinect.pointCloud[i].z < 0.001) ? 1 : kinect.pointCloud[i].z;

        float prox = (1.0 - z);
        float sz = pow(prox, 3) * inc;
        ofCircle(kinect.pointCloud[i].x * (float)ofGetWidth(), kinect.pointCloud[i].y * (float)ofGetHeight(), sz);

        int limit_w = kinect.getOutputWidth();
        if(i % limit_w == 0) {
            i += inc * limit_w;
        }
    }
}

//--------------------------------------------------------------
void testApp::drawPanels(ofFbo fbo) {
	glPushMatrix();	
	glTranslatef(0, 0, 0);
	
    ofTexture &t = fbo.getTextureReference();
    ofPoint c;
    
	t.bind();	
	glBegin(GL_QUADS);    

    int ow = OUTPUT_SCREEN_W/3;
    int oh = OUTPUT_SCREEN_H;
    
    int iw = FBO_W/5;
    int ih = FBO_H;
    
    // --------------------------------------------
    // PANEL 1 
    
    // left / top
    c = t.getCoordFromPoint(0, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(0, 0);    
    // right / top
    c = t.getCoordFromPoint(iw, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow, 0);    
    // right / bottom
    c = t.getCoordFromPoint(iw, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow, oh);
    // left / bottom
    c = t.getCoordFromPoint(0, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(0, oh);        

    // --------------------------------------------
    // PANEL 2
    
    // left / top    
    c = t.getCoordFromPoint(iw*2, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow, 0);    
    // right / top
    c = t.getCoordFromPoint(iw*3, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*2, 0);    
    // right / bottom
    c = t.getCoordFromPoint(iw*3, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*2, oh);        
    // left / bottom
    c = t.getCoordFromPoint(iw*2, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow, oh);        

    // --------------------------------------------
    // PANEL 3
    
    // left / top    
    c = t.getCoordFromPoint(iw*4, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*2, 0);    
    // right / top
    c = t.getCoordFromPoint(iw*5, 0);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*3, 0);    
    // right / bottom
    c = t.getCoordFromPoint(iw*5, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*3, oh);
    // left / bottom
    c = t.getCoordFromPoint(iw*4, ih);
	glTexCoord2f(c.x, c.y); 
    glVertex2f(ow*2, oh);        

    glEnd();
	t.unbind();
	glPopMatrix();

}

//--------------------------------------------------------------
void testApp::drawGUI() {
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, 127);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofDisableAlphaBlending();
    
    ofSetColor(255, 255, 255);
    ofNoFill();
    
    if(iMode == 0) {
        kinect.drawDepth(iLeftMargin, iTopMargin, iDrawWidth, iDrawHeight);
        ofRect(iLeftMargin, iTopMargin, iDrawWidth, iDrawHeight);
        ofDrawBitmapString("Kinect 1", iLeftMargin + 5, iTopMargin + 15);
        
        kinect.drawThreshImg(iLeftMargin, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight);
        kinect.drawContour(iLeftMargin, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight);
        ofRect(iLeftMargin, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight);
        
#ifdef USE_TWO_KINECTS
        
        kinect.drawDepth(iLeftMargin + iDrawWidth + 20, iTopMargin, iDrawWidth, iDrawHeight, true);
        ofRect(iLeftMargin + iDrawWidth + 20, iTopMargin, iDrawWidth, iDrawHeight);
        ofDrawBitmapString("Kinect 2", iLeftMargin + iDrawWidth + 25, iTopMargin + 15);
        
        kinect.drawThreshImg(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight, true);
        kinect.drawContour(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight, true);
        
        ofRect(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight);
        
#endif
        
    }
    
    else if(iMode == 1) {
        kinect.drawThreshImg(iLeftMargin, iTopMargin, 640, 480);
        kinect.drawThreshImg(iLeftMargin + 320, iTopMargin, 640, 480, true);
        kinect.drawNormBlobs(iLeftMargin, iTopMargin, 960, 480);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Blobs drawn from normalized blobs", iLeftMargin, iTopMargin + 500);
    }
    
    else if(iMode == 2) {
        kinect.drawDepthFromCloud(iLeftMargin, iTopMargin, 960, 480);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Depth map drawn from normalized point cloud", iLeftMargin, iTopMargin + 500);
    }
    
    gui.draw();
    
}


//--------------------------------------------------------------
// MARK: EXIT
//--------------------------------------------------------------
void testApp::exit() {

    kinect.close();

}


//--------------------------------------------------------------
// MARK: EVENTS
//--------------------------------------------------------------
//--------------------------------------------------------------
// KEYS MAPPING
//
// G: controls (turns on/off) the GUI drawing
//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == 'g' || key == 'G') {
        isGUIActive = !isGUIActive;
        if(isGUIActive) {
            gui.show();
        } else { 
            gui.hide();
        }
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    isMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    isMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    isMousePressed = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}