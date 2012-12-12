#pragma once

#include "ofMain.h"

#include "ofxSimpleGuiToo.h"

#include "kinectCapture.h"
#include "ituitaBlobTracker.h"
#include "ituitaData.h"
#include "Particle.h"
#include "ParticlesPath.h"
#include "Attractor.h"


#define OUTPUT_SCREEN_W 576
#define OUTPUT_SCREEN_H 288
//#define OUTPUT_SCREEN_W 1152
//#define OUTPUT_SCREEN_H 576
#define FBO_W (OUTPUT_SCREEN_W / 3) * 5
#define FBO_H OUTPUT_SCREEN_H


// ---------------------------------------------
// MARK: "USE TWO KINECTS" SWITCH (COMMENT TO USE JUST ONE)
#define USE_TWO_KINECTS


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
    
// --------------------------------------------
// MARK: CONTROL VARIABLES

        int   iLeftKinectId, iRightKinectId;
        int   iFarThreshold, iNearThreshold;
        int   iMinBlobSize, iMaxBlobSize, iMaxNumBlobs;
        
        int   iFboAlpha;
        int   fPathRadius;
        bool  bHighlightApproximation;

        int   iMaxRandomParticles, iDeltaRandomParticles;
        bool  bResetData;
        
        float fProxFactor;
        float fMinParticleSize, fMaxParticleSize;
        
// --------------------------------------------
// MARK: DATA

        ituitaData data;
        int personalData[3];
        int neighborhoodData[3];
        int cityData[3];
    
        void setupData();
    
// --------------------------------------------
// MARK: PARTICLES

        vector<Particle> personalParticles;
        vector<Particle> neighborhoodParticles;
        vector<Particle> cityParticles;
    
        ParticlesPath* personalPath;
        ParticlesPath* neighborhoodPath;
        ParticlesPath* cityPath;
    
        void initParticles();
        void initPaths();
        void addParticles(vector<Particle>&, int, ofColor, ParticlesPath&);    
        void runParticles(vector<Particle>&, ParticlesPath&);
    
        bool isMousePressed;
    
        map<int, Attractor> attractors;
        Attractor averageAttractor;
        bool doAttraction;
    
    
// --------------------------------------------
// MARK: GRAPHICS

        ofFbo fbo;
        ofColor GREEN, YELLOW, RED, GRAY, HIGHLIGHT; 
    
        void drawPanels(ofFbo fbo);
    
    
};


