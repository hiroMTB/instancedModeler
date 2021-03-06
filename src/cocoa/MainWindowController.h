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
    
    // LOAD pane
    IBOutlet NSSlider *loadModelResolutionSlider;
    IBOutlet NSTextField *loadModelResolutionNBox;
    IBOutlet NSButton *loadCsvButton;
    IBOutlet NSButton *LoadModelDataButton;
    
    // Shape pane
    IBOutlet NSSlider *sphereRadiusSlider;
    IBOutlet NSSlider *sphereResolutionSlider;
    IBOutlet NSSlider *sphereCollisionMarginSlider;
    IBOutlet NSTextField *sphereRadiusNBox;
    IBOutlet NSTextField *sphereResolutionNBox;
    IBOutlet NSTextField *sphereCollisionMarginNBox;
    IBOutlet NSSlider *cylinderRadiusSlider;
    IBOutlet NSSlider *cylinderResolutionSlider;
    IBOutlet NSSlider *cylinderCollisionMarginSlider;
    IBOutlet NSTextField *cylinderRadiusNBox;
    IBOutlet NSTextField *cylinderResolutionNBox;
    IBOutlet NSTextField *cylinderCollisionMarginNBox;
    
    // Sculpt pane
    IBOutlet NSSlider *connectRandomCylinderNumSlider;
    IBOutlet NSSlider *connectRandomMinDistanceSlider;
    IBOutlet NSSlider *connectRandomMaxDistanceSlider;
    IBOutlet NSTextField *connectRandomCylinderNumNBox;
    IBOutlet NSTextField *connectRandomMinDistanceNBox;
    IBOutlet NSTextField *connectRandomMaxDistanceNBox;
    IBOutlet NSButton *connectRandomButton;
    IBOutlet NSSlider *connectGroupCylinderNumSlider;
    IBOutlet NSSlider *connectGroupMinDistanceSlider;
    IBOutlet NSSlider *connectGroupMaxDistanceSlider;
    IBOutlet NSTextField *connectGroupCylinderNumNBox;
    IBOutlet NSTextField *connectGroupMinDistanceNBox;
    IBOutlet NSTextField *connectGroupMaxDistanceNBox;
    IBOutlet NSButton *manualConnectButton;
    IBOutlet NSButton *connectGroupButton;
    IBOutlet NSButton *collisionTestButton;
    IBOutlet NSSlider *removeGroupsMinNumSlider;
    IBOutlet NSTextField *removeGroupsMinNumNBox;
    IBOutlet NSButton *removeGroupButton;
    IBOutlet NSButton *removeSingleSphereButton;
    IBOutlet NSButton *removeSingleCylinderButton;
    IBOutlet NSButton *removeAllSpheresButton;
    IBOutlet NSButton *removeAllCylinderButton;

    
    // Draw pane
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
    IBOutlet NSButton *showWireframeCheck;
    IBOutlet NSButton *showCollisionShapeCheck;
    IBOutlet NSButton *showCollisionDistanceCheck;
    IBOutlet NSButton *showReferenceBoxCheck;
    
    // Save pane
    IBOutlet NSButton *exportToCsvButton;
    IBOutlet NSButton *removeDuplicateButton;
}


- (void) setupDefault;
- (void) initializeParameters;


// Load Panel
- (IBAction)changeLoadModelResolutionSlider:(NSSlider *)sender;
- (IBAction)changeLoadModelResolutionNBox:(NSTextField *)sender;
- (IBAction)pushNoiseFilterSegment:(NSSegmentedControl *)sender;
- (IBAction)pushLoadCsvbutton:(NSButton *)sender;
- (IBAction)puchLoadModelDataButton:(NSButton *)sender;

// Shape
- (IBAction)changeSphereRadiusSlider:(NSSlider *)sender;
- (IBAction)changeSphereResolutionSlider:(NSSlider *)sender;
- (IBAction)changeSphereCollisionMarginSlider:(NSSlider *)sender;
- (IBAction)changeCylinderRadiusSlider:(NSSlider *)sender;
- (IBAction)changeCylinderResolutionSlider:(NSSlider *)sender;
- (IBAction)changeCylinderCollisionMarginSlider:(NSSlider *)sender;
- (IBAction)changeSphereRadiusNBox:(NSTextField *)sender;
- (IBAction)changeSphereResolutionNBox:(NSTextField *)sender;
- (IBAction)changeSphereCollisionMarginNBox:(NSTextField *)sender;
- (IBAction)changeCylinderRadiusNBox:(NSTextField *)sender;
- (IBAction)changeCylinderResolutionNBox:(NSTextField *)sender;
- (IBAction)changeCylinderCollisionMarginNBox:(NSTextField *)sender;


// Sculpt
- (IBAction)changeConnectRandomCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMaxDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMaxDistanceSlider:(NSSlider *)sender;
- (IBAction)changeRemoveGroupsMinNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMaxDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMaxDistanceNBox:(NSTextField *)sender;
- (IBAction)pushManualConnectButton:(NSButton *)sender;
- (IBAction)changeRemoveGroupsMinNumNBox:(NSTextField *)sender;
- (IBAction)pushConnectRandomButton:(NSButton *)sender;
- (IBAction)pushConnectGroupButton:(NSButton *)sender;
- (IBAction)pushCollisionTestButton:(NSButton *)sender;
- (IBAction)pushRemoveGroupsButton:(NSButton *)sender;
- (IBAction)pushRemoveSphereButton:(NSButton *)sender;
- (IBAction)pushRemoveCylinderButton:(NSButton *)sender;
- (IBAction)pushRemoveAllSpheres:(NSButton *)sender;
- (IBAction)pushRemoveAllCylinderButton:(NSButton *)sender;


// Draw Panel
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
- (IBAction)changeShowWireframeCheck:(NSButton *)sender;
- (IBAction)changeShowCollisionShapeCheck:(NSButton *)sender;
- (IBAction)changeShowCollisionDistanceCheck:(NSButton *)sender;
- (IBAction)changeShowReferenceBoxCheck:(NSButton *)sender;
- (IBAction)changeShowSphereCheck:(NSButton *)sender;
- (IBAction)changeShowCylinderCheck:(NSButton *)sender;


// Save
- (IBAction)pushRemoveDuplicateButton:(NSButton *)sender;
- (IBAction)pushExportToCsvButton:(NSButton *)sender;

@end
