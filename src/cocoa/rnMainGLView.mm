#import "rnMainGLView.h"
#include "testApp.h"
#include "MainWindowController.h"

@implementation rnMainGLView


- (void)setup{

    //[MainWindowController setupDefault];

    string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    ofSetDataPathRoot(newPath);
    
    
    testApp::init();
    testApp::get()->setup();

    // resize
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didResize:)
                                                 name:NSWindowDidResizeNotification
                                               object:nil];

    
    bWindowing = false;
}


- (void)didResize:(NSNotification *)notif{
}


- (void)update{
    testApp::get()->update();
}

- (void)draw{
    testApp::get()->draw();
}

- (void)exit
{
    testApp::get()->exit();
}

- (void)keyPressed:(int)key
{
    testApp::get()->keyPressed(key);
}

- (void)keyReleased:(int)key
{
    testApp::get()->keyReleased(key);
}

- (void)mouseMoved:(NSPoint)p
{
    testApp::get()->mouseMoved(p.x, p.y);
}

- (void)mouseDragged:(NSPoint)p button:(int)button
{
    testApp::get()->mouseDragged(p.x, p.y, button);
}

- (void)mousePressed:(NSPoint)p button:(int)button
{
    testApp::get()->mousePressed(p.x, p.y, button);
}

- (void)mouseReleased:(NSPoint)p button:(int)button
{
    testApp::get()->mouseReleased(p.x, p.y, button);
}

- (void)windowResized:(NSSize)size
{
    testApp::get()->windowResized(size.width, size.height);
}

static bool bWindowing;
+ (bool)getWindowing{
    return bWindowing;
}

@end
