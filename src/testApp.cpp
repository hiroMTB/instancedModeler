#include "testApp.h"
#include "ofxMtb.h"

// GUI parts
const string testApp::RENDER_NORMALS = "RENDER_NORMALS";
const string testApp::FLAT_SHADING = "FLAT_SHADING";
const string testApp::CONNECT_RANDOM = "CONNECT_RANDOM";
const string testApp::CONNECT_NEAR = "CONNECT_NEAR";
const string testApp::RESET_CYLINDERS = "RESET_CYLINDERS";
const string testApp::RESET_SPHERES = "RESET_SPHERES";
const string testApp::COLLISION_TEST = "COLLISION_TEST";
const string testApp::REMOVE_GROUPS = "REMOVE_GROUPS";
const string testApp::REMOVE_GROUPS_MIN_NUM = "REMOVE_MIN_NUM";

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
    bCollisionDebugDraw = false;
    posModelPath_P = "models/bee_100k_MASTER_mesh_wN.ply";
    
    compScale = 1;
    posScale = 100;
    
//	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofSetColor(255);
    setupCameraLightMaterial();
    setupGui();

    updateShaders();

    
    {
        ofSetSphereResolution(5);
        ofMesh sphere = ofGetGLRenderer()->ofGetSphereMesh();
        //ofMesh sphere = createQuadSphere(1, 12, 10);
        float radius = 3;
        spheres.setInstanceType(INSTANCE_SPHERE);
        spheres.loadInstanceMesh(sphere, ofVec3f(radius, radius, radius));
        tester.resetSphereShape(radius);

#define SETUP_SPHERE
#ifdef SETUP_SPHERE
        myLogDebug("setup Spheres");
#if 0
        spheres.loadInstancePositionFromModel(posModelPath_P, INSTANCE_SPHERE, 100);
#else
        float pos = 100;
        int size = 4000;
        ofMatrix4x4 * ms = new ofMatrix4x4[size];
        ofVec3f * scales = new ofVec3f[size];
        
        for(int i=0; i<size; i++){
            ms[i].makeIdentityMatrix();
            ms[i].translate(ofRandom(-pos, pos), ofRandom(-pos, pos), ofRandom(-pos, pos));
            scales[i].set(1, 1, 1);
        }
        spheres.loadInstancePositionFromMatrices(ms, scales, size);
#endif
#endif
    }

    
    
    {
        ofMesh cylinder = createCylinderZ(0.2, 1, 10, 1);
        cylinders.setInstanceType(INSTANCE_CYLINDER);
        cylinders.loadInstanceMesh(cylinder);
        tester.resetCylinderShape(ofVec3f(0.2, 123, 0.5));  // do not use y value
#define SETUP_CYLINDER 1
#ifdef SETUP_CYLINDER
        myLogDebug("setup Cylinders");

        int size = 1000;
#if 1
        connectRandom(&spheres, &cylinders, size, 1, 10000);
#else

        ofMatrix4x4 * ms = new ofMatrix4x4[size];
        ofVec3f * scales = new ofVec3f[size];

        float pos = 5;
        for(int i=0; i<size; i++){
            ms[i].makeIdentityMatrix();
            ms[i].rotate(12, ofRandom(-180, 180), ofRandom(-180, 180), ofRandom(-180, 180));
            ms[i].translate(ofRandom(-pos, pos), ofRandom(-pos, pos), ofRandom(-pos, pos));
            scales[i].set(1,1,1);
        }
        
        
        cylinders.loadInstancePositionFromMatrices(ms, INSTANCE_CYLINDER, size);

#endif
#endif
    }
    
    //instancedComponent::printGroupData();
    
    tester.initAlgo();
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
    int x = 20;
    int h = 20;
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
            y=startY; x += 180;
        }
        
    }
    
}

