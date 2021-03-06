////////////////////////////////////////////////////////////
//                                                        //
// This code is part of "ituita-outdoor", an interactive  //
// software developed by Andr� Mintz and Felipe Turcheti, //
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

//
//  kinectCapture.cpp
//  ituita-outdoor
//
//  Created by Andr� Goes Mintz on 7/11/12.
//

#include "kinectCapture.h"
#include <iostream>

kinectCapture::kinectCapture() {
    
    //SETUP NORMALIZATION TABLES
    
    for (int i = 0 ; i < 640 ; i++) {
        norm640[i] = ofNormalize(i, 0, 640);
    }
    
    for (int i = 0; i < 480; i++) {
        norm480[i] = ofNormalize(i, 0, 480);
    }
    
    for (int i = 0; i < 960; i++) {
        norm960[i] = ofNormalize(i, 0, 960);
    }
    
    for (int i = 0; i < 4000; i++) {
        norm4000[i] = ofNormalize(i, 0, 4000);
    }

}

kinectCapture::~kinectCapture() {
    close();
}

void kinectCapture::setup(bool _bTwoKinects, int _iOpeningTries) {
    setup(_bTwoKinects, 0, 1, _iOpeningTries);
}

void kinectCapture::setup(bool _bTwoKinects, int _iLeftKinectId, int _iRightKinectId, int _iOpeningTries) {
    
    
    bTwoKinects = _bTwoKinects;
    
    bMovementDetection = true;
    
    // SETUP KINECT ONE
    
    fKin1Angle = 0;
    
    bool success = false;
    int counter = 0;
    
    kinect1.init(false, false);
    kinect1.open(_iLeftKinectId);
    
    if(bTwoKinects) {
        fKin2Angle = 0;
        
        kinect2.init(false, false);
        kinect2.open(_iRightKinectId);
    
    }
    
    kinect1.update();
    kinect1.setCameraTiltAngle(0);
    kinect1.close();
    
    if(bTwoKinects) {
        kinect2.update();
        kinect2.setCameraTiltAngle(0);
        kinect2.close();
    }
    
    kinect1.init(false, false);
    success = kinect1.open(_iLeftKinectId);
    
    while (!success && counter < _iOpeningTries) {
        cout << "Problems found in connecting with Kinect 1. Trying again!" << endl;
        kinect1.close();
        kinect1.init(false, false);
        success = kinect1.open(_iLeftKinectId);
        counter++;
    }
    
    kinect1.setCameraTiltAngle(fKin1Angle);
    cvGrayKin1.allocate(640, 480);
    cvGrayKin1Prev.allocate(640,480);
    
    bKin1Refreshed = false;
    
    //IF USING TWO KINECTS, SETUP KINECT TWO
	
    if(bTwoKinects) {
    
        fKin2Angle = 0;
        
        success = false;
        counter = 0;
        
        kinect2.init(false, false);
        success = kinect2.open(_iRightKinectId);
        
        kinect2.update();
        kinect2.close();
        
        kinect2.init(false, false);
        
        success = kinect2.open(_iRightKinectId);
        
        while (!success && counter < _iOpeningTries) {
            cout << "Problems found in connecting with Kinect 2. Trying again!" << endl;
            kinect2.close();
            kinect2.init(false, false);
            success = kinect2.open(_iRightKinectId);
            counter++;
        }
        
        kinect2.setCameraTiltAngle(fKin2Angle);
        cvGrayKin2.allocate(640, 480);
        cvGrayKin2Prev.allocate(640, 480);
        
        bKinectsStarted = false;
        bKin2Refreshed = false;
        
    }
    
    iNearThreshold  = 0;
    iFarThreshold   = 255;
    iMinBlobSize    = 20;
    iMaxBlobSize    = 20000;
    iMaxNumBlobs    = 10;

}

