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

collisionTester::collisionTester(){
    initCollisionWorld();
}

void collisionTester::resetSphereShape(float radius, float collisionMargin){

    float r = radius + collisionMargin;
    if( sphereShape == NULL ){
        sphereShape = new btSphereShape(r);
    }else{
        sphereShape->setImplicitShapeDimensions(btVector3(r, 1,1) );
        sphereShape->setMargin(r);
    }
    cout << "setup sphere shape, radius: " << radius << ", margin: " << collisionMargin << endl;
}

void collisionTester::resetCylinderShape(ofVec3f halfExtent, float collisionMargin){
    btVector3 half(halfExtent.x+collisionMargin , 12345, halfExtent.z);

    if(cylinderShape == NULL ){
        cylinderShape = new btCylinderShapeZ(half);
        //cylinderShape->setMargin(0);
    }else{
//        setSafeMargin(halfExtents);
//        btVector3 margin(getMargin(),getMargin(),getMargin());
//          m_implicitShapeDimensions = (halfExtents * m_localScaling) - margin;
        //cylinderShape->setSafeMargin(half);
        cylinderShape->setImplicitShapeDimensions( half );
    }
    cout << "setup Cylinder shape, radius: " << half.x() << ", margin: " << collisionMargin << endl;
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
    collisionWorld->debugDrawObject(ins.colObj.getWorldTransform(), ins.colObj.getCollisionShape(), color);
}

void collisionTester::scaleCylinderShape(const ofVec3f &scale){
    cylinderShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
}

void collisionTester::setTransformFromOF(const ofMatrix4x4& mat, const ofVec3f& s, btCollisionObject& obj){
    
    btTransform trans;
    trans.setFromOpenGLMatrix(mat.getPtr());
    obj.setWorldTransform(trans);
    obj.getCollisionShape()->setLocalScaling(btVector3(s.x, s.y, s.z));
}

float collisionTester::collisionTest(ofMatrix4x4& matA, ofVec3f& sA, ofMatrix4x4& matB, ofVec3f& sB, btCollisionObject & objA, btCollisionObject & objB ){
    //  see bulletPhysics CollisionInterfaceDemo.cpp
    //setTransformFromOF(matA, sA, objA);
    //setTransformFromOF(matB, sB, objB);
        
    float dist = std::numeric_limits<float>::max();
    
    btCollisionObjectWrapper w0(0,, &objA, objA.getWorldTransform());
    btCollisionObjectWrapper w1(0,objB.getCollisionShape(), &objB, objB.getWorldTransform());
	btManifoldResult contactPointResult(&w0,&w1);
    btCollisionAlgorithm * algo = collisionWorld->getDispatcher()->findAlgorithm(&w0, &w1);
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



//btCollisionAlgorithm * collisionTester::prepareAlgo(const btCollisionObject &objA, const btCollisionObject &objB){
//    btCollisionObjectWrapper wA(0,objA.getCollisionShape(), &objA, objA.getWorldTransform());
//    btCollisionObjectWrapper wB(0,objB.getCollisionShape(), &objB, objB.getWorldTransform());
//    return collisionWorld->getDispatcher()->findAlgorithm(&wA, &wB);
//}
