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
//  ituitaData.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 8/15/12.
//  

#ifndef ituita_outdoor_ituitaData_h
#define ituita_outdoor_ituitaData_h

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define NEGATIVE 0
#define NEUTRAL  1
#define POSITIVE 2

class ituitaData {
    
    public:
    
        ituitaData();
        
        int getStreetNegatives();
        int getStreetNeutrals();
        int getStreetPositives();
    
        int getNeighborhoodNegatives();
        int getNeighborhoodNeutrals();
        int getNeighborhoodPositives();
        
        int getCityNegatives();
        int getCityNeutrals();
        int getCityPositives();

        int getNewStreetNegatives();
        int getNewStreetNeutrals();
        int getNewStreetPositives();

        int getNewNeighborhoodNegatives();
        int getNewNeighborhoodNeutrals();
        int getNewNeighborhoodPositives();

        int getNewCityNegatives();
        int getNewCityNeutrals();
        int getNewCityPositives();

        void generateRandomValues(int min, int max);
        void getResultsFromBuffer(string buffer);
    
        // controllers for the number of particles shown
        int maxParticlesPerPanel;
        int particlesMultiplier;
    
    private:
    
        void initDataArrays();
        void loadRegionData(int *old, int *current, int pos, int neu, int neg);
    
        // DATA ARRAYS
        int streetData[3]; 
        int neighborhoodData[3];
        int cityData[3];
        // DATA ARRAYS to keep track of modified data
        int oldStreetData[3];
        int oldNeighborhoodData[3];
        int oldCityData[3];
    
        ofxXmlSettings xml;
    
};

#endif