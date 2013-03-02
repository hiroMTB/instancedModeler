#include "testApp.h"
#include "ofxMtb.h"

#include<set>


collisionTester * testApp::tester = NULL;

// GUI parts
string testApp::CURRENT_PROCESS = "NONE";

const string testApp::CONNECT_RANDOM = "CONNECT_RANDOM";
const string testApp::CONNECT_GROUP = "CONNECT_GROUP";
const string testApp::CONNECT_NEAR = "CONNECT_NEAR";

const string testApp::COLLISION_MARGIN = "COLLISION_MARGIN";
const string testApp::COLLISION_TEST = "COLLISION_TEST";

const string testApp::REMOVE_GROUPS = "REMOVE_GROUPS";
const string testApp::REMOVE_GROUPS_MIN_NUM = "REMOVE_GROUPS_MIN_NUM";
const string testApp::REMOVE_ALL_CYLINDERS = "REMOVE_ALL_CYLINDERS";

const string testApp::RESET_INSTSANCE_SHAPE = "RESET_INSTSANCE_SHAPE";
const string testApp::SAVE_DATA = "SAVE_DATA";
const string testApp::REMOVE_DUPLICATE = "REMOVE_DUPLICATE";

string testApp::PROCESS_NAME[] = {
    CONNECT_RANDOM,
    CONNECT_NEAR,
    CONNECT_GROUP,
    COLLISION_TEST,
    REMOVE_GROUPS,
    REMOVE_DUPLICATE,
    REMOVE_ALL_CYLINDERS,
    RESET_INSTSANCE_SHAPE,
    SAVE_DATA
};

// param
const string testApp::SPHERE_RADIUS                 = "SPHERE_RADIUS";
const string testApp::SPHERE_RESOLUTION             = "SPHERE_RESOLUTION";
const string testApp::CYLINDER_RADIUS               = "CYLINDER_RADIUS";
const string testApp::CYLINDER_RESOLUTION           = "CYLINDER_RESOLUTION";

const string testApp::CONNECT_GROUP_CYLINDER_NUM    = "CONNECT_GROUP_CYLINDER_NUM";
const string testApp::CONNECT_GROUP_MIN_DIST        = "CONNECT_GROUP_MIN_DIST";
const string testApp::CONNECT_GROUP_MAX_DIST        = "CONNECT_GROUP_MAX_DIST";

const string testApp::CONNECT_RANDOM_CYLINDER_NUM   = "CONNECT_RANDOM_CYLINDER_NUM";
const string testApp::CONNECT_RANDOM_MIN_DIST       = "CONNECT_RANDOM_MIN_DIST";
const string testApp::CONNECT_RANDOM_MAX_DIST       = "CONNECT_RANDOM_MAX_DIST";

const string testApp::DRAW_WIREFRAME                = "DRAW_WIREFRAME";
const string testApp::DRAW_COLLISION_SHAPE          = "DRAW_COLLISION_SHAPE";
const string testApp::DRAW_COLLISION_DISTANCE       = "DRAW_COLLISION_DISTANCE";

#include "assimp.h"
#include "aiScene.h"

