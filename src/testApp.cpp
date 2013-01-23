#include "testApp.h"
#include "ofxMtb.h"

#include<set>

collisionTester testApp::tester;

// GUI parts
string testApp::CURRENT_PROCESS = "NONE";

const string testApp::CONNECT_RANDOM = "CONNECT_RANDOM";
const string testApp::CONNECT_NEAR = "CONNECT_NEAR";
const string testApp::COLLISION_TEST = "COLLISION_TEST";
const string testApp::REMOVE_GROUPS = "REMOVE_GROUPS";
const string testApp::REMOVE_GROUPS_MIN_NUM = "REMOVE_MIN_NUM";
const string testApp::RESET_CYLINDERS = "RESET_CYLINDERS";
const string testApp::RESET_SPHERES = "RESET_SPHERES";
const string testApp::RESET_INSTSANCE_SHAPE = "RESET_INSTSANCE_SHAPE";
const string testApp::SAVE_DATA = "SAVE_DATA";

string testApp::PROCESS_NAME[] = {  CONNECT_RANDOM,
    CONNECT_NEAR,
    
    COLLISION_TEST,
    REMOVE_GROUPS,
    
    RESET_CYLINDERS,
    RESET_SPHERES,
    RESET_INSTSANCE_SHAPE,
    SAVE_DATA
};

// param
const string testApp::RENDER_NORMALS = "RENDER_NORMALS";
const string testApp::FLAT_SHADING = "FLAT_SHADING";
const string testApp::SPHERE_RADIUS = "SPHERE_RADIUS";
const string testApp::SPHERE_RESOLUTION = "SPHERE_RESOLUTION";
const string testApp::CYLINDER_RADIUS = "CYLINDER_RADIUS";
const string testApp::CYLINDER_RESOLUTION = "CYLINDER_RESOLUTION";

void testApp::setup(){
    
#ifndef NDEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#else
    ofSetLogLevel(OF_LOG_WARNING);
#endif

    char mes[255];
    sprintf(mes, "renature modeller start (compiled on %s at time %s JST)\n",__DATE__,__TIME__);
    myLogRelease(mes);
    sprintf(mes, "using openFrameworks %d.%d", OF_VERSION, OF_VERSION_MINOR);
    myLogRelease(mes);
    
    isShaderDirty = true;
    bWireframe = false;
    bCollisionDebugDraw = false;
    nowProcessing = false;
    posModelPath_P = "models/bee_60k_MASTER_mesh_wN.ply";
    mShdInstanced = NULL;
    //ofSetFrameRate(60);
	ofSetVerticalSync(false);
	setupCameraLightMaterial();
    setupGui();
    updateShaders();

    
    // ---------------------------------------------------
    compScale = 1;
    posScale = 100;

    int cylNum = 300;
    int sphNum = 5000;
    {
        setupSphereShape(1, 8);

#define SETUP_SPHERE
#ifdef SETUP_SPHERE
        myLogDebug("setup Spheres");
#if 1
        spheres.loadInstancePositionFromModel(posModelPath_P, posScale);
#else
       

        ofMatrix4x4 * ms = new ofMatrix4x4[sphNum];
        ofVec3f * scales = new ofVec3f[sphNum];
        
        for(int i=0; i<sphNum; i++){
            ms[i].makeIdentityMatrix();
            ms[i].translate(ofRandom(-posScale, posScale), ofRandom(-posScale, posScale), ofRandom(-posScale, posScale));
            scales[i].set(1, 1, 1);
        }
        
        spheres.loadInstancePositionFromMatrices(ms, scales, sphNum);
#endif
#endif
    }
    
    {
        setupCylinderShape(0.2, 8);
#define SETUP_CYLINDER 1
#ifdef SETUP_CYLINDER
        myLogDebug("setup Cylinders");

        
#if 1
        connectRandom(&spheres, &cylinders, cylNum, 1, 10000);
#else

        ofMatrix4x4 * ms = new ofMatrix4x4[cylNum];
        ofVec3f * scales = new ofVec3f[cylNum];

        float pos = 5;
        for(int i=0; i<cylNum; i++){
            ms[i].makeIdentityMatrix();
            ms[i].rotate(12, ofRandom(-180, 180), ofRandom(-180, 180), ofRandom(-180, 180));
            ms[i].translate(ofRandom(-pos, pos), ofRandom(-pos, pos), ofRandom(-pos, pos));
            scales[i].set(1,1,1);
        }
        
        
        cylinders.loadInstancePositionFromMatrices(ms, INSTANCE_CYLINDER, cylNum);

#endif
#endif
    }
    
    //instancedComponent::printGroupData();
    
    tester.initAlgo();
}