void testApp::keyPressed(int key){}

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

    if (mainPnl.getButton(CONNECT_RANDOM)) {
        connectRandom(&spheres, &cylinders, ofRandom(100,300), ofRandom(200, 400), ofRandom(401, 600));
    }else

    if (mainPnl.getButton(RESET_CYLINDERS)){
        cylinders.reset();
    }else
        
    if(mainPnl.getButton(COLLISION_TEST)){
        processCollisionTest();
    }else
    
    if(mainPnl.getButton(REMOVE_GROUPS)){
        int min = prmInt[REMOVE_GROUPS_MIN_NUM];
        spheres.removeSmallGroup(min);          // should be static
        cylinders.removeSmallGroup(min);
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
    
    mainPnl.add(prmFloat["COLOR_R"].set("Red", 1.0, 0.0, 1.0));
    mainPnl.add(prmFloat["COLOR_G"].set("Green", 1.0, 0.0, 1.0));
    mainPnl.add(prmFloat["COLOR_B"].set("Blue", 1.0, 0.0, 1.0));
    
    
    ofxButton * btn = new ofxButton();
    btn->setup(CONNECT_RANDOM);
    mainPnl.add(btn);
    
    
    ofxButton * btn2 = new ofxButton();
    btn2->setup(RESET_CYLINDERS);
    mainPnl.add(btn2);

    ofxButton * btn3 = new ofxButton();
    btn3->setup(COLLISION_TEST);
    mainPnl.add(btn3);

    
    mainPnl.add(prmInt[REMOVE_GROUPS_MIN_NUM].set(REMOVE_GROUPS_MIN_NUM, 1, 0, 30));

    ofxButton * btn4 = new ofxButton();
    btn4->setup(REMOVE_GROUPS);
    mainPnl.add(btn4);

	
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

#include<set>
void testApp::connectRandom(instancedComponent *ic, instancedComponent *ic2, int numAllCylinders, float minDist, float maxDist){



    vector<idPair> checkList;
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    int numGroups = STL_UTIL::getAllKeySize(instanceMap);
    
    int index = 0;
    int numFind = 0;
    for(int i=0; i<numAllCylinders; i++){
        int numTry = 0;
        int maxTry = 10000;
        bool find = false;
        
        do{
            // 1. select group
            int groupIdA = ofRandom(-2, numGroups-2);               // todo: invalid
            int groupIdB = ofRandom(-2, numGroups-2);
            
            int numInstancesA = instanceMap.count(groupIdA);
            int numInstancesB = instanceMap.count(groupIdB);
            
            // 2. select instance
            int indexA = ofRandom(numInstancesA);
            int indexB = ofRandom(numInstancesB);
            
            bool same = false;
            idPair idp(indexA, indexB);
            for(int i=0; i<checkList.size(); i++){
                if(idp == checkList[i]){
                    same = true;
                    cout << "same";
                    break;
                }
            }
            
            if(!same){
            
                INSTANCE_MAP_ITR itrA = instanceMap.find(groupIdA);
                INSTANCE_MAP_ITR itrB = instanceMap.find(groupIdB);
                
                std::advance(itrA, indexA);
                std::advance(itrB, indexB);
                
                instance instA = itrA->second;
                instance instB = itrB->second;
                
                if(instA.type!= INSTANCE_CYLINDER && instB.type!=INSTANCE_CYLINDER ){
                    
                    // 3. check A - B distance
                    ofVec3f vA = instA.matrix.getTranslation();
                    ofVec3f vB = instB.matrix.getTranslation();
                    
                    ofVec3f vAB = vB - vA;
                    float dist = vAB.length();
                    
                    if(minDist<dist && dist<maxDist){
                        
                        // 4. put
                        ofMatrix4x4 mat;
                        ofVec3f scale;
                        ofVec3f pos = vA + vAB*0.5;
                        
                        ofVec3f yAxis(0,0,1);
                        float angle = yAxis.angle(vAB);
                        ofVec3f prep = yAxis.cross(vAB);
                        
                        scale.set(1, 1, dist);
                        mat.rotate(angle, prep.x, prep.y, prep.z);
                        mat.translate(pos);
                        
                        // add to deafault group
                        ic2->addInstanceMatrix(mat, scale);
                        find = true;
                        numFind++;
                        
                        checkList.push_back(idp);
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

}

void testApp::processCollisionTest(){
    
    instancedComponent::resetGroup();
    myLogRelease("Process CollisionTest");

#ifndef NDEBUG
    instancedComponent::printData();
#endif
    
    INSTANCE_MAP& instanceMap = instancedComponent::getInstanceMap();
    INSTANCE_MAP_ITR itrA = instanceMap.begin();
    INSTANCE_MAP_ITR itrB = itrA;
    INSTANCE_MAP_ITR end  = instanceMap.end();
    
    typedef map<instance*, int> TMAP;
    typedef pair<instance*, int> TPAIR;
    typedef TMAP::iterator TITR;
    TMAP tmap;
    
    char m[255];
    for(int i=0; itrA!=end; itrA++, i++){
        sprintf(m, "processCollisionTest i:%d", i);
        myLogDebug(ofToString(m));
        
        instance& insA = itrA->second;
        ofMatrix4x4& matA = insA.matrix;
        ofVec3f& sA = insA.scale;
        itrB = itrA;
        for(itrB++; itrB!=end; itrB++){
            instance& insB = itrB->second;
            ofMatrix4x4& matB = insB.matrix;
            ofVec3f& sB = insB.scale;
            
            float dist=9999;
            INSTANCE_TYPE tA = insA.type;
            INSTANCE_TYPE tB = insB.type;
            
            if(tA == INSTANCE_SPHERE && tB==INSTANCE_SPHERE)
                dist = tester.testSphereSphere(matA,sA,matB,sB);
            else if(tA==INSTANCE_SPHERE && tB==INSTANCE_CYLINDER)
                dist = tester.testSphereCylinder(matA,sA,matB,sB);
            else if (tA==INSTANCE_CYLINDER && tB==INSTANCE_SPHERE)
                dist = tester.testSphereCylinder(matA,sA,matB,sB);
            else if(tA==INSTANCE_CYLINDER && tB==INSTANCE_CYLINDER)
                dist = tester.testCylinderCylinder(matA,sA,matB,sB);
            else{
                myLogRelease("Can not test collision");
                return;
            }
            
            if(dist<0.0) {
                int groupIdA = itrA->first;
                int groupIdB = itrB->first;
                
                TITR titrA = tmap.find(&itrA->second);
                if(titrA!=tmap.end())
                    groupIdA = titrA->second;
                
                TITR titrB = tmap.find(&itrB->second);
                if(titrB!=tmap.end())
                    groupIdB = titrB->second;
                
                
                if( (groupIdA!=-1 && groupIdB!=-1) && groupIdA == groupIdB)
                    continue;
                
                if(groupIdA==-1){
                    if(groupIdB==-1){
                        // move instance A, B to new group
                        int newId = spheres.incGroupIdMaster();
                        TITR titrA = tmap.find(&itrA->second);
                        if(titrA==tmap.end())
                            tmap.insert(TPAIR(&itrA->second, newId));
                        else
                            titrA->second = newId;
                        
                        TITR titrB = tmap.find(&itrB->second);
                        if(titrB==tmap.end())
                            tmap.insert(TPAIR(&itrB->second, newId));
                        else
                            titrB->second = newId;
                    }else{
                        // move A instance to B group
                        TITR titrA = tmap.find(&itrA->second);
                        if(titrA==tmap.end())
                            tmap.insert(TPAIR(&itrA->second, groupIdB));
                        else
                            titrA->second = groupIdB;
                    }
                }else{
                    if(groupIdB==-1){
                        // move B instance to A group
                        
                        TITR titrB = tmap.find(&itrB->second);
                        if(titrB==tmap.end())
                            tmap.insert(TPAIR(&itrB->second, groupIdA));
                        else
                            titrB->second = groupIdA;
                    }else{
                        // at first move B instance to A group
                        TITR titrB = tmap.find(&itrB->second);
                        if(titrB==tmap.end())
                            tmap.insert(TPAIR(&itrB->second, groupIdA));
                        else
                            titrB->second = groupIdA;
                        
                        // second, move ALL B group instance to A
                        // search from instanceMap
                        pair<INSTANCE_MAP_ITR, INSTANCE_MAP_ITR> p = instanceMap.equal_range(groupIdB);
                        INSTANCE_MAP_ITR ritrB = p.first;
                        for(; ritrB!=p.second; ritrB++){
                            tmap.insert(TPAIR(&ritrB->second, groupIdA));
                        }
                        
                        // search from tmap
                        TITR titr = tmap.begin();
                        
                        for(; titr!=tmap.end(); titr++){
                            if(titr->second == groupIdB){
                                titr->second =groupIdA;
                            }
                        }
                    }
                }
                
//                ofFloatColor red = ofFloatColor(1.0, 0.0, 0.0);
//                spheres.setInstanceColor(itrA, red);
//                spheres.setInstanceColor(itrB, red);
            }
        }
    }
    
    myLogDebug("change group");
    
    
    TITR itr = tmap.begin();
    for(; itr!=tmap.end(); itr++){
        instance * ins = itr->first;
        int g = itr->second;
        sprintf(m, "group %03d", g);
        myLogDebug(m);
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
    
    myLogRelease("finish CollisionTest");
}