void testApp::setup(){

//    int m = aiGetVersionMajor();
//    
//    
    
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
    
    bWireframe = false;
    bCollisionDebugDraw = false;
    bNowProcessing = false;
    
    posModelPath_P = "models/bee_20k_MASTER_mesh_wN.ply";
    mShdInstanced = NULL;
    
    //ofSetFrameRate(60);
	ofSetVerticalSync(false);
	setupCameraLightMaterial();
    setupGui();
    setupShaders();

    
    tester = new collisionTester();
    
    // ---------------------------------------------------
    compScale = 1;
    posScale = 33; // 33cm box sizes

    int sphNum = 10;
    int cylNum = 4;
    {
        float   sRad = prmFloat[SPHERE_RADIUS];
        int     sRes = prmInt[SPHERE_RESOLUTION];
        float   cRad = prmFloat[CYLINDER_RADIUS];
        int     cRes = prmInt[CYLINDER_RESOLUTION];
        setupSphereShape(sRad, sRes);
        setupCylinderShape(cRad, cRes);
        
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

    
    finishSound.loadSound("sound/finishSound.wav");
}

void testApp::setupSphereShape(float radius, int resolution){
    ofSetSphereResolution(resolution);
    ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
    spheres.setInstanceType(INSTANCE_SPHERE);
    spheres.loadInstanceMesh(sphere, ofVec3f(radius, radius, radius));
    collisionTester::resetSphereShape(radius);
}


void testApp::setupCylinderShape(float radius, int resolution){
    ofMesh cylinder = createCylinderZ(radius, 1, resolution, 1);
    cylinders.setInstanceType(INSTANCE_CYLINDER);
    cylinders.loadInstanceMesh(cylinder);
    collisionTester::resetCylinderShape(ofVec3f(radius, 123, 0.5));  // do not use y value
}

void testApp::update(){
    
    processRequest();


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
	
    ofNoFill();
    ofBox(0, 0, 0, posScale*10);
    
    ofEnableLighting();
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	ofEnableSeparateSpecularLight();

    glShadeModel(GL_FLAT);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    
	mShdInstanced->begin();

        mLigDirectional.enable();
        mMatMainMaterial.begin();
    
        ofSetColor(ofFloatColor(prmFloat["COLOR_R"], prmFloat["COLOR_G"], prmFloat["COLOR_B"]));
        if (prmBool[DRAW_WIREFRAME]) {
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

    if(prmBool[DRAW_COLLISION_SHAPE]){
        instancedComponent::debugDraw();
    }
    
    if(prmBool[DRAW_COLLISION_DISTANCE]){
        tester->drawAllContanctPts();
    }


    
    camMain.end();
	
    //  GUI draw
	pnlMain.draw();
    pnlShape.draw();
	pnlRemove.draw();
	pnlCollision.draw();
    pnlConnectR.draw();
    pnlConnectG.draw();
    
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
    
    ofDrawBitmapString("Instance Info ------", x, y+=(h*2));    
    ofDrawBitmapString("particle total : "+ofToString(spheres.getInstanceNum()), x, y+=h);
    ofDrawBitmapString("line total : "+ofToString(cylinders.getInstanceNum()), x, y+=h);
    ofDrawBitmapString("instance total : "+ofToString(instancedComponent::getInstanceMap().size()), x, y+=h);
    
    
    vector<string> strings = instancedComponent::printGroupData(false);

    ofDrawBitmapString("Group Info ------", x, y+=(h*2));
    ofDrawBitmapString("group total : "+ofToString(instancedComponent::getGroupTotalNum()), x, y+=h);

    y+=h;
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
	
//        case 'w':
//			bWireframe = !bWireframe;
//			break;
//            
//        case 'd':
//            bCollisionDebugDraw = !bCollisionDebugDraw;
//            break;
            
		default:
			break;
	}
	
}

bool testApp::guiMouseCheck(int x, int y){
    if(pnlMain.getShape().inside(x, y))
        return true;
    if(pnlRemove.getShape().inside(x, y))
        return true;
    if(pnlConnectR.getShape().inside(x, y))
        return true;
    if(pnlConnectG.getShape().inside(x, y))
        return true;
    if(pnlCollision.getShape().inside(x, y))
        return true;
    if(pnlShape.getShape().inside(x, y))
        return true;

    return false;
}

void testApp::mouseMoved(int x, int y ){
    if(!guiMouseCheck(x, y))
        camMain.mouseMoved(x, y);
}
void testApp::mouseDragged(int x, int y, int button){
    if(!guiMouseCheck(x, y))
        camMain.mouseDragged(x, y, button);
}
void testApp::mousePressed(int x, int y, int button){
    if(!guiMouseCheck(x, y))
        camMain.mousePressed(x, y, button);
}
void testApp::mouseReleased(int x, int y, int button){
//    if(!guiMouseCheck(x, y))
        camMain.mouseReleased(x, y, button);
}
void testApp::windowResized(int w, int h){
    int space = 10;
    int x = w - pnlMain.getShape().width - space;
    int y = 10;
    pnlMain.setPosition(x, y);
    y += pnlMain.getShape().height + space;
    
    pnlShape.setPosition(x, y);
    y += pnlShape.getShape().height + space;

    pnlConnectR.setPosition(x, y);
    y += pnlConnectR.getShape().height + space;

    pnlConnectG.setPosition(x, y);
    y += pnlConnectG.getShape().height + space;

    pnlCollision.setPosition(x, y);
    y += pnlCollision.getShape().height + space;

    pnlRemove.setPosition(x, y);
    y += pnlRemove.getShape().height + space;

}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}



