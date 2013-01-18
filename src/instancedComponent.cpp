//
//  instancedComponent.cpp
//

#include "instancedComponent.h"
#include "ofxMtb.h"
#include "ofxAssimpModelLoader.h"
#include "collisionTester.h"

int instancedComponent::groupIdMaster = -1;
INSTANCE_MAP instancedComponent::instanceMap;

instancedComponent::instancedComponent():
bVtxtexNeedUpdate(true),
bCltexNeedUpdate(true),
instanceNum(0),
vtxtexId(GL_NONE),
cltexId(GL_NONE)
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

void instancedComponent::setInstanceType(INSTANCE_TYPE t){
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(; itr!=instanceMap.end(); itr++){
        itr->second.type = t;
    }
}

void instancedComponent::update(INSTANCE_TYPE t){
    updateVertexTexture(t);
    
    updateColorTexture(t);
}

void instancedComponent::updateVertexTexture(INSTANCE_TYPE t){
    // VERTEX UPDATE
    if(bVtxtexNeedUpdate){

        float * matrices = new float[VTX_TEX_WIDTH*VTX_TEX_HEIGHT*4];

        INSTANCE_MAP_ITR itr = instanceMap.begin();
        for(int id =0; itr!=instanceMap.end(); itr++){
            
            instance& d = itr->second;
            if(d.type == t){
                // instance.matrix does not have scale info, here we construct for GL

                ofMatrix4x4 m;
                
                m.scale(d.scale);
                m.rotate(d.matrix.getRotate());
                m.translate(d.matrix.getTranslation());
                
                
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

void instancedComponent::updateColorTexture(INSTANCE_TYPE t){
    
    // COLOR UPDATE
    if(bCltexNeedUpdate){
        
        float * colors = new float[CL_TEX_WIDTH*CL_TEX_HEIGHT*4];
        int id = 0;
        INSTANCE_MAP_ITR itr = instanceMap.begin();
        for(; itr!=instanceMap.end(); itr++){
            instance& d = itr->second;
            if (d.type == t) {
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

    if(vtxtexId!=GL_NONE)
        shader->setUniformTexture(shaderVtxTextureName.c_str(), GL_TEXTURE_2D, vtxtexId, 0);
    
    if(cltexId!=GL_NONE)
        shader->setUniformTexture(shaderColorTextureName.c_str(), GL_TEXTURE_2D, cltexId, 1);

    vmi->setPrimCount(instanceNum);
    vmi->drawWireframe();
    
    glPopMatrix();
    
}

void instancedComponent::debugDraw(){
    
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(; itr!=instanceMap.end(); itr++){
        instance& d = itr->second;
        //if (d.type == t) {
            collisionTester::debugDraw(d.matrix, d.scale, d.type);  // type should be same with bullet shape type
        //}
    }
}



// instance param
//
void instancedComponent::addInstanceMatrix(ofMatrix4x4 m, ofVec3f s, INSTANCE_TYPE t, int groupId){
    if(index<0){
        myLogRelease("invalid value for index num");
        return;
    }else if(groupId<-2){
        myLogRelease("invalid value for groupId");
        return;
    }
    
    instance ins(t);
    ins.matrix = m;
    ins.scale = s;
    instanceMap.insert(pair<int, instance>(groupId, ins));
    bVtxtexNeedUpdate = true;
    
    //instanceNum++;
}

void instancedComponent::loadInstancePositionFromModel(string path, INSTANCE_TYPE t, float posScale=1){

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
        //clearInstanceMatrices();
    
    ofMatrix4x4 m;
    for(int i=0; i<instanceNum; i++){
        ofVec3f position = (mesh.getVertex(i)* posScale);     // SCALE POSITION!!
        //float scale = 1;
        //float angle = 0;
        
        m.makeIdentityMatrix();
        //m.rotate(angle, 1, 0, 0);
        //m.scale(scale, scale, scale);
        m.translate(position);
        ofVec3f s(1,1,1);
        addInstanceMatrix(m, s, t);
    }

    vmi->setPrimCount(instanceNum);
    
    bVtxtexNeedUpdate = true;
    model.clear();
}

void instancedComponent::loadInstancePositionFromMatrices(ofMatrix4x4 *ms, ofVec3f *ss, INSTANCE_TYPE t, int size){

    //clearInstanceMatrices();
    
    for(int i=0; i<size; i++){
        addInstanceMatrix(ms[i], ss[i], t);
    }
    
    instanceNum = size;
    bVtxtexNeedUpdate = true;
}

void instancedComponent::clearInstanceMatrices(){

    instanceMap.clear();
    
    myLogDebug("clear all group data");
    bVtxtexNeedUpdate = true;
    bCltexNeedUpdate  = true;
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

#if 0
#ifndef NDEBUG
    // just check
    for(int i=0; i<mesh.getNumVertices(); i++){
        ofVec3f pos = vmi->getVertex(i);
        char mes[256];
        sprintf(mes, "vertices[%d] =  %03f, %03f, %03f", i, pos.x, pos.y, pos.z);
        myLogDebug(string(mes));
    }
#endif
#endif
}

void instancedComponent::reset(){
    clearInstanceMatrices();
}

void instancedComponent::setInstanceColor(int index, ofFloatColor color){
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(int i=0; itr!=instanceMap.end(); itr++, i++){
        if(i==index){
            setInstanceColor(itr, color);
        }
    }
}

void instancedComponent::setInstanceColor(INSTANCE_MAP_ITR itr, ofFloatColor color){
    instance& ins = itr->second;
    ins.color = color;
    bCltexNeedUpdate = true;
}

void instancedComponent::setInstanceGroupColor(int groupId, ofFloatColor color){
    if(index<0){
        myLogRelease("invalid value for index num");
        return;
    }else if(groupId<-2){
        myLogRelease("invalid value for groupId");
        return;
    }


    INSTANCE_MAP_ITR itr = instanceMap.find(groupId);
    INSTANCE_MAP_ITR end = instanceMap.end();
    if(itr == end){
        myLogRelease("Can not find group with groupId: " + ofToString(groupId));
        return;
    }

    pair<INSTANCE_MAP_ITR, INSTANCE_MAP_ITR> fe = instanceMap.equal_range(groupId);
    itr = fe.first;
    for(;itr!=fe.second; itr++){
        itr->second.color = color;
    }

    bCltexNeedUpdate = true;
}

void instancedComponent::changeInstanceGroupId(INSTANCE_MAP_ITR& itr, int groupId){
    STL_UTIL::changeKey(instanceMap, itr, groupId);
}

//
//  groupA += groupB
//
//  result:
//      A = A+B
//      B = 0
//
void instancedComponent::mergeInstanceGroup(int groupIdA, int groupIdB){
    
    INSTANCE_MAP_ITR end = instanceMap.end();
    
    INSTANCE_MAP_ITR itrA = instanceMap.find(groupIdA);
    INSTANCE_MAP_ITR itrB = instanceMap.find(groupIdB);

    if(itrA!=end || itrB!=end){
        myLogRelease("Can not merge instanceGroup: " + ofToString(groupIdA) + " + " + ofToString(groupIdB));
        return;
    }
    
    myLogRelease("merge group " +  ofToString(groupIdA) + " + " + ofToString(groupIdB));
    
    STL_UTIL::replace_key(instanceMap, groupIdA, groupIdB);
}

#include <set>
void instancedComponent::mergeInstanceGroupAll(int groupId){

    set<int> tmp;
    
    INSTANCE_MAP_ITR mbegin = instanceMap.begin();
    INSTANCE_MAP_ITR mitr = ++mbegin;
    
    for(; mitr!=instanceMap.end(); mitr=instanceMap.upper_bound(mitr->first)){
        tmp.insert(mitr->first);
    }
    
    set<int>::iterator sitr = tmp.begin();

    for(; sitr!=tmp.end(); sitr++){
        mergeInstanceGroup(groupId, (*sitr));
    }
}

void instancedComponent::printData(){
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    char m[255];
    for(int i=0; itr!=instanceMap.end(); itr++, i++){
        instance& ins = itr->second;
        ofMatrix4x4& mat = ins.matrix;
        sprintf(m, "instanceMap[%d]: group %d, type %d, x=%0.3f, y=%0.3f, z=%0.4f", i, itr->first, ins.type, mat.getTranslation().x, mat.getTranslation().y, mat.getTranslation().z);
        myLogRelease(m);
    }    
}
