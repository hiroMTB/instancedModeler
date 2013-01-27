//
//  connect.cpp
//  instancedModeler
//
//  Created by matobahiroshi on 13/01/25.
//
//

#include "testApp.h"

//bool testApp::connectInstanace(instance &instA, instance &instB, float minDist, float maxDist, instance& newIns){
//    
//    // 3. check A - B distance
//    ofVec3f vA = instA.matrix.getTranslation();
//    ofVec3f vB = instB.matrix.getTranslation();
//    
//    ofVec3f vAB = vB - vA;
//    float dist = vAB.length();
//    
//    if(minDist<dist && dist<maxDist){
//        
//        // 4. put
//        //        ofMatrix4x4 mat;
//        //        ofVec3f scale;
//        //        ofVec3f pos = vA + vAB*0.5;
//        
//        ofVec3f yAxis(0,0,1);
//        float angle = yAxis.angle(vAB);
//        ofVec3f prep = yAxis.cross(vAB);
//        
//        newIns.scale.set(1, 1, dist);
//        newIns.matrix.rotate(angle, prep.x, prep.y, prep.z);
//        newIns.matrix.translate(vA + vAB*0.5);
//        return true;
//    }else{
//        return false;
//    }
//}


bool testApp::connectInstanace(INSTANCE_MAP_ITR& itrA, INSTANCE_MAP_ITR& itrB, float minDist, float maxDist, instance& newIns){
    
    minDist = MIN(minDist, maxDist);
    maxDist = MAX(minDist, maxDist);
    
    instance& instA = itrA->second;
    instance& instB = itrB->second;
    
    if(instA.type!= INSTANCE_CYLINDER && instB.type!=INSTANCE_CYLINDER ){
        instance newCylinder;
        newCylinder.type = INSTANCE_CYLINDER;
        
            
        // check A - B distance
        ofVec3f vA = instA.matrix.getTranslation();
        ofVec3f vB = instB.matrix.getTranslation();
    
        ofVec3f vAB = vB - vA;
        float dist = vAB.length();
    
        if(minDist<=dist && dist<=maxDist){
            // put cylinder
            ofVec3f yAxis(0,0,1);
            float angle = yAxis.angle(vAB);
            ofVec3f prep = yAxis.cross(vAB);
            
            newIns.scale.set(1, 1, dist);
            newIns.matrix.rotate(angle, prep.x, prep.y, prep.z);
            newIns.matrix.translate(vA + vAB*0.5);
            return true;
        }
    }
    return false;    
}

