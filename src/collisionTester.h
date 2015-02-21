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

    static void setTransformFromOF(ofMatrix4x4& mat, ofVec3f& scale, btCollisionObject& obj);
    static void debugDraw(ofMatrix4x4& mat, ofVec3f& scale, int shapeType);
    
    // should be call before usinge this class
    static void resetSphereShape(float radius, float collisionMargin);
    static void resetCylinderShape(ofVec3f halfExtent, float collisionMargin);
    
    float collisionTest(btCollisionObject * objectA, btCollisionObject * objectB, btCollisionAlgorithm * algo);
    float testSphereSphere(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    float testSphereCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    float testCylinderCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    
    void initAlgo();
    void clearContanctPts();
    void drawAllContanctPts();
    void initCollisionWorld();
    
    static btCollisionWorld *	collisionWorld;
    static btCollisionObject	sphereA, sphereB;
    static btCollisionObject   cylinderA, cylinderB;
  
    btCollisionAlgorithm * algoSS;
    btCollisionAlgorithm * algoSC;
    btCollisionAlgorithm * algoCC;
    
    GLDebugDrawer debugDrawer;
   	GL_ShapeDrawer* m_shapeDrawer;
    vector<btVector3> contactPts;
        
};