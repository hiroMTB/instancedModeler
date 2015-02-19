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
        sinCache2[i] = sin(angle);
        cosCache2[i] = cos(angle);
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
                float nx = sinCache[i];
                float ny = cosCache[i];
                
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
//    if(slices<2 || stacks<1 || radius<0.0 || height<0.0)
//        myLogRelease("invalid parameter for cylinder mesh creation");
//
//  TODO
}


//
//  buggy
//
ofMesh createQuadSphere(float r, int lats, int longs){

    ofMesh sp;
  
    
    for(int i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);
        
        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        //glBegin(GL_QUAD_STRIP);
        
        for(int j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

//            glNormal3f(x * zr0, y * zr0, z0);
//            glVertex3f(x * zr0, y * zr0, z0);
//            glNormal3f(x * zr1, y * zr1, z1);
//            glVertex3f(x * zr1, y * zr1, z1);

            sp.addNormal(ofVec3f(x * zr0, y * zr0, z0));
            sp.addVertex(ofVec3f(x * zr0, y * zr0, z0));
            sp.addNormal(ofVec3f(x * zr1, y * zr1, z1));
            sp.addVertex(ofVec3f(x * zr1, y * zr1, z1));
        }
        
        //glEnd();
    }
    
      sp.setMode(OF_PRIMITIVE_QUAD_STRIP);
    return sp;
}


idPair::idPair(){}
    
idPair::idPair(int _a, int _b):
a(_a),
b(_b)
{}

idPair::idPair(const idPair& ip){
    a=ip.a; b=ip.b;
}


bool operator<(const idPair& lhs, const idPair& rhs ){
    return lhs.a<rhs.a;
}


bool operator==(const idPair& lhs, const idPair& rhs ){
    bool aabb = (lhs.a==rhs.a) && (lhs.b==rhs.b);
    bool abba =(lhs.a==rhs.b) && (lhs.b==rhs.a);
    return aabb || abba;
}




float fBm1f( float x, int octave){
    
    float noise = 0;
    float amp = 0.5;
    for (int i=0; i<octave; i++){
        noise += ofSignedNoise(x) * amp;
        amp *= 0.5;
        x *= 2.0;
    }
    return noise;
}

float fBm1uf( float x, int octave){
    float noise = 0;
    float amp = 0.5;
    for (int i=0; i<octave; i++){
        noise += ofNoise(x) * amp;
        amp *= 0.5;
        x *= 2.0;
    }
    return noise;
}

float fbm3f( float x, float y, float z, int octave){
    
    float noise = 0;
    float amp = 0.5;
    for (int i=0; i<octave; i++){
        noise += ofSignedNoise(x,y,z) * amp;
        amp *= 0.5;
        x *= 2.0;
        y *= 2.0;
        z *= 2.0;
    }
    return noise;
}

float fbm3uf( float x, float y, float z, int octave){
    
    float noise = 0;
    float amp = 0.5;
    for (int i=0; i<octave; i++){
        noise += ofNoise(x,y,z) * amp;
        amp *= 0.5;
        x *= 2.0;
        y *= 2.0;
        z *= 2.0;
    }
    return noise;
}


float distSeg2Seg( ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4){
    ofVec3f   u = p2 - p1;
    ofVec3f   v = p4 - p3;
    ofVec3f   w = p1 - p3;
    float    a = u.dot(u);         // always >= 0
    float    b = u.dot(v);
    float    c = v.dot(v);         // always >= 0
    float    d = u.dot(w);
    float    e = v.dot(w);
    float    D = a*c - b*b;        // always >= 0
    float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
    
    
    float SMALL_NUM = std::numeric_limits<float>::min();
    
    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }
    
    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
            else if (-d > a)
                sN = sD;
                else {
                    sN = -d;
                    sD = a;
                }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
            else if ((-d + b) > a)
                sN = sD;
                else {
                    sN = (-d +  b);
                    sD = a;
                }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);
    
    // get the difference of the two closest points
    ofVec3f   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
    
    return dP.length();   // return the closest distance
}