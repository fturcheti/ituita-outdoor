//
//  ituitaData.h
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 8/15/12.
//  

#ifndef ituita_outdoor_ituitaData_h
#define ituita_outdoor_ituitaData_h

#include "ofMain.h"

#define NEGATIVE 0
#define NEUTRAL  1
#define POSITIVE 2

class ituitaData {
    
    public:
    
        ituitaData();
        
        int getPersonalNegatives();
        int getPersonalNeutrals();
        int getPersonalPositives();
        
        int getNeighborhoodNegatives();
        int getNeighborhoodNeutrals();
        int getNeighborhoodPositives();
        
        int getCityNegatives();
        int getCityNeutrals();
        int getCityPositives();
        
        void generateRandomValues(int min, int max);
    
    private:
    
        void initDataArrays();
        
        // DATA ARRAYS
        int personalData[3]; 
        int neighborhoodData[3];
        int cityData[3];   
    
};

#endif