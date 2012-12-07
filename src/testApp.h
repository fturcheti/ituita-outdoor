#pragma once

#include "ofMain.h"

#include "ofxSimpleGuiToo.h"

#include "kinectCapture.h"
#include "ituitaBlobTracker.h"
#include "ituitaData.h"
#include "Particle.h"
#include "ParticlesPath.h"


#define OUTPUT_SCREEN_W 576
#define OUTPUT_SCREEN_H 288
#define FBO_W 960
#define FBO_H 288

#define GREEN  0x00B653
#define YELLOW 0xFAEB34
#define RED    0xED2849
#define GRAY   0x666666


// ---------------------------------------------
// MARK: "USE TWO KINECTS" SWITCH (COMMENT TO USE JUST ONE)
// #define USE_TWO_KINECTS


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

        int   iMaxRandomParticles, iDeltaRandomParticles;
        bool  bResetData;
        
        float fProx;
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
        void addParticles(vector<Particle>&, int, int, ParticlesPath&);    
        void runParticles(vector<Particle>&, ParticlesPath&);
    
        bool isMousePressed;
    
// --------------------------------------------
// MARK: GRAPHICS

        ofFbo fbo;
    
        void drawPanels(ofFbo fbo);
    
};


