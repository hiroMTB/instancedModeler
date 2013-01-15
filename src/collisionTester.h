//
//  collisionTester.h
//  instancedModeler
//
//  Created by matobahiroshi on 13/01/15.
//
//

#pragma once


#include "ofMain.h"
#include "btBulletCollisionCommon.h"


#include "LinearMath/btIDebugDraw.h"
#include "GL_ShapeDrawer.h"
#include "GLDebugDrawer.h"

class collisionTester{
    
    
public:
    collisionTester();
    ~collisionTester();

    // should be call before usinge this class
    void resetSphereShape(float radius);
    void resetCylinderShape(ofVec3f halfExtent);

    
    float collisionTest(btCollisionObject * objectA, btCollisionObject * objectB);
    float testSphereSphere(ofMatrix4x4& matA, ofMatrix4x4& matB);
    float testSphereCylinder(float radA, float radB, ofMatrix4x4 matA, ofMatrix4x4 matB);
    float testCylinderCylinder(float radA, float radB, ofMatrix4x4 matA, ofMatrix4x4 matB);
    
    void debugDraw();
    
    
  
private:
    void initCollisionWorld();
    
    void destroy();
    
    btCollisionWorld *	collisionWorld;
    
    btCollisionObject	sphereA, sphereB;
    btCollisionObject   cylinderA, cylinderB;
  
    btCollisionAlgorithm * algoSS;
    btCollisionAlgorithm * algoSC;
    btCollisionAlgorithm * algoCC;
    
    void initAlgo();
    
    // option
    GLDebugDrawer debugDrawer;
    
};