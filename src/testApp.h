#pragma once

#include "btBulletCollisionCommon.h"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"
#include "ofxAssimpModelLoader.h"

#include "instancedComponent.h"
#include "collisionTester.h"


//#define USE_TBB

class testApp : public ofBaseApp{

	bool isShaderDirty;
    bool bWireframe;
    bool bCollisionDebugDraw;
    
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
	map<string, ofxParameter<int> > prmInt;
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
    
   // void checkIntersection(ofVec3f posA, ofVec3f posB, InstanceType type);
    
    
    static const string RENDER_NORMALS;
    static const string FLAT_SHADING;
    static const string CONNECT_RANDOM;
    static const string CONNECT_NEAR;
    static const string RESET_CYLINDERS;
    static const string RESET_SPHERES;
    static const string COLLISION_TEST;
    static const string REMOVE_GROUPS;
    static const string REMOVE_GROUPS_MIN_NUM;
    
    void updateShaders(bool doLink=true);
    
    // collision
    static collisionTester tester;
    void processCollision();
    static float getCollisionDistance(instance& insA, instance& insB);

    int collisionStart();
    void collisionEnd(int startTime);
    
#ifdef USE_TBB
    void processCollisionParallel();
#endif
    
};



#ifdef USE_TBB
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/concurrent_hash_map.h"

using namespace tbb;
using namespace std;

//struct MyHashCompare{
//
//};



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
