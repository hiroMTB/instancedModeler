//
//  ofxMtb.cpp
//

#include "ofMain.h"
#include "ofxMtb.h"
#include <IOStream>

void myLogDebug(string message){
#ifndef NDEBUG
    cout << " " + myModule + " D " + message << endl;
#endif
}


void myLogRelease(string message){
    cout << " " + myModule + " R " + message << endl;
}




ofMesh createCylinderZ(float radius, float height, int slices, int stacks){
    if(slices<2 || stacks<1 || radius<0.0 || height<0.0)
        myLogRelease("invalid parameter for cylinder mesh creation");
    
    int cacheSize = slices+1;
    // for verteices and normals
    float sinCache[cacheSize];
    float cosCache[cacheSize];
    
    // for face
    float sinCache2[cacheSize];
    float cosCache2[cacheSize];
    
    float angle;
    float zLow, zHigh;
    float sinTemp, cosTemp;
    float xyNormalRatio;
    
    xyNormalRatio = 1;
    
    for(int i=0; i<slices; i++){
        angle = TWO_PI * i / (float)slices;
        sinCache[i] = sin(angle);
        cosCache[i] = cos(angle);
    }
    
    // outside
    for(int i=0; i<slices; i++){
        angle = TWO_PI * (i-0.5) / (float)slices;
        sinCache[i] = sin(angle);
        cosCache[i] = cos(angle);
    }
    
    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    
    sinCache2[slices] = sinCache2[0];
    cosCache2[slices] = cosCache2[0];
    
    ofMesh cylinder;
    ofVec3f v1, v2, n1,n2;
    ofVec2f t1, t2;
    
    // construct mesh for TRIANGLE STRIP
    for(int j=0; j<stacks; j++){
        
        float zLow = (j * height/stacks) - height*0.5;
        float zHigh = ((j+1) * height/stacks) -height*0.5;
        
        for(int i=0; i<=slices; i++){
            float x = radius*sinCache[i];
            float y = radius*cosCache[i];
            
            v1.set(x, y, zLow);
            v2.set(x, y, zHigh);
            t1.set(1.0-(float)i/slices, (float)j/stacks);
            t2.set(1.0-(float)i/slices, (float)(j+1)/stacks);
            
            cylinder.addVertex(v1);
            cylinder.addVertex(v2);
            cylinder.addTexCoord(t1);
            cylinder.addTexCoord(t2);
            
//            if(i!=0){
                float nx = radius*sinCache2[i];
                float ny = radius*cosCache2[i];
                
                n1.set(nx, ny, 0);
                n2.set(nx, ny, 0);
                cylinder.addNormal(n1);
                cylinder.addNormal(n2);
//            }
        }
    }
    
    cylinder.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    return cylinder;
}


ofMesh createCylinderY(float radius, float height, int slices, int stacks){
    if(slices<2 || stacks<1 || radius<0.0 || height<0.0)
        myLogRelease("invalid parameter for cylinder mesh creation");
    

}