void kinectCapture::update() {
    
    if(bTwoKinects && !bKinectsStarted) {
        kinect1.update();
        kinect2.update();
        if (kinect1.isFrameNew() && kinect2.isFrameNew()) {
            bKinectsStarted = true;
            return;
        }
        else {
            return;
        }
    }
    
    kinect1.update();
    bKin1Refreshed = false;
    
    // IF KINECT ONE FRAME IS NEW ---------------------------------
    
    if (kinect1.isFrameNew()) {
        
        bKin1Refreshed = true;
        
        // DO: UPDATE ALL CV STUFF
        
        if (bMovementDetection) {
            cvGrayKin1Prev = cvGrayKin1;
        }
    
        cvGrayKin1.setFromPixels(kinect1.getDepthPixels(), kinect1.width, kinect1.height);
        
        if (bMovementDetection) {
            kin1BlobTracker.update(cvGrayKin1, cvGrayKin1Prev, iNearThreshold, iFarThreshold,iMinBlobSize, iMaxBlobSize, iMaxNumBlobs, 20, false, true);
        }
        else {
            kin1BlobTracker.update(cvGrayKin1, iNearThreshold, iFarThreshold, iMinBlobSize, iMaxBlobSize, iMaxNumBlobs, 20, false, true);
        }
        
        kin1FoundBlobs.clear();
        
        // IF THERE ARE BLOBS -------------------------------------
        
        if (kin1BlobTracker.size() > 0) {
         
         // DO: UPDATE ALL BLOB STUFF
         
             for (int i = 0; i < kin1BlobTracker.trackedBlobs.size(); i++) {
                 if(kin1BlobTracker.trackedBlobs[i].pts.size() > 0) {
             
                     kin1FoundBlobs.push_back(ofxBlob());
             
                     kin1FoundBlobs[i].id = -1 * kin1BlobTracker.trackedBlobs[i].id;
                     
                     kin1FoundBlobs[i].angle = kin1BlobTracker.trackedBlobs[i].angle;
                     kin1FoundBlobs[i].maccel = kin1BlobTracker.trackedBlobs[i].maccel;
             
                     kin1FoundBlobs[i].centroid.x = setInRangeWidth(kin1BlobTracker.trackedBlobs[i].centroid.x, bTwoKinects, false);
                     kin1FoundBlobs[i].centroid.y = kin1BlobTracker.trackedBlobs[i].centroid.y;
                     kin1FoundBlobs[i].boundingRect.x = setInRangeWidth(kin1BlobTracker.trackedBlobs[i].boundingRect.x, bTwoKinects, false);
                     kin1FoundBlobs[i].boundingRect.y = kin1BlobTracker.trackedBlobs[i].boundingRect.y;
                     kin1FoundBlobs[i].boundingRect.width = setInRangeWidth(kin1BlobTracker.trackedBlobs[i].boundingRect.width, bTwoKinects, false);
                     kin1FoundBlobs[i].boundingRect.height =kin1BlobTracker.trackedBlobs[i].boundingRect.height;
                    
                     kin1FoundBlobs[i].angleBoundingRect.x = setInRangeWidth(kin1BlobTracker.trackedBlobs[i].angleBoundingRect.x, bTwoKinects, false);
                     kin1FoundBlobs[i].angleBoundingRect.y = kin1BlobTracker.trackedBlobs[i].angleBoundingRect.y;
                     kin1FoundBlobs[i].angleBoundingRect.width = setInRangeWidth(kin1BlobTracker.trackedBlobs[i].angleBoundingRect.width, bTwoKinects, false);
                     kin1FoundBlobs[i].angleBoundingRect.height =kin1BlobTracker.trackedBlobs[i].angleBoundingRect.height;
                     
                     for (int j = 0; j < kin1BlobTracker.trackedBlobs[i].pts.size(); j++) {
                         kin1FoundBlobs[i].pts.push_back(ofPoint(setInRangeWidth(kin1BlobTracker.trackedBlobs[i].pts[j].x, bTwoKinects, false),kin1BlobTracker.trackedBlobs[i].pts[j].y));
                     }
                 }
             }
         }
        
        // END IF THERE ARE BLOBS ---------------------------------
        
    }
    
    // ENDIF KINECT ONE FRAME IS NEW

    // IF USING TWO KINECTS ---------------------------------------
    
    if (bTwoKinects) {
        
        kinect2.update();
        bKin2Refreshed = false;
        
        // IF KINECT TWO FRAME IS NEW -----------------------------
        
        if (kinect2.isFrameNew()) {
            
            bKin2Refreshed = true;
            
            // DO: UPDATE ALL CV STUFF
            
            if(bMovementDetection) {
                cvGrayKin2Prev = cvGrayKin2;
            }
            cvGrayKin2.setFromPixels(kinect2.getDepthPixels(), kinect2.width, kinect2.height);
            
            if(bMovementDetection) {
                kin2BlobTracker.update(cvGrayKin2, cvGrayKin2Prev,iNearThreshold, iFarThreshold, iMinBlobSize, iMaxBlobSize, iMaxNumBlobs, 20, false, true);
            }
            else {
                kin2BlobTracker.update(cvGrayKin2, iNearThreshold, iFarThreshold, iMinBlobSize, iMaxBlobSize, iMaxNumBlobs, 20, false, true);
            }
                        
            
            kin2FoundBlobs.clear();
            
            // IF THERE ARE BLOBS ---------------------------------
            
            if (kin2BlobTracker.size() > 0) {
                
                // DO: UPDATE ALL BLOB STUFF
                
                for (int i = 0; i < kin2BlobTracker.trackedBlobs.size(); i++) {
                    if(kin2BlobTracker.trackedBlobs[i].pts.size() > 0) {
                        
                        kin2FoundBlobs.push_back(ofxBlob());
                        
                        kin2FoundBlobs[i].id = kin2BlobTracker.trackedBlobs[i].id;
                        
                        kin2FoundBlobs[i].angle = kin2BlobTracker.trackedBlobs[i].angle;
                        kin2FoundBlobs[i].maccel = kin2BlobTracker.trackedBlobs[i].maccel;
                        
                        kin2FoundBlobs[i].centroid.x = setInRangeWidth(kin2BlobTracker.trackedBlobs[i].centroid.x, true, true);
                        kin2FoundBlobs[i].centroid.y = kin2BlobTracker.trackedBlobs[i].centroid.y;
                        kin2FoundBlobs[i].boundingRect.x = setInRangeWidth(kin2BlobTracker.trackedBlobs[i].boundingRect.x, true, true);
                        kin2FoundBlobs[i].boundingRect.y = kin2BlobTracker.trackedBlobs[i].boundingRect.y;
                        kin2FoundBlobs[i].boundingRect.width = setInRangeWidth(kin2BlobTracker.trackedBlobs[i].boundingRect.width, true, false);
                        kin2FoundBlobs[i].boundingRect.height =kin2BlobTracker.trackedBlobs[i].boundingRect.height;
                       
                        kin2FoundBlobs[i].angleBoundingRect.x = setInRangeWidth(kin2BlobTracker.trackedBlobs[i].angleBoundingRect.x, true, true);
                        kin2FoundBlobs[i].angleBoundingRect.y = kin2BlobTracker.trackedBlobs[i].angleBoundingRect.y;
                        kin2FoundBlobs[i].angleBoundingRect.width = setInRangeWidth(kin2BlobTracker.trackedBlobs[i].angleBoundingRect.width, true, false);
                        kin2FoundBlobs[i].angleBoundingRect.height =kin2BlobTracker.trackedBlobs[i].angleBoundingRect.height;
                        
                        
                        for (int j = 0; j < kin2BlobTracker.trackedBlobs[i].pts.size(); j++) {
                            kin2FoundBlobs[i].pts.push_back(ofPoint(setInRangeWidth(kin2BlobTracker.trackedBlobs[i].pts[j].x, true, true), kin2BlobTracker.trackedBlobs[i].pts[j].y));
                        }
                    }
                }
            }
            
            // ENDIF THERE ARE BLOBS ------------------------------
                   
        }
        
        // ENDIF KINECT TWO FRAME IS NEW --------------------------
        
        // IF EITHER KINECT FRAME IS NEW --------------------------
        
        if (bKin1Refreshed || bKin2Refreshed) {
            
            // DO: ASSIGN NEW BLOBS TO <FOUND BLOBS>
            
            foundBlobs.clear();
            foundBlobs = kin1FoundBlobs;
            foundBlobs.insert(foundBlobs.end(), kin2FoundBlobs.begin(), kin2FoundBlobs.end());
            
            activeBlobsIds.clear();
            foundBlobsMap.clear();
            
            for (int i = 0; i < kin1FoundBlobs.size(); i++) {
                foundBlobsMap[kin1FoundBlobs[i].id] = kin1FoundBlobs[i];
                activeBlobsIds.push_back(kin1FoundBlobs[i].id);
            }
            for (int i = 0; i < kin2FoundBlobs.size(); i++) {
                foundBlobsMap[kin2FoundBlobs[i].id] = kin2FoundBlobs[i];
                activeBlobsIds.push_back(kin2FoundBlobs[i].id);
            }
            
            
            // DO: ASSIGN NEW CLOUD TO <POINT CLOUD>
            pointCloud.clear();
                     
            for (int y = 0; y < KIN_H; y++) {
                for (int x = KIN_OUTPUT_W; x > 0; x--) {
                    if ( x <= ( KIN_OUTPUT_W/2 ) ) {
                        pointCloud.push_back(ofPoint(normWidth(x, true), normHeight(y), normDepth((int)kinect1.getDistanceAt(x, y))));
                    }
                    else {
                        pointCloud.push_back(ofPoint(normWidth(x, true), normHeight(y), normDepth((int)kinect2.getDistanceAt(x - KIN2_INTERS_W, y))));
                    }
                }
            }
            
        }
        
        // ENDIF EITHER KINECT FRAME IS NEW -----------------------
    
    }
    
    // ELSE (NOT USING TWO KINECTS) -------------------------------
    
    else {
        
        // IF KINECT ONE FRAME IS NEW
        
        if (bKin1Refreshed) {
            
            // DO: ASSIGN NEW BLOBS TO <FOUND BLOBS>
            
            foundBlobs.clear();
            foundBlobs = kin1FoundBlobs;
            
            foundBlobsMap.clear();
            activeBlobsIds.clear();
            
            for (int i = 0; i < kin1FoundBlobs.size(); i++) {
                foundBlobsMap[kin1FoundBlobs[i].id] = kin1FoundBlobs[i];
                activeBlobsIds.push_back(kin1FoundBlobs[i].id);
            }
            
            // DO: ASSIGN NEW CLOUD TO <POINT CLOUD>
            
            pointCloud.clear();
            
            for (int y = 0; y < KIN_H; y++) {
                for (int x = KIN_W; x > 0; x--) {
                    pointCloud.push_back(ofPoint(normWidth(x, false), normHeight(y), normDepth((int)kinect1.getDistanceAt(x,y))));
                }
            }
            
        }
        
    }
    
    // ENDIF USING TWO KINECTS ------------------------------------
}

