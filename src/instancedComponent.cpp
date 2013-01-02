//
//  instancedComponent.cpp
//  renature_rev01
//
//  Created by matobahiroshi on 13/01/02.
//
//

#include "instancedComponent.h"
#include "ofxMtb.h"
#include "ofxAssimpModelLoader.h"



instancedComponent::instancedComponent(){
    allocate();
    shaderTextureName = "vtxtex";
    offsetPosition.set(0,0,0);
    offsetRotation.set(0, 0, 0, 0);
    offsetScale.set(1,1,1);
}

instancedComponent::~instancedComponent(){
    destroy();
}


void instancedComponent::allocate(){

    if(!bTexAllocated){
        if(matrices==NULL)
            matrices = new float[TEX_WIDTH * TEX_WIDTH * 16];

        // position data sender(textrue)
        if(texId!=GL_NONE)
            glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        myLogDebug("texture id = " + ofToString(texId));
        bTexAllocated = true;
        
        myLogDebug("instancedComponent allocated");
    }
}


void instancedComponent::destroy(){
    glDeleteTextures(1, &texId);
    delete matrices;
    
    vmi->clear();
    
    myLogDebug("instancedComponent destroyed");
}


void instancedComponent::update(){
    if(bTexNeedUpdate){
        if(bTexAllocated){
            glBindTexture(GL_TEXTURE_2D, texId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            bTexNeedUpdate = false;
        }
    }
}

void instancedComponent::draw(ofShader * shader){
    
    glPushMatrix();
    
    glScalef(offsetScale.x, offsetScale.y, offsetScale.z);
    glRotatef(offsetRotation.w, offsetRotation.x, offsetRotation.y, offsetRotation.z);
    glTranslatef(offsetPosition.x, offsetPosition.y, offsetPosition.z);
    
    shader->setUniformTexture(shaderTextureName.c_str(), GL_TEXTURE_2D, texId, 0);
    vmi->setPrimCount(instanceNum);
    vmi->draw();

    glPopMatrix();
    
}


void instancedComponent::drawWireframe(ofShader * shader){
    
}



// component param
//
void instancedComponent::setOffsetPosition(ofVec3f p){
    offsetPosition = p;
    //bTexNeedUpdate = true;
}

void instancedComponent::setOffsetRotation(ofVec4f r){
    offsetRotation = r;
    //bTexNeedUpdate = true;
}

void instancedComponent::setOffsetScale(ofVec3f s){
    offsetScale = s;
    //bTexNeedUpdate = true;
}


// instance param
//
void instancedComponent::setInstanceMatrix(int index, ofMatrix4x4 m){
    if(bTexAllocated){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                *(matrices+16*index + i*4+j) = m(i, j);
            }
        }
        
        bTexNeedUpdate = true;
    }else{
        myLogRelease("ERROR: Can not set instance matrix, texture is nor allocated");
    }
}

void instancedComponent::setInstanceMatrix(int index, ofVec3f p, ofVec4f r, ofVec3f s){
    ofMatrix4x4 m;
    //ofVec3f position = trans + (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
    
    m.makeIdentityMatrix();
    m.rotate(r[3], r[0], r[1], r[2]);
    m.scale(s);
    m.translate(p);
        
    setInstanceMatrix(index, m);
}



//void instancedComponent::setInstancePosition(int index, ofVec3f){
//    
//}
//
//
//void instancedComponent::setInstanceRotattion(int index, ofVec4f){
//    
//}
//
//
//void instancedComponent::setInstanceScale(int index, ofVec3f){
//    
//}




void instancedComponent::loadInstancePositionFromModel(string path, float posScale=1){

    
    vector<string> strs = ofSplitString(path, ".");
    string ext = strs.back();
    if( ext == "dae"){
        myLogRelease("ERROR Do not use .dae file. Use .ply instead");
        return;
    }
    
    
    ofVboMesh mesh;	
    ofxAssimpModelLoader model;
    if(model.loadModel(path)){
        mesh = model.getMesh(0);
    }
    
    int numVertices = mesh.getNumVertices();
    int numNormals = mesh.getNumNormals();
    int numIndices = mesh.getNumIndices();
    
    myLogDebug("load model for position");
    myLogDebug("numVertices = " + ofToString(numVertices));
    myLogDebug("numNormals = "  + ofToString(numNormals) );
    myLogDebug("numIndices = " + ofToString(numIndices));
    
    instanceNum = numVertices;
    
    if (bTexAllocated)
        clearInstanceMatrices();
    
    ofMatrix4x4 m;
    for(int i=0; i<instanceNum; i++){
        ofVec3f position = (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
        //float scale = 1;
        //float angle = 0;
        
        m.makeIdentityMatrix();
        //m.rotate(angle, 1, 0, 0);
        //m.scale(scale, scale, scale);
        m.translate(position);
        
        setInstanceMatrix(i, m);        
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
    
    
    bTexNeedUpdate = true;
    model.clear();
}

void instancedComponent::loadInstancePositionFromMatrices(ofMatrix4x4 *ms, int size){

    clearInstanceMatrices();
    
    for(int i=0; i<size; i++){
        setInstanceMatrix(i, ms[i]);
    }
    
    bTexNeedUpdate = true;
}



void instancedComponent::clearInstanceMatrices(){
    if(bTexAllocated){
        for(int i=0; i<TEX_WIDTH*TEX_HEIGHT*16; i++){
            matrices[i] = 0;
        }
    }
    
    bTexNeedUpdate = true;
}


void instancedComponent::loadInstanceMesh(ofMesh mesh, ofVec3f scale){
    
    // scale
    for(int i=0; i<mesh.getNumVertices(); i++){
        ofVec3f pos = mesh.getVertex(i) * scale;
        mesh.setVertex(i, pos);
    }
    
    vmi = new ofxVboMeshInstanced(mesh);
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
}



