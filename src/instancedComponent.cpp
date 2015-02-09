//
//  instancedComponent.cpp
//

#include "instancedComponent.h"
#include "ofxMtb.h"
#include "ofxAssimpModelLoader.h"
#include "collisionTester.h"

int instancedComponent::groupIdMaster = -1;
int instancedComponent::groupTotalNum = 1;
INSTANCE_MAP instancedComponent::instanceMap;

instancedComponent::instancedComponent():
bVtxtexNeedUpdate(true),
bCltexNeedUpdate(true),
instanceNum(0),
vtxtexId(GL_NONE),
cltexId(GL_NONE)
{
}


instancedComponent::instancedComponent(INSTANCE_TYPE t, INSTANCE_MAP& m):
insType(t),
bVtxtexNeedUpdate(true),
bCltexNeedUpdate(true),
instanceNum(0),
vtxtexId(GL_NONE),
cltexId(GL_NONE)
{
//    instanceMap.insert(m);
}

instancedComponent::~instancedComponent(){
    destroy();
}

void instancedComponent::destroy(){
    glDeleteTextures(1, &vtxtexId);
    glDeleteTextures(1, &cltexId);
    //delete matrices;
    
    vmi.clear();
    myLogDebug("instancedComponent destroyed");
}

void instancedComponent::setInstanceType(INSTANCE_TYPE t){
    insType = t;
}

void instancedComponent::update(){
    updateVertexTexture();
    
    updateColorTexture();
}

void instancedComponent::updateRequest(){
    updateInstanceNum();
    bCltexNeedUpdate = true;
	bVtxtexNeedUpdate = true;
}