void testApp::processGui(){
    int size = sizeof(PROCESS_NAME)/sizeof(string);
    for (int i=0; i<size; i++) {
        if(prmBool[PROCESS_NAME[i]] == true){
            CURRENT_PROCESS = PROCESS_NAME[i];
            bNowProcessing = true;
            break;
        }
    }
}



void testApp::processRequest(){

    if(bNowProcessing){
        if(CURRENT_PROCESS == CONNECT_RANDOM){
            int num = prmInt[CONNECT_RANDOM_CYLINDER_NUM];
            float min = prmFloat[CONNECT_RANDOM_MIN_DIST];
            float max = prmFloat[CONNECT_RANDOM_MAX_DIST];
            connectRandom(&spheres, &cylinders, num, min, max);
        
        }else if(CURRENT_PROCESS == CONNECT_GROUP){
            int num = prmInt[CONNECT_GROUP_CYLINDER_NUM];
            float min = prmFloat[CONNECT_GROUP_MIN_DIST];
            float max = prmFloat[CONNECT_GROUP_MAX_DIST];
            connectGroup(&spheres, &cylinders, num, min, max);
            playFinishSound();
        }else if (CURRENT_PROCESS == CONNECT_NEAR){
            
        
        }else if (CURRENT_PROCESS == COLLISION_TEST){
            processCollision();
            playFinishSound();
        }else if(CURRENT_PROCESS == REMOVE_GROUPS){
            int min = prmInt[REMOVE_GROUPS_MIN_NUM];
            spheres.removeSmallGroup(min);          // should be static
            cylinders.removeSmallGroup(min);
        
        }else if(CURRENT_PROCESS == REMOVE_DUPLICATE){
            instancedComponent::removeDuplication();
            spheres.updateRequest();
            cylinders.updateRequest();
            playFinishSound();
        }else if(CURRENT_PROCESS == REMOVE_ALL_CYLINDERS){
            cylinders.reset();
            
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
            playFinishSound();
        }
        
        bNowProcessing = false;
        prmBool[CURRENT_PROCESS] = false;
        CURRENT_PROCESS = "DONE";
    }
}

