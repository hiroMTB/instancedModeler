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
#include "instancedComponent.h"

class collisionTester{
    
public:
    collisionTester();
    ~collisionTester();

    static void scaleCylinderShape( const ofVec3f& scale );
    static void setTransformFromOF( const ofMatrix4x4& mat, const ofVec3f& scale, btCollisionObject& obj);
    static void resetSphereShape(float radius, float collisionMargin);
    static void resetCylinderShape(ofVec3f halfExtent, float collisionMargin);
    static void debugDraw( const instance & ins);
    
    float collisionTest(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB, /*const*/ btCollisionObject & objA, /*const*/ btCollisionObject & objB );
    
    void clearContanctPts();
    void drawAllContanctPts();
    void initCollisionWorld();
    void destroy();
    
    static btCollisionWorld * collisionWorld;
  
    GLDebugDrawer debugDrawer;
   	GL_ShapeDrawer* m_shapeDrawer;    
    vector<btVector3> contactPts;
      
    //btCollisionAlgorithm * prepareAlgo(const btCollisionObject &objA, const btCollisionObject &objB);
    
};