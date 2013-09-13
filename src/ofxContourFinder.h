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

/*
* ContourFinder.h
*
* Finds white blobs in binary images and identifies
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of Blob objects.
*
* Created on 2/2/09.
* Adapted from openframeworks ofxCvContourFinder
*
*/

#ifndef OFXCONTOURFINDER
#define OFXCONTOURFINDER

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBlob.h"

#define TOUCH_MAX_CONTOURS			128
#define TOUCH_MAX_CONTOUR_LENGTH	1024

class ofxContourFinder {
public:

    ofxContourFinder();
    ~ofxContourFinder();
	
	int findContours(	ofxCvGrayscaleImage& input,
						int minArea,	int maxArea,
						int nConsidered,	double hullPress,
						bool bFindHoles,	bool bUseApproximation = true);
                       // approximation = don't do points for all points of the contour, if the contour runs
                       // along a straight line, for example...
	
    vector <ofxBlob>	blobs;      // the blobs, in a std::vector...
    int                 nBlobs;
	
protected:
    // this is stuff, not for general public to touch -- we need
    // this to do the blob detection, etc.
    ofxCvGrayscaleImage inputCopy;
    CvMemStorage*       contour_storage;
    CvMemStorage*       storage;
    CvMoments*          myMoments;

    
    
    // internally, we find cvSeqs, they will become blobs.
    int                 nCvSeqsFound;
    CvSeq*              cvSeqBlobs[TOUCH_MAX_CONTOURS];

    // imporant!!
    void                reset();
	float               width, height;
};
#endif
