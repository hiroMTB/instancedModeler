#pragma once

#include "myDef.h"

#include "btBulletCollisionCommon.h"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"

#include "instancedComponent.h"
#include "collisionTester.h"




class testApp : public ofBaseApp{

    bool    bWireframe;
    bool    bCollisionDebugDraw;
    bool    bNowProcessing;

    float   compScale;
    float   posScale;
    
    string  posModelPath_P;
    
    ofShader*       mShdInstanced;
	ofEasyCam       camMain;
	ofLight         mLigDirectional;
	ofMaterial      mMatMainMaterial;


    static collisionTester * tester;

    instancedComponent spheres;
    instancedComponent cylinders;
    
    // GUI
    ofxPanel    pnlMain;
    ofxPanel    pnlRemove;
    ofxPanel    pnlCollision;
    ofxPanel    pnlConnectR, pnlConnectG;
    ofxPanel    pnlShape;
    
    
	map<string, ofxParameter<int> > prmInt;
	map<string, ofxParameter<float> > prmFloat;
	map<string, ofxParameter<bool> > prmBool;
    
    static string       CURRENT_PROCESS;
    static string       PROCESS_NAME[];

    static const string CONNECT_GROUP;
    static const string CONNECT_GROUP_MIN_DIST;
    static const string CONNECT_GROUP_MAX_DIST;
    static const string CONNECT_GROUP_CYLINDER_NUM;

    static const string CONNECT_RANDOM;
    static const string CONNECT_RANDOM_MIN_DIST;
    static const string CONNECT_RANDOM_MAX_DIST;
    static const string CONNECT_RANDOM_CYLINDER_NUM;

    static const string CONNECT_NEAR;
    
    static const string COLLISION_MARGIN;
    static const string COLLISION_TEST;

    static const string REMOVE_GROUPS;
    static const string REMOVE_GROUPS_MIN_NUM;
    static const string REMOVE_DUPLICATE;
    static const string REMOVE_ALL_CYLINDERS;
    
    static const string SPHERE_RADIUS;
    static const string SPHERE_RESOLUTION;
    static const string CYLINDER_RADIUS;
    static const string CYLINDER_RESOLUTION;
    static const string RESET_INSTSANCE_SHAPE;

    static const string DRAW_WIREFRAME;
    static const string DRAW_COLLISION_SHAPE;
    static const string DRAW_COLLISION_DISTANCE;
    static const string SAVE_DATA;

    // shader uniform name
    static const string RENDER_NORMALS;
    static const string FLAT_SHADING;


	
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    void setupGui();
    void setupCameraLightMaterial();
    void setupCylinderShape(float radius, int resolution);
    void setupSphereShape(float radius, int resolution);
    void setupShaders(bool doLink=true);

    void mainDraw();
    void testDraw();
    void waitDraw();
    
    bool guiMouseCheck(int x, int y);
    void processGui();
    void processRequest();

    
    
        //
        // testApp_collision.cpp
        //
        void processCollision();
        static float getCollisionDistance(instance& insA, instance& insB);
        int collisionStart();
        void collisionEnd(int startTime);
        
        #if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
        void processCollisionParallel();
        #endif
    
    
    
        //
        // testApp_connect.cpp
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
                
                float dist = testApp::getCollisionDistance(insA, insB);
                
                
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
