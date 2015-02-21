//
//  rnTestCase.cpp
//  instancedModeler_rev02
//
//  Created by MatobaHiroshi on 2/9/15.
//
//

#include "rnTestCase.h"

#include "rnApp.h"


void rnTestCase::setup_mergin_test(){
    
    static rnApp * app = rnApp::app;
    
    float radius = 1.75;
    rnApp::SPHERE_RADIUS = radius;
    rnApp::SPHERE_RESOLUTION = 12;
    rnApp::CURRENT_PROCESS = rnApp::RESET_INSTSANCE_SHAPE;

    vector<ofMatrix4x4> ms(2);
    vector<ofVec3f> scales(2);

    // SphereA (10, 1, 1);
    ms[0].makeIdentityMatrix();
    ms[0].translate(10, 1, 1);
    scales[0].set(1, 1, 1);

    // SphereB (10.1, 1, 1);
    ms[1].makeIdentityMatrix();
    ms[1].translate(10 + radius*2 + 0.5, 1, 1);
    scales[1].set(1, 1, 1);
    app->spheres.loadInstancePositionFromMatrices(&ms[0], &scales[0], 2);
    
 
}


void rnTestCase::loadRandomSphere(int num, float pos_max){

    static rnApp * app = rnApp::app;

    vector<ofMatrix4x4> ms(num);
    vector<ofVec3f> scales(num);
    for(int i=0; i<num; i++){
        ms[i].makeIdentityMatrix();
        ms[i].translate(ofRandomf()*pos_max, ofRandomf()*pos_max, ofRandomf()*pos_max);
        scales[i].set(1, 1, 1);
    }
    app->spheres.loadInstancePositionFromMatrices(&ms[0], &scales[0], num);
}

void rnTestCase::loadRandomCylinder(int num, float pos_max){
    
    static rnApp * app = rnApp::app;

    vector<ofMatrix4x4> ms(num);
    vector<ofVec3f> scales(num);
    for(int i=0; i<num; i++){
        ms[i].makeIdentityMatrix();
        ms[i].rotate(12, ofRandom(-180, 180), ofRandom(-180, 180), ofRandom(-180, 180));
        ms[i].translate(ofRandomf()*pos_max, ofRandomf()*pos_max, ofRandomf()*pos_max);
        scales[i].set(1,1,100);
    }
    app->cylinders.loadInstancePositionFromMatrices(&ms[0], &scales[0], num);
}