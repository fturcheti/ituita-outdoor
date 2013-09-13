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

#include "testApp.h"


//--------------------------------------------------------------
// MARK: SETUP
//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(30);

    // --------------------------------------------
    // MARK: CONTROL VARIABLES SETUP
    
    iNearThreshold  = 0;
    iFarThreshold   = 255;
    iMinBlobSize    = 1000;
    iMaxBlobSize    = 300000;
    iMaxNumBlobs    = 10;
    
    bTwoKinects     = 1;
    iLeftKinectId   = 0;
    iRightKinectId  = 1;

    iFboAlpha   = 60;
    fPathRadius = (FBO_W / 3.0) / 2.4;

    bResetData            = false;
    bRandomizeParticles   = false;
    iMaxRandomParticles   = 200;
    iDeltaRandomParticles = 60;

    fProxFactor      = 20.0;
    fMinParticleSize = 1.4;
    fMaxParticleSize = 6.0;
    
    fAttractionVelocity = 2.0;
    fAttractorLife      = 0.5;
    
    f4Green[0]     = 0.0/255.0;   f4Green[1]     = 182.0/255.0; f4Green[2]     = 83.0/255.0;  f4Green[3]     = 255.0/255.0;
    f4Yellow[0]    = 250.0/255.0; f4Yellow[1]    = 235.0/255.0; f4Yellow[2]    = 52.0/255.0;  f4Yellow[3]    = 255.0/255.0;
    f4Red[0]       = 237.0/255.0; f4Red[1]       = 40.0/255.0;  f4Red[2]       = 73.0/255.0;  f4Red[3]       = 255.0/255.0;
    f4Gray[0]      = 102.0/255.0; f4Gray[1]      = 102.0/255.0; f4Gray[2]      = 102.0/255.0; f4Gray[3]      = 255.0/255.0;
    f4Highlight[0] = 255.0/255.0; f4Highlight[1] = 255.0/255.0; f4Highlight[2] = 255.0/255.0; f4Highlight[3] = 255.0/255.0;    
    
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
    iNetDataSource  = 0;
    iLogLevel       = 0;
    
    bLockKinTilt   = true;
    fKin1TiltAngle = 0;
    fKin2TiltAngle = 0;
    
    // --------------------------------------------
    // SETTINGS PAGES
    // --------------------------------------------
    // PAGE 1: GENERAL
    gui.addSlider("Display Modes", iMode, 0, 2);
	string dataSourceTitles[] = {"FINAL", "TEST"};
    gui.addComboBox("Net Data Source", iNetDataSource, 2, dataSourceTitles);
	string logLevelTitles[] = {"NOTICE", "VERBOSE"};
    gui.addComboBox("Log Level", iLogLevel, 2, logLevelTitles);
    
    // PAGE 2: KINECTS
    gui.addPage("Kinects");
    gui.addToggle("Two Kinects", bTwoKinects);
    gui.addSlider("Left Kinect ID", iLeftKinectId, 0, 1);
    gui.addSlider("Right Kinect ID", iRightKinectId, 0, 1);
    gui.addToggle("Lock Tilt Angle", bLockKinTilt);
    gui.addSlider("Kin 1 Tilt Angle", fKin1TiltAngle, -30, 30);
    gui.addSlider("Kin 2 Tilt Angle", fKin2TiltAngle, -30, 30);
    
    // PAGE 3: DETECTION
    gui.addPage("Detection");
    gui.addSlider("Near Threshold", iNearThreshold, 0, 255);
    gui.addSlider("Far Threshold", iFarThreshold, 0, 255);
    gui.addSlider("Min Blob Size", iMinBlobSize, 0, 40000);
    gui.addSlider("Max Blob Size", iMaxBlobSize, 1, 307200);
    gui.addSlider("Max Num Blobs", iMaxNumBlobs, 1, 30);
    
    // PAGE 4: PARTICLES
    gui.addPage("Particles");
    gui.addTitle("REALTIME");
    gui.addSlider("Prox Factor", fProxFactor, 1.0f, 20.0f);
    gui.addSlider("Attractor Vel", fAttractionVelocity, 0.1f, 2.0f);
    gui.addSlider("Attractor Life", fAttractorLife, 0.1f, 2.0f);
    gui.addSlider("FBO Alpha", iFboAlpha, 0, 255);
    gui.addTitle("RESET NEEDED");
    gui.addSlider("Path Radius", fPathRadius, 2.0f, 60.0f);
    gui.addSlider("Min Particle Size", fMinParticleSize, 1.0f, 4.0f);
    gui.addSlider("Max Particle Size", fMaxParticleSize, 1.0f, 20.0f);
    gui.addToggle("Randomize", bRandomizeParticles);
    gui.addSlider("Random Max", iMaxRandomParticles, 50, 500);
    gui.addSlider("Random Delta", iDeltaRandomParticles, 0, 100);
    gui.addButton("Reset particles", bResetData);
    
    // PAGE 5: COLORS
    gui.addPage("Colors");
    gui.addColorPicker("Positivo", f4Green); 
    gui.addColorPicker("Neutro", f4Yellow); 
    gui.addColorPicker("Negativo", f4Red); 
    gui.addColorPicker("Ghosts", f4Gray); 
    gui.addColorPicker("Highlight", f4Highlight);
    
    gui.loadFromXML();
    gui.show();
    
    isGUIActive = false;
    
    
    // --------------------------------------------
    // MARK: LOG LEVEL
    if(iLogLevel == 0) {
        ofSetLogLevel(OF_LOG_NOTICE);
    } else {
        ofSetLogLevel(OF_LOG_VERBOSE);
    }

    // --------------------------------------------
    // MARK: KINECT SETUP
    
    if(bTwoKinects) {
        kinect.setup(true, iLeftKinectId, iRightKinectId);
    } else {
        kinect.setup(false);
    }

    
    // --------------------------------------------
    // MARK: FBO SETUP
    
    fbo.allocate(FBO_W, FBO_H);
    
    
    // --------------------------------------------
    // MARK: DATA SETUP
    
    // LOAD URLs (final and test) from XML
    // the URLs XML is located in the data folder and named "_URL.xml"
    // the finalURL node contains the final (production) URL
    // the testURL node contains the test URL
    ofxXmlSettings urls;
    urls.loadFile("_URL.xml");
    urls.pushTag("urls");
    string finalURL = urls.getValue("finalURL", "http://ituita.com.br/site/sugestoes/total/");
    string testURL  = urls.getValue("testURL", "http://fronte.co/dev/ituita/resultados.php");
    urls.popTag();
    
    // COMPLETE the finalURL with the current month and year
    // the finalURL format is: http://ituita.com.br/site/sugestoes/total/[YEAR]/[MONTH].xml
    finalURL += ofToString(ofGetYear()) + "/";
    finalURL += ofToString(ofGetMonth()) + ".xml";
    
    // SETUP the thread that will load the data from the internet
    xmlThread.setFinalURL(finalURL);
    xmlThread.setTestURL(testURL);
    xmlThread.setActiveURL(iNetDataSource);
    xmlThread.setLoadingInterval(3000);
    
    // START the loading thread
    xmlThread.startThread(true, false);
    hasInitiated = false;
    
    
    // --------------------------------------------
    // MARK: PARTICLES SETUP
    
    doStreetPanelAttraction       = false;
    doNeighborhoodPanelAttraction = false;
    doCityPanelAttraction         = false;
    isMousePressed                = false;
    
    initPaths();
    initParticles();
        
}


