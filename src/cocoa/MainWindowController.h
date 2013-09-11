//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

#import "rnMainGLView.h"

@interface MainWindowController: NSWindowController {
    
    IBOutlet rnMainGLView *mainGLView;
    
    IBOutlet NSColorWell *colorSphere;
    IBOutlet NSColorWell *colorCylinder;
    
    IBOutlet NSMatrix *bgTypeMatrix;
    IBOutlet NSColorWell *bgPickerNormal;
    IBOutlet NSColorWell *bgPickerLinear0;
    IBOutlet NSColorWell *bgPickerLinear1;
    IBOutlet NSColorWell *bgPickerCircular0;
    IBOutlet NSColorWell *bgPickerCircular1;
    IBOutlet NSColorWell *bgPickerBar0;
    IBOutlet NSColorWell *bgPickerBar1;
}

+ (void) setupDefault;
+ (void) initializeParameters;

- (IBAction)changeColorSphere:(NSColorWell *)sender;
- (IBAction)changeColorCylinder:(NSColorWell *)sender;

- (IBAction)changeSphereRadius:(NSSlider *)sender;
- (IBAction)changeBgType:(NSMatrix *)sender;
- (IBAction)changeBgNormal:(NSColorWell *)sender;
- (IBAction)changeBgLinear0:(NSColorWell *)sender;
- (IBAction)changeBgLinear1:(NSColorWell *)sender;
- (IBAction)changeBgCircular0:(NSColorWell *)sender;
- (IBAction)changeBgCircular1:(NSColorWell *)sender;
- (IBAction)changeBgBar0:(NSColorWell *)sender;
- (IBAction)changeBgBar1:(NSColorWell *)sender;

@end
