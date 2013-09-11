//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

@interface rnUIController: NSViewController {
    NSColorWell *backgroundColorPicker;
}




+ (void) setupDefault;
+ (void) initializeParameters;

@end




