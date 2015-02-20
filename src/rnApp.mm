#include "rnApp.h"
#include "ofxMtb.h"
#include <set>
#include "AppDelegate.h"
#include "ofxModifierKeys.h"

collisionTester * rnApp::tester = NULL;
rnApp * rnApp::singleton = NULL;
ofColor rnApp::colorSphere    = ofColor(100);
ofColor rnApp::colorCylinder  = ofColor(100, 100, 50);
string  rnApp::posModelPath_P = "none";
ofMesh rnApp::sphereMesh;
ofMesh rnApp::cylinderMesh;
int rnApp::bgType = 0;
ofColor rnApp::bgNormal    = ofColor(100);
ofColor rnApp::bgLinear0   = ofColor(255);
ofColor rnApp::bgLinear1   = ofColor(0);
ofColor rnApp::bgCircular0 = ofColor(255);
ofColor rnApp::bgCircular1 = ofColor(0);
ofColor rnApp::bgBar0      = ofColor(255);
ofColor rnApp::bgBar1      = ofColor(0);
float rnApp::SPHERE_RADIUS            = 1.5;
int   rnApp::SPHERE_RESOLUTION        = 8;
float rnApp::SPHERE_COLLISION_MARGIN  = 0;
float rnApp::CYLINDER_RADIUS          = 1;
int   rnApp::CYLINDER_RESOLUTION      = 14;
float rnApp::CYLINDER_COLLISION_MARGIN = 0;
int   rnApp::CONNECT_RANDOM_CYLINDER_NUM  = 1;
float rnApp::CONNECT_RANDOM_MIN_DIST      = 0;
float rnApp::CONNECT_RANDOM_MAX_DIST      = 888;
int   rnApp::CONNECT_GROUP_CYLINDER_NUM   = 1;
float rnApp::CONNECT_GROUP_MIN_DIST       = 0;
float rnApp::CONNECT_GROUP_MAX_DIST       = 888;
int   rnApp::REMOVE_GROUPS_MIN_NUM    = 1;
bool  rnApp::DRAW_WIREFRAME           = false;
bool  rnApp::DRAW_COLLISION_SHAPE     = false;
bool  rnApp::DRAW_COLLISION_DISTANCE  = false;
bool  rnApp::DRAW_REFERENCE_BOX       = true;
bool  rnApp::DRAW_SPHERE              = true;
bool  rnApp::DRAW_CYLINDER            = true;
int rnApp::LOAD_MODEL_RESOLUTION = 10;
bool rnApp::LOAD_MODEL_WITH_NOISE_FILTER = true;

static bool RENDER_NORMALS; // shader uniform name
static bool FLAT_SHADING;

rnApp::PROCESS_NAME rnApp::CURRENT_PROCESS = rnApp::NO_PROCESS;

void rnApp::setup(){

#ifndef NDEBUG
    cout << "app running on DEBUG MODE" << endl;
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
    mShdInstanced = NULL;
    
	ofSetVerticalSync(false);
	setupCameraLightMaterial();
    setupShaders();

    tester = new collisionTester();
    finishSound.loadSound("sound/finishSound.wav");

    // ---------------------------------------------------
    posModelPath_P = "none";
    setupSphereShape(   SPHERE_RADIUS,   SPHERE_RESOLUTION, SPHERE_COLLISION_MARGIN);
    setupCylinderShape( CYLINDER_RADIUS, CYLINDER_RESOLUTION, CYLINDER_COLLISION_MARGIN);

    compScale = 1;
    posScale = 10;
    boxSize = 330;
    
//    testCase.loadRandomSphere(30, 100);
}
void rnApp::loadModelData(){
    //cylinders.reset();
    //spheres.reset();
    
    {
        char mes[255];
        sprintf(mes, "start loading renature model %s", posModelPath_P.c_str());
        myLogRelease(mes);
        spheres.loadInstancePositionFromModel(posModelPath_P, LOAD_MODEL_RESOLUTION, posScale, LOAD_MODEL_WITH_NOISE_FILTER);
    }
}
void rnApp::setupSphereShape(float radius, int resolution, float collisionMargin){
    ofSetSphereResolution(resolution);
    sphereMesh = ofGetGLRenderer()->ofGetSphereMesh();
    spheres.setInstanceType(INSTANCE_SPHERE);
    spheres.loadInstanceMesh(sphereMesh, ofVec3f(radius, radius, radius));
    collisionTester::resetSphereShape(radius, collisionMargin);
}
void rnApp::setupCylinderShape(float radius, int resolution, float collisionMargin){
    cylinderMesh = createCylinderZ(radius, 1, resolution, 1);
    cylinders.setInstanceType(INSTANCE_CYLINDER);
    cylinders.loadInstanceMesh(cylinderMesh);
    collisionTester::resetCylinderShape(ofVec3f(radius, 123, 0.5), collisionMargin);  // do not use y value
}

