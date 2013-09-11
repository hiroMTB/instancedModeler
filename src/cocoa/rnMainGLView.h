//
//  dialGLView.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/05.
//
//

#pragma once
#include "ofMain.h"
#include "ofxCocoaGLView.h"

@interface rnMainGLView : ofxCocoaGLView {

}


- (void)setup;
- (void)update;
- (void)draw;
- (void)exit;

- (void)keyPressed:(int)key;
- (void)keyReleased:(int)key;
- (void)mouseMoved:(NSPoint)p;
- (void)mouseDragged:(NSPoint)p button:(int)button;
- (void)mousePressed:(NSPoint)p button:(int)button;
- (void)mouseReleased:(NSPoint)p button:(int)button;
- (void)windowResized:(NSSize)size;


+ (bool)getWindowing;
@end
