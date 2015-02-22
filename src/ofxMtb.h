//
//  ofxMtb.h
//
// ここに定義を書いたらLinkエラーになるので注意。

#pragma once

#include <string>

using namespace std;

namespace ofxMtb{
    
    static string myModule = "instancedModeler";
    
    void myLogDebug(string message);
    void myLogRelease(string message);
    ofMesh createCylinderZ(float radius, float height, int slices, int stacks);
    ofMesh createCylinderY(float radius, float height, int slices, int stacks);
    ofMesh createIcosphere(float radius, int iteration);
    ofMesh createQuadSphere(float r, int lats, int longs);
    ofMesh createIcosahedron(float radius);
    
    struct idPair{
    private:
        idPair();
        
    public:
        int a;
        int b;
        idPair(int _a, int _b);
        idPair(const idPair& ip);
    };
    
    bool operator<(const idPair& lhs, const idPair& rhs );
    bool operator==(const idPair& lhs, const idPair& rhs );
    
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

    
    float fBm1f( float x, int octave=4);
    float fBm1uf( float x, int octave=4);
    float fbm3f( float x, float y, float z, int octave=4);
    float fbm3uf( float x, float y, float z, int octave=4);
    float distSeg2Seg( ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4);
    
    string getGlVersion();
    
}