void testApp::setupSphereShape(float radius, int resolution){
    ofSetSphereResolution(resolution);
    ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
    spheres.setInstanceType(INSTANCE_SPHERE);
    spheres.loadInstanceMesh(sphere, ofVec3f(radius, radius, radius));
    tester.resetSphereShape(radius);
}


void testApp::setupCylinderShape(float radius, int resolution){
    ofMesh cylinder = createCylinderZ(radius, 1, resolution, 1);
    cylinders.setInstanceType(INSTANCE_CYLINDER);
    cylinders.loadInstanceMesh(cylinder);
    tester.resetCylinderShape(ofVec3f(radius*0.5, 123, 0.5));  // do not use y value
}

void testApp::update(){
    
    processRequest();
	updateShaders();
	
	camMain.setNearClip(prmFloat["zNear"]);
	camMain.setFarClip(prmFloat["zFar"]);

    processGui();
    spheres.update();
    cylinders.update();

}

void testApp::draw(){
    mainDraw();
    //testDraw();
}

void testApp::testDraw(){
    ofBackground(200, 200, 200);
    
    camMain.begin();
    ofEnableLighting();
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	ofEnableSeparateSpecularLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);
    
    mLigDirectional.enable();
    mMatMainMaterial.begin();
    
    {
        glPushMatrix();
        glTranslatef(-10, 0, 0);
        ofMesh cyl = createCylinderZ(4, 7, 12, 1);
        ofSetColor(200,200,200);
        //cyl.drawWireframe();
        cyl.draw();
        glPopMatrix();
    }
    
    {
        glPushMatrix();
        glTranslatef(0, 0, 0);
        ofSetSphereResolution(5);
        ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
        ofSetColor(200,200,200);
        //sphere.drawWireframe();
        sphere.draw();
        glPopMatrix();
    }

    {
        glPushMatrix();
        glTranslatef(10, 0, 0);
        ofMesh sphere = createQuadSphere(1, 12, 12);
        ofSetColor(200,200,200);
        //sphere.drawWireframe();
        sphere.draw();
        glPopMatrix();
    }
    
    mMatMainMaterial.end();
    mLigDirectional.disable();
    camMain.end();
}