//--------------------------------------------------------------
// MARK: PARTICLES
//--------------------------------------------------------------
void testApp::initParticles() {
    
    GREEN     = ofColor(f4Green[0]*255,     f4Green[1]*255,     f4Green[2]*255,     f4Green[3]*255);
    YELLOW    = ofColor(f4Yellow[0]*255,    f4Yellow[1]*255,    f4Yellow[2]*255,    f4Yellow[3]*255);
    RED       = ofColor(f4Red[0]*255,       f4Red[1]*255,       f4Red[2]*255,       f4Red[3]*255);
    GRAY      = ofColor(f4Gray[0]*255,      f4Gray[1]*255,      f4Gray[2]*255,      f4Gray[3]*255);
    HIGHLIGHT = ofColor(f4Highlight[0]*255, f4Highlight[1]*255, f4Highlight[2]*255, f4Highlight[3]*255);     
    
    streetParticles.clear();    
    neighborhoodParticles.clear();
    cityParticles.clear();
    
    // logging the data
    string datalog = "\n";
    datalog += "- DATA --------------------------------------------- \n";
    datalog += "street       (";
    datalog += ofToString(data.getStreetPositives()+data.getStreetNeutrals()+data.getStreetNegatives()) + "): ";
    datalog += ofToString(data.getStreetPositives()) + " / ";
    datalog += ofToString(data.getStreetNeutrals()) + " / ";
    datalog += ofToString(data.getStreetNegatives()) + "\n";
    datalog += "neighborhood (";
    datalog += ofToString(data.getNeighborhoodPositives()+data.getNeighborhoodNeutrals()+data.getNeighborhoodNegatives()) + "): ";
    datalog += ofToString(data.getNeighborhoodPositives()) + " / ";
    datalog += ofToString(data.getNeighborhoodNeutrals()) + " / ";
    datalog += ofToString(data.getNeighborhoodNegatives()) + "\n";
    datalog += "city         (";
    datalog += ofToString(data.getCityPositives()+data.getCityNeutrals()+data.getCityNegatives()) + "): ";
    datalog += ofToString(data.getCityPositives()) + " / ";
    datalog += ofToString(data.getCityNeutrals()) + " / ";
    datalog += ofToString(data.getCityNegatives()) + "\n";
    datalog += "---------------------------------------------------- \n";
    ofLogNotice() << datalog;
    
    addParticles(streetParticles, 0,  GRAY,   *streetPath); // GHOSTS
    addParticles(streetParticles, data.getStreetNegatives(), RED,    *streetPath);
    addParticles(streetParticles, data.getStreetNeutrals(),  YELLOW, *streetPath);
    addParticles(streetParticles, data.getStreetPositives(), GREEN,  *streetPath);
    
    addParticles(neighborhoodParticles, 0,  GRAY,   *neighborhoodPath); // GHOSTS
    addParticles(neighborhoodParticles, data.getNeighborhoodNegatives(), RED,    *neighborhoodPath);
    addParticles(neighborhoodParticles, data.getNeighborhoodNeutrals(),  YELLOW, *neighborhoodPath);
    addParticles(neighborhoodParticles, data.getNeighborhoodPositives(), GREEN,  *neighborhoodPath);
    
    addParticles(cityParticles, 0,  GRAY,   *cityPath); // GHOSTS
    addParticles(cityParticles, data.getCityNegatives(), RED,    *cityPath);
    addParticles(cityParticles, data.getCityNeutrals(),  YELLOW, *cityPath);
    addParticles(cityParticles, data.getCityPositives(), GREEN,  *cityPath);
    
}

