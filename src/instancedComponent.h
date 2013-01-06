//
//  instancedComponent.h
//


//
//  Data Hierarchy
//
//  model - component - instance(sphere, cylinder, etc)
//
//  GUI should show data itself without transformation
//
//
//  :::Usage:::
//
//  setup
//      instancedComponent ic;
//      ic.setInstanceMesh(sphere);
//      ic.setInstancePositionFromModel("myBee.pcl");
//
//
//  update
//      ic.setInstanceMatrix(12, newMat);
//      ic.update();
//
//
//  draw
//      ic.draw(myShader);
//
//
#pragma once

#include "ofMain.h"
#include "ofxVboMeshInstanced.h"
#include "ofxMtb.h"

#include <map>

#define TEX_WIDTH 1024
#define TEX_HEIGHT 512

//
//  <map> instanceGroups
//      class instanceGroup
//          <vector> instances
//              class instance
//

struct instance{
public:
    //int instanceId;
    ofMatrix4x4 matrix;
};


typedef vector<instance> INSTANCES;


struct instanceGroup{
public:
    instanceGroup(){
        //groupIdMaster++;
    }
    
    //int groupId;
    INSTANCES instances;
    ofColor color;
};

typedef map<int, instanceGroup> INSTANCE_GROUPS;


class instancedComponent{
public:

    INSTANCE_GROUPS &getInstanceGroups(){ return instanceGroups; }
    
private:
    INSTANCE_GROUPS instanceGroups;
    int groupIdMaster;
    

public:
    instancedComponent();
    ~instancedComponent();
    
    //void allocate();
    
    void destroy();
    void reset();


    void update();
    void draw(ofShader * shader);
    void drawWireframe(ofShader * shader);
    
    
    void loadInstanceMesh(ofMesh mesh, ofVec3f scale=ofVec3f(1,1,1));
    void loadInstancePositionFromModel(string path, float posScale);
    void loadInstancePositionFromMatrices(ofMatrix4x4 * ms, int size);

    
    // component param
    //
    void setOffsetPosition(ofVec3f p);
    void setOffsetRotation(ofVec4f r);
    void setOffsetScale(ofVec3f s);
    
    
    // instance param
    //
    void setInstanceMatrix      (int groupId, int index, ofMatrix4x4 m);
    void setInstanceMatrix      (int groupId, int index, ofVec3f p, ofVec4f r=ofVec4f(0,0,0,0), ofVec3f s=ofVec3f(1,1,1));
    void clearInstanceMatrices();

    int initGroup();
    
    ofxVboMeshInstanced * getVboMeshInstanced(){ return vmi; }
    
    inline void setInstanceNum(int i){ instanceNum = i; }
    inline int getInstanceNum(){ return instanceNum; }
    
    
private:

    // component param
    //
    ofVec3f offsetPosition;
    ofVec4f offsetRotation;
    ofVec3f offsetScale;
    
    
    // instance param
    //
    bool    bTexNeedUpdate;
    int     instanceNum;

    GLuint  texId;
    
    ofxVboMeshInstanced * vmi;
    
    string shaderTextureName;

    
    
    
    //
    //  Rycycle bin
    //
    
    //bool    bTexAllocated;
    //float * matrices;       // don't want to keep this here. but new float[????] is expensive and glGetTexImage is slow as well.


    //    void setInstancePosition    (int index, ofVec3f p);
    //    void setInstanceRotattion   (int index, ofVec4f r);
    //    void setInstanceScale       (int index, ofVec3f s);

};