void testApp::mainDraw(){
    ofSetColor(255);
	ofBackgroundGradient(ofColor::fromHsb(0, 0, 175), ofColor::fromHsb(0, 0, 20), OF_GRADIENT_LINEAR);
	camMain.begin();
	
    ofEnableLighting();
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	ofEnableSeparateSpecularLight();

	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    
	mShdInstanced->begin();
        if (prmBool[RENDER_NORMALS]){
            mShdInstanced->setUniform1f(RENDER_NORMALS.c_str(), 1.0);
        } else {
            mShdInstanced->setUniform1f(RENDER_NORMALS.c_str(), 0.0);
        }

        if (prmBool[FLAT_SHADING]){
            mShdInstanced->setUniform1f(FLAT_SHADING.c_str(), 1.0);
            glShadeModel(GL_FLAT);
            glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION
        } else {
            mShdInstanced->setUniform1f(FLAT_SHADING.c_str(), 0.0);
            glShadeModel(GL_SMOOTH);
            glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
        }


        mLigDirectional.enable();
        mMatMainMaterial.begin();
    
        ofSetColor(ofFloatColor(prmFloat["COLOR_R"], prmFloat["COLOR_G"], prmFloat["COLOR_B"]));
        if (bWireframe) {
            spheres.drawWireframe(mShdInstanced);
            cylinders.drawWireframe(mShdInstanced);
        }else{
            spheres.draw(mShdInstanced);
            cylinders.draw(mShdInstanced);
        }
    
    
        mMatMainMaterial.end();
        mLigDirectional.disable();
	mShdInstanced->end();
    
    
	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

    ofSetColor(255,255,255);
	ofDisableLighting();

    if(bCollisionDebugDraw){
        instancedComponent::debugDraw();
        tester.drawAllContanctPts();
    }

    camMain.end();
	
    //  GUI draw
	mainPnl.draw();
    
    
    int y = 20;
    int x = 15;
    int h = 20;
    
    ofSetColor(0,1,2,100);
    ofRect(0, 0, 280, ofGetHeight()-50);

    ofSetColor(255,255,255,255);
	ofDrawBitmapString("fps : "+ofToString(ofGetFrameRate()), x, y);
    
    ofVec3f cp = camMain.getPosition();
    ofDrawBitmapString("camera position: "+ofToString(cp.x)+", "+ofToString(cp.y)+", "+ofToString(cp.z), x, y+=h);

    //ofVec3f tp = camMain.getTarget().getPosition();
    //ofDrawBitmapString("target position: "+ofToString(tp.x)+", "+ofToString(tp.y)+", "+ofToString(tp.z), x, y+=h);
    
    ofDrawBitmapString("instance total : "+ofToString(instancedComponent::getInstanceMap().size()), x, y+=h);
    ofDrawBitmapString("group total : "+ofToString(instancedComponent::getGroupTotalNum()), x, y+=h);
    
    ofDrawBitmapString("particle total : "+ofToString(spheres.getInstanceNum()), x, y+=h);
    ofDrawBitmapString("line total : "+ofToString(cylinders.getInstanceNum()), x, y+=h);
    
    
    vector<string> strings = instancedComponent::printGroupData(false);

    int startY = y;
    for(int i=0; i<strings.size(); i++){
        
        ofDrawBitmapString(strings[i], x, y+=h);
        if(ofGetHeight()-2*h<y){
            y=startY; x += 140;
        }
    }
    
    waitDraw();
    
}

void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){

	switch (key) {
			case 'f':
			ofToggleFullscreen();
			break;
	
        case 'w':
			bWireframe = !bWireframe;
			break;
            
        case 'd':
            bCollisionDebugDraw = !bCollisionDebugDraw;
            break;
            
		default:
			break;
	}
	
}
void testApp::mouseMoved(int x, int y ){
    ofRectangle area = mainPnl.getShape();
    bool in = area.inside(x, y);

    if(!in){
        camMain.mouseMoved(x, y);
    }
}
void testApp::mouseDragged(int x, int y, int button){
    ofRectangle area = mainPnl.getShape();
    bool in = area.inside(x, y);
    
    if(!in){
        camMain.mouseDragged(x, y, button);
    }
}
void testApp::mousePressed(int x, int y, int button){
    ofRectangle area = mainPnl.getShape();
    bool in = area.inside(x, y);
    
    if(!in){
        camMain.mousePressed(x, y, button);
    }
}
void testApp::mouseReleased(int x, int y, int button){
    ofRectangle area = mainPnl.getShape();
    bool in = area.inside(x, y);
    
    if(!in){
        camMain.mouseReleased(x, y, button);
    }
}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}



void testApp::processGui(){
    int size = sizeof(PROCESS_NAME)/sizeof(string);
    for (int i=0; i<size; i++) {
        if(prmBool[PROCESS_NAME[i]] == true){
            CURRENT_PROCESS = PROCESS_NAME[i];
            nowProcessing = true;
            break;
        }
    }
}

/*
 CONNECT_RANDOM,
 CONNECT_NEAR,
 COLLISION_TEST,
 REMOVE_GROUPS,
 RESET_CYLINDERS,
 RESET_SPHERES,
 RESET_INSTSANCE_SHAPE,
 SAVE_DATA
 */
