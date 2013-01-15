//
//  instancedComponent.cpp
//

#include "instancedComponent.h"
#include "ofxMtb.h"
#include "ofxAssimpModelLoader.h"


instancedComponent::instancedComponent():
bVtxtexNeedUpdate(false),
bCltexNeedUpdate(true),
instanceNum(0),
vtxtexId(GL_NONE),
cltexId(GL_NONE),
groupIdMaster(-1)
{
    //allocate();
    shaderVtxTextureName = "vtxtex";
    shaderColorTextureName = "cltex";
    
    offsetPosition.set(0,0,0);
    offsetRotation.set(0, 0, 0, 0);
    offsetScale.set(1,1,1);
}

instancedComponent::~instancedComponent(){
    destroy();
}

void instancedComponent::destroy(){
    glDeleteTextures(1, &vtxtexId);
    glDeleteTextures(1, &cltexId);
    //delete matrices;
    
    vmi->clear();
    
    myLogDebug("instancedComponent destroyed");
}


void instancedComponent::update(){
    updateVertexTexture();
    
    updateColorTexture();
}


void instancedComponent::updateVertexTexture(){
    // VERTEX UPDATE
    if(bVtxtexNeedUpdate){
        int id = 0;
        float * matrices = new float[VTX_TEX_WIDTH*VTX_TEX_HEIGHT*4];

        INSTANCE_GROUPS::iterator itr = instanceGroups.begin();
        for(; itr!=instanceGroups.end(); itr++){
            
            instanceGroup& g = (itr->second);
            int n = g.instances.size();
            
            for(int j=0; j<n; j++){
                instance& d = g.instances[j];
                ofMatrix4x4& m = d.matrix;
                
                for(int k=0; k<4; k++){
                    for(int l=0; l<4; l++){
                        matrices[id*16 + k*4 + l] = m(k, l);
                    }
                }
                
                id++;
            }
        }
    
        if(vtxtexId==GL_NONE){
            glGenTextures(1, &vtxtexId);
            myLogRelease("Generate vertex texture with id: " + ofToString(vtxtexId));
        }

        glBindTexture(GL_TEXTURE_2D, vtxtexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, VTX_TEX_WIDTH, VTX_TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bVtxtexNeedUpdate = false;
        delete matrices;
    }
}


void instancedComponent::updateColorTexture(){
    
    // COLOR UPDATE
    if(bCltexNeedUpdate){
        
        float * colors = new float[CL_TEX_WIDTH*CL_TEX_HEIGHT*4];
        int id = 0;
        INSTANCE_GROUPS::iterator itr = instanceGroups.begin();
        for(; itr!=instanceGroups.end(); itr++){
            instanceGroup& g = (itr->second);
            int n = g.instances.size();
            for(int j=0; j<n; j++){
                instance& d = g.instances[j];
                ofFloatColor& color = d.color;
                colors[id*4+0] =color.r;
                colors[id*4+1] =color.g;
                colors[id*4+2] =color.b;
                colors[id*4+3] =color.a;
                
                id++;
            }
        }
        
        if(cltexId == GL_NONE){
            glGenTextures(1, &cltexId);
            myLogRelease("Generate color texture with id: " + ofToString(cltexId));
        }
        
        glBindTexture(GL_TEXTURE_2D, cltexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, CL_TEX_WIDTH, CL_TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, colors);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bCltexNeedUpdate = false;
        delete colors;
    }
}

void instancedComponent::draw(ofShader * shader){
    
    glPushMatrix();
    
//    glScalef(offsetScale.x, offsetScale.y, offsetScale.z);
//    glRotatef(offsetRotation.w, offsetRotation.x, offsetRotation.y, offsetRotation.z);
//    glTranslatef(offsetPosition.x, offsetPosition.y, offsetPosition.z);
    
    if(vtxtexId!=GL_NONE)
        shader->setUniformTexture(shaderVtxTextureName.c_str(), GL_TEXTURE_2D, vtxtexId, 0);
    
    if(cltexId!=GL_NONE)
        shader->setUniformTexture(shaderColorTextureName.c_str(), GL_TEXTURE_2D, cltexId, 1);
    
    vmi->setPrimCount(instanceNum);
    vmi->draw();

    glPopMatrix();
    
}


void instancedComponent::drawWireframe(ofShader * shader){
    glPushMatrix();
    
    //    glScalef(offsetScale.x, offsetScale.y, offsetScale.z);
    //    glRotatef(offsetRotation.w, offsetRotation.x, offsetRotation.y, offsetRotation.z);
    //    glTranslatef(offsetPosition.x, offsetPosition.y, offsetPosition.z);
    
    if(vtxtexId!=GL_NONE)
        shader->setUniformTexture(shaderVtxTextureName.c_str(), GL_TEXTURE_2D, vtxtexId, 0);
    
    if(cltexId!=GL_NONE)
        shader->setUniformTexture(shaderColorTextureName.c_str(), GL_TEXTURE_2D, cltexId, 1);

    vmi->setPrimCount(instanceNum);
    vmi->drawWireframe();
    
    glPopMatrix();
    
}



// component param
//
void instancedComponent::setOffsetPosition(ofVec3f p){
    offsetPosition = p;
    //bVtxtexNeedUpdate = true;
}

void instancedComponent::setOffsetRotation(ofVec4f r){
    offsetRotation = r;
    //bVtxtexNeedUpdate = true;
}

void instancedComponent::setOffsetScale(ofVec3f s){
    offsetScale = s;
    //bVtxtexNeedUpdate = true;
}


// instance param
//
void instancedComponent::setInstanceMatrix(int groupId, int index, ofMatrix4x4 m){
    if(index<0){
        myLogRelease("invalid value for index num");
        return;
    }else if(groupId<0){
        myLogRelease("invalid value for groupId");
        return;
    }
    
    INSTANCE_GROUPS::iterator itr;
    INSTANCE_GROUPS::iterator end = instanceGroups.end();
    itr = instanceGroups.find(groupId);

    if(itr == end){
        initGroup();
        itr = instanceGroups.find(groupId);
    }
    
    instanceGroup &g = (itr->second);
    instance d;
    //d.instanceId = index;
    d.matrix = m;
    g.instances.push_back(d);
    
    bVtxtexNeedUpdate = true;
}

void instancedComponent::setInstanceMatrix(int groupId, int index, ofVec3f p, ofVec4f r, ofVec3f s){
    ofMatrix4x4 m;
    //ofVec3f position = trans + (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
    
    m.makeIdentityMatrix();
    m.rotate(r[3], r[0], r[1], r[2]);
    m.scale(s);
    m.translate(p);
        
    setInstanceMatrix(groupId, index, m);
}



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
    
//    if (bTexAllocated)
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
        
        setInstanceMatrix(0, i, m);
    }

    vmi->setPrimCount(instanceNum);
    
    bVtxtexNeedUpdate = true;
    model.clear();
}

