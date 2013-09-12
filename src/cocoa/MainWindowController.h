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
    
    
    IBOutlet NSSlider *sphereRadiusSlider;
    IBOutlet NSSlider *sphereResolutionSlider;
    IBOutlet NSSlider *cylinderRadiusSlider;
    IBOutlet NSSlider *cylinderResolutionSlider;
    IBOutlet NSSlider *connectRandomCylinderNumSlider;
    IBOutlet NSSlider *connectRandomMinDistanceSlider;
    IBOutlet NSSlider *connectRandomMaxDistanceSlider;
    IBOutlet NSSlider *connectGroupCylinderNumSlider;
    IBOutlet NSSlider *connectGroupMinDistanceSlider;
    IBOutlet NSSlider *connectGroupMaxDistanceSlider;
    IBOutlet NSSlider *collisionMarginSlider;

    IBOutlet NSTextField *sphereRadiusNBox;
    IBOutlet NSTextField *sphereResolutionNBox;
    IBOutlet NSTextField *cylinderRadiusNBox;
    IBOutlet NSTextField *cylinderResolutionNBox;
    IBOutlet NSTextField *connectRandomCylinderNumNBox;
    IBOutlet NSTextField *connectRandomMinDistanceNBox;
    IBOutlet NSTextField *connectRandomMaxDistanceNBox;
    IBOutlet NSTextField *connectGroupCylinderNumNBox;
    IBOutlet NSTextField *connectGroupMinDistanceNBox;
    IBOutlet NSTextField *connectGroupMaxDistanceNBox;
    IBOutlet NSTextField *collisionMarginNBox;
    
    IBOutlet NSButton *resetInstanceShapeButton;
    
    IBOutlet NSButton *connectRandomButton;
    IBOutlet NSButton *connectGroupButton;
    IBOutlet NSButton *collisionTestButton;
    IBOutlet NSButton *removeGroupButton;
    IBOutlet NSButton *removeDuplicateButton;
    IBOutlet NSButton *removeAllCylinderButton;
    IBOutlet NSButton *exportToCsvButton;
    
    //
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


- (IBAction)changeSphereRadiusSlider:(NSSlider *)sender;
- (IBAction)changeSphereResolutionSlider:(NSSlider *)sender;
- (IBAction)changeCylinderRadiusSlider:(NSSlider *)sender;
- (IBAction)changeCylinderResolutionSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMaxDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMaxDistanceSlider:(NSSlider *)sender;
- (IBAction)changeCollisionMarginSlider:(NSSlider *)sender;

- (IBAction)changeSphereRadiusNBox:(NSTextField *)sender;
- (IBAction)changeSphereResolutionNBox:(NSTextField *)sender;
- (IBAction)changeCylinderRadiusNBox:(NSTextField *)sender;
- (IBAction)changeCylinderResolutionNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMaxDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMaxDistanceNBox:(NSTextField *)sender;
- (IBAction)changeCollisionMarginNBox:(NSTextField *)sender;


- (IBAction)pushResetInstanceShapeButton:(NSButton *)sender;
- (IBAction)pushConnectRandomButton:(NSButton *)sender;
- (IBAction)pushConnectGroupButton:(NSButton *)sender;
- (IBAction)pushCollisionTestButton:(NSButton *)sender;
- (IBAction)pushRemoveGroupsButton:(NSButton *)sender;
- (IBAction)pushRemoveDuplicateButton:(NSButton *)sender;
- (IBAction)pushRemoveAllCylinderButton:(NSButton *)sender;
- (IBAction)pushExportToCsvButton:(NSButton *)sender;


- (IBAction)changeColorSphere:(NSColorWell *)sender;
- (IBAction)changeColorCylinder:(NSColorWell *)sender;
- (IBAction)changeBgType:(NSMatrix *)sender;
- (IBAction)changeBgNormal:(NSColorWell *)sender;
- (IBAction)changeBgLinear0:(NSColorWell *)sender;
- (IBAction)changeBgLinear1:(NSColorWell *)sender;
- (IBAction)changeBgCircular0:(NSColorWell *)sender;
- (IBAction)changeBgCircular1:(NSColorWell *)sender;
- (IBAction)changeBgBar0:(NSColorWell *)sender;
- (IBAction)changeBgBar1:(NSColorWell *)sender;

















@end







































