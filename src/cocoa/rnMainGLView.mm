#import "rnMainGLView.h"
#include "rnApp.h"
#include "MainWindowController.h"

@implementation rnMainGLView


- (void)setup{

    //[MainWindowController setupDefault];

    string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    ofSetDataPathRoot(newPath);
    
    
    rnApp::init();
    rnApp::app->setup();

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
    rnApp::app->update();
}

- (void)draw{
    rnApp::app->draw();
}

- (void)exit
{
    rnApp::app->exit();
}

- (void)keyPressed:(int)key
{
    rnApp::app->keyPressed(key);
}

- (void)keyReleased:(int)key
{
    rnApp::app->keyReleased(key);
}

- (void)mouseMoved:(NSPoint)p
{
    rnApp::app->mouseMoved(p.x, p.y);
}

- (void)mouseDragged:(NSPoint)p button:(int)button
{
    rnApp::app->mouseDragged(p.x, p.y, button);
}

- (void)mousePressed:(NSPoint)p button:(int)button
{
    rnApp::app->mousePressed(p.x, p.y, button);
}

- (void)mouseReleased:(NSPoint)p button:(int)button
{
    rnApp::app->mouseReleased(p.x, p.y, button);
}

- (void)windowResized:(NSSize)size
{
    rnApp::app->windowResized(size.width, size.height);
}

static bool bWindowing;
+ (bool)getWindowing{
    return bWindowing;
}

@end
