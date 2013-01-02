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


#define TEX_WIDTH 1024
#define TEX_HEIGHT 512


class instancedComponent{
    
public:
    instancedComponent();
    ~instancedComponent();
    
    void allocate();
    
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
    void setInstanceMatrix      (int index, ofMatrix4x4 m);
    void setInstanceMatrix      (int index, ofVec3f p, ofVec4f r=ofVec4f(0,0,0,0), ofVec3f s=ofVec3f(1,1,1));
//    void setInstancePosition    (int index, ofVec3f p);
//    void setInstanceRotattion   (int index, ofVec4f r);
//    void setInstanceScale       (int index, ofVec3f s);
    
    void clearInstanceMatrices();

    
private:

    
    // component param
    //
    ofVec3f offsetPosition;
    ofVec4f offsetRotation;
    ofVec3f offsetScale;
    
    
    // instance param
    //
    bool    bTexNeedUpdate;
    bool    bTexAllocated;
    int     instanceNum;

    float * matrices;
    GLuint  texId;
    
    ofxVboMeshInstanced * vmi;
    
    string shaderTextureName;
};