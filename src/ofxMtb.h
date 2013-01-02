//
//  ofxMtb.h
//

#pragma once

#include <string>

using namespace std;

static string myModule = "instancedModeler";

void myLogDebug(string message);
void myLogRelease(string message);
ofMesh createCylinder(float radius, float height, int slices, int stacks);