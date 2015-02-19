#pragma once

#import <Cocoa/Cocoa.h>

#include "MainWindowController.h"
#include "PreferenceController.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    MainWindowController * mainWindowController;
    PreferenceController * prefController;
}


-(IBAction)showPreference:(id)sender;

@end