void instancedComponent::loadInstancePositionFromMatrices(ofMatrix4x4 *ms, int size){

    clearInstanceMatrices();
    
    for(int i=0; i<size; i++){
        setInstanceMatrix(0, i, ms[i]);
    }
    
    instanceNum = size;
    bVtxtexNeedUpdate = true;
}



void instancedComponent::clearInstanceMatrices(){

    INSTANCE_GROUPS::iterator itr = instanceGroups.begin();
    for(; itr!=instanceGroups.end(); itr++){
        instanceGroup g = (itr->second);
        g.instances.clear();
    }
    instanceGroups.clear();
    
    myLogDebug("clear all group data");
    bVtxtexNeedUpdate = true;
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
    
#ifndef NDEBUG
    // just check
    for(int i=0; i<mesh.getNumVertices(); i++){
        ofVec3f pos = vmi->getVertex(i);
        char mes[256];
        sprintf(mes, "vertices[%d] =  %03f, %03f, %03f", i, pos.x, pos.y, pos.z);
        myLogDebug(string(mes));
    }
#endif
}


// create empty group
int instancedComponent::initGroup(){
    instanceGroup group;
    int groupId = ++groupIdMaster;
    instanceGroups.insert(pair<int, instanceGroup>(groupId, group));
    myLogDebug("create new group with id: " + ofToString(groupId));
    return groupId;
}


