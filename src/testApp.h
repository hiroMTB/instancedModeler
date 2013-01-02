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
    
	ofEasyCam	mCamMain;
	
	ofLight	mLigDirectional;
	ofMaterial mMatMainMaterial;
	
	ofxPanel	mPnlMain;
	map<string, ofxParameter<float> > mPrmFloat;
	map<string, ofxParameter<bool> > mPrmBool;
	
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
    
    
    //
    float * createPositionTexture(GLuint *texId);
    void updatePositionTexture(GLuint *texId, ofMatrix4x4 matrices[], ofVec3f trans, float _posScale);
    void setPositionTexture(string modelPath, GLuint *texId, ofVec3f trans, float _posScale);
    
    
    int loadPositionFromModel(string modelPath, ofVec3f trans, float _posScale, GLuint * texId);
    ofMatrix4x4 loadMatrixFromModel();
    
    ofxVboMeshInstanced * loadComponent(ofMesh component, float scale);

    
    ofMesh getOFSphere();
    
    ofMesh createCylinder(float radius, float height, int slices, int stacks);
    
    instancedComponent ic;
    
    
};
