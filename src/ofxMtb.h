//
//  ofxMtb.h
//

#pragma once

#include <string>

using namespace std;

static string myModule = "instancedModeler";

void myLogDebug(string message);
void myLogRelease(string message);
ofMesh createCylinderZ(float radius, float height, int slices, int stacks);
ofMesh createCylinderY(float radius, float height, int slices, int stacks);

ofMesh createQuadSphere(float r, int lats, int longs);


namespace STL_UTIL{
    template <typename CONTAINER>
    void replace_key(CONTAINER& container,
                     const typename CONTAINER::key_type& oldKey,
                     const typename CONTAINER::key_type& newKey)
    {
        if(!container.key_comp()(oldKey,newKey) && !container.key_comp()(newKey,oldKey)){
            return;}
        typename CONTAINER::iterator begin(container.find(oldKey));
        for(;;){
            if(begin != container.end()){
                container.insert(typename CONTAINER::value_type(newKey, begin->second));
                container.erase(begin);
                begin = container.find(oldKey);
            }
            else{
                return;
            }
        }
    }
    
    template <typename CONTAINER>
    int getAllKeySize(CONTAINER& container){
        int size=0;
        
        typename CONTAINER::iterator itr = container.begin();
        typename CONTAINER::iterator end = container.end();
        for(; itr!=end; itr=container.upper_bound(itr->first)){
            size++;
        }
        return size;
    }
    
    template<typename CONTAINER>
    void changeKey( CONTAINER& container,
                    const typename CONTAINER::iterator& itr,
                    const typename CONTAINER::key_type& newKey)
    {
        container.insert(typename CONTAINER::value_type(newKey, itr->second));
        container.erase(itr);
    }
    
    

    // for multimap only?
    template <typename CONTAINER>
    int getElementSize(CONTAINER& container, const typename CONTAINER::key_type& key){
        int size=0;
        
        pair<typename CONTAINER::iterator, typename CONTAINER::iterator> fe;
        fe = container.equal_range(key);
        
        typename CONTAINER::iterator itr = fe.first;
        
        for(; itr!=fe.second; itr++){
            size++;
        }
        return size;
    }
}


struct idPair{
    
private:
    idPair();
    int a;
    int b;
public:
    
    idPair(int _a, int _b):
    a(_a),
    b(_b)
    {}
    
    idPair(const idPair& ip){
        a=ip.a; b=ip.b;
    }
    

    
    bool operator<(const idPair& other){
        return a<other.a;
    }
        
    bool operator==(const idPair& other){
        bool aabb = (a==other.a) && (b==other.b);
        bool abba =(a==other.b) && (b==other.a);
        return aabb || abba;
    }
};



