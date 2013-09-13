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
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, FBO_W, FBO_H, OF_FULLSCREEN);
	ofRunApp(new testApp()); // start the app
}
