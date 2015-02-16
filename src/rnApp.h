#pragma once

#include "myDef.h"
#include "btBulletCollisionCommon.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"
#include "instancedComponent.h"
#include "collisionTester.h"
#include "rnTestCase.h"


class rnApp{  // : public ofBaseApp{
    
    static rnApp * singleton;
    
    friend class rnTestCase;
    rnTestCase testCase;
    
public:
    
    static rnApp * get(){ return singleton; }
    static void init(){
        if(!singleton){ singleton = new rnApp(); }
    }
    
    bool    bNowProcessing;

    float   compScale;
    float   posScale;
    float   boxSize;
    static string  posModelPath_P;
    
    ofShader*       mShdInstanced;
	ofEasyCam       camMain;
	ofLight         mLigDirectional;
	ofMaterial      mMatMainMaterial;


    static collisionTester * tester;

    instancedComponent spheres;
    instancedComponent cylinders;
    
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
    
    
    // element color
    static ofColor colorSphere;
    static ofColor colorCylinder;
    
    
    static int selectedSphere;
    static int selectedCylinder;
    
    // bg
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
    
    // shader uniform name
    static bool RENDER_NORMALS;
    static bool FLAT_SHADING;
    
    enum PROCESS_NAME{
        NO_PROCESS          = 0,
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
    
    static PROCESS_NAME CURRENT_PROCESS;


    
private:
    void setupCameraLightMaterial();
    void setupCylinderShape(float radius, int resolution, float collisionMargin);
    void setupSphereShape(float radius, int resolution, float collisionMargin);
    void setupShaders(bool doLink=true);

    
    void mainDraw();
    void testDraw();
    void waitDraw();
    
    void processGui();
    void doProcess();
    void requestProcess();
    
        //
        // rnApp_collision.cpp
        //
        void processCollision();
        static float getCollisionDistance(instance& insA, instance& insB);
        int collisionStart();
        void collisionEnd(int startTime);
        
        #if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
        void processCollisionParallel();
        #endif
    
    
    
        //
        // rnApp_connect.cpp
        //
        bool connectInstanace   (INSTANCE_MAP_ITR& itrA, INSTANCE_MAP_ITR& itrB, float minDist, float maxDist, instance& newIns);
        bool connectInstanace   (instance& insA, instance& insB, float minDist, float maxDist, instance& newIns);
        void connectRandom      (instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
        void connectGroup       (instancedComponent * ic, instancedComponent * ic2, int numAllCylinders, float minDist, float maxDist);
        void connectNear        (int numNearCylinders, float minDist=0, float maxDist=9999999999);
    
    
    ofSoundPlayer finishSound;
    void playFinishSound(){ finishSound.play(); }
};



#ifdef USE_TBB
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/concurrent_hash_map.h"

using namespace tbb;
using namespace std;

#endif

//struct MyHashCompare{
//
//};


#if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
typedef concurrent_hash_map<instance*, int> CollisionTable;
typedef CollisionTable::iterator CollisionTableItr;

struct Tally{
    CollisionTable& table;
    INSTANCE_MAP & instanceMap;
    
    Tally(CollisionTable& table_, INSTANCE_MAP& instanceMap_)
    : table(table_),
    instanceMap(instanceMap_)
    {}
    
    
    typedef pair<instance*, int> TPAIR;    
    
    void operator()( const blocked_range<size_t>& range) const {
        for( size_t i=range.begin(); i!=range.end(); ++i){
            INSTANCE_MAP_ITR itrA = instanceMap.begin();
            INSTANCE_MAP_ITR itrB = instanceMap.begin();
            
            std::advance(itrA, i);
            std::advance(itrB, i+1);
            
            for(; itrB!=instanceMap.end(); itrB++){
                instance& insA = itrA->second;
                instance& insB = itrB->second;
                
                float dist = rnApp::getCollisionDistance(insA, insB);
                
                
                if(dist<0.0) {
                    int groupIdA = itrA->first;
                    int groupIdB = itrB->first;
                    
                    //
                    //  check instance is in tempolaly container.
                    //  if existm, it sohuld have non -1 groupId.
                    //
                    CollisionTable::accessor a;
                    bool findA = table.find(a, &itrA->second);
                    if(findA)
                        groupIdA = a->second;

                    CollisionTable::accessor b;
                    bool findB = table.find(b, &itrB->second);
                    if(findB)
                       groupIdB = b->second;
                            
                            
                    //
                    // it's already same group.
                    // ** this line may break correct grouping process
                    //
                    if( (groupIdA!=-1 && groupIdB!=-1) && groupIdA == groupIdB)
                        continue;
                    
                    //
                    //  grouId -1 means this instance does not have any group
                    //
                    
                    if(groupIdA==-1){
                        if(groupIdB==-1){
                            // move instance A, B to new group
                            int newId = instancedComponent::incGroupIdMaster();
                            
                            table.insert(a, TPAIR(&itrA->second, newId));
                            table.insert(b, TPAIR(&itrB->second, newId));

                        }else{
                            // move A instance to B group
                            table.insert(a, TPAIR(&itrA->second, groupIdB));
                        }
                    }else{
                        if(groupIdB==-1){
                            // move B instance to A group
                            table.insert(b, TPAIR(&itrB->second, groupIdA));
                        }else{
                            // at first move B instance to A group
                            bool findB = table.find(b, &itrB->second);
                            b->second = groupIdA;
                            
                            // search from tmap
                            CollisionTableItr citr = table.begin();
                            
                            for(; citr!=table.end(); citr++){
                                if(citr->second == groupIdB){
                                    citr->second =groupIdA;
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
};
#endif
