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

    myLogRelease("renature::insecta setup");
    
    isShaderDirty = true;
    mShdInstanced = NULL;
    bWireframe = false;
    posModelPath_P = "models/bee_20k_MASTER_mesh_wN.ply";
    posModelPath_L = "models/bee_20k_MASTER_mesh_wN.ply";
    
    // important
    compScale = 1;
    posScale = 100;

    texId_P = GL_NONE;
    texId_L = GL_NONE;
    
#ifndef NDEBUG
    ofSetLogLevel(OF_LOG_VERBOSE);
#else
    ofSetLogLevel(OF_LOG_WARNING);
#endif
    
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetColor(255);

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

#if 0
    // sphere
    ofSetSphereResolution(8);
    mMshSphere = loadComponent(getOFSphere(), compScale);
    //mMshSphere->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    // position
    int instanceNum = loadPositionFromModel(posModelPath_P, ofVec3f(0, 0,0), posScale, &texId_P);
    if(instanceNum==-1)
        ofExit();
    else
        mMshSphere->setPrimCount(instanceNum);

    
    // cylinder
    ofMesh cyl = createCylinder(5, 30, 12, 1);
    mMshCylinder = loadComponent(cyl, compScale);
    mMshCylinder->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    instanceNum = loadPositionFromModel(posModelPath_L, ofVec3f(300,300,0), posScale, &texId_L);
    if(instanceNum==-1)
        ofExit();
    else
        mMshCylinder->setPrimCount(instanceNum);
#endif
    
    ofSetSphereResolution(8);
    ic.loadInstanceMesh(getOFSphere());
    ic.loadInstancePositionFromModel(posModelPath_L, 100);
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
    
#if 0
    mShdInstanced->setUniformTexture("vtxtex", GL_TEXTURE_2D, texId_P, 0);

    glPushMatrix();
    if(bWireframe){
        //glLineWidth(0.001);
        ofSetColor(250,100,255);
        mMshSphere->drawWireframe();
    }else{
        ofFill();
        ofSetColor(200,200,200);
          mMshSphere->drawFaces();
    }
    glPopMatrix();

    mShdInstanced->setUniformTexture("vtxtex", GL_TEXTURE_2D, texId_L, 0);
    glPushMatrix();
    if(bWireframe){
        glLineWidth(0.001);
        ofSetColor(250,100,255);
        mMshCylinder->drawWireframe();
    }else{
        ofSetColor(200,200,200);
        mMshCylinder->draw();
    }
    glPopMatrix();
    
    ofSetColor(255);
#endif

#if 0
    int maxNum = 32;
    int pos = 0;

    glPushMatrix();
    mMshSphere->setPrimCount(maxNum);
    for(int i=0; i<INSTANCE_NUM/maxNum; i++){

        int index = i * 16 * maxNum;
        glUniformMatrix4fv(0, maxNum, 0, matrices+index);
        mMshSphere->draw();
        
        if(i==(INSTANCE_NUM/maxNum) -1  ){
            int rest = INSTANCE_NUM%maxNum;
            mMshSphere->setPrimCount(rest);
            index += maxNum * 16;
            glUniformMatrix4fv(0, rest, 0, matrices+index);
            mMshSphere->draw();
        }
    }
    glPopMatrix();
    ofSetColor(255);
#endif
    

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
            posScale *= 1.2;
            loadPositionFromModel(posModelPath_P, ofVec3f(0,0,0), posScale, &texId_P);
            break;
            
        case '2':
            posScale *= 0.9;
            loadPositionFromModel(posModelPath_P, ofVec3f(0,0,0), posScale, &texId_P);
            break;
            
        case '3':
            compScale *= 1.2;
            loadComponent(getOFSphere(), compScale);
            loadPositionFromModel(posModelPath_P,ofVec3f(0,0,0),  posScale, &texId_P);
            break;

        case '4':
            compScale *= 0.9;
            
            loadComponent(getOFSphere(), compScale);
            loadPositionFromModel(posModelPath_P, ofVec3f(0,0,0), posScale, &texId_P);
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




