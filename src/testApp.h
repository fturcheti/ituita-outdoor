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

#pragma once

#include "ofMain.h"

#include "ofxSimpleGuiToo.h"

#include "XMLThread.h"

#include "kinectCapture.h"
#include "ituitaBlobTracker.h"
#include "ituitaData.h"
#include "Particle.h"
#include "ParticlesPath.h"
#include "Attractor.h"


#define OUTPUT_SCREEN_W 576
#define OUTPUT_SCREEN_H 288
#define FBO_W OUTPUT_SCREEN_W
#define FBO_H OUTPUT_SCREEN_H


// ---------------------------------------------


class testApp : public ofBaseApp{
    
	public:
    
// --------------------------------------------
// MARK: OF BASICS
    
		void setup();
		void update();
		void draw();
        void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
// --------------------------------------------
// MARK: GRAPHIC INTERFACE DECLARATION
   
        void            drawGUI();
        ofxSimpleGuiToo gui;
        bool            isGUIActive;

// --------------------------------------------
// MARK: KINECT AND RELATED OBJECTS DECLARATION
    
        kinectCapture kinect;
        bool          bLockKinTilt;
        float         fKin1TiltAngle, fKin2TiltAngle;
    
        void drawKinectPointCloud();
    
// --------------------------------------------
// MARK: INTERFACE VARIABLES

        bool    bDrawDepthMap;
        bool    bDrawThreshold;
        bool    bDrawBlobs;
        
        int     iDrawWidth, iDrawHeight;
        int     iTopMargin, iLeftMargin;
        
        int     iMode;
        int     iNetDataSource;
        int     iLogLevel;
    
// --------------------------------------------
// MARK: CONTROL VARIABLES

        bool  bTwoKinects;
        int   iLeftKinectId, iRightKinectId;
        int   iKinectsOpeningTries;
        int   iFarThreshold, iNearThreshold;
        int   iMinBlobSize, iMaxBlobSize, iMaxNumBlobs;
        
        int   iFboAlpha;
        int   fPathRadius;

        bool  bResetData;
        bool  bRandomizeParticles;
        int   iGhostParticles;
        int   iMaxRandomParticles, iDeltaRandomParticles;
    
        float fProxFactor;
        float fMinParticleSize, fMaxParticleSize;
    
        float fAttractionVelocity;
        float fAttractorLife;
        float fAttractorRadius;
    
        int iMaxParticlesPerPanel;
        int iParticlesMultiplier;
    
// --------------------------------------------
// MARK: DATA

        ituitaData data;
        bool hasInitiated;
        XMLThread xmlThread;
    
    
// --------------------------------------------
// MARK: PARTICLES

        vector<Particle> streetParticles;
        vector<Particle> neighborhoodParticles;
        vector<Particle> cityParticles;
        
    
        ParticlesPath* streetPath;
        ParticlesPath* neighborhoodPath;
        ParticlesPath* cityPath;
    
        void initParticles();
        void initPaths();
        void addParticles(vector<Particle>&, int, ofColor, ParticlesPath&);    
        void runParticles(vector<Particle>&, ParticlesPath&);
    
        bool isMousePressed;
    
        map<int, Attractor> attractors;
    
        Attractor attractorStreetPanel;
        Attractor attractorNeighborhoodPanel;
        Attractor attractorCityPanel;
    
        bool doStreetPanelAttraction;
        bool doNeighborhoodPanelAttraction;
        bool doCityPanelAttraction;
    
    
// --------------------------------------------
// MARK: GRAPHICS

        ofFbo fbo;
        ofColor GREEN, YELLOW, RED, GRAY, HIGHLIGHT; 
        float f4Green[4], f4Yellow[4], f4Red[4], f4Gray[4], f4Highlight[4]; 
    
        void drawPanels(ofFbo fbo);

    
};


