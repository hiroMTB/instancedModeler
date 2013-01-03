#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"
#include "instancedComponent.h"

class testApp : public ofBaseApp{

	bool isShaderDirty;
    bool bWireframe;

    float compScale;
    float posScale;
    
    GLuint matLoc;
    GLuint texLoc;
    
    GLuint texId_P;
    GLuint texId_L;
    
    string posModelPath_P;
    string posModelPath_L;
    
    ofShader* mShdInstanced;
    ofShader* mShdInstanced_L;
	
	ofxVboMeshInstanced * mMshSphere;
   	ofxVboMeshInstanced * mMshCylinder;
    
	ofEasyCam	camMain;
	
	ofLight	mLigDirectional;
	ofMaterial mMatMainMaterial;

	
    // GUI
	ofxPanel	mainPnl;
	map<string, ofxParameter<float> > prmFloat;
	map<string, ofxParameter<bool> > prmBool;
	
	public:
		void setup();
		void update();
		void draw();
        void mainDraw();
        void testDraw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void setupGui();
    
        void setupCameraLightMaterial();
    
        instancedComponent ic;
    
    
};
