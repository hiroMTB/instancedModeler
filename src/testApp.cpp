#include "testApp.h"
#include "ofxMtb.h"


//
// max transfer matrix num
// 1024*1024/4 = 262,144
//
#define TEX_WIDTH 1024
#define TEX_HEIGHT 512



// GUI parts
const string testApp::RENDER_NORMALS = "RENDER NORMALS";
const string testApp::FLAT_SHADING = "FLAT SHADING";
const string testApp::CONNECT_RANDOM = "CONNECT_RANDOM";
const string testApp::CONNECT_NEAR = "CONNECT NEAR";
const string testApp::RESET_CYLINDERS = "RESET CYLINDERS";
const string testApp::RESET_SPHERES = "RESET SPHERES";

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
    setupGui();
    
    {
        // model setup
        ofSetSphereResolution(5);
        ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
        spheres.loadInstanceMesh(sphere);
        spheres.loadInstancePositionFromModel(posModelPath_P, 100);

//        int size =20;
//        ofMatrix4x4 * ms = new ofMatrix4x4[size];
//        for(int i=0; i<size; i++){
//            ms[i].makeIdentityMatrix();
//            ms[i].scale(4, 4, 4);
//            ms[i].translate(ofRandom(-50, 50), ofRandom(-50, 50), ofRandom(-50, 50));
//        }
//        spheres.loadInstancePositionFromMatrices(ms, size);
    
    }
    
#if 1
    {
        ofMesh cylinder = createCylinderZ(0.2, 1, 12, 1);
        cylinders.loadInstanceMesh(cylinder);

    }

    connectRandom(&spheres, &cylinders, 1000, 100, 1000);
#endif
    
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

    
    ofMesh cyl = createCylinderZ(10, 40, 12, 1);
    ofSetColor(0,0,0);
    ///cyl.drawWireframe();
    cyl.draw();
    
    mMatMainMaterial.end();
    mLigDirectional.disable();
    camMain.end();
}

void testApp::mainDraw(){
    ofSetColor(255);
	ofBackgroundGradient(ofColor::fromHsb(0, 0, 100), ofColor::fromHsb(0, 0, 50), OF_GRADIENT_LINEAR);
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
        
        if (bWireframe) {
            ofSetColor(0,0,0);
            spheres.drawWireframe(mShdInstanced);
            cylinders.drawWireframe(mShdInstanced);
        }else{
    
            ofSetColor(200,200,200);
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

    ofSetColor(255,0,0);
	ofDisableLighting();
	camMain.end();
	
    //  GUI draw
	mainPnl.draw();
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
            
            spheres.setInstanceMatrix(0, ofRandom(100),ofVec3f(100,100,100), ofVec4f(100, 1,0,0), ofVec3f(ofRandom(10),ofRandom(10),ofRandom(10)));
            
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


void testApp::setupCameraLightMaterial(){
    camMain.setupPerspective(false);
    camMain.setDistance(1500);
    camMain.disableMouseInput();
    
	mLigDirectional.setup();
	mLigDirectional.setDirectional();
	mLigDirectional.setAmbientColor(ofColor::fromHsb(0, 0, 200));
	mLigDirectional.setDiffuseColor(ofColor::fromHsb(120, 120, 128));
	mLigDirectional.setSpecularColor(ofColor(255,255,255));
	
	mMatMainMaterial.setDiffuseColor(ofColor(0,0,0));
	mMatMainMaterial.setSpecularColor(ofColor(200,200,200));
	mMatMainMaterial.setShininess(25.0f);
}



void testApp::connectRandom(instancedComponent *ic, instancedComponent *ic2, int numAllCylinders, float minDist, float maxDist){
    INSTANCE_GROUPS groups = ic->getInstanceGroups();

    int newGroupId = ic2->initGroup();
    int index = 0;
    int numGroups = groups.size();
    int numFind = 0;
    for(int i=0; i<numAllCylinders; i++){
        int numTry = 0;
        int maxTry = 100;
        bool find = false;
        
        do{
            // 1. select group
            int groupIdA = ofRandom(numGroups);
            int groupIdB = ofRandom(numGroups);
            
            instanceGroup &groupA = groups.find(groupIdA)->second;
            instanceGroup &groupB = groups.find(groupIdB)->second;
            
            int numInstancesA = groupA.instances.size();
            int numInstancesB = groupB.instances.size();
            
            // 2. select instance
            int indexA = ofRandom(numInstancesA);
            int indexB = ofRandom(numInstancesB);
            
            instance instA = groupA.instances[indexA];
            instance instB = groupB.instances[indexB];
            
            // 3. check A - B distance
            ofVec3f vA = instA.matrix.getTranslation();
            ofVec3f vB = instB.matrix.getTranslation();
            
            ofVec3f vAB = vB - vA;
            float dist = vAB.length();
            
            if(minDist<dist && dist<maxDist){
                
                // 4. put
                ofMatrix4x4 mat;
                ofVec3f pos = vA + vAB*0.5;
                
                ofVec3f yAxis(0,0,1);
                float angle = yAxis.angle(vAB);
                ofVec3f prep = yAxis.cross(vAB);

                mat.scale(1, 1, dist);
                mat.rotate(angle, prep.x, prep.y, prep.z);
                mat.translate(pos);
                

                ic2->setInstanceMatrix(newGroupId, index, mat);
                find = true;
                numFind++;
            }
            
            numTry++;
            
            if(maxTry < numTry)
                find = true;
        }while (!find);
    }
    
    int instanceNum = ic2->getInstanceNum();
    ic2->setInstanceNum(instanceNum + numFind);
    
}


void testApp::processGui(){
    bool connect_random = mainPnl.getButton(CONNECT_RANDOM).value;
    bool reset_cylinders = mainPnl.getButton(RESET_CYLINDERS).value;
    
    
    
    if (connect_random) {
        connectRandom(&spheres, &cylinders, ofRandom(100,300), ofRandom(200, 400), ofRandom(401, 600));
    }
    
    if (reset_cylinders){
        cylinders.reset();
    }
    
}

void testApp::setupGui(){
    
	mainPnl.setup("MAIN SETTINGS", "settings.xml", ofGetWidth() - 300, 50);
	mainPnl.add(prmFloat["zNear"].set("Near Clip",0,0,100));
	mainPnl.add(prmFloat["zFar"].set("Far Clip",0,0,5000.0));
	mainPnl.add(prmBool[RENDER_NORMALS].set(RENDER_NORMALS, true));
	mainPnl.add(prmBool[FLAT_SHADING].set(FLAT_SHADING, true));
	
    mainPnl.add(prmFloat["Particle_Diam"].set("Particle Diam",0,0,1.0));
    mainPnl.add(prmFloat["Particle_Resolution"].set("Particle Resolution",0,3,12));
    
    ofxButton * btn = new ofxButton();
    btn->setup(CONNECT_RANDOM);
    mainPnl.add(btn);
    
    
    ofxButton * btn2 = new ofxButton();
    btn2->setup(RESET_CYLINDERS);
    mainPnl.add(btn2);
    
	mainPnl.loadFromFile("settings.xml");
}


