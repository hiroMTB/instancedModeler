//
//  collision.cpp
//  instancedModeler
//
//  Created by matobahiroshi on 13/01/25.
//
//

#include "testApp.h"


float testApp::getCollisionDistance(instance &insA, instance &insB){
    float dist=99999;
    
    ofMatrix4x4& matA = insA.matrix;
    ofVec3f& sA = insA.scale;
    INSTANCE_TYPE tA = insA.type;
    
    ofMatrix4x4& matB = insB.matrix;
    ofVec3f& sB = insB.scale;
    INSTANCE_TYPE tB = insB.type;
    
    if(tA == INSTANCE_SPHERE && tB==INSTANCE_SPHERE)
        dist = tester->testSphereSphere(matA,sA,matB,sB);
    else if(tA==INSTANCE_SPHERE && tB==INSTANCE_CYLINDER)
        dist = tester->testSphereCylinder(matA,sA,matB,sB);
    else if (tA==INSTANCE_CYLINDER && tB==INSTANCE_SPHERE)
        dist = tester->testSphereCylinder(matB,sB, matA,sA);
    else if(tA==INSTANCE_CYLINDER && tB==INSTANCE_CYLINDER)
        dist = tester->testCylinderCylinder(matA,sA,matB,sB);
    else{
        myLogRelease("Can not test collision");
        return 98765432;
    }
    return dist;
    
}

void testApp::processCollision(){
#if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
    processCollisionParallel();
#else
    
    int startTime = collisionStart();
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    INSTANCE_MAP_ITR itrA = instanceMap.begin();
    
    typedef map<instance*, int> TMAP;
    typedef pair<instance*, int> TPAIR;
    typedef TMAP::iterator TITR;
    TMAP tmap;
    
    char m[255];
    for(int i=0; itrA!=instanceMap.end(); itrA++, i++){
        //sprintf(m, "processCollisionTest i:%d", i);
        //myLogDebug(ofToString(m));
        
        INSTANCE_MAP_ITR itrB = instanceMap.begin();
    std:advance(itrB, i+1);
        for(; itrB!=instanceMap.end(); itrB++){
            instance& insA = itrA->second;
            instance& insB = itrB->second;
            
            float dist=getCollisionDistance(insA, insB);
            
            if(dist<0.0) {
                int groupIdA = itrA->first;
                int groupIdB = itrB->first;
                
                //
                //  check instance is in tempolaly container.
                //  if existm, it sohuld have non -1 groupId.
                //
                TITR titrA = tmap.find(&itrA->second);
                if(titrA!=tmap.end())
                    groupIdA = titrA->second;
                
                TITR titrB = tmap.find(&itrB->second);
                if(titrB!=tmap.end())
                    groupIdB = titrB->second;
                
                
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
                        tmap.insert(TPAIR(&itrA->second, newId));
                        tmap.insert(TPAIR(&itrB->second, newId));
                    }else{
                        // move A instance to B group
                        tmap.insert(TPAIR(&itrA->second, groupIdB));
                    }
                }else{
                    if(groupIdB==-1){
                        // move B instance to A group
                        tmap.insert(TPAIR(&itrB->second, groupIdA));
                    }else{
                        // at first move B instance to A group
                        TITR titrB = tmap.find(&itrB->second);
                        titrB->second = groupIdA;
                        
                        // search from tmap
                        TITR titr = tmap.begin();
                        
                        for(; titr!=tmap.end(); titr++){
                            if(titr->second == groupIdB){
                                titr->second =groupIdA;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // change group id
    {
        TITR itr = tmap.begin();
        for(; itr!=tmap.end(); itr++){
            instance * ins = itr->first;
            int groupId = itr->second;
            
            if(ins!=NULL){
                INSTANCE_MAP_ITR  iitr = instanceMap.begin();
                for(; iitr!=instanceMap.end(); iitr++){
                    if(&iitr->second == ins){
                        spheres.changeInstanceGroupId(iitr, groupId);
                        
                        break;
                    }
                }
            }
        }
    }
    
    collisionEnd(startTime);
#endif
}

int testApp::collisionStart(){
    instancedComponent::resetGroup();
    
    int startTime = ofGetElapsedTimeMillis();
    myLogRelease("collisionTest startTime:  "+ ofToString(startTime));
    
#ifndef NDEBUG
    instancedComponent::printData();
#endif
    return startTime;
    
}

void testApp::collisionEnd(int startTime){
    // update group totalNum
    instancedComponent::updateGroupTotalNum();
    
    // coloring
    ofColor c;
    spheres.setGroupColorGradient();
    spheres.setGroupColor(-1, ofColor(0,0,0));
    
    
    spheres.setCltexNeedUpdate(true);
    spheres.setVtxtexNeedUpdate(true);
    
    cylinders.setCltexNeedUpdate(true);
    cylinders.setVtxtexNeedUpdate(true);
    
    
    int endTime = ofGetElapsedTimeMillis();
    myLogRelease("collisionTest endTime:  "+ ofToString(endTime)+", elapsed: " + ofToString((float)(endTime-startTime)/1000.0));
    myLogRelease("finish CollisionTest");
}


#if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
void testApp::processCollisionParallel(){
    
    int startTime = collisionStart();
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int N = instanceMap.size();
    CollisionTable table;
    parallel_for(blocked_range<size_t>(0, N), Tally(table, instanceMap));
    
    // change group id
    CollisionTable::iterator itr = table.begin();
    for(; itr!=table.end(); ++itr){
        instance * ins = itr->first;
        int groupId = itr->second;
        if(ins!=NULL){
            INSTANCE_MAP_ITR  iitr = instanceMap.begin();
            for(; iitr!=instanceMap.end(); iitr++){
                if(&iitr->second == ins){
                    spheres.changeInstanceGroupId(iitr, groupId);
                    break;
                }
            }
        }
    }
    
    collisionEnd(startTime);
    
}
#endif