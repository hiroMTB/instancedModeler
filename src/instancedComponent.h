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

#include "myDef.h"
#include "ofMain.h"
#include "ofxVboMeshInstanced.h"
#include "ofxMtb.h"
#include "btBulletCollisionCommon.h"

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

enum INSTANCE_TYPE{
    INSTANCE_NONE = 0,
    INSTANCE_SPHERE = SPHERE_SHAPE_PROXYTYPE,
    INSTANCE_CYLINDER = CYLINDER_SHAPE_PROXYTYPE
};

struct instance{
public:
//    instance(INSTANCE_TYPE t){
//        color = ofFloatColor(1.0, 1.0, 1.0);
//        type = t;
//    }
    instance(const instance& i){
        color = i.color;
        type = i.type;
        matrix = i.matrix;
        scale = i.scale;
    }
    ofMatrix4x4     matrix;     // NOTICE dont use scaling func
    ofVec3f         scale;
    ofFloatColor    color;
    INSTANCE_TYPE   type;
//private:
    instance(){};
};


typedef multimap<int, instance> INSTANCE_MAP;
typedef INSTANCE_MAP::iterator INSTANCE_MAP_ITR;

class instancedComponent{
public:

    static INSTANCE_MAP &getInstanceMap(){ return instanceMap; }
    
private:
    static INSTANCE_MAP instanceMap;
    static int groupIdMaster;
    static int groupTotalNum;

    INSTANCE_TYPE insType;
    
public:
    instancedComponent();
    ~instancedComponent();
    
    //void allocate();
    
    void destroy();
    void reset();

    void setInstanceType(INSTANCE_TYPE t);

    void update();
    void updateVertexTexture();
    void updateColorTexture();
    void draw(ofShader * shader);
    void drawWireframe(ofShader * shader);
    static void debugDraw();
    
    void loadInstanceMesh(ofMesh mesh, ofVec3f scale=ofVec3f(1,1,1));
    void loadInstancePositionFromModel(string path, float posScale);
    void loadInstancePositionFromMatrices(ofMatrix4x4 * ms, ofVec3f * ss, int size);

    // instance param
    //
    void addInstance(instance ins, int groupId=-1);
    void addInstanceMatrix      (ofMatrix4x4 m, ofVec3f s, int groupId=-1);
    //void addInstanceMatrix      (INSTANCE_TYPE t, ofVec3f p, ofVec4f r=ofVec4f(0,0,0,0), ofVec3f s=ofVec3f(1,1,1), int groupId=-1);
    void clearInstanceMatrices();

    void setGroupColor(int groupId, ofFloatColor color);
    void setGroupColorGradient();
    
    void setInstanceColor(INSTANCE_MAP_ITR itr, ofFloatColor color);
    void setInstanceColor(int index, ofFloatColor color);
    int initGroup();
    
    ofxVboMeshInstanced * getVboMeshInstanced(){ return vmi; }
    
    inline void setInstanceNum(int i){ instanceNum = i; }
    inline int getInstanceNum(){ return instanceNum; }
    
    
    static void changeInstanceGroupId(INSTANCE_MAP_ITR& itr, int groupId);
    static void mergeInstanceGroup(int groupIdA, int groupIdB);

    static void mergeInstanceGroupAll(int groupId);
    
    static vector<string> printData(bool outputConsole=true);
    static vector<string> printGroupData(bool outputConsole=true);
    
    static int getGroupIdMaster(){ return groupIdMaster;}
    static int incGroupIdMaster(){ return ++groupIdMaster; }
    
    void setCltexNeedUpdate(bool b){ bCltexNeedUpdate = b; }
    void setVtxtexNeedUpdate(bool b){ bVtxtexNeedUpdate = b; }
    
    static int    updateGroupTotalNum();
    static int    getGroupTotalNum(){ return groupTotalNum;}
    
    void removeGroup(int groupId);
    void removeSmallGroup(int minNum);
  
    void updateInstanceNum();
    
    // group func
    static void resetGroup();
    
    void saveInstanceDataToCsv(string dirName);
    
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