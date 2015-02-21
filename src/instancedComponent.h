//
//  instancedComponent.h
//
#pragma once

#include "ofMain.h"
#include "ofxVboMeshInstanced.h"
#include "ofxMtb.h"
#include "btBulletCollisionCommon.h"
#include "ofxRay.h"
#include <map>

#define VTX_TEX_WIDTH       2048    // 2048*2048/4 = 1,048,576
#define VTX_TEX_HEIGHT      2048
#define CL_TEX_WIDTH        1024     // 1024 * 1024 = 1,048,576
#define CL_TEX_HEIGHT       1024

enum INSTANCE_TYPE{
    INSTANCE_NONE = 0,
    INSTANCE_SPHERE = SPHERE_SHAPE_PROXYTYPE,
    INSTANCE_CYLINDER = CYLINDER_SHAPE_PROXYTYPE
};

struct instance{
public:
    instance(const instance& i)
    :color(i.color), type(i.type), matrix(i.matrix),scale(i.scale){}
    
    ofMatrix4x4     matrix;     // NOTICE dont set scaling value
    ofVec3f         scale;
    ofFloatColor    color;
    INSTANCE_TYPE   type;
    instance(){};
    
    bool operator==(const instance& o){

        if(type == o.type)
            if(scale == o.scale)
                if(matrix.getTranslation() == o.matrix.getTranslation())
                        return true;
        return false;
    }    
};


typedef multimap<int, instance> INSTANCE_MAP;
typedef INSTANCE_MAP::iterator INSTANCE_MAP_ITR;

class instancedComponent{
    
public:
    static void resetGroup();
    static void changeInstanceGroupId(INSTANCE_MAP_ITR& itr, int groupId);
    static void mergeInstanceGroup(int groupIdA, int groupIdB);
    static void mergeInstanceGroupAll(int groupId);
    static int  updateGroupTotalNum();
    static int  getGroupTotalNum(){ return groupTotalNum;}
    static vector<string> printData(bool outputConsole=true);
    static vector<string> printGroupData(bool outputConsole=true);
    static void removeDuplication();
    static INSTANCE_MAP_ITR getInstanceIterator( int index, INSTANCE_TYPE type );
    static void addSelectedInstance( int index );
    static void mousePick( ofVec3f winpos, INSTANCE_TYPE type, int mode);
    static void drawSelector();
    static void debugDraw();

    instancedComponent();
    instancedComponent(INSTANCE_TYPE t, INSTANCE_MAP& m);
    ~instancedComponent();
    
    void reset();
    void update();
    void updateVertexTexture();
    void updateColorTexture();
    void updateRequest();
    
    void draw(ofShader * shader);
    void drawWireframe(ofShader * shader);    
    void loadInstanceMesh(ofMesh mesh, ofVec3f scale=ofVec3f(1,1,1));
    void loadInstancePositionFromModel(string path, int res, float posScale=1, bool noiseFilter=false);
    void loadInstancePositionFromMatrices(ofMatrix4x4 * ms, ofVec3f * ss, int size);
    INSTANCE_MAP_ITR addInstance(instance ins, int groupId=-1);
    void addInstanceMatrix      (ofMatrix4x4 m, ofVec3f s, int groupId=-1);
    void clearInstanceMatrices();
    void setInstanceColor(INSTANCE_MAP_ITR itr, ofFloatColor color);
    void setInstanceColor(int index, ofFloatColor color);
    void updateInstanceNum();
    void removeGroup(int groupId);
    void removeSmallGroup(int minNum);
    void setGroupColor(int groupId, ofFloatColor color);
    void setGroupColorGradient();
    void removeSelectedInstance();
    void removeInstance( INSTANCE_MAP_ITR itr );
    void saveInstanceDataToCsv(string dirName);
    void loadInstanceDataFromCsv(string dirName);
    
    static vector<INSTANCE_MAP_ITR> selectedInsVec;
    static INSTANCE_MAP instanceMap;
    static int groupIdMaster;
    static int groupTotalNum;
    static btVector3 rayTo;
    bool    bVtxtexNeedUpdate;
    bool    bCltexNeedUpdate;
    int     instanceNum;
    GLuint  vtxtexId;
    GLuint  cltexId;
    INSTANCE_TYPE insType;
    ofxVboMeshInstanced vmi;

};