//--------------------------------------------------------------
void testApp::initPaths() {

    streetPath       = new ParticlesPath( fPathRadius,
                                         ofVec2f(FBO_W/6.0, FBO_W), 
                                         ofVec2f(FBO_W/6.0, 0) );
    neighborhoodPath = new ParticlesPath( fPathRadius, 
                                         ofVec2f(FBO_W/6.0 * 3, FBO_H), 
                                         ofVec2f(FBO_W/6.0 * 3, 0) );
    cityPath         = new ParticlesPath( fPathRadius, 
                                         ofVec2f(FBO_W/6.0 * 5, FBO_H), 
                                         ofVec2f(FBO_W/6.0 * 5, 0) );
    
}

//--------------------------------------------------------------
void testApp::addParticles(vector<Particle> &particles, int number, ofColor color, ParticlesPath &path) {
    
    for(int i = 0; i < number; i++) {
        Particle p(path, FBO_W/5.0, FBO_H, color);
        particles.push_back(p);
    }
    
}

//--------------------------------------------------------------
void testApp::runParticles(vector<Particle> &particles, ParticlesPath &path) {
    for(int i = 0; i < particles.size(); i++) {
        Particle* p = &particles[i];

        
        // KINECT INTERACTOR ATTRACTORS
        // defining the particle path
        bool doAttraction = false;
        Attractor * attractor;
        
        // verify the panel which the particle belongs
        // and if there is an average attractor on it
        if(&path == streetPath && doStreetPanelAttraction) {
            doAttraction = true;
            attractor = &attractorStreetPanel;
        }
        else if(&path == neighborhoodPath && doNeighborhoodPanelAttraction) {
            doAttraction = true;
            attractor = &attractorNeighborhoodPanel;
        }
        else if(&path == cityPath && doCityPanelAttraction) {
            doAttraction = true;
            attractor = &attractorCityPanel;
        }
        
        // if there is an average attractor in this particle's panel, follow it
        if(doAttraction) {
            p->maxSpeed = fAttractionVelocity;
            p->seek(attractor->location);
        // else, follow the path
        } else {
            p->maxSpeed = p->originalMaxSpeed;
            p->follow(path);
        }
        
        
        // KINECT INTERACTOR APPROXIMATION
        // check if kinect's pointcloud is filled
        if(kinect.pointCloud.size() > 0) {
            // calculating the relative position of the particle in the point cloud
            int relativeX = ofMap(p->location.x, 0, FBO_W, 0, kinect.getOutputWidth());            
            int relativeY = ofMap(p->location.y, 0, FBO_H, 0, kinect.getOutputHeight());
            int relativeKinectIndex = relativeX + (kinect.getOutputWidth() * relativeY);
            
            // check if the relative index is actually contained in the pointcloud
            if(relativeKinectIndex < kinect.pointCloud.size()) {
                // get relative kinect point
                ofPoint kinectPoint = kinect.pointCloud[relativeKinectIndex];
                // prevent invalid depth input
                float z = (kinectPoint.z < 0.001) ? 0.0 : 1.0-kinectPoint.z;

                // calculate destination size
                float prox = (1 - 4 * (z/2 - 0.5) * (z/2 - 0.5));
                float sz = ofClamp(prox * fProxFactor, fMinParticleSize, fMaxParticleSize);
                
                // set the particle radius
                float r = p->r;
                // property = (target - property) / speed
                r += (sz - r) / 4.0;
                p->r = r;
                
                // calculate highlight color relative to approximation
                ofColor auxColor = p->originalColor; 
                p->highlightColor = auxColor.lerp( HIGHLIGHT, sz/fMaxParticleSize * 0.8 );
                
            }
        }        
        
        // update particle location based on actuating forces and render it
        p->run();
    }    
}