void testApp::processRequest(){

    if(nowProcessing){
        if(CURRENT_PROCESS == CONNECT_RANDOM){
            connectRandom(&spheres, &cylinders, 100, 1, 1000);
        }else if (CURRENT_PROCESS == CONNECT_NEAR){
            
        }else if (CURRENT_PROCESS == COLLISION_TEST){
            processCollision();
        }else if(CURRENT_PROCESS == REMOVE_GROUPS){
            int min = prmInt[REMOVE_GROUPS_MIN_NUM];
            spheres.removeSmallGroup(min);          // should be static
            cylinders.removeSmallGroup(min);

        }else if(CURRENT_PROCESS == RESET_CYLINDERS){
            cylinders.reset();
            connectionList.clear();
        }else if(CURRENT_PROCESS == RESET_SPHERES){
            
        }else if(CURRENT_PROCESS == RESET_INSTSANCE_SHAPE){
            setupSphereShape(prmFloat[SPHERE_RADIUS], prmInt[SPHERE_RESOLUTION]);
            setupCylinderShape(prmFloat[CYLINDER_RADIUS], prmInt[CYLINDER_RESOLUTION]);
        }else if(CURRENT_PROCESS == SAVE_DATA){
            
            // TOP dir
            string topDirName = "instanceData";
            ofDirectory topDir(topDirName);
            if(!topDir.exists()){
                topDir.createDirectory(topDirName);
            }
            // SUB timestamped dir
            string subDirName = topDirName + "/" + ofGetTimestampString("%m%d_%H%M_%S");
            ofDirectory subDir(subDirName);
            subDir.createDirectory(subDirName);

            string timestamp = ofGetTimestampString();
            spheres.saveInstanceDataToCsv(subDirName);
            cylinders.saveInstanceDataToCsv(subDirName);
        }
        
        nowProcessing = false;
        prmBool[CURRENT_PROCESS] = false;
        CURRENT_PROCESS = "DONE";
    }
}