void testApp::waitDraw(){
    if(bNowProcessing){
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
    
    int x = ofGetWidth() - 350;
    int y = 50;
    int h = 50;
    int w = 250;
	pnlMain.setup("MAIN", "gui/mainSettings.xml", x, y);
    pnlMain.add(prmFloat["COLOR_R"].set("Red", 1.0, 0.0, 1.0));
    pnlMain.add(prmFloat["COLOR_G"].set("Green", 1.0, 0.0, 1.0));
    pnlMain.add(prmFloat["COLOR_B"].set("Blue", 1.0, 0.0, 1.0));
    pnlMain.add(prmBool[DRAW_WIREFRAME].set(DRAW_WIREFRAME, false));
    pnlMain.add(prmBool[DRAW_COLLISION_SHAPE].set(DRAW_COLLISION_SHAPE, false));
    pnlMain.add(prmBool[DRAW_COLLISION_DISTANCE].set(DRAW_COLLISION_DISTANCE, false));
    
    pnlMain.add(prmBool[SAVE_DATA].set(SAVE_DATA, false));
	y += (pnlMain.getHeight() + h);
    
   	pnlShape.setup("SHAPE", "gui/shapeSettings.xml", x, y);
    pnlShape.add(prmFloat [SPHERE_RADIUS].set(SPHERE_RADIUS,0,0,4.0));
    pnlShape.add(prmInt   [SPHERE_RESOLUTION].set(SPHERE_RESOLUTION,0,3,20));
    pnlShape.add(prmFloat [CYLINDER_RADIUS].set(CYLINDER_RADIUS,0,0,4.0));
    pnlShape.add(prmInt   [CYLINDER_RESOLUTION].set(CYLINDER_RESOLUTION,0,3,20));
    pnlShape.add(prmBool  [RESET_INSTSANCE_SHAPE].set(RESET_INSTSANCE_SHAPE, false));
    y += (pnlShape.getHeight() + h);
    
    pnlConnectR.setup("CONNECT RANDOM", "gui/connectRandomSettings.xml", x, y);
    pnlConnectR.add(prmInt[CONNECT_RANDOM_CYLINDER_NUM].set(CONNECT_RANDOM_CYLINDER_NUM, 1, 1, 300));
    pnlConnectR.add(prmFloat[CONNECT_RANDOM_MIN_DIST].set(CONNECT_RANDOM_MIN_DIST, 0.0, 0.0, 1000));
    pnlConnectR.add(prmFloat[CONNECT_RANDOM_MAX_DIST].set(CONNECT_RANDOM_MAX_DIST, 9999, 0.0, 1000));
    pnlConnectR.add(prmBool[CONNECT_RANDOM].set(CONNECT_RANDOM, false));
    y += pnlConnectR.getHeight() + h;


    pnlConnectG.setup("CONNECT GROUP", "gui/connectGroupSettings.xml", x, y);
    pnlConnectG.add(prmInt[CONNECT_GROUP_CYLINDER_NUM].set(CONNECT_GROUP_CYLINDER_NUM, 1, 1, 10));
    pnlConnectG.add(prmFloat[CONNECT_GROUP_MIN_DIST].set(CONNECT_GROUP_MIN_DIST, 0.0, 0.0, 1000));
    pnlConnectG.add(prmFloat[CONNECT_GROUP_MAX_DIST].set(CONNECT_GROUP_MAX_DIST, 9999, 0.0, 1000));
    pnlConnectG.add(prmBool[CONNECT_GROUP].set(CONNECT_GROUP, false));
    y += pnlConnectG.getHeight() + h;
    
    pnlCollision.setup("COLLISION", "gui/collisionSettings.xml", x, y);
    pnlCollision.add(prmFloat[COLLISION_MARGIN].set(COLLISION_MARGIN, 0, -3.0, 3.0));
    pnlCollision.add(prmBool[COLLISION_TEST].set(COLLISION_TEST, false));
    y+= (pnlCollision.getHeight() + h);
    
    pnlRemove.setup("REMOVE", "gui/removeSettings.xml", x, y);
    pnlRemove.add(prmInt [REMOVE_GROUPS_MIN_NUM].set(REMOVE_GROUPS_MIN_NUM, 1, 0, 30));
    pnlRemove.add(prmBool[REMOVE_GROUPS].set(REMOVE_GROUPS, false));
    
    pnlRemove.add(prmBool[REMOVE_DUPLICATE].set(REMOVE_DUPLICATE, false));
    pnlRemove.add(prmBool[REMOVE_ALL_CYLINDERS].set(REMOVE_ALL_CYLINDERS, false));
    

    pnlMain.loadFromFile("gui/mainSettings.xml");
    pnlShape.loadFromFile("gui/shapeSettings.xml");
    pnlConnectR.loadFromFile("gui/connectRandomSettings.xml");
    pnlConnectG.loadFromFile("gui/connectGroupSettings.xml");
    pnlCollision.loadFromFile("gui/collisionSettings.xml");
    pnlRemove.loadFromFile("gu/removeSettings.xml");
    
}

void testApp::setupCameraLightMaterial(){
    camMain.setupPerspective(false);
    camMain.setDistance(800);
    camMain.disableMouseInput();
    camMain.setNearClip(1);
	camMain.setFarClip(10000);
    
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

void testApp::setupShaders(bool doLink){
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
}