void instancedComponent::updateVertexTexture(){
    // VERTEX UPDATE
    if(bVtxtexNeedUpdate){

        float * matrices = new float[VTX_TEX_WIDTH*VTX_TEX_HEIGHT*4];

        INSTANCE_MAP_ITR itr = instanceMap.begin();
        for(int id =0; itr!=instanceMap.end(); itr++){
            
            instance& d = itr->second;
            if(d.type == insType){
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
            myLogDebug("Generate vertex texture with id: " + ofToString(vtxtexId));
        }

        glBindTexture(GL_TEXTURE_2D, vtxtexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, VTX_TEX_WIDTH, VTX_TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, matrices);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bVtxtexNeedUpdate = false;
        delete matrices;
        matrices = 0;
    }
}

void instancedComponent::updateColorTexture(){
    
    // COLOR UPDATE
    if(bCltexNeedUpdate){
        
        float * colors = new float[CL_TEX_WIDTH*CL_TEX_HEIGHT*4];
        int id = 0;
        INSTANCE_MAP_ITR itr = instanceMap.begin();
        for(; itr!=instanceMap.end(); itr++){
            instance& d = itr->second;
            if (d.type == insType) {
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
            myLogDebug("Generate color texture with id: " + ofToString(cltexId));
        }
        
        glBindTexture(GL_TEXTURE_2D, cltexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, CL_TEX_WIDTH, CL_TEX_HEIGHT, 0, GL_RGBA, GL_FLOAT, colors);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bCltexNeedUpdate = false;
        delete colors;
        colors = 0;
    }
}

void instancedComponent::draw(ofShader * shader){
    
    glPushMatrix();
    
    if(vtxtexId!=GL_NONE)
        shader->setUniformTexture("vtxtex", GL_TEXTURE_2D, vtxtexId, 0);
    
    if(cltexId!=GL_NONE)
        shader->setUniformTexture("cltex", GL_TEXTURE_2D, cltexId, 1);
    
    vmi.setPrimCount(instanceNum);
    vmi.draw();

    glPopMatrix();
    
}

void instancedComponent::drawWireframe(ofShader * shader){
    glPushMatrix();

    if(vtxtexId!=GL_NONE)
        shader->setUniformTexture("vtxtex", GL_TEXTURE_2D, vtxtexId, 0);
    
    if(cltexId!=GL_NONE)
        shader->setUniformTexture("cltex", GL_TEXTURE_2D, cltexId, 1);

    vmi.setPrimCount(instanceNum);
    vmi.drawWireframe();
    
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

void instancedComponent::addInstance(instance ins, int groupId){
    instanceMap.insert(pair<int, instance>(groupId, ins));
    bVtxtexNeedUpdate = true;
    bCltexNeedUpdate = true;
//    instanceNum++;
}


void instancedComponent::addInstanceMatrix(ofMatrix4x4 m, ofVec3f s, int groupId){
    if(index<0){
        myLogRelease("invalid value for index num");
        return;
    }else if(groupId<-2){
        myLogRelease("invalid value for groupId");
        return;
    }
    
    instance ins;
    ins.matrix = m;
    ins.scale = s;
    ins.type = insType;
    addInstance(ins);
}

void instancedComponent::loadInstancePositionFromModel(string path, int res, float posScale=1){

    vector<string> strs = ofSplitString(path, ".");
    string ext = strs.back();
    if( ext == "dae"){
        myLogRelease("ERROR Do not use .dae file. Use .ply instead");
        return;
    }
    
    
    ofxAssimpModelLoader model;
    
    if(model.loadModel(path)){
    
        ofMatrix4x4 m;
        int numMeshs = model.getNumMeshes();

        myLogRelease("load model for position, including "+ofToString(numMeshs)+" meshs.");
        
        for(int i=0; i<numMeshs; i++){
            
            myLogRelease("load mesh(" +ofToString(i)+ ")");
            ofVboMesh mesh;
            mesh = model.getMesh(i);
            int numVertices = mesh.getNumVertices();
            int numNormals = mesh.getNumNormals();
            int numIndices = mesh.getNumIndices();
            
            myLogRelease("numVertices = " + ofToString(numVertices));
            myLogRelease("numNormals = "  + ofToString(numNormals) );
            myLogRelease("numIndices = " + ofToString(numIndices));

            // vertices
            for(int j=0; j<numVertices; j++){
                if( j%res == 0){
                    ofVec3f position = (mesh.getVertex(j)* posScale);     // SCALE POSITION!!
                    m.makeIdentityMatrix();
                    m.translate(position);
                    ofVec3f s(1,1,1);
                    
                    addInstanceMatrix(m, s, insType);
                }
            }
            
            // edge
            
            
        }
    }
    
    

    bVtxtexNeedUpdate = true;
    
    updateInstanceNum();
    
    model.clear();
}

void instancedComponent::loadInstancePositionFromMatrices(ofMatrix4x4 *ms, ofVec3f *ss, int size){

    //clearInstanceMatrices();
    
    for(int i=0; i<size; i++){
        addInstanceMatrix(ms[i], ss[i], insType);
    }
    
    bVtxtexNeedUpdate = true;
    updateInstanceNum();
}

void instancedComponent::clearInstanceMatrices(){

    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(; itr!=instanceMap.end(); ){
        if(itr->second.type==insType){
            instanceMap.erase(itr++);
        }else{
            ++itr;
        }
    }
    
    updateInstanceNum();
    
    myLogDebug("clear instance data");
    bVtxtexNeedUpdate = true;
    bCltexNeedUpdate  = true;
}

void instancedComponent::loadInstanceMesh(ofMesh mesh, ofVec3f scale){
    
    // scale
    for(int i=0; i<mesh.getNumVertices(); i++){
        ofVec3f pos = mesh.getVertex(i) * scale;
        mesh.setVertex(i, pos);
    }
    
    vmi.clear();
    vmi = mesh;
    int numVertices = vmi.getNumVertices();
    int numNormals = vmi.getNumNormals();
    int numIndices = vmi.getNumIndices();
    
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

void instancedComponent::setGroupColorGradient(){

    int instanceAllNum = instanceMap.size();
    int keySize = STL_UTIL::getAllKeySize(instanceMap);
    
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(int i=0; itr!=instanceMap.end(); itr=instanceMap.upper_bound(itr->first), i++){

        int key = itr->first;
        int n = STL_UTIL::getElementSize(instanceMap, key);
        ofFloatColor color;
        color.setHsb(ofRandom(0.9),ofRandom(0.9), ofRandom(0.9));
        
        setGroupColor(key, color);
    }
}

void instancedComponent::setGroupColor(int groupId, ofFloatColor color){
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
        myLogRelease("Can not set color to group, Id: " + ofToString(groupId));
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

    myLogRelease("merge group " +  ofToString(groupIdA) + " + " + ofToString(groupIdB));

    if(itrB==end){
        myLogRelease("Can not merge instanceGroup: " + ofToString(groupIdA) + " + " + ofToString(groupIdB));
        return;
    }
    
    if(itrA==end){
        myLogRelease("Can not find group so merge to new group : " + ofToString(groupIdA));
       STL_UTIL::replace_key(instanceMap, groupIdB, groupIdA);
    }else{
        STL_UTIL::replace_key(instanceMap, groupIdA, groupIdB);
    }
    
    
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

vector<string> instancedComponent::printData(bool console){
    vector<string> data;
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    char m[255];
    for(int i=0; itr!=instanceMap.end(); itr++, i++){
        instance& ins = itr->second;
        ofMatrix4x4& mat = ins.matrix;
        sprintf(m, "instance[%03d]: group %03d, type %02d, x=%0.3f, y=%0.3f, z=%0.4f", i, itr->first, ins.type, mat.getTranslation().x, mat.getTranslation().y, mat.getTranslation().z);

        if(console)
            myLogRelease(m);
        else
            data.push_back(m);
    }
    return data;
}

vector<string> instancedComponent::printGroupData(bool console){
    vector<string>  data;
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    char m[255];
    
    for(; itr!=instanceMap.end(); itr=instanceMap.upper_bound(itr->first) ){
        int key = itr->first;
        int n = STL_UTIL::getElementSize(instanceMap, key);
        if( key == -1){
            sprintf(m, "Single Spheres n=%04d", n);
        }else{
            sprintf(m, "GRP %03d n=%04d", key, n);
        }
        
        if(console)
            myLogRelease(m);
        else
            data.push_back(m);
    }
    return data;
}

int instancedComponent::updateGroupTotalNum(){
    groupTotalNum =  STL_UTIL::getAllKeySize(instanceMap);
    return groupTotalNum;
}


void instancedComponent::removeGroup(int groupId){
    instanceMap.erase(groupId);
    bCltexNeedUpdate = true;
    bVtxtexNeedUpdate = true;
}

void instancedComponent::removeSmallGroup(int minNum){

    // remove -1 group
    removeGroup(-1);
    
    INSTANCE_MAP_ITR itr = instanceMap.begin();
    for(; itr!=instanceMap.end(); itr=instanceMap.upper_bound(itr->first)){       // iterate each key
        int groupSize = STL_UTIL::getElementSize(instanceMap, itr->first);
        if(groupSize<minNum){
            removeGroup(itr->first);
        }
    }
    
    bCltexNeedUpdate = true;
    bVtxtexNeedUpdate = true;
    updateInstanceNum();
}


void instancedComponent::updateInstanceNum(){
    instanceNum = 0;
    INSTANCE_MAP_ITR itr = instanceMap.begin();

    for(; itr!=instanceMap.end(); itr++){
        instance& ins = itr->second;
        if(insType==ins.type){
            instanceNum++;
        }
    }
}



void instancedComponent::resetGroup(){
    
    int defaultGroup = -1;
//    INSTANCE_MAP_ITR itr = instanceMap.begin();
//    
//    for(; itr!=instanceMap.end(); itr=instanceMap.upper_bound(itr->first)){
//        myLogDebug("replace");
//        STL_UTIL::replace_key(instanceMap, itr->first, defaultGroup);
//    }
    
    char d[255];
    INSTANCE_MAP temp = instanceMap;
    instanceMap.clear();
    INSTANCE_MAP_ITR itr = temp.begin();
    for(; itr!=temp.end(); itr++){
        instanceMap.insert(pair<int, instance>(-1, itr->second));
    }
    
    groupIdMaster = 0;
}


//
//  save tp csv
//
//  NOTICE: first 3 lines are version and format info
//
void instancedComponent::saveInstanceDataToCsv(string dirpath){

    
    float meshScale = 0.00000001;    // TODO:
    
    string version = "v0.3";
    string meshName;
    
    
    if(insType==INSTANCE_SPHERE){
        meshName = "Spheres";
    }else if(insType==INSTANCE_CYLINDER){
        meshName = "Cylinders";
    }

    string fileName =  meshName + ".csv";
    ofDirectory dir;
    dir.createDirectory(dirpath);
    string filePath = dirpath + "/" + fileName;
    ofstream myfile(filePath.c_str());

    if(myfile.is_open()){
        char d[255];
        sprintf(d, "renature,instance data,format=%s", version.c_str());       // line 1
        myfile << d << "\n";
        
        sprintf(d, "mesh=%s, posScale=%f, instanceNum=%d", meshName.c_str(), meshScale, instanceNum);    // line 2
        myfile << d << "\n";

        sprintf(d, "name, position.x,position.y,position.z,angle, axis.x,axis.y,axis.z,scale.x,scale.y,scale.z, instanceType, groupId");    // line 3
        myfile << d << "\n";
        

        INSTANCE_MAP_ITR itr = instanceMap.begin();
        for(int i=0; itr!=instanceMap.end(); itr++, i++){
            int groupId         = itr->first;
            instance& ins       = itr->second;
            INSTANCE_TYPE t     = ins.type;
            string typeName;
            if (t == INSTANCE_SPHERE) {
                typeName = "sphere";
            }else if(t == INSTANCE_CYLINDER){
                typeName = "cylinder";
            }else{
                typeName = "unknownType";
            }
            
            if(t==insType){
                char    name[255];
                sprintf(name, "%s%05d", typeName.c_str(), i);
                ofMatrix4x4& mat    = ins.matrix;
                ofVec3f pos         = mat.getTranslation();
                ofQuaternion quat   = mat.getRotate();       // TODO double check blender side rotation compatibility
                ofVec3f axis;
                float angle;
                quat.getRotate(angle, axis);
                axis.normalize();
                ofVec3f& sc         = ins.scale;
                
                sprintf(d, "%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%i,%i",
                        name,
                        pos.x, pos.y, pos.z,
                        angle, axis.x, axis.y, axis.z,
                        sc.x, sc.y, sc.z,
                        t,
                        groupId);
                myfile << d << "\n";
            }
        }
        
        myfile.close();
    }
}


void instancedComponent::loadInstanceDataFromCsv(string filePath){
    
    ofFile file(filePath);
    string fileName = file.getFileName();
    if(fileName=="Spheres.csv" && insType == INSTANCE_SPHERE){
    }else if(fileName=="Cylinders.csv" && insType == INSTANCE_CYLINDER){
    }else{
        return;
    }

    string line;
    ifstream data(filePath.c_str());
    int skip=0;
    while(std::getline(data, line)){
        if(skip++<3) continue;
        std::stringstream lineStream(line);
        string name, posx, posy, posz, angle, axisx, axisy, axisz, scalex, scaley, scalez, type, groupId;
        std::getline(lineStream, name, ',');
        std::getline(lineStream, posx, ',');
        std::getline(lineStream, posy, ',');
        std::getline(lineStream, posz, ',');
        std::getline(lineStream, angle, ',');
        std::getline(lineStream, axisx, ',');
        std::getline(lineStream, axisy, ',');
        std::getline(lineStream, axisz, ',');
        std::getline(lineStream, scalex, ',');
        std::getline(lineStream, scaley, ',');
        std::getline(lineStream, scalez, ',');
        std::getline(lineStream, type, ',');
        std::getline(lineStream, groupId, ',');
        
        instance ins;
        ins.matrix.setTranslation(ofToFloat(posx), ofToFloat(posy), ofToFloat(posz));
        ins.matrix.setRotate(ofQuaternion(ofToFloat(angle), ofVec3f(ofToFloat(axisx), ofToFloat(axisy), ofToFloat(axisz))));
        
        ins.scale.set(ofToFloat(scalex), ofToFloat(scaley),ofToFloat(scalez));
        ins.type    = (INSTANCE_TYPE)ofToInt(type);
        addInstance(ins, ofToInt(groupId) );
        
        if(insType != ins.type){
            cout << "error: strange instance type !!" << endl;
        }
    }
    
//    setGroupColorGradient();
    setGroupColor(-1, ofColor(0,0,0));
    
    
    bVtxtexNeedUpdate = true;
    bCltexNeedUpdate = true;
    updateInstanceNum();
}



void instancedComponent::removeDuplication(){
    
    INSTANCE_MAP_ITR itrA = instanceMap.begin();
    int indexA = -1;
    for(; itrA!=instanceMap.end(); itrA++){
        indexA++;
        instance& insA = itrA->second;

        int indexB = indexA+1;
        INSTANCE_MAP_ITR itrB = instanceMap.begin();
        std::advance(itrB, indexB);

        for(; itrB!=instanceMap.end(); itrB++, indexB++){
            instance& insB = itrB->second;
            if(insA == insB){
                instanceMap.erase(itrB++);
                myLogRelease("remove dupliated instance");
            }
        }
    }
}


