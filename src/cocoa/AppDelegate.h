#pragma once

#import <Cocoa/Cocoa.h>

#import "PreferenceController.h"

#include "ofMain.h"
    
@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow * window;
    PreferenceController * _prefController;
}

@property (assign) IBOutlet NSWindow *window;

@property (retain) PreferenceController * prefController;

-(IBAction)showPreference:(id)sender;

@end
