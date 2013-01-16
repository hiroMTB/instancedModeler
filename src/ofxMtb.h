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
            return;} //Thanks to Graham for this Fix
        typename CONTAINER::iterator begin(container.find(oldKey));
        for(;;){
            if(begin != container.end()){
                container.insert(typename CONTAINER::value_type(newKey, begin->second));
                container.erase(begin); // Thanks to Graham for this potential fix
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
}