void instancedComponent::reset(){
    clearInstanceMatrices();
}


/*
void instancedComponent::sendVertexData(ofShader * shader, GLuint loc){

    int id = 0;
    INSTANCE_GROUPS::iterator itr = instanceGroups.begin();
    
    for(; itr!=instanceGroups.end(); itr++){
        
        instanceGroup g = (itr->second);
        
        int n= g.instances.size();
        
        for(int j=0; j<n; j++){
            
            instance d = g.instances[j];
            ofMatrix4x4 mat = d.matrix;
            ofVec4f p = mat.getTranslation();

            shader->setAttribute4f(loc, p.x, p.y, p.z, 1);
        }
    }
}
*/



void instancedComponent::setInstanceColor(int groupId, int index, ofFloatColor color){
    if(index<0){
        myLogRelease("invalid value for index num");
        return;
    }else if(groupId<0){
        myLogRelease("invalid value for groupId");
        return;
    }
    
    INSTANCE_GROUPS::iterator itr;
    INSTANCE_GROUPS::iterator end = instanceGroups.end();
    itr = instanceGroups.find(groupId);
    
    if(itr == end){
        myLogRelease("Can not find group with groupId: " + ofToString(groupId));
    }
    
    instanceGroup &g = (itr->second);
    int n = g.instances.size();
    
    if(n<=index){
        myLogRelease("out of range for instanceGroups["+ ofToString(groupId)+ "].instances["+ofToString(index)+"]");
    }
    
    instance& ins =  g.instances[index];
    ins.color = color;

    bCltexNeedUpdate = true;
}



//
//  groupA += groupB
//
//  result:
//      A = A+B
//      B = 0
//
void instancedComponent::mergeInstanceGroup(int groupIdA, int groupIdB){
    
    INSTANCE_GROUPS::iterator end = instanceGroups.end();
    
    INSTANCE_GROUPS::iterator itrA = instanceGroups.find(groupIdA);
    INSTANCE_GROUPS::iterator itrB = instanceGroups.find(groupIdB);

    if(itrA!=end || itrB!=end){
        myLogRelease("Can not merge instanceGroup: " + ofToString(groupIdA) + " + " + ofToString(groupIdB));
        return;
    }
    
    myLogRelease("merge group " +  ofToString(groupIdA) + " + " + ofToString(groupIdB));
    
    mergeInstanceGroup(itrA, itrB);
    instanceGroups.erase(itrB);
}

void instancedComponent::mergeInstanceGroup(INSTANCE_GROUPS::iterator itrA, INSTANCE_GROUPS::iterator itrB){
    
    instanceGroup &gA = itrA->second;
    instanceGroup &gB = itrB->second;
    
    INSTANCES& insA = gA.instances;
    INSTANCES& insB = gB.instances;
    
    insA.insert(insA.end(), insB.begin(), insB.end());
    insB.clear();
    
    // dont forget erase groupB from instanceGroups
}

void instancedComponent::mergeInstanceGroupAll(){
    
    INSTANCE_GROUPS::iterator begin = instanceGroups.begin();
    INSTANCE_GROUPS::iterator itr =  ++begin;

    while(itr!=instanceGroups.end()){
        mergeInstanceGroup(begin, itr);
        instanceGroups.end();
        itr = begin++;
    }
}

