//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

@interface PreferenceController : NSWindowController

//+ (NSDictionary*)getPreferencePaneDictionay;
- (void)setPreferencePaneNotification;
- (IBAction)closePreference:(id)sender;

@end
