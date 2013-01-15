#pragma once

#include "btBulletCollisionCommon.h"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"

#include "instancedComponent.h"
#include "collisionTester.h"



class testApp : public ofBaseApp{

	bool isShaderDirty;
    bool bWireframe;

    float compScale;
    float posScale;
    
    string posModelPath_P;
    
    ofShader* mShdInstanced;
	
	ofxVboMeshInstanced * mMshSphere;
   	ofxVboMeshInstanced * mMshCylinder;
    
	ofEasyCam       camMain;
	ofLight         mLigDirectional;
	ofMaterial      mMatMainMaterial;

	
    // GUI
	ofxPanel        mainPnl;
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
        void processGui();
    
        void setupCameraLightMaterial();
    
        instancedComponent spheres;
        instancedComponent cylinders;
    
    
    // operation func
    void connectRandom(instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
    void connectNear(int numNearCylinders, float minDist=0, float maxDist=9999999999);
    
    void makeGroup();
    void removeGroup();
    
    enum InstanceType{
        INSTANCE_SPHERE = 0,
        INSTANCE_CUBE = 1,
        INSTANCE_CYLINDER = 2
    };
    
    void checkIntersection(ofVec3f posA, ofVec3f posB, InstanceType type);
    
    
    static const string RENDER_NORMALS;
    static const string FLAT_SHADING;
    static const string CONNECT_RANDOM;
    static const string CONNECT_NEAR;
    static const string RESET_CYLINDERS;
    static const string RESET_SPHERES;
    static const string COLLISION_TEST;
    
    void updateShaders(bool doLink=true);
    
    // collision
    collisionTester tester;
    void processCollisionTest();
};
