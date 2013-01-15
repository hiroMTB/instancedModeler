//
//  collisionTester.cpp
//  instancedModeler
//
//  Created by matobahiroshi on 13/01/15.
//
//

#include "collisionTester.h"


collisionTester::collisionTester(){
    initCollisionWorld();
    resetSphereShape(0.5);
    resetCylinderShape(ofVec3f(0.5, 0.5, 0.5));
    initAlgo();
}


collisionTester::~collisionTester(){
    
    destroy();
}


void collisionTester::initCollisionWorld(){
     btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btVector3	worldAabbMin(-10000,-10000,-10000);
	btVector3	worldAabbMax( 10000, 10000, 10000);
	btAxisSweep3*	broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax);
	collisionWorld = new btCollisionWorld(dispatcher,broadphase,collisionConfiguration);
	collisionWorld->setDebugDrawer(&debugDrawer);    
}


void collisionTester::resetSphereShape(float radius){
    btSphereShape * sA = new btSphereShape(radius);
    btSphereShape * sB = new btSphereShape(radius);
    sA->setMargin(0.0);
    sB->setMargin(0.0);
    sphereA.setCollisionShape(sA);
    sphereB.setCollisionShape(sB);
}

void collisionTester::resetCylinderShape(ofVec3f halfExtent){
    btCylinderShape * cA = new btCylinderShape(btVector3(halfExtent.x, halfExtent.y, halfExtent.z));
    btCylinderShape * cB = new btCylinderShape(btVector3(halfExtent.x, halfExtent.y, halfExtent.z));
    cA->setMargin(0.0);
    cB->setMargin(0.0);
    
    cylinderA.setCollisionShape(cA);
    cylinderB.setCollisionShape(cB);
}

void collisionTester::destroy(){
    delete sphereA.getCollisionShape();
    delete sphereB.getCollisionShape();
    delete cylinderA.getCollisionShape();
    delete cylinderB.getCollisionShape();
}


void collisionTester::debugDraw(){

}



void collisionTester::initAlgo(){
    
    // sphere : sphere
    btCollisionObjectWrapper sA(0,sphereA.getCollisionShape(), &sphereA, sphereA.getWorldTransform());
    btCollisionObjectWrapper sB(0,sphereB.getCollisionShape(), &sphereB, sphereB.getWorldTransform());
	algoSS = collisionWorld->getDispatcher()->findAlgorithm(&sA, &sB);
    

}


//
//  see bulletPhysics CollisionInterfaceDemo.cpp
//
float collisionTester::testSphereSphere(ofMatrix4x4& matA, ofMatrix4x4& matB){
    btTransform transA, transB;
    
    transA.setFromOpenGLMatrix(matA.getPtr());
    transB.setFromOpenGLMatrix(matB.getPtr());
    
    sphereA.setWorldTransform(transA);
    sphereB.setWorldTransform(transB);
    
    return collisionTest(&sphereA, &sphereB);
}


float collisionTester::collisionTest(btCollisionObject * objectA, btCollisionObject * objectB){
    
    float dist = 9999999;
    
    btCollisionObjectWrapper ob0(0,objectA->getCollisionShape(),objectA, objectA->getWorldTransform());
    btCollisionObjectWrapper ob1(0,objectB->getCollisionShape(),objectB, objectB->getWorldTransform());
//    
//	btCollisionAlgorithm* algo = collisionWorld->getDispatcher()->findAlgorithm(&ob0, &ob1);

    btCollisionAlgorithm * algo = algoSS;
    
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
        
		glDisable(GL_DEPTH_TEST);
		int numContacts = contactManifold->getNumContacts();
		//bool swap = obA == objectA;
        
        //printf("numManifolds=%i, numContact = %i \n", numManifolds, numContacts);
        
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            
//			glBegin(GL_LINES);
//			glColor3f(0, 0, 0);            
//			btVector3 ptA = swap ?pt.getPositionWorldOnA():pt.getPositionWorldOnB();
//			btVector3 ptB = swap ? pt.getPositionWorldOnB():pt.getPositionWorldOnA();
//            
//			glVertex3d(ptA.x(),ptA.y(),ptA.z());
//			glVertex3d(ptB.x(),ptB.y(),ptB.z());
//            
//			glEnd();
            
            float distTmp = pt.getDistance();
            if(distTmp<dist)
                dist=distTmp;
                        
		}
        
		//you can un-comment out this line, and then all points are removed
		contactManifold->clearManifold();	
	}
    
    
    return dist;
}

