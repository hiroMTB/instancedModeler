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

    
    float collisionTest(btCollisionObject * objectA, btCollisionObject * objectB, btCollisionAlgorithm * algo);
    float testSphereSphere(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    float testSphereCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    float testCylinderCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB);
    
    static void debugDraw(ofMatrix4x4& mat, ofVec3f& scale, int shapeType);
    
    
    void initAlgo();
  
    void clearContanctPts();
    void drawAllContanctPts();
    
private:
    void initCollisionWorld();
    
    void destroy();
    
    static btCollisionWorld *	collisionWorld;
    
    static btCollisionObject	sphereA, sphereB;
    static btCollisionObject   cylinderA, cylinderB;
  
    btCollisionAlgorithm * algoSS;
    btCollisionAlgorithm * algoSC;
    btCollisionAlgorithm * algoCC;
    
    
    static void setTransformFromOF(ofMatrix4x4& mat, ofVec3f& scale, btCollisionObject& obj);

    // option
    GLDebugDrawer debugDrawer;
   	GL_ShapeDrawer* m_shapeDrawer;
    
    vector<btVector3> contactPts;
    
    
    class	btThreadSupportInterface*		m_threadSupportCollision;

};