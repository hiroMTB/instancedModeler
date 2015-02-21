#import "rnMainGLView.h"
#include "rnApp.h"
#include "MainWindowController.h"

@implementation rnMainGLView


- (void)setup{

    //[MainWindowController setupDefault];

    string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    ofSetDataPathRoot(newPath);
    
    
    rnApp::init();
    rnApp::get()->setup();

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
    rnApp::get()->update();
}

- (void)draw{
    rnApp::get()->draw();
}

- (void)exit
{
    rnApp::get()->exit();
}

- (void)keyPressed:(int)key
{
    rnApp::get()->keyPressed(key);
}

- (void)keyReleased:(int)key
{
    rnApp::get()->keyReleased(key);
}

- (void)mouseMoved:(NSPoint)p
{
    rnApp::get()->mouseMoved(p.x, p.y);
}

- (void)mouseDragged:(NSPoint)p button:(int)button
{
    rnApp::get()->mouseDragged(p.x, p.y, button);
}

- (void)mousePressed:(NSPoint)p button:(int)button
{
    rnApp::get()->mousePressed(p.x, p.y, button);
}

- (void)mouseReleased:(NSPoint)p button:(int)button
{
    rnApp::get()->mouseReleased(p.x, p.y, button);
}

- (void)windowResized:(NSSize)size
{
    rnApp::get()->windowResized(size.width, size.height);
}

static bool bWindowing;
+ (bool)getWindowing{
    return bWindowing;
}

@end
