#include "testApp.h"
#include "ofxMtb.h"



// GUI parts
const string testApp::RENDER_NORMALS = "RENDER_NORMALS";
const string testApp::FLAT_SHADING = "FLAT_SHADING";
const string testApp::CONNECT_RANDOM = "CONNECT_RANDOM";
const string testApp::CONNECT_NEAR = "CONNECT_NEAR";
const string testApp::RESET_CYLINDERS = "RESET_CYLINDERS";
const string testApp::RESET_SPHERES = "RESET_SPHERES";

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
    
    compScale = 1;
    posScale = 100;
    
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetColor(255);
    setupCameraLightMaterial();
    setupGui();

    updateShaders();

    
    {
        // model setup
        ofSetSphereResolution(5);
        ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
        //ofMesh sphere = createQuadSphere(1, 12, 10);
        spheres.loadInstanceMesh(sphere);
        spheres.loadInstancePositionFromModel(posModelPath_P, 100);
    }
    
#if 1
    {
        ofMesh cylinder = createCylinderZ(0.2, 1, 20, 1);
        cylinders.loadInstanceMesh(cylinder);
    }

    connectRandom(&spheres, &cylinders, 1000, 100, 1000);
#endif
    
    GLint max;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);
    myLogRelease("GL_MAX_VERTEX_ATTRIBS : " + ofToString(max));
    
}

void testApp::update(){
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
            ofSetColor(222,0,0);
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

    ofSetColor(255,255,255);
	ofDisableLighting();
	camMain.end();
	
    //  GUI draw
	mainPnl.draw();
    
    
    int y = 20;
    int x = 20;
    int h = 25;
	ofDrawBitmapString(ofToString(ofGetFrameRate()), x, y);
    
    ofVec3f cp = camMain.getPosition();
    ofDrawBitmapString("camera position: "+ofToString(cp.x)+", "+ofToString(cp.y)+", "+ofToString(cp.z), x, y+=h);

    ofVec3f tp = camMain.getTarget().getPosition();
    ofDrawBitmapString("camera position: "+ofToString(tp.x)+", "+ofToString(tp.y)+", "+ofToString(tp.z), x, y+=h);
    
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
	mainPnl.add(prmBool[RENDER_NORMALS].set(RENDER_NORMALS, false));
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



void testApp::setupCameraLightMaterial(){
    camMain.setupPerspective(false);
    camMain.setDistance(1500);
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