void kinectCapture::updateThreshPar(int _iFarThreshold, int _iNearThreshold) {
    
    iFarThreshold = _iFarThreshold;
    iNearThreshold= _iNearThreshold;
}

void kinectCapture::updateBlobPar(int _iMinBlobSize, int _iMaxBlobSize, int _iMaxNumBlobs) {
    
    iMinBlobSize = _iMinBlobSize;
    iMaxBlobSize = _iMaxBlobSize;
    iMaxNumBlobs = _iMaxNumBlobs;
}

void kinectCapture::drawDepth(int x, int y, int w, int h, bool kin2) {
    
    if (!kin2) {
        kinect1.drawDepth(x, y, w, h);
    }
    else if (bTwoKinects) {
        kinect2.drawDepth(x, y, w, h);
    }
    
}

void kinectCapture::drawThreshImg(int x, int y, int w, int h, bool kin2) {
    
    if (!kin2) {
        cvGrayKin1.draw(x, y, w, h);
    }
    else if(bTwoKinects) {
        cvGrayKin2.draw(x, y, w, h);
    }
    
}

void kinectCapture::drawContour(int x, int y, int w, int h, bool kin2) {
    
    if (!kin2) {
        kin1BlobTracker.draw(x, y, w, h);
    }
    else if(bTwoKinects) {
        kin2BlobTracker.draw(x, y, w, h);
    }
    
}