void rnApp::update(){
    
    doProcess();

    processGui();
    spheres.update();
    cylinders.update();

}

void rnApp::draw(){
    ofDisableAlphaBlending();
    mainDraw();
    //testDraw();
}
void rnApp::testDraw(){

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
void rnApp::mainDraw(){
	
    glLineWidth(1);
    
    // bg
    switch (bgType) {
        case 0: ofBackground(bgNormal); break;
        case 1: ofBackgroundGradient(bgLinear0, bgLinear1, OF_GRADIENT_LINEAR); break;
        case 2: ofBackgroundGradient(bgCircular0, bgCircular1, OF_GRADIENT_CIRCULAR); break;
        case 3: ofBackgroundGradient(bgBar0, bgBar1, OF_GRADIENT_BAR); break;
        default: ofBackground(0); break;
    }
    
	camMain.begin();{
        if(DRAW_REFERENCE_BOX){
            // reference box
            ofSetColor(255);
            ofNoFill();
            ofBox(0, 0, 0, boxSize);
            
            ofSetColor(255, 0, 0); ofLine(0, 0, 0, 10, 0, 0);
            ofSetColor(  0, 255, 0); ofLine(0, 0, 0, 0, 10, 0);
            ofSetColor(  0, 0, 255); ofLine(0, 0, 0, 0, 0, 10);
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
                if( DRAW_CYLINDER ) ofSetColor(colorCylinder); cylinders.drawWireframe(mShdInstanced);
                if( DRAW_SPHERE   ) ofSetColor(colorSphere); spheres.drawWireframe(mShdInstanced);
            }else{
                if( DRAW_CYLINDER ) ofSetColor(colorCylinder); cylinders.draw(mShdInstanced);
                if( DRAW_SPHERE   ) ofSetColor(colorSphere); spheres.draw(mShdInstanced);
            }

            mMatMainMaterial.end();
            mLigDirectional.disable();
        }mShdInstanced->end();

        
        glProvokingVertex(GL_LAST_VERTEX_CONVENTION);

        
        ofSetColor(255,255,255);
        ofDisableLighting();
        
        ofPushStyle();
        instancedComponent::drawSelector();
        ofPopStyle();
        
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);

        
        if(DRAW_COLLISION_SHAPE){
            instancedComponent::debugDraw();
        }
        
        if(DRAW_COLLISION_DISTANCE){
            tester->drawAllContanctPts();
        }

    }camMain.end();
	   
    
    ofSetColor(25,25,25);
    stringstream ss;
    ss << "camera position  : " << camMain.getGlobalPosition() << "\n";
    ss << "particle total   : " << spheres.getInstanceNum() << "\n";
    ss << "line total       : " << cylinders.getInstanceNum() << "\n";
    ss << "instance total   : " << instancedComponent::getInstanceMap().size() << "\n";
    
    ofDrawBitmapString(ss.str(), 20, 20);
    waitDraw();
}