void testApp::waitDraw(){
    if(nowProcessing){
        float x = ofGetWidth()  * 0.5;
        float y = ofGetHeight() * 0.5;
        float w = ofGetWidth()  * 0.27;
        float h = ofGetHeight() * 0.27;

        glPushMatrix();
        glTranslatef(x, y, 0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(0, 10, 20, 200);
        ofRect(0, 0, w, h);
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("PROCESS " + CURRENT_PROCESS + " ...", -w/3,0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        glPopMatrix();
    }
}

void testApp::setupGui(){
    
	mainPnl.setup("MAIN SETTINGS", "settings.xml", ofGetWidth() - 300, 50);
	mainPnl.add(prmFloat["zNear"].set("Near Clip",0,0,100));
	mainPnl.add(prmFloat["zFar"].set("Far Clip",0,0,5000.0));
	mainPnl.add(prmBool[RENDER_NORMALS].set(RENDER_NORMALS, false));
	mainPnl.add(prmBool[FLAT_SHADING].set(FLAT_SHADING, true));
	
    mainPnl.add(prmFloat[SPHERE_RADIUS].set(SPHERE_RADIUS,0,0,2.0));
    mainPnl.add(prmInt[SPHERE_RESOLUTION].set(SPHERE_RESOLUTION,0,3,20));
    mainPnl.add(prmFloat[CYLINDER_RADIUS].set(CYLINDER_RADIUS,0,0,2.0));
    mainPnl.add(prmInt[CYLINDER_RESOLUTION].set(CYLINDER_RESOLUTION,0,3,20));
    
    mainPnl.add(prmFloat["COLOR_R"].set("Red", 1.0, 0.0, 1.0));
    mainPnl.add(prmFloat["COLOR_G"].set("Green", 1.0, 0.0, 1.0));
    mainPnl.add(prmFloat["COLOR_B"].set("Blue", 1.0, 0.0, 1.0));

    mainPnl.add(prmBool[CONNECT_RANDOM].set(CONNECT_RANDOM, false));
    mainPnl.add(prmBool[COLLISION_TEST].set(COLLISION_TEST, false));
    
    
    mainPnl.add(prmInt[REMOVE_GROUPS_MIN_NUM].set(REMOVE_GROUPS_MIN_NUM, 1, 0, 30));

    mainPnl.add(prmBool[REMOVE_GROUPS].set(REMOVE_GROUPS, false));

    mainPnl.add(prmBool[RESET_SPHERES].set(RESET_SPHERES, false));
    mainPnl.add(prmBool[RESET_CYLINDERS].set(RESET_CYLINDERS, false));
    mainPnl.add(prmBool[RESET_INSTSANCE_SHAPE].set(RESET_INSTSANCE_SHAPE, false));
    mainPnl.add(prmBool[SAVE_DATA].set(SAVE_DATA, false));

    
    mainPnl.loadFromFile("settings.xml");
}

void testApp::setupCameraLightMaterial(){
    camMain.setupPerspective(false);
    camMain.setDistance(400);
    camMain.disableMouseInput();
    
	mLigDirectional.setup();
	mLigDirectional.setDirectional();
	mLigDirectional.setAmbientColor(ofFloatColor(0.4, 0.4, 0.4));
	mLigDirectional.setDiffuseColor(ofFloatColor(0.7, 0.7, 0.7));
	mLigDirectional.setSpecularColor(ofFloatColor(0.1,0.1,0.1));
    
    mMatMainMaterial.setAmbientColor(ofFloatColor(0.8, 0.1, 0.1));
	mMatMainMaterial.setDiffuseColor(ofFloatColor(0.8, 0.1, 0.1));
	mMatMainMaterial.setSpecularColor(ofFloatColor(0,0,0));
	mMatMainMaterial.setShininess(10.1f);
}

void testApp::updateShaders(bool doLink){
    if (isShaderDirty){
		
		GLuint err = glGetError();	// we need this to clear out the error buffer.
        
		if (mShdInstanced != NULL ) delete mShdInstanced;
		mShdInstanced = new ofShader();
        
        //		mShdInstanced->load("shaders/instancedTexTrans");
        mShdInstanced->setupShaderFromFile(GL_VERTEX_SHADER, "shaders/instancedTexTrans.vert");
        mShdInstanced->setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/instancedTexTrans.frag");
        
        if(doLink){
            mShdInstanced->linkProgram();
            err = glGetError();
            ofLogNotice() << "Loaded instanced Shader: " << err;
        }
        
		isShaderDirty = false;
	}
}

bool testApp::connectInstanace(instance &instA, instance &instB, float minDist, float maxDist, instance& newIns){
    
    // 3. check A - B distance
    ofVec3f vA = instA.matrix.getTranslation();
    ofVec3f vB = instB.matrix.getTranslation();

    ofVec3f vAB = vB - vA;
    float dist = vAB.length();

    if(minDist<dist && dist<maxDist){

        // 4. put
//        ofMatrix4x4 mat;
//        ofVec3f scale;
//        ofVec3f pos = vA + vAB*0.5;

        ofVec3f yAxis(0,0,1);
        float angle = yAxis.angle(vAB);
        ofVec3f prep = yAxis.cross(vAB);

        newIns.scale.set(1, 1, dist);
        newIns.matrix.rotate(angle, prep.x, prep.y, prep.z);
        newIns.matrix.translate(vA + vAB*0.5);
        return true;
    }else{
        return false;
    }
}

// should be used parallel_for
void testApp::connectRandom(instancedComponent *ic, instancedComponent *ic2, int numAllCylinders, float minDist, float maxDist){

    int startTime = ofGetElapsedTimeMillis();
    myLogRelease("Start ConnectRandom Process : time : " + ofToString(startTime));
    

    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int numGroups = STL_UTIL::getAllKeySize(instanceMap);
    int numInstances = instanceMap.size();
    
    int index = 0;
    int numFind = 0;
    for(int i=0; i<numAllCylinders; i++){
        int numTry = 0;
        int maxTry = 10000;
        bool find = false;
        
        do{
            // 1. select instance
            int indexA = ofRandom(numInstances);               // todo: invalid
            int indexB = ofRandom(numInstances);
            
            // 2. check if pair is already connected
            bool same = false;
            idPair idp(indexA, indexB);
            for(int i=0; i<connectionList.size(); i++){
                if(idp == connectionList[i]){
                    same = true;
                    cout << "same";
                    break;
                }
            }
            
            if(!same){
                INSTANCE_MAP_ITR itrA = instanceMap.begin();
                INSTANCE_MAP_ITR itrB = instanceMap.begin();
                std::advance(itrA, indexA);
                std::advance(itrB, indexB);
                
                instance instA = itrA->second;
                instance instB = itrB->second;

                if(instA.type!= INSTANCE_CYLINDER && instB.type!=INSTANCE_CYLINDER ){
                    instance newCylinder;
                    newCylinder.type = INSTANCE_CYLINDER;
                    find = connectInstanace(instA, instB, minDist, maxDist, newCylinder);
                    if(find){
                        // add to deafault group
                        ic2->addInstance(newCylinder);
                        numFind++;
                        connectionList.push_back(idp);
                    }
                }
            }
            
            numTry++;
            
            if(maxTry < numTry)
                find = true;
        }while (!find);
    }
    
//    int instanceNum = ic2->getInstanceNum();
//    ic2->setInstanceNum(instanceNum + numFind);
    
    cylinders.updateInstanceNum();
    
    
    int endTime = ofGetElapsedTimeMillis();
    myLogRelease("Finish ConnectRandom process : elapsed " + ofToString((float)(endTime-startTime)/1000.0)+" sec");

}

float testApp::getCollisionDistance(instance &insA, instance &insB){
    float dist=99999;

    ofMatrix4x4& matA = insA.matrix;
    ofVec3f& sA = insA.scale;
    INSTANCE_TYPE tA = insA.type;
    
    ofMatrix4x4& matB = insB.matrix;
    ofVec3f& sB = insB.scale;
    INSTANCE_TYPE tB = insB.type;
    
    if(tA == INSTANCE_SPHERE && tB==INSTANCE_SPHERE)
        dist = tester.testSphereSphere(matA,sA,matB,sB);
    else if(tA==INSTANCE_SPHERE && tB==INSTANCE_CYLINDER)
        dist = tester.testSphereCylinder(matA,sA,matB,sB);
    else if (tA==INSTANCE_CYLINDER && tB==INSTANCE_SPHERE)
        dist = tester.testSphereCylinder(matB,sB, matA,sA);
    else if(tA==INSTANCE_CYLINDER && tB==INSTANCE_CYLINDER)
        dist = tester.testCylinderCylinder(matA,sA,matB,sB);
    else{
        myLogRelease("Can not test collision");
        return 98765432;
    }
    return dist;

}

void testApp::processCollision(){
#if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
    processCollisionParallel();
#else
    
    int startTime = collisionStart();
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    INSTANCE_MAP_ITR itrA = instanceMap.begin();
    
    typedef map<instance*, int> TMAP;
    typedef pair<instance*, int> TPAIR;
    typedef TMAP::iterator TITR;
    TMAP tmap;
    
    char m[255];
    for(int i=0; itrA!=instanceMap.end(); itrA++, i++){
        sprintf(m, "processCollisionTest i:%d", i);
        myLogDebug(ofToString(m));        
        
        INSTANCE_MAP_ITR itrB = instanceMap.begin();
        std:advance(itrB, i+1);
        for(; itrB!=instanceMap.end(); itrB++){
            instance& insA = itrA->second;
            instance& insB = itrB->second;
            
            float dist=getCollisionDistance(insA, insB);
            
            if(dist<0.0) {
                int groupIdA = itrA->first;
                int groupIdB = itrB->first;
                
                //
                //  check instance is in tempolaly container.
                //  if existm, it sohuld have non -1 groupId.
                //
                TITR titrA = tmap.find(&itrA->second);
                if(titrA!=tmap.end())
                    groupIdA = titrA->second;
                
                TITR titrB = tmap.find(&itrB->second);
                if(titrB!=tmap.end())
                    groupIdB = titrB->second;
                
                
                //
                // it's already same group.
                // ** this line may break correct grouping process
                //
                if( (groupIdA!=-1 && groupIdB!=-1) && groupIdA == groupIdB)
                    continue;
                
                //
                //  grouId -1 means this instance does not have any group
                //
                
                if(groupIdA==-1){
                    if(groupIdB==-1){
                        // move instance A, B to new group
                        int newId = instancedComponent::incGroupIdMaster();
                        tmap.insert(TPAIR(&itrA->second, newId));
                        tmap.insert(TPAIR(&itrB->second, newId));
                    }else{
                        // move A instance to B group
                        tmap.insert(TPAIR(&itrA->second, groupIdB));
                    }
                }else{
                    if(groupIdB==-1){
                        // move B instance to A group
                       tmap.insert(TPAIR(&itrB->second, groupIdA));
                    }else{
                        // at first move B instance to A group
                        TITR titrB = tmap.find(&itrB->second);
                         titrB->second = groupIdA;
                        
                        // search from tmap
                        TITR titr = tmap.begin();
                        
                        for(; titr!=tmap.end(); titr++){
                            if(titr->second == groupIdB){
                                titr->second =groupIdA;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // change group id
    {
        TITR itr = tmap.begin();
        for(; itr!=tmap.end(); itr++){
            instance * ins = itr->first;
            int groupId = itr->second;
            
            if(ins!=NULL){
                INSTANCE_MAP_ITR  iitr = instanceMap.begin();
                for(; iitr!=instanceMap.end(); iitr++){
                    if(&iitr->second == ins){
                        spheres.changeInstanceGroupId(iitr, groupId);
                        
                        break;
                    }
                }
            }
        }
    }
    
    collisionEnd(startTime);
#endif
}

int testApp::collisionStart(){
    instancedComponent::resetGroup();
    
    int startTime = ofGetElapsedTimeMillis();
    myLogRelease("collisionTest startTime:  "+ ofToString(startTime));
    
#ifndef NDEBUG
    instancedComponent::printData();
#endif
    return startTime;
    
}

void testApp::collisionEnd(int startTime){
    // update group totalNum
    instancedComponent::updateGroupTotalNum();

    // coloring
    ofColor c;
    spheres.setGroupColorGradient();
    spheres.setGroupColor(-1, ofColor(0,0,0));

    
    spheres.setCltexNeedUpdate(true);
    spheres.setVtxtexNeedUpdate(true);
    
    cylinders.setCltexNeedUpdate(true);
    cylinders.setVtxtexNeedUpdate(true);
    
    
    int endTime = ofGetElapsedTimeMillis();
    myLogRelease("collisionTest endTime:  "+ ofToString(endTime)+", elapsed: " + ofToString((float)(endTime-startTime)/1000.0));
    myLogRelease("finish CollisionTest");
}


#if defined (USE_TBB) && defined(USE_TBB_COLLISIION)
void testApp::processCollisionParallel(){
    
    int startTime = collisionStart();
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int N = instanceMap.size();
    CollisionTable table;
    parallel_for(blocked_range<size_t>(0, N), Tally(table, instanceMap));
    
    // change group id
    CollisionTable::iterator itr = table.begin();
    for(; itr!=table.end(); ++itr){
        instance * ins = itr->first;
        int groupId = itr->second;
        if(ins!=NULL){
            INSTANCE_MAP_ITR  iitr = instanceMap.begin();
            for(; iitr!=instanceMap.end(); iitr++){
                if(&iitr->second == ins){
                    spheres.changeInstanceGroupId(iitr, groupId);
                    break;
                }
            }
        }
    }
    
    collisionEnd(startTime);
    
}
#endif