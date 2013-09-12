#include "testApp.h"
#include "ofxMtb.h"

#include<set>


collisionTester * testApp::tester = NULL;
testApp * testApp::singleton = NULL;

ofColor testApp::colorSphere    = ofColor(100);
ofColor testApp::colorCylinder  = ofColor(100, 100, 50);

int testApp::bgType = 0;
ofColor testApp::bgNormal    = ofColor(100);
ofColor testApp::bgLinear0   = ofColor(255);
ofColor testApp::bgLinear1   = ofColor(0);
ofColor testApp::bgCircular0 = ofColor(255);
ofColor testApp::bgCircular1 = ofColor(0);
ofColor testApp::bgBar0      = ofColor(255);
ofColor testApp::bgBar1      = ofColor(0);

float testApp::SPHERE_RADIUS            = 1.5;
int   testApp::SPHERE_RESOLUTION        = 8;
float testApp::CYLINDER_RADIUS          = 1;
int   testApp::CYLINDER_RESOLUTION      = 14;

int   testApp::CONNECT_RANDOM_CYLINDER_NUM  = 1;
float testApp::CONNECT_RANDOM_MIN_DIST      = 0;
float testApp::CONNECT_RANDOM_MAX_DIST      = 9999;

int   testApp::CONNECT_GROUP_CYLINDER_NUM   = 1;
float testApp::CONNECT_GROUP_MIN_DIST       = 0;
float testApp::CONNECT_GROUP_MAX_DIST       = 9999;

float testApp::COLLISION_MARGIN         = 0;

int   testApp::REMOVE_GROUPS_MIN_NUM    = 1;

bool  testApp::DRAW_WIREFRAME           = false;
bool  testApp::DRAW_COLLISION_SHAPE     = false;
bool  testApp::DRAW_COLLISION_DISTANCE  = false;

// shader uniform name
static bool RENDER_NORMALS;
static bool FLAT_SHADING;

testApp::PROCESS_NAME testApp::CURRENT_PROCESS = testApp::NO_PROCESS;

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
    sprintf(mes, "renature modeller start (compiled on %s at time %s JST)",__DATE__,__TIME__);
    myLogRelease(mes);
    sprintf(mes, "using openFrameworks %d.%d", OF_VERSION, OF_VERSION_MINOR);
    myLogRelease(mes);
    
    glEnable(GL_DEPTH_TEST);
    
    bNowProcessing = false;
    
    posModelPath_P = "models/bee_40k_MASTER_mesh_wN.ply";
    
    mShdInstanced = NULL;
    
    //ofSetFrameRate(60);
	ofSetVerticalSync(false);
	setupCameraLightMaterial();
    setupShaders();

    
    tester = new collisionTester();
    
    // ---------------------------------------------------
    compScale = 1;
    posScale = 33; // 33cm box sizes

    int sphNum = 10;
    int cylNum = 4;
    {
        float   sRad = SPHERE_RADIUS;
        int     sRes = SPHERE_RESOLUTION;
        float   cRad = CYLINDER_RADIUS;
        int     cRes = CYLINDER_RESOLUTION;
        setupSphereShape(sRad, sRes);
        setupCylinderShape(cRad, cRes);
        
#define SETUP_SPHERE
#ifdef SETUP_SPHERE
        myLogDebug("setup Spheres");
#if 1
        sprintf(mes, "starting renature model %s", posModelPath_P.c_str());
        myLogRelease(mes);
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
    
    doProcess();

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
	
    // bg
    switch (bgType) {
        case 0: ofBackground(bgNormal); break;
        case 1: ofBackgroundGradient(bgLinear0, bgLinear1, OF_GRADIENT_LINEAR); break;
        case 2: ofBackgroundGradient(bgCircular0, bgCircular1, OF_GRADIENT_CIRCULAR); break;
        case 3: ofBackgroundGradient(bgBar0, bgBar1, OF_GRADIENT_BAR); break;
        default: ofBackground(0); break;
    }
    
	camMain.begin();{
        {
            // reference box
            ofSetColor(255);
            ofNoFill();
            ofBox(0, 0, 0, posScale*10);
        }
        
        ofEnableLighting();
        mLigDirectional.setGlobalPosition(1000, 1000, 1000);
        mLigDirectional.lookAt(ofVec3f(0,0,0));
        ofEnableSeparateSpecularLight();

        glShadeModel(GL_FLAT);
        glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        mShdInstanced->begin();{
            mLigDirectional.enable();
            mMatMainMaterial.begin();
        
            if (DRAW_WIREFRAME) {
                ofSetColor(colorSphere); spheres.drawWireframe(mShdInstanced);
                ofSetColor(colorCylinder); cylinders.drawWireframe(mShdInstanced);
            }else{
                ofSetColor(colorSphere); spheres.draw(mShdInstanced);
                ofSetColor(colorCylinder); cylinders.draw(mShdInstanced);
            }

            mMatMainMaterial.end();
            mLigDirectional.disable();
        }mShdInstanced->end();
        
        
        glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);

        ofSetColor(255,255,255);
        ofDisableLighting();

        if(DRAW_COLLISION_SHAPE){
            instancedComponent::debugDraw();
        }
        
        if(DRAW_COLLISION_DISTANCE){
            tester->drawAllContanctPts();
        }


    
    }camMain.end();
	   
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
void testApp::keyReleased(int key){}

void testApp::mouseMoved(int x, int y ){
    camMain.mouseMoved(x, y);
}
void testApp::mouseDragged(int x, int y, int button){
    camMain.mouseDragged(x, y, button);
}
void testApp::mousePressed(int x, int y, int button){
    camMain.mousePressed(x, y, button);
}
void testApp::mouseReleased(int x, int y, int button){
    camMain.mouseReleased(x, y, button);
}
void testApp::windowResized(int w, int h){
}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}

void testApp::processGui(){
    if(CURRENT_PROCESS!=0){
        bNowProcessing = true;
    }
}

void testApp::doProcess(){
    if(bNowProcessing){
        if(CURRENT_PROCESS == CONNECT_RANDOM){
            int num = CONNECT_RANDOM_CYLINDER_NUM;
            float min = CONNECT_RANDOM_MIN_DIST;
            float max = CONNECT_RANDOM_MAX_DIST;
            connectRandom(&spheres, &cylinders, num, min, max);
        
        }else if(CURRENT_PROCESS == CONNECT_GROUP){
            int num = CONNECT_GROUP_CYLINDER_NUM;
            float min = CONNECT_GROUP_MIN_DIST;
            float max = CONNECT_GROUP_MAX_DIST;
            connectGroup(&spheres, &cylinders, num, min, max);
            playFinishSound();
        }else if (CURRENT_PROCESS == CONNECT_NEAR){
            
        
        }else if (CURRENT_PROCESS == COLLISION_TEST){
            processCollision();
            playFinishSound();
        }else if(CURRENT_PROCESS == REMOVE_GROUPS){
            int min = REMOVE_GROUPS_MIN_NUM;
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
            setupSphereShape(SPHERE_RADIUS, SPHERE_RESOLUTION);
            setupCylinderShape(CYLINDER_RADIUS, CYLINDER_RESOLUTION);
    
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
        CURRENT_PROCESS = NO_PROCESS;
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
        ofDrawBitmapString("NOW PROCESSING " + ofToString(CURRENT_PROCESS) + " ...", -w/3,0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        glPopMatrix();
    }
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