void kinectCapture::drawNormBlobs(int x, int y, int w, int h){
    
    ofPushMatrix();
    ofTranslate(x, y);
    
    for (int i = 0; i < activeBlobsIds.size(); i++) {
        ofSetColor(0, 255, 0);
        ofBeginShape();
        for (int j = 0; j < foundBlobsMap[activeBlobsIds[i]].pts.size(); j++) {
            ofVertex(foundBlobsMap[activeBlobsIds[i]].pts[j].x * (float)w, foundBlobsMap[activeBlobsIds[i]].pts[j].y * (float)h);
        }
        ofEndShape();
        ofSetColor(0, 0, 255);
        
        // DRAW MINIMAL SIZED ANGLED BOUNDING RECT
        ofPushMatrix();
        ofTranslate(foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.x * w, foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.y * h);
        ofRotate(foundBlobsMap[activeBlobsIds[i]].angle+90, 0.0f, 0.0f, 1.0f);
        ofTranslate(-(foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.x * w), -(foundBlobs[i].angleBoundingRect.y * h));                
        ofNoFill();
        
        ofPushStyle();
        ofNoFill();
        ofRect((foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.x - foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.width/2) * w, (foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.y - foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.height/2) * h, foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.width * w, foundBlobsMap[activeBlobsIds[i]].angleBoundingRect.height * h);
        
        ofPopStyle();
        ofPopMatrix();        
        
        ofSetColor(255, 0, 0);
        ofDrawBitmapString(ofToString(foundBlobsMap[activeBlobsIds[i]].id), foundBlobsMap[activeBlobsIds[i]].centroid.x*(float)w, foundBlobsMap[activeBlobsIds[i]].centroid.y*(float)h);
    }
    
    ofPopMatrix();
    
}

