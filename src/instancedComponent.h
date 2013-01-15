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

#define VTX_TEX_WIDTH       1024
#define VTX_TEX_HEIGHT      512

#define CL_TEX_WIDTH        512     // 262,144
#define CL_TEX_HEIGHT       512


//
//  <map> instanceGroups
//      class instanceGroup
//          <vector> instances
//              class instance
//

struct instance{
public:
    instance(){color = ofFloatColor(1.0, 1.0, 1.0);}
    ofMatrix4x4 matrix;
    ofFloatColor color;
};


typedef vector<instance> INSTANCES;


struct instanceGroup{
public:
    INSTANCES instances;
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
    void updateVertexTexture();
    void updateColorTexture();
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

    void setInstanceColor(int groupId, int index, ofFloatColor color);
    
    int initGroup();
    
    ofxVboMeshInstanced * getVboMeshInstanced(){ return vmi; }
    
    inline void setInstanceNum(int i){ instanceNum = i; }
    inline int getInstanceNum(){ return instanceNum; }
    
    void mergeInstanceGroup(int groupIdA, int groupIdB);
    void mergeInstanceGroupAll();
    
private:

    // component param
    //
    ofVec3f offsetPosition;
    ofVec4f offsetRotation;
    ofVec3f offsetScale;
    
    
    // instance param
    //
    bool    bVtxtexNeedUpdate;
    bool    bCltexNeedUpdate;
    int     instanceNum;

    GLuint  vtxtexId;
    GLuint  cltexId;
    
    ofxVboMeshInstanced * vmi;
    
    string shaderVtxTextureName;
    string shaderColorTextureName;
    
    
    
    void mergeInstanceGroup(INSTANCE_GROUPS::iterator itrA, INSTANCE_GROUPS::iterator itrB);
    //
    //  Rycycle bin
    //
    
    //bool    bTexAllocated;
    //float * matrices;       // don't want to keep this here. but new float[????] is expensive and glGetTexImage is slow as well.


    //    void setInstancePosition    (int index, ofVec3f p);
    //    void setInstanceRotattion   (int index, ofVec4f r);
    //    void setInstanceScale       (int index, ofVec3f s);
 
    //void sendVertexData(ofShader * shader, GLuint loc);
    
};