int testApp::loadPositionFromModel(string modelPath, ofVec3f trans, float _posScale, GLuint * texId){
    
    vector<string> strs;
    boost::split(strs, modelPath, boost::is_any_of("."));
    string ext = strs.back();
    if( ext == "dae"){
        ofLogFatalError("Do not use .dae file. Use .ply instead");
        return -1;
    }
        
    
    ofVboMesh mesh;
    ofxAssimpModelLoader model;
    if(model.loadModel(modelPath)){
        mesh = model.getMesh(0);
    }
    
    int numVertices = mesh.getNumVertices();
    int numNormals = mesh.getNumNormals();
    int numIndices = mesh.getNumIndices();

    myLogDebug("load model for position");
    myLogDebug("numVertices = " + ofToString(numVertices));
    myLogDebug("numNormals = "  + ofToString(numNormals) );
    myLogDebug("numIndices = " + ofToString(numIndices));
    
    int instanceNum = numVertices;
    
    float * matrices = new float[TEX_WIDTH*TEX_HEIGHT*16];
    ofMatrix4x4 * mat = new ofMatrix4x4[instanceNum];
    for(int i=0; i<instanceNum; i++){
        ofVec3f position = trans + (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
        //float scale = 1;
        //float angle = 0;
        
        mat[i].makeIdentityMatrix();
        //beeMat[i].rotate(angle, 1, 0, 0);
        //beeMat[i].scale(scale, scale, scale);
        mat[i].translate(position);
        
        for(int j=0; j<4; j++){
            for(int k=0; k<4; k++){
                int index = i*16 + j*4 + k;
                matrices[index] = mat[i](j, k);
            }
        }

    }
    
    {
        int randTest = ofRandom(instanceNum);
        int a00 = randTest*16;
        char mes[256], row1[256], row2[256], row3[256], row4[256];
        sprintf(mes, "matrices[%d]-[%d] for instance #%d", a00, a00+15, randTest);
        sprintf(row1, "%04f %04f %04f %04f", matrices[a00+0], matrices[a00+4], matrices[a00+8], matrices[a00+12]);
        sprintf(row2, "%04f %04f %04f %04f", matrices[a00+1], matrices[a00+5], matrices[a00+9], matrices[a00+13]);
        sprintf(row3, "%04f %04f %04f %04f", matrices[a00+2], matrices[a00+6], matrices[a00+10], matrices[a00+14]);
        sprintf(row4, "%04f %04f %04f %04f", matrices[a00+3], matrices[a00+7], matrices[a00+11], matrices[a00+15]);
        
        myLogDebug(string(mes));
        myLogDebug(string(row1));
        myLogDebug(string(row2));
        myLogDebug(string(row3));
        myLogDebug(string(row4));
    }
    
    // position data sender(textrue)
    //if(*texId!=GL_NONE)
    glGenTextures(1, texId);
    glBindTexture(GL_TEXTURE_2D, *texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    myLogDebug("texture id = " + ofToString(*texId));
    delete[] matrices;
    delete[] mat;
    matrices = NULL;
    mat = NULL;
    model.clear();
    
    return instanceNum;
}



ofxVboMeshInstanced * testApp::loadComponent(ofMesh component, float _scale){

    // scale
    for(int i=0; i<component.getNumVertices(); i++){
        ofVec3f pos = component.getVertex(i) * _scale;
        component.setVertex(i, pos);
    }
    
    ofxVboMeshInstanced * vmi = new ofxVboMeshInstanced(component);
    int numVertices = vmi->getNumVertices();
    int numNormals = vmi->getNumNormals();
    int numIndices = vmi->getNumIndices();
    
    myLogDebug("load model for particle component");
    myLogDebug("numVerttices = " + ofToString(numVertices));
    myLogDebug("numNormals = " + ofToString(numNormals));
    myLogDebug("numIndices = " + ofToString(numIndices));

    // just check
    for(int i=0; i<10; i++){
        ofVec3f pos = vmi->getVertex(i);
        char mes[256];
        sprintf(mes, "vertices[%d] =  %03f, %03f, %03f", i, pos.x, pos.y, pos.z);
        myLogDebug(string(mes));
    }
    
    //component.clear();
    return vmi;
}

ofMesh testApp::getOFSphere(){
    ofPtr<ofGLRenderer> r = ofGetGLRenderer();
    return r->ofGetSphereMesh();
}



ofMesh testApp::createCylinder(float radius, float height, int slices, int stacks){
    if(slices<2 || stacks<1 || radius<0.0 || height<0.0)
        myLogRelease("invalid parameter for cylinder mesh creation");
    
    int cacheSize = slices+1;
    // for verteices and normals
    float sinCache[cacheSize];
    float cosCache[cacheSize];

    // for face
    float sinCache2[cacheSize];
    float cosCache2[cacheSize];

    float angle;
    float zLow, zHigh;
    float sinTemp, cosTemp;
    float xyNormalRatio;
    
    xyNormalRatio = 1;
    
    for(int i=0; i<slices; i++){
        angle = TWO_PI * i / (float)slices;
        sinCache[i] = sin(angle);
        cosCache[i] = cos(angle);
    }
    
    // outside
    for(int i=0; i<slices; i++){
        angle = TWO_PI * (i-0.5) / (float)slices;
        sinCache[i] = sin(angle);
        cosCache[i] = cos(angle);
    }
    
    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    
    sinCache2[slices] = sinCache2[0];
    cosCache2[slices] = cosCache2[0];
    
    ofMesh cylinder;
    ofVec3f v1, v2, n1,n2;
    ofVec2f t1, t2;
    
    // construct mesh for TRIANGLE STRIP
    for(int j=0; j<stacks; j++){
        
        float zLow = (j * height/stacks) - height*0.5;
        float zHigh = ((j+1) * height/stacks) -height*0.5;
        
        for(int i=0; i<=slices; i++){
            float x = radius*sinCache[i];
            float y = radius*cosCache[i];

            v1.set(x, y, zLow);
            v2.set(x, y, zHigh);
            t1.set(1.0-(float)i/slices, (float)j/stacks);
            t2.set(1.0-(float)i/slices, (float)(j+1)/stacks);
            
            cylinder.addVertex(v1);
            cylinder.addVertex(v2);
            cylinder.addTexCoord(t1);
            cylinder.addTexCoord(t2);
            
            if(i!=0){
                float nx = radius*sinCache2[i];
                float ny = radius*cosCache2[i];

                n1.set(nx, ny, 0);
                n2.set(nx, ny, 0);
                cylinder.addNormal(n1);
                cylinder.addNormal(n2);
            }
        }
    }
    
    cylinder.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    return cylinder;
}




float * testApp::createPositionTexture(GLuint *texId){
    float * matrices = new float[TEX_WIDTH*TEX_HEIGHT*16];

    glGenTextures(1, texId);
    glBindTexture(GL_TEXTURE_2D, *texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    myLogDebug("texture id = " + ofToString(*texId));
    return matrices;
}

void testApp::updatePositionTexture(GLuint *texId, ofMatrix4x4 mat[], ofVec3f trans=ofVec3f(0,0,0), float posScale=0){
    
    //int size = sizeof mat/sizeof(ofMatrix4x4);
    
//    
//    for(int i=0; i<size; i++){
//        for(int j=0; j<4; j++){
//            for(int k=0; k<4; k++){
//                int index = i*16 + j*4 + k;
//                matrices[index] = mat[i](j, k);
//            }
//        }
//    }
//    
//    setPositionTexture(matrices, texId);
}

void testApp::setPositionTexture(string modelPath, GLuint * texId, ofVec3f trans=ofVec3f(0,0,0), float posScale=0){
    
    vector<string> strs;
    boost::split(strs, modelPath, boost::is_any_of("."));
    string ext = strs.back();
    if( ext == "dae"){
        ofLogFatalError("Do not use .dae file. Use .ply instead");
        return -1;
    }
    
    
    ofVboMesh mesh;
    ofxAssimpModelLoader model;
    if(model.loadModel(modelPath)){
        mesh = model.getMesh(0);
    }
    
    int numVertices = mesh.getNumVertices();
    int numNormals = mesh.getNumNormals();
    int numIndices = mesh.getNumIndices();
    
    myLogDebug("load model for position");
    myLogDebug("numVertices = " + ofToString(numVertices));
    myLogDebug("numNormals = "  + ofToString(numNormals) );
    myLogDebug("numIndices = " + ofToString(numIndices));
    
    int instanceNum = numVertices;
    
    float * matrices = new float[TEX_WIDTH*TEX_HEIGHT*16];
    ofMatrix4x4 * mat = new ofMatrix4x4[instanceNum];
    for(int i=0; i<instanceNum; i++){
        ofVec3f position = trans + (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
        //float scale = 1;
        //float angle = 0;
        
        mat[i].makeIdentityMatrix();
        //beeMat[i].rotate(angle, 1, 0, 0);
        //beeMat[i].scale(scale, scale, scale);
        mat[i].translate(position);
        
        for(int j=0; j<4; j++){
            for(int k=0; k<4; k++){
                int index = i*16 + j*4 + k;
                matrices[index] = mat[i](j, k);
            }
        }
        
    }
    
    {
        int randTest = ofRandom(instanceNum);
        int a00 = randTest*16;
        char mes[256], row1[256], row2[256], row3[256], row4[256];
        sprintf(mes, "matrices[%d]-[%d] for instance #%d", a00, a00+15, randTest);
        sprintf(row1, "%04f %04f %04f %04f", matrices[a00+0], matrices[a00+4], matrices[a00+8], matrices[a00+12]);
        sprintf(row2, "%04f %04f %04f %04f", matrices[a00+1], matrices[a00+5], matrices[a00+9], matrices[a00+13]);
        sprintf(row3, "%04f %04f %04f %04f", matrices[a00+2], matrices[a00+6], matrices[a00+10], matrices[a00+14]);
        sprintf(row4, "%04f %04f %04f %04f", matrices[a00+3], matrices[a00+7], matrices[a00+11], matrices[a00+15]);
        
        myLogDebug(string(mes));
        myLogDebug(string(row1));
        myLogDebug(string(row2));
        myLogDebug(string(row3));
        myLogDebug(string(row4));
    }
    
    // position data sender(textrue)
    //if(*texId!=GL_NONE)
    glGenTextures(1, texId);
    glBindTexture(GL_TEXTURE_2D, *texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    myLogDebug("texture id = " + ofToString(*texId));
    delete[] matrices;
    delete[] mat;
    matrices = NULL;
    mat = NULL;
    model.clear();
    
    return instanceNum;
}