void kinectCapture::drawDepthFromCloud(int x, int y, int w, int h, int inc) {
    
    ofPushMatrix();
    ofTranslate(x,y);
    
    for (int i = 0; i < pointCloud.size(); i+=inc) {
        ofSetColor( pointCloud[i].z * (float)255 );
        ofFill();
        ofCircle( pointCloud[i].x * (float)w, pointCloud[i].y * (float)h, inc/2);

        // add the increment to the height
        if(i % getOutputWidth() == getOutputWidth()-inc) i += inc * getOutputWidth();
    }
    
    ofPopMatrix();
    
}

void kinectCapture::close() {
    
    kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
    
    kinect2.setCameraTiltAngle(0);
	kinect2.close();
    
}

float kinectCapture::normWidth(int val, bool _bTwoKinects) {
    
    if(_bTwoKinects && val < 960) {
        return norm960[val];
    }
    else if(val < 640) {
        return norm640[val];
    }

}

float kinectCapture::normHeight(int val) {
    
    if (val < 480) {
        return norm480[val];
    }
}

float kinectCapture::normDepth(int val) {
    
    if(val < 4000) {
        return norm4000[val];
    }    
    
}

float kinectCapture::setInRangeWidth(float val, bool _bTwoKinects, bool isKinect2) {
    
    if (_bTwoKinects && !isKinect2) {
        return (val * 640) / 960;
    }
    else if (_bTwoKinects && isKinect2) {
        return ((val * 640) + 320) / 960;
    }
    else {
        return val;
    }
    
}

bool kinectCapture::isTwoKinects() {
    return bTwoKinects;
}

int kinectCapture::getOutputWidth() {
    if(bTwoKinects) return KIN_OUTPUT_W;
    return KIN_W;
}

int kinectCapture::getOutputHeight() {
    return KIN_H;
}

void kinectCapture::setKinTiltAngle(bool bKinect2, float angle) {
    if (bKinect2 && bTwoKinects) {
        if (angle > 30) {
            angle = 30;
        }
        else if (angle < -30) {
            angle = -30;
        }
        kinect2.setCameraTiltAngle(angle);
    }
    else {
        if (angle > 30) {
            angle = 30;
        }
        else if (angle < -30) {
            angle = -30;
        }
        kinect1.setCameraTiltAngle(angle);
    }
}
