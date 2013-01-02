#include "testApp.h"
#include <boost/algorithm/string.hpp>
#include "ofxMtb.h"


//
// max transfer matrix num
// 1024*1024/4 = 262,144
//
#define TEX_WIDTH 1024
#define TEX_HEIGHT 512

void testApp::setup(){

#ifndef NDEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#else
    ofSetLogLevel(OF_LOG_WARNING);
#endif

    char mes[255];
    sprintf(mes, "using openFrameworks %d.%d", OF_VERSION, OF_VERSION_MINOR);
    myLogDebug(mes);
    myLogRelease("renature::insecta setup");
    
    isShaderDirty = true;
    mShdInstanced = NULL;
    bWireframe = false;
    posModelPath_P = "models/bee_100k_MASTER_mesh_wN.ply";
    posModelPath_L = "models/bee_20k_MASTER_mesh_wN.ply";
    
    compScale = 1;
    posScale = 100;

    texId_P = GL_NONE;
    texId_L = GL_NONE;
    
    
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetColor(255);
    setupCameraLightMaterial();

    
    // model setup
    ofSetSphereResolution(5);
    ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
    ic.loadInstanceMesh(sphere);
    ic.loadInstancePositionFromModel(posModelPath_P, 100);
}

void testApp::update(){
	if (isShaderDirty){
		
		GLuint err = glGetError();	// we need this to clear out the error buffer.
    
		if (mShdInstanced != NULL ) delete mShdInstanced;
		mShdInstanced = new ofShader();
		mShdInstanced->load("shaders/instancedTexTrans");
		err = glGetError();
		ofLogNotice() << "Loaded instanced Shader: " << err;

		isShaderDirty = false;
        matLoc = mShdInstanced->getAttributeLocation("mymat");
        myLogDebug("mymat location is " + ofToString(matLoc));
        
        texLoc = mShdInstanced->getAttributeLocation("vtxtex");
        myLogDebug("vtxtex location is "+ofToString(texLoc));
        
        
        //
        if (mShdInstanced_L != NULL ) delete mShdInstanced_L;
		mShdInstanced_L = new ofShader();
		mShdInstanced_L->load("shaders/instancedTexTrans");
		err = glGetError();
		ofLogNotice() << "Loaded instanced Shader: " << err;

	}
	
	mCamMain.setNearClip(mPrmFloat["zNear"]);
	mCamMain.setFarClip(mPrmFloat["zFar"]);

    ic.update();
}


void testApp::draw(){
    mainDraw();
    //testDraw();
}

void testApp::testDraw(){
    
}

void testApp::mainDraw(){
    ofSetColor(255);
	ofBackgroundGradient(ofColor::fromHsb(0, 0, 100), ofColor::fromHsb(0, 0, 50), OF_GRADIENT_LINEAR);
	mCamMain.begin();
	ofEnableLighting();
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	ofEnableSeparateSpecularLight();

	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mShdInstanced->begin();
        if (mPrmBool["shouldRenderNormals"]){
            mShdInstanced->setUniform1f("shouldRenderNormals", 1.0);
        } else {
            mShdInstanced->setUniform1f("shouldRenderNormals", 0.0);
        }

        if (mPrmBool["shouldUseFlatShading"]){
            mShdInstanced->setUniform1f("shouldUseFlatShading", 1.0);
            glShadeModel(GL_FLAT);
            glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION
        } else {
            mShdInstanced->setUniform1f("shouldUseFlatShading", 0.0);
            glShadeModel(GL_SMOOTH);
            glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
        }


        mLigDirectional.enable();
        mMatMainMaterial.begin();
        
        ofSetColor(250,100,255);
        ic.draw(mShdInstanced);

        mMatMainMaterial.end();
        mLigDirectional.disable();
	mShdInstanced->end();
	
	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

    ofSetColor(255,0,0);
	ofDisableLighting();
	mCamMain.end();
	
    //  GUI draw
	mPnlMain.draw();
	ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth()-100, 20);
}


void testApp::keyPressed(int key){
}

void testApp::keyReleased(int key){
	
	switch (key) {
		case ' ':{
			isShaderDirty = true;
		}
			break;
			
			case 'f':
			ofToggleFullscreen();
			break;
	
        case 'w':
			bWireframe = !bWireframe;
			break;
            
        case '1':
            
            ic.setInstanceMatrix(ofRandom(100),ofVec3f(100,100,100), ofVec4f(100, 1,0,0), ofVec3f(ofRandom(10),ofRandom(10),ofRandom(10)));
            
            break;
            
		default:
			break;
	}
	
}

void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}
void testApp::setupCameraLightMaterial(){
    mCamMain.setupPerspective(false);
    mCamMain.setDistance(1500);
    
	mPnlMain.setup("MAIN SETTINGS", "settings.xml", 20, 20);
	mPnlMain.add(mPrmFloat["zNear"].set("zNear",0,0,100));
	mPnlMain.add(mPrmFloat["zFar"].set("zFar",0,0,5000.0));
	mPnlMain.add(mPrmBool["shouldRenderNormals"].set("shouldRenderNormals", false));
	mPnlMain.add(mPrmBool["shouldUseFlatShading"].set("shouldUseFlatShading", true));
	mPnlMain.loadFromFile("settings.xml");
	
	mLigDirectional.setup();
	mLigDirectional.setDirectional();
	mLigDirectional.setAmbientColor(ofColor::fromHsb(0, 0, 200));
	mLigDirectional.setDiffuseColor(ofColor::fromHsb(120, 120, 128));
	mLigDirectional.setSpecularColor(ofColor(255,255,255));
	
	mMatMainMaterial.setDiffuseColor(ofColor(0,0,0));
	mMatMainMaterial.setSpecularColor(ofColor(200,200,200));
	mMatMainMaterial.setShininess(25.0f);
}