void rnApp::exit(){
    spheres.destroy();
    cylinders.destroy();
    delete tester;
    tester = 0;
}
void rnApp::keyPressed(int key){

    switch (key) {
            
        case 'd':
        case OF_KEY_BACKSPACE:
            //spheres.removeSelectedInstance();
            cylinders.removeSelectedInstance();
            break;
        case 'c':
            connectSelected();
            break;

        case OF_KEY_ESC:
            spheres.clearSelectedInstance();
            cylinders.clearSelectedInstance();
            break;
            
    }
    
}
void rnApp::keyReleased(int key){}
void rnApp::mouseMoved(int x, int y ){
    camMain.mouseMoved(x, y);
}
void rnApp::mouseDragged(int x, int y, int button){
    camMain.mouseDragged(x, y, button);
    bMouseDragging = true;
}
void rnApp::mousePressed(int x, int y, int button){
    
    mousePressedStartFrame = ofGetFrameNum();
    camMain.mousePressed(x, y, button);

}
void rnApp::mouseReleased(int x, int y, int button){
    cout << ofGetWindowWidth() << endl;
    
    camMain.mouseReleased(x, y, button);
    bool isClick =( ofGetFrameNum() - mousePressedStartFrame ) < 30;
    if( isClick && !bMouseDragging){
        INSTANCE_TYPE type = INSTANCE_SPHERE;
        int mode = 0;
        bool shift = ofGetModifierPressed(OF_KEY_SHIFT);
        bool alt = ofGetModifierPressed(OF_KEY_ALT);
        if( shift ) mode = 1;
        if( alt ) type = INSTANCE_CYLINDER;
        instancedComponent::mousePick( ofVec3f(x, y, -10), type, mode);
    }else{
        bMouseDragging = false;
    }
}
void rnApp::windowResized(int w, int h){
    cout << "window resized" << ofGetWindowWidth() << "," << ofGetWindowHeight() << endl;

}
void rnApp::gotMessage(ofMessage msg){}
void rnApp::dragEvent(ofDragInfo dragInfo){}

void rnApp::processGui(){
    if(CURRENT_PROCESS!=0){
        bNowProcessing = true;
    }
}
void rnApp::doProcess(){
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
        }else if(CURRENT_PROCESS == REMOVE_ALL_SPHERES){
            spheres.reset();
        }else if(CURRENT_PROCESS == REMOVE_ALL_CYLINDERS){
            cylinders.reset();
        }else if(CURRENT_PROCESS == RESET_INSTSANCE_SHAPE){
            setupSphereShape(SPHERE_RADIUS, SPHERE_RESOLUTION, SPHERE_COLLISION_MARGIN);
            setupCylinderShape(CYLINDER_RADIUS, CYLINDER_RESOLUTION, CYLINDER_COLLISION_MARGIN);    
        }else{
            cout << "unknown process for process#" << (int)CURRENT_PROCESS << endl;
            
            bNowProcessing = false;
            CURRENT_PROCESS = NO_PROCESS;
        }


        bNowProcessing = false;
        CURRENT_PROCESS = NO_PROCESS;
    }
}
void rnApp::waitDraw(){
    if(bNowProcessing){
        float x = ofGetWidth()  * 0.5;
        float y = ofGetHeight() * 0.5;
        float w = ofGetWidth()  * 0.27;
        float h = ofGetHeight() * 0.27;

        glPushMatrix();
        glTranslatef(x, y, 0);
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofEnableAlphaBlending();
        ofSetColor(0, 10, 20, 200);
        ofRect(0, 0, w, h);
        ofDisableAlphaBlending();
        
        ofSetColor(255,255,255);
        ofDrawBitmapString("PROCESS #" + ofToString(CURRENT_PROCESS) + " ...", -w/3,0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        glPopMatrix();
    }
}
void rnApp::setupCameraLightMaterial(){
    camMain.setupPerspective(false);
    camMain.setDistance(400);
    camMain.setFov(50);
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
void rnApp::setupShaders(bool doLink){
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
void rnApp::saveCsvData(string path){
    spheres.saveInstanceDataToCsv(path);
    cylinders.saveInstanceDataToCsv(path);
    playFinishSound();
}
void rnApp::loadCsvData(string path){
    spheres.loadInstanceDataFromCsv(path);
    cylinders.loadInstanceDataFromCsv(path);
    playFinishSound();
}
