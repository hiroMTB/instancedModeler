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

btCollisionWorld * collisionTester::collisionWorld = NULL;

btCollisionAlgorithm * collisionTester::algoSS = NULL;
btCollisionAlgorithm * collisionTester::algoSC = NULL;
btCollisionAlgorithm * collisionTester::algoCS = NULL;
btCollisionAlgorithm * collisionTester::algoCC = NULL;

btSphereShape * collisionTester::sShape[2];
btCylinderShape * collisionTester::cShape[2];

collisionTester::collisionTester(){
    initCollisionWorld();
    resetSphereShape(1, 0);
    resetCylinderShape(ofVec3f(1,1,1), 0);
}

void collisionTester::resetSphereShape(float radius, float collisionMargin){

    float r = radius + collisionMargin;
    for(int i=0; i<2; i++){

        if( sShape[i] == NULL ){
            sShape[i] = new btSphereShape(r);
        }else{
            sShape[i]->setImplicitShapeDimensions(btVector3(r, 1,1) );
            sShape[i]->setMargin(0); // this looks not working
        }
    }
}

void collisionTester::resetCylinderShape(ofVec3f halfExtent, float collisionMargin){
    btVector3 half(halfExtent.x+collisionMargin , 12345, halfExtent.z);

    for(int i=0; i<2; i++){
        if(cShape[i] == NULL ){
            cShape[i] = new btCylinderShapeZ(half);
            cShape[i]->setMargin(0); //// this is working
        }else{
            cShape[i]->setImplicitShapeDimensions( half );
            cShape[i]->setMargin(0);
        }
    }
}

collisionTester::~collisionTester(){
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

void collisionTester::debugDraw( const instance & ins){
	glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    btVector3 color(1,1,0);
    
    btCollisionShape * cs = getShape(ins);
    collisionWorld->debugDrawObject(ins.colObj.getWorldTransform(), cs, color);
}

void collisionTester::setTransformFromOF(const ofMatrix4x4& mat, btCollisionObject& obj){
    btTransform trans;
    trans.setFromOpenGLMatrix(mat.getPtr());
    obj.setWorldTransform(trans);
}

void collisionTester::prepareAlgo(){
    btCollisionObject o1, o2, o3, o4;
    o1.setCollisionShape(sShape[0]);
    o2.setCollisionShape(sShape[1]);
    o3.setCollisionShape(cShape[0]);
    o4.setCollisionShape(cShape[1]);
    btCollisionObjectWrapper sA(0, sShape[0], &o1, o1.getWorldTransform());
    btCollisionObjectWrapper sB(0, sShape[1], &o2, o2.getWorldTransform());
    btCollisionObjectWrapper cA(0, cShape[0], &o3, o3.getWorldTransform());
    btCollisionObjectWrapper cB(0, cShape[1], &o4, o4.getWorldTransform());
    algoSS = collisionWorld->getDispatcher()->findAlgorithm(&sA, &sB);
    algoSC = collisionWorld->getDispatcher()->findAlgorithm(&sA, &cA);
    algoCS = collisionWorld->getDispatcher()->findAlgorithm(&cA, &sA);
    algoCC = collisionWorld->getDispatcher()->findAlgorithm(&cA, &cB);
}

btCollisionAlgorithm * collisionTester::getAlgo(const instance &insA, const instance &insB){
    if(insA.type == INSTANCE_SPHERE ){
        if( insB.type == INSTANCE_SPHERE ){
            return algoSS;
        }else if( insB.type == INSTANCE_CYLINDER){
            return algoSC;
        }
    }else if( insA.type == INSTANCE_CYLINDER){
        if( insB.type == INSTANCE_SPHERE ){
            return algoCS;
        }else if( insB.type == INSTANCE_CYLINDER){
            return algoCC;
        }
    }
    
    return NULL;
}

float collisionTester::collisionTest(instance & insA, instance &insB ){
    //  see bulletPhysics CollisionInterfaceDemo.cpp
    float dist = std::numeric_limits<float>::max();
    
    btCollisionShape * colA = getShape(insA, 0);
    btCollisionShape * colB = getShape(insB, 1);
    btCollisionObject & objA = insA.colObj;
    btCollisionObject & objB = insB.colObj;
    objA.setCollisionShape(colA);
    objB.setCollisionShape(colB);
    
    btCollisionObjectWrapper w0(0, colA, &objA, objA.getWorldTransform());
    btCollisionObjectWrapper w1(0, colB, &objB, objB.getWorldTransform());
	btManifoldResult contactPointResult(&w0,&w1);
    
    btCollisionAlgorithm * algo = getAlgo(insA, insB);
    //btCollisionAlgorithm * algo = collisionWorld->getDispatcher()->findAlgorithm(&w0, &w1);
    algo->processCollision(&w0,&w1,collisionWorld->getDispatchInfo(),&contactPointResult);
	
	btManifoldArray manifoldArray;
	algo->getAllContactManifolds(manifoldArray);
    
	int numManifolds = manifoldArray.size();
	for(int i=0;i<numManifolds;i++)	{
		btPersistentManifold* contactManifold = manifoldArray[i];
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
	
		int numContacts = contactManifold->getNumContacts();
		bool swap = obA == &objA;
        
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            
			btVector3 ptA = swap ?pt.getPositionWorldOnA():pt.getPositionWorldOnB();        // no need?
			btVector3 ptB = swap ? pt.getPositionWorldOnB():pt.getPositionWorldOnA();
            //contactPts.push_back(ptA);
            //contactPts.push_back(ptB);
            
            float distTmp = pt.getDistance();
            if(distTmp<dist)
                dist=distTmp;
                        
        }
		contactManifold->clearManifold();
	}
    
    //algo->~btCollisionAlgorithm();
    //collisionWorld->getDispatcher()->freeCollisionAlgorithm(algo);
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

btCollisionShape * collisionTester::getShape(const instance &ins, int cid){
    if( ins.type == INSTANCE_SPHERE){
        return sShape[cid];
    }else if( ins.type == INSTANCE_CYLINDER){
        cShape[cid]->setLocalScaling(btVector3(1, 1, ins.scale.z)); // this is only for length
        return cShape[cid];
    }
}

//btCollisionAlgorithm * collisionTester::prepareAlgo(const btCollisionObject &objA, const btCollisionObject &objB){
//    btCollisionObjectWrapper wA(0,objA.getCollisionShape(), &objA, objA.getWorldTransform());
//    btCollisionObjectWrapper wB(0,objB.getCollisionShape(), &objB, objB.getWorldTransform());
//    return collisionWorld->getDispatcher()->findAlgorithm(&wA, &wB);
//}
