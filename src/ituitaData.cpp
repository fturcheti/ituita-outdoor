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

//
//  ituitaData.cpp
//  ituita-outdoor
//
//  Created by Felipe Turcheti on 8/15/12.
//

#include "ituitaData.h"

ituitaData::ituitaData() {
    initDataArrays();
    
    maxParticlesPerPanel = 500;
    particlesMultiplier = 2;
}

void ituitaData::initDataArrays() {
    for(int i = 0; i < 3; i++) {
        streetData[i]       = 0;
        neighborhoodData[i] = 0;
        cityData[i]         = 0;
        
        oldStreetData[i]       = 0;
        oldNeighborhoodData[i] = 0;
        oldCityData[i]         = 0;
    }
}

void ituitaData::generateRandomValues(int min, int max) {
    for(int i = 0; i < 3; i++) {
        streetData[i] = ofRandom(min, max);
        neighborhoodData[i] = ofRandom(min, max);
        cityData[i] = ofRandom(min, max);
    }
}

void ituitaData::getResultsFromBuffer(string buffer) {
    xml.loadFromBuffer(buffer);
    
    xml.pushTag("resultados");
    
        xml.pushTag("rua");
        loadRegionData(oldStreetData, streetData,
                       xml.getValue("positivo", 0),
                       xml.getValue("neutro", 0),
                       xml.getValue("negativo", 0));
        xml.popTag();
        
        xml.pushTag("bairro");
        loadRegionData(oldNeighborhoodData, neighborhoodData,
                       xml.getValue("positivo", 0),
                       xml.getValue("neutro", 0),
                       xml.getValue("negativo", 0));
        xml.popTag();
        
        xml.pushTag("cidade");
        loadRegionData(oldCityData, cityData,
                       xml.getValue("positivo", 0),
                       xml.getValue("neutro", 0),
                       xml.getValue("negativo", 0));
        xml.popTag();
    
    xml.popTag();
}

void ituitaData::loadRegionData(int *old, int *current, int pos, int neu, int neg) {
    old[POSITIVE] = current[POSITIVE];
    old[NEUTRAL]  = current[NEUTRAL];
    old[NEGATIVE] = current[NEGATIVE];
    
    int sum = pos+neu+neg;
    int limit = maxParticlesPerPanel / particlesMultiplier;
    
    if(sum > limit) {
        pos = ofMap(pos, 0, sum, 0, limit);
        neu = ofMap(neu, 0, sum, 0, limit);
        neg = ofMap(neg, 0, sum, 0, limit);
    }
    current[POSITIVE] = pos * particlesMultiplier;
    current[NEUTRAL]  = neu * particlesMultiplier;
    current[NEGATIVE] = neg * particlesMultiplier;
}

// --------------------------------------------
// MARK: STREET DATA

int ituitaData::getStreetNegatives() {
    return streetData[NEGATIVE];
}
int ituitaData::getStreetNeutrals() {
    return streetData[NEUTRAL];
}
int ituitaData::getStreetPositives() {
    return streetData[POSITIVE];
}

int ituitaData::getNewStreetNegatives() {
    return streetData[NEGATIVE] - oldStreetData[NEGATIVE];
}
int ituitaData::getNewStreetNeutrals() {
    return streetData[NEUTRAL] - oldStreetData[NEUTRAL];
}
int ituitaData::getNewStreetPositives() {
    return streetData[POSITIVE] - oldStreetData[POSITIVE];
}


// --------------------------------------------
// MARK: NEIGHBOORHOOD DATA

int ituitaData::getNeighborhoodNegatives() {
    return neighborhoodData[NEGATIVE];
}
int ituitaData::getNeighborhoodNeutrals() {
    return neighborhoodData[NEUTRAL];
}
int ituitaData::getNeighborhoodPositives() {
    return neighborhoodData[POSITIVE];
}

int ituitaData::getNewNeighborhoodNegatives() {
    return neighborhoodData[NEGATIVE] - oldNeighborhoodData[NEGATIVE];
}
int ituitaData::getNewNeighborhoodNeutrals() {
    return neighborhoodData[NEUTRAL] - oldNeighborhoodData[NEUTRAL];
}
int ituitaData::getNewNeighborhoodPositives() {
    return neighborhoodData[POSITIVE] - oldNeighborhoodData[POSITIVE];
}

// --------------------------------------------
// MARK: CITY DATA

int ituitaData::getCityNegatives() {
    return cityData[NEGATIVE];
}
int ituitaData::getCityNeutrals() {
    return cityData[NEUTRAL];
}
int ituitaData::getCityPositives() {
    return cityData[POSITIVE];
}

int ituitaData::getNewCityNegatives() {
    return cityData[NEGATIVE] - oldCityData[NEGATIVE];
}
int ituitaData::getNewCityNeutrals() {
    return cityData[NEUTRAL] - oldCityData[NEUTRAL];
}
int ituitaData::getNewCityPositives() {
    return cityData[POSITIVE] - oldCityData[POSITIVE];
}


