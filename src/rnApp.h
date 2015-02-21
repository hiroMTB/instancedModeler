#pragma once

#include "btBulletCollisionCommon.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"
#include "instancedComponent.h"
#include "collisionTester.h"
#include "rnTestCase.h"

class rnApp{

public:
    
    static void init();
    void loadModelData();
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void saveCsvData(string path);
    void loadCsvData(string path);
    
    enum PROCESS_NAME{
        NO_PROCESS = 0,
        RESET_INSTSANCE_SHAPE,
        CONNECT_RANDOM,
        CONNECT_GROUP,
        CONNECT_NEAR,
        COLLISION_TEST,
        REMOVE_GROUPS,
        REMOVE_DUPLICATE,
        REMOVE_ALL_SPHERES,
        REMOVE_ALL_CYLINDERS,
        SAVE_CSV,
        LOAD_CSV,
        LOAD_MODEL_DATA
    };
    
    
    // Call from Cocoa GUI func
    void setupCameraLightMaterial();
    void setupCylinderShape(float radius, int resolution, float collisionMargin);
    void setupSphereShape(float radius, int resolution, float collisionMargin);
    void setupShaders(bool doLink=true);
    void mainDraw();
    void waitDraw();
    void processGui();
    void doProcess();
    void requestProcess();
    
        // rnApp_collision.cpp
        void processCollision();
        static float getCollisionDistance(instance& insA, instance& insB);
        int collisionStart();
        void collisionEnd(int startTime);
    
        // rnApp_connect.cpp
        void connectSelected();
        void connectInstance( int indexA, int indexB );
        bool connectInstanace   (INSTANCE_MAP_ITR& itrA, INSTANCE_MAP_ITR& itrB, float minDist, float maxDist, instance& newIns);
        bool connectInstanace   (instance& insA, instance& insB, float minDist, float maxDist, instance& newIns);
        void connectRandom      (instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
        void connectFloating    (instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
        void connectGroup       (instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
        void connectNear        (int numNearCylinders, float minDist=0, float maxDist=9999999999);
    
    static rnApp * app;
    static string  posModelPath_P;
    static ofMesh sphereMesh;
    static ofMesh cylinderMesh;
    static collisionTester * tester;
    static ofColor colorSphere;
    static ofColor colorCylinder;
    static int bgType;
    static ofColor bgNormal;
    static ofColor bgLinear0,   bgLinear1;
    static ofColor bgCircular0, bgCircular1;
    static ofColor bgBar0,      bgBar1;
    static float CONNECT_GROUP_MIN_DIST;
    static float CONNECT_GROUP_MAX_DIST;
    static int CONNECT_GROUP_CYLINDER_NUM;
    static float CONNECT_RANDOM_MIN_DIST;
    static float CONNECT_RANDOM_MAX_DIST;
    static int CONNECT_RANDOM_CYLINDER_NUM;
    static float COLLISION_MARGIN;
    static int REMOVE_GROUPS_MIN_NUM;
    static float SPHERE_RADIUS;
    static int SPHERE_RESOLUTION;
    static float SPHERE_COLLISION_MARGIN;
    static float CYLINDER_RADIUS;
    static int CYLINDER_RESOLUTION;
    static float CYLINDER_COLLISION_MARGIN;
    static bool DRAW_WIREFRAME;
    static bool DRAW_REFERENCE_BOX;
    static bool DRAW_COLLISION_SHAPE;
    static bool DRAW_COLLISION_DISTANCE;
    static int LOAD_MODEL_RESOLUTION;
    static bool LOAD_MODEL_WITH_NOISE_FILTER;
    static bool DRAW_SPHERE;
    static bool DRAW_CYLINDER;
    static bool RENDER_NORMALS;
    static bool FLAT_SHADING;
    static PROCESS_NAME CURRENT_PROCESS;
    
    bool    bMouseDragging;
    bool    bNowProcessing;
    int     mousePressedStartFrame;
    float   compScale;
    float   posScale;
    float   boxSize;
    
    ofShader*       mShdInstanced;
    ofEasyCam       camMain;
    ofLight         mLigDirectional;
    ofMaterial      mMatMainMaterial;
    instancedComponent  spheres;
    instancedComponent  cylinders;
    ofSoundPlayer       finishSound;
    friend class        rnTestCase;
    rnTestCase testCase;
    
};
