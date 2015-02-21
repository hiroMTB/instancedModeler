//
//  collisionTester.cpp
//  instancedModeler
//
//  Created by matobahiroshi on 13/01/15.
//
//

#include "collisionTester.h"
#include "ofxMtb.h"
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#include "BulletMultiThreaded/PlatformDefinitions.h"
#include "BulletMultiThreaded/PosixThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"

btCollisionObject collisionTester::sphereA;
btCollisionObject collisionTester::sphereB;
btCollisionObject collisionTester::cylinderA;
btCollisionObject collisionTester::cylinderB;

btCollisionWorld * collisionTester::collisionWorld = NULL;

collisionTester::collisionTester(){
    initCollisionWorld();
    //resetSphereShape(0.5);
    //resetCylinderShape(ofVec3f(0.5, 0.5, 0.5));
    
    float radius = 1;
    btSphereShape * sA = new btSphereShape(radius);
    btSphereShape * sB = new btSphereShape(radius);
    sA->setMargin(0.0);
    sB->setMargin(0.0);
    sphereA.setCollisionShape(sA);
    sphereB.setCollisionShape(sB);

    btVector3 half(1,1,1);
    btCylinderShape * cA = new btCylinderShapeZ(half);
    btCylinderShape * cB = new btCylinderShapeZ(half);
    cA->setMargin(0.0);
    cB->setMargin(0.0);
        
    cylinderA.setCollisionShape(cA);
    cylinderB.setCollisionShape(cB);
    
    initAlgo();
}


collisionTester::~collisionTester(){
    //    ???
    //    if(algoSS){ delete algoSS; algoSS=0; }
    //    if(algoCC){ delete algoCC; algoCC=0; }
    //    if(algoSC){ delete algoSC; algoSC=0; }
    
    delete sphereA.getCollisionShape();
    delete sphereB.getCollisionShape();
    delete cylinderA.getCollisionShape();
    delete cylinderB.getCollisionShape();
    if(collisionWorld){ delete collisionWorld; collisionWorld=0; }
}


void collisionTester::initCollisionWorld(){
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btVector3	worldAabbMin(-10000,-10000,-10000);
	btVector3	worldAabbMax( 10000, 10000, 10000);
	btAxisSweep3*	broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax);
	collisionWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	collisionWorld->setDebugDrawer(&debugDrawer);    
}

void collisionTester::initAlgo(){
    btCollisionObjectWrapper sA(0,sphereA.getCollisionShape(), &sphereA, sphereA.getWorldTransform());
    btCollisionObjectWrapper sB(0,sphereB.getCollisionShape(), &sphereB, sphereB.getWorldTransform());
    btCollisionObjectWrapper cA(0,cylinderA.getCollisionShape(), &cylinderA, sphereA.getWorldTransform());
    btCollisionObjectWrapper cB(0,cylinderB.getCollisionShape(), &cylinderB, sphereB.getWorldTransform());
    
	algoSS = collisionWorld->getDispatcher()->findAlgorithm(&sA, &sB);
  	algoSC = collisionWorld->getDispatcher()->findAlgorithm(&sA, &cA);
    algoCC = collisionWorld->getDispatcher()->findAlgorithm(&cA, &cB);
}


void collisionTester::resetSphereShape(float radius, float collisionMargin){
    
    btCollisionShape * sA = sphereA.getCollisionShape();
    btCollisionShape * sB = sphereB.getCollisionShape();
    
    if(sA) delete sA;
    if(sB) delete sB;
    
    sA = new btSphereShape(radius + collisionMargin);
    sB = new btSphereShape(radius + collisionMargin);

    sA->setMargin(0);
    sB->setMargin(0);

    sphereA.setCollisionShape(sA);
    sphereB.setCollisionShape(sB);
}

