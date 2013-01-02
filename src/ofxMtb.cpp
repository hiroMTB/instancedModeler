//
//  ofxMtb.cpp
//  renature_rev01
//
//  Created by matobahiroshi on 13/01/02.
//
//

#include "ofxMtb.h"
#include <IOStream>

void myLogDebug(string message){
#ifndef NDEBUG
    cout << " " + myModule + " D " + message << endl;
#endif
}


void myLogRelease(string message){
    cout << " " + myModule + " R " + message << endl;
}