//--------------------------------------------------------------
// MARK: UPDATE
//--------------------------------------------------------------
void testApp::update(){
    
    // XML update > data update
    if(xmlThread.isAvailable() && !bRandomizeParticles) {
        data.getResultsFromBuffer(xmlThread.getXML());
        
        // if it is starting, load the first batch
        if(!hasInitiated) {
            hasInitiated = true;
            bResetData = true;
        // else, if it already started, add new particles (if it is the case)
        } else {
            addParticles(streetParticles, data.getNewStreetPositives(), GREEN,  *streetPath);
            addParticles(streetParticles, data.getNewStreetNeutrals(), YELLOW,  *streetPath);
            addParticles(streetParticles, data.getNewStreetNegatives(), RED,  *streetPath);

            addParticles(neighborhoodParticles, data.getNewNeighborhoodPositives(), GREEN,  *neighborhoodPath);
            addParticles(neighborhoodParticles, data.getNewNeighborhoodNeutrals(), YELLOW,  *neighborhoodPath);
            addParticles(neighborhoodParticles, data.getNewNeighborhoodNegatives(), RED,  *neighborhoodPath);

            addParticles(cityParticles, data.getNewCityPositives(), GREEN,  *cityPath);
            addParticles(cityParticles, data.getNewCityNeutrals(), YELLOW,  *cityPath);
            addParticles(cityParticles, data.getNewCityNegatives(), RED,  *cityPath);
            
            // logging the addition of new particles
            if(data.getNewStreetPositives() != 0) ofLogVerbose() << "- new street positives: " << data.getStreetPositives() << endl;
            if(data.getNewStreetNeutrals() != 0)  ofLogVerbose() << "- new street neutrals: " << data.getStreetNeutrals() << endl;
            if(data.getNewStreetNegatives() != 0) ofLogVerbose() << "- new street negatives: " << data.getStreetNegatives() << endl;
            if(data.getNewNeighborhoodPositives() != 0) ofLogVerbose() << "- new neighborhood positives: " << data.getNeighborhoodPositives() << endl;
            if(data.getNewNeighborhoodNeutrals() != 0)  ofLogVerbose() << "- new neighborhood neutrals: " << data.getNeighborhoodNeutrals() << endl;
            if(data.getNewNeighborhoodNegatives() != 0) ofLogVerbose() << "- new neighborhood negatives: " << data.getNeighborhoodNegatives() << endl;
            if(data.getNewCityPositives() != 0) ofLogVerbose() << "- new city positives: " << data.getCityPositives() << endl;
            if(data.getNewCityNeutrals() != 0)  ofLogVerbose() << "- new city neutrals: " << data.getCityNeutrals() << endl;
            if(data.getNewCityNegatives() != 0) ofLogVerbose() << "- new city negatives: " << data.getCityNegatives() << endl;
        }
    }
    
    // KINECT update
    kinect.updateThreshPar(iFarThreshold, iNearThreshold);
    kinect.updateBlobPar(iMinBlobSize, iMaxBlobSize, iMaxNumBlobs);
    
    if(!bLockKinTilt) {
        kinect.setKinTiltAngle(false, fKin1TiltAngle);
        kinect.setKinTiltAngle(true, fKin2TiltAngle);
    }
    
    kinect.update();
    
    if(bResetData) {
        
        if(bRandomizeParticles) {
            // generating random values
            int delta = iMaxRandomParticles * ((100.0-iDeltaRandomParticles)/100.0);
            data.generateRandomValues(delta, iMaxRandomParticles);
        }
        
        bResetData = false;
        initPaths();
        initParticles();
        
    }

    // --------------------------------------------
    // MARK: ATTRACTORS FROM BLOBS

    // variables to calculate the average attractor
    float sumx[3]    = {0.0, 0.0, 0.0};
    float sumy[3]    = {0.0, 0.0, 0.0};
    int   counter[3] = {0, 0, 0};
    
    // destroy dead attractors    
    for(map<int, Attractor>::iterator it = attractors.begin(); it != attractors.end(); it++) {
        if( kinect.foundBlobsMap.find((*it).first) == kinect.foundBlobsMap.end() ) {
            // get attractor
            Attractor a  = (*it).second;            
            // and check if it's time to die            
            if ( time(0) - a.bornTime > a.lifeTime ) {
                attractors.erase(it);
            } else {
                // if attractor is located in the first panel - the street panel
                if(a.location.x < FBO_W / 3) {
                    sumx[0] += a.location.x;
                    sumy[0] += a.location.y;
                    counter[0]++;
                }
                // or if attractor is located in the second panel - the neighborhood panel
                else if(a.location.x < FBO_W / 3 * 2) {
                    sumx[1] += a.location.x;
                    sumy[1] += a.location.y;
                    counter[1]++;
                }
                // or if attractor is located in the third panel - the city panel
                else {
                    sumx[2] += a.location.x;
                    sumy[2] += a.location.y;
                    counter[2]++;
                }
            }
        }
    }
        
    // update and add attractors
    for(int i = 0; i < kinect.activeBlobsIds.size(); i++) {
        
        // search for the key on the map
        int theKey = kinect.activeBlobsIds[i];
        ofxBlob b = kinect.foundBlobsMap[theKey];
        map<int, Attractor>::iterator it = attractors.find(theKey);

        // if it's already on it, update attractor
        if( it != attractors.end() ) {
            // inverting blob x
            float x = ofMap(b.centroid.x, 1.0f, 0.0f, 0.0f, 1.0f);
            attractors[theKey].location.set(x * FBO_W, b.centroid.y * FBO_H);
        }        
        // else, add attractor to the map
        else {
            // inverting blob x
            float x = ofMap(b.centroid.x, 1.0f, 0.0f, 0.0f, 1.0f);
            // add attractor to the map
            Attractor attract(ofVec2f(x * FBO_W, b.centroid.y * FBO_H), fAttractorLife);
            attractors[theKey] = attract;
        }

        
        // if attractor is located in the first panel - the street panel
        if(attractors[theKey].location.x < FBO_W / 3) {
            sumx[0] += attractors[theKey].location.x;
            sumy[0] += attractors[theKey].location.y;
            counter[0]++;
        }
        // or if attractor is located in the second panel - the neighborhood panel
        else if(attractors[theKey].location.x < FBO_W / 3 * 2) {
            sumx[1] += attractors[theKey].location.x;
            sumy[1] += attractors[theKey].location.y;
            counter[1]++;
        }
        // or if attractor is located in the third panel - the city panel
        else {
            sumx[2] += attractors[theKey].location.x;
            sumy[2] += attractors[theKey].location.y;
            counter[2]++;
        }

    }
    
    
    // if there are attractors in the first panel - the street panel,
    // set the panel average attractor
    if(counter[0] > 0) {
        doStreetPanelAttraction = true;
        attractorStreetPanel.setLocation(ofVec2f(sumx[0]/(float)counter[0], sumy[0]/(float)counter[0]));
    } else {
        doStreetPanelAttraction = false;
    }

    // if there are attractors in the second panel - the neighborhood panel,
    // set the panel average attractor
    if(counter[1] > 0) {
        doNeighborhoodPanelAttraction = true;
        attractorNeighborhoodPanel.setLocation(ofVec2f(sumx[1]/(float)counter[1], sumy[1]/(float)counter[1]));
    } else {
        doNeighborhoodPanelAttraction = false;
    }
    
    // if there are attractors in the third panel - the city panel,
    // set the panel average attractor
    if(counter[2] > 0) {
        doCityPanelAttraction = true;
        attractorCityPanel.setLocation(ofVec2f(sumx[2]/(float)counter[2], sumy[2]/(float)counter[2]));
    } else {
        doCityPanelAttraction = false;
    }
    
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
    
    ofSetColor(0, iFboAlpha);
    ofRect(0, 0, FBO_W, FBO_H);
    
    runParticles(streetParticles, *streetPath);
    runParticles(neighborhoodParticles, *neighborhoodPath);
    runParticles(cityParticles, *cityPath);
    
    glDisable(GL_BLEND);  
    glPopAttrib(); 
        
    fbo.end();
    
    ofEnableAlphaBlending();
//    ofSetColor(255);
    fbo.draw(0, 0);
//    drawPanels(fbo);
    ofDisableAlphaBlending();
    
    if(isGUIActive) {
        ofShowCursor();
        drawGUI();
    } else {
        //ofHideCursor();
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
        
        if(bTwoKinects) {
            kinect.drawDepth(iLeftMargin + iDrawWidth + 20, iTopMargin, iDrawWidth, iDrawHeight, true);
            ofRect(iLeftMargin + iDrawWidth + 20, iTopMargin, iDrawWidth, iDrawHeight);
            ofDrawBitmapString("Kinect 2", iLeftMargin + iDrawWidth + 25, iTopMargin + 15);
            
            kinect.drawThreshImg(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight, true);
            kinect.drawContour(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight, true);
            
            ofRect(iLeftMargin + iDrawWidth + 20, iTopMargin + iDrawHeight + 20, iDrawWidth, iDrawHeight);
        }
        
    }
    
    else if(iMode == 1) {
        kinect.drawThreshImg(iLeftMargin, iTopMargin, 640, 480);
        kinect.drawThreshImg(iLeftMargin + 320, iTopMargin, 640, 480, true);
        kinect.drawNormBlobs(iLeftMargin, iTopMargin, 960, 480);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Blobs drawn from normalized blobs", iLeftMargin, iTopMargin + 500);
    }
    
    else if(iMode == 2) {
        // the increment to minimize the drawing loop iterations
        int inc = 4;
        
        kinect.drawDepthFromCloud(iLeftMargin, iTopMargin, kinect.getOutputWidth(), kinect.getOutputHeight(), inc);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Depth map drawn from normalized point cloud", iLeftMargin, iTopMargin + 500);
    }
    
    gui.draw();
    
}


//--------------------------------------------------------------
// MARK: EXIT
//--------------------------------------------------------------
void testApp::exit() {

    xmlThread.stopThread();
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