void collisionTester::resetCylinderShape(ofVec3f halfExtent, float collisionMargin){
    btVector3 half(halfExtent.x+collisionMargin , halfExtent.y, halfExtent.z);
    
    btCollisionShape * cA = cylinderA.getCollisionShape();
    btCollisionShape * cB = cylinderB.getCollisionShape();
    
    if(cA) delete cA;
    if(cB) delete cB;

    cA = new btCylinderShapeZ(half);
    cB = new btCylinderShapeZ(half);

    cA->setMargin(0);
    cB->setMargin(0);

    cylinderA.setCollisionShape(cA);
    cylinderB.setCollisionShape(cB);
}

void collisionTester::debugDraw(ofMatrix4x4& mat, ofVec3f& scale, int shapeType){
	glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    btCollisionObject * obj;
    if(shapeType==SPHERE_SHAPE_PROXYTYPE){
        obj = &sphereA;
    }else if(shapeType == CYLINDER_SHAPE_PROXYTYPE){
        obj=&cylinderA;
    }

    setTransformFromOF(mat,  scale, *obj);
    btVector3 color(1,1,0);
    
    collisionWorld->debugDrawObject(obj->getWorldTransform(),obj->getCollisionShape(), color);
}


void collisionTester::setTransformFromOF(ofMatrix4x4& mat, ofVec3f& s, btCollisionObject& obj){
    
    btTransform trans;
    trans.setFromOpenGLMatrix(mat.getPtr());
    obj.setWorldTransform(trans);
    obj.getCollisionShape()->setLocalScaling(btVector3(s.x, s.y, s.z));
}

//
//  see bulletPhysics CollisionInterfaceDemo.cpp
//
float collisionTester::testSphereSphere(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB){
    setTransformFromOF(matA, sA, sphereA);
    setTransformFromOF(matB, sB, sphereB);
    return collisionTest(&sphereA, &sphereB, algoSS);
}

float collisionTester::testSphereCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB){
    setTransformFromOF(matA, sA, sphereA);
    setTransformFromOF(matB, sB, cylinderB);
    return collisionTest(&sphereA, &cylinderB, algoSC);
}

float collisionTester::testCylinderCylinder(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB){
    setTransformFromOF(matA, sA, cylinderA);
    setTransformFromOF(matB, sB, cylinderB);
    return collisionTest(&cylinderA, &cylinderB, algoCC);
}


float collisionTester::collisionTest(btCollisionObject * objectA, btCollisionObject * objectB, btCollisionAlgorithm * algo){
    
    float dist = 99999;
    
    btCollisionObjectWrapper ob0(0,objectA->getCollisionShape(),objectA, objectA->getWorldTransform());
    btCollisionObjectWrapper ob1(0,objectB->getCollisionShape(),objectB, objectB->getWorldTransform());
    
	btManifoldResult contactPointResult(&ob0,&ob1);
	algo->processCollision(&ob0,&ob1,collisionWorld->getDispatchInfo(),&contactPointResult);
	
	btManifoldArray manifoldArray;
	algo->getAllContactManifolds(manifoldArray);
    
	int numManifolds = manifoldArray.size();
	for(int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = manifoldArray[i];
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        //	btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
        
	
		int numContacts = contactManifold->getNumContacts();
		bool swap = obA == objectA;
        
        //printf("numManifolds=%i, numContact = %i \n", numManifolds, numContacts);
        
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            
			btVector3 ptA = swap ?pt.getPositionWorldOnA():pt.getPositionWorldOnB();
			btVector3 ptB = swap ? pt.getPositionWorldOnB():pt.getPositionWorldOnA();
            
            //contactPts.push_back(ptA);
            //contactPts.push_back(ptB);
            
            float distTmp = pt.getDistance();
            if(distTmp<dist)
                dist=distTmp;
                        
		}
        
		//you can un-comment out this line, and then all points are removed
		contactManifold->clearManifold();	
	}
    
    return dist;
}

void collisionTester::drawAllContanctPts(){
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);

    for (int i=0; i<contactPts.size(); i++) {
        btVector3& ptA = contactPts[i];
		glVertex3d(ptA.x(),ptA.y(),ptA.z());
    }
    glEnd();

}

void collisionTester::clearContanctPts(){
    contactPts.clear();
}