void testApp::connectGroup(instancedComponent *ic, instancedComponent *ic2, int numAllCylinders, float minDist, float maxDist){
    
    char m[255];
    
    int startTime = ofGetElapsedTimeMillis();
    myLogRelease("Start ConnectGroup Process : time : " + ofToString(startTime));
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int numGroups = STL_UTIL::getAllKeySize(instanceMap);
    
    if(numGroups<2){
        myLogRelease("cancel connect group process. only 1 group found.");
        return;
    }
    
    int numFind = 0;

    map<idPair, instance>  connectionList;   // store connection

    INSTANCE_MAP_ITR itr  = instanceMap.begin();
    
    // loop all group
    int groupIndexA = 0;
    int groupSizeA = 0;
    int groupIdA = 0;
    int groupKeyA = 0;
    
    
    for(groupIdA=0; itr!=instanceMap.end(); itr=instanceMap.upper_bound(itr->first), groupIdA++){
        groupKeyA = itr->first;
        myLogDebug("start process connect groupKeyA " + ofToString(groupKeyA) + ", groupIdA " + ofToString(groupIdA));
        
        groupSizeA = STL_UTIL::getElementSize(instanceMap, itr->first);
        groupIndexA += groupSizeA;
        
        for(int i=0; i<numAllCylinders; i++){
            int numTry = 0;
            int maxTry = 1000;
            bool find = false;
        
            do{
                // 1. select group and get group top itr
                int groupIndexB      = 0;
                int groupSizeB  = 0;
                int groupKeyB   = 0;
                int groupIdB    = 0;
                
                groupIdB = ofRandom(numGroups);
                if(groupIdA == groupIdB){
        
                    continue;
                }
                
                cout << "   groupId " << ofToString(groupIdA) + " + " + ofToString(groupIdB) << endl;
                
                INSTANCE_MAP_ITR itrB = instanceMap.begin();

                for(int j=0; itrB!=instanceMap.end(); itrB=instanceMap.upper_bound(itrB->first), j++){
                    groupSizeB = STL_UTIL::getElementSize(instanceMap, itrB->first);
                    groupIndexB += groupSizeB;

                    if(j==groupIdB){
                        groupKeyB = itrB->first;
                        break;
                    }
                }

//                myLogDebug(" try groupKeyB " + ofToString(groupKeyB) + ", groupIdB" + ofToString(groupIdB));

                // 2. select instance from group
                int indexInGroupA = ofRandom(groupSizeA);
                int indexInGroupB = ofRandom(groupSizeB);
                
                int indexA = groupIndexA + indexInGroupA;
                int indexB = groupIndexB + indexInGroupB;
                
                // 3. check if pair is already connected
                idPair idp(indexA, indexB);
                bool unique = (connectionList.find(idp) == connectionList.end());
                
                if(unique){
                    INSTANCE_MAP_ITR itrA = instanceMap.begin();
                    std::advance(itrA, indexA);
                    std::advance(itrB, indexInGroupB);
                    instance newCylinder;
                    find = connectInstanace(itrA, itrB, minDist, maxDist, newCylinder);
                    if(find){
                        sprintf(m, "connect group (key-index) %d_%d + %d_%d", groupKeyA, indexA, groupKeyB, indexB);
                        myLogRelease(m);

                        // add
                        newCylinder.type = INSTANCE_CYLINDER;
                        //ic2->addInstance(newCylinder);
                        numFind++;
                        connectionList.insert(pair<idPair, instance>(idp, newCylinder));
                    }
                }
                
                numTry++;
                
                if(maxTry < numTry)
                    find = true;
            }while (!find);
        }
    }
    
    {
        map<idPair, instance>::iterator itr = connectionList.begin();
        for(; itr!=connectionList.end(); itr++){
            int indexA = itr->first.a;
            int indexB = itr->first.b;
            instance& cyl = itr->second;
            ic2->addInstance(cyl);
        }
    }
    
    // insert process
    cylinders.updateRequest();
    
    int endTime = ofGetElapsedTimeMillis();
    myLogRelease("Finish ConnectGroup process : elapsed " + ofToString((float)(endTime-startTime)/1000.0)+" sec");

}


// should be used parallel_for
void testApp::connectRandom(instancedComponent *ic, instancedComponent *ic2, int numAllCylinders, float minDist, float maxDist){
    
    int startTime = ofGetElapsedTimeMillis();
    myLogRelease("Start ConnectRandom Process : time : " + ofToString(startTime));
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int numInstances = instanceMap.size();
    
    int numFind = 0;

    map<idPair, instance>  connectionList;   // store connection
    
    for(int i=0; i<numAllCylinders; i++){
        int numTry = 0;
        int maxTry = 10000;
        bool find = false;
        
        do{
            // 1. select instance
            int indexA = ofRandom(numInstances);               // todo: invalid
            int indexB = ofRandom(numInstances);
            
            // 2. check if pair is already connected
            idPair idp(indexA, indexB);
            bool unique = connectionList.find(idp) == connectionList.end();
            
            if(unique){
                INSTANCE_MAP_ITR itrA = instanceMap.begin();
                INSTANCE_MAP_ITR itrB = instanceMap.begin();
                std::advance(itrA, indexA);
                std::advance(itrB, indexB);
                instance newCylinder;
                find = connectInstanace(itrA, itrB, minDist, maxDist, newCylinder);
                if(find){
                    // add
                    newCylinder.type = INSTANCE_CYLINDER;
//                  ic2->addInstance(newCylinder);
                    numFind++;
                    connectionList.insert(pair<idPair, instance>(idp, newCylinder));
                }
            }
            
            numTry++;
            
            if(maxTry < numTry)
                find = true;
        }while (!find);
    }
    
    {
        map<idPair, instance>::iterator itr = connectionList.begin();
        for(; itr!=connectionList.end(); itr++){
            int indexA = itr->first.a;
            int indexB = itr->first.b;
            instance& cyl = itr->second;
            ic2->addInstance(cyl);
        }
    }
    
    cylinders.updateInstanceNum();
    
    int endTime = ofGetElapsedTimeMillis();
    myLogRelease("Finish ConnectRandom process : elapsed " + ofToString((float)(endTime-startTime)/1000.0)+" sec");
    
}


