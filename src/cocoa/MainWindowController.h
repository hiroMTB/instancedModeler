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
    IBOutlet NSSlider *sphereCollisionMarginSlider;
    
    IBOutlet NSSlider *cylinderRadiusSlider;
    IBOutlet NSSlider *cylinderResolutionSlider;
    IBOutlet NSSlider *cylinderCollisionMarginSlider;

    IBOutlet NSSlider *connectRandomCylinderNumSlider;
    IBOutlet NSSlider *connectRandomMinDistanceSlider;
    IBOutlet NSSlider *connectRandomMaxDistanceSlider;
    IBOutlet NSSlider *connectGroupCylinderNumSlider;
    IBOutlet NSSlider *connectGroupMinDistanceSlider;
    IBOutlet NSSlider *connectGroupMaxDistanceSlider;
    IBOutlet NSSlider *removeGroupsMinNumSlider;

    IBOutlet NSTextField *sphereRadiusNBox;
    IBOutlet NSTextField *sphereResolutionNBox;
    IBOutlet NSTextField *sphereCollisionMarginNBox;

    IBOutlet NSTextField *cylinderRadiusNBox;
    IBOutlet NSTextField *cylinderResolutionNBox;
    IBOutlet NSTextField *cylinderCollisionMarginNBox;
    
    IBOutlet NSTextField *connectRandomCylinderNumNBox;
    IBOutlet NSTextField *connectRandomMinDistanceNBox;
    IBOutlet NSTextField *connectRandomMaxDistanceNBox;
    IBOutlet NSTextField *connectGroupCylinderNumNBox;
    IBOutlet NSTextField *connectGroupMinDistanceNBox;
    IBOutlet NSTextField *connectGroupMaxDistanceNBox;
    IBOutlet NSTextField *removeGroupsMinNumNBox;
    
    IBOutlet NSButton *resetInstanceShapeButton;
    
    IBOutlet NSButton *connectRandomButton;
    IBOutlet NSButton *connectGroupButton;
    IBOutlet NSButton *collisionTestButton;
    IBOutlet NSButton *removeGroupButton;
    IBOutlet NSButton *removeDuplicateButton;

    IBOutlet NSButton *exportToCsvButton;
    IBOutlet NSButton *loadCsvButton;
    
    IBOutlet NSButton *LoadModelDataButton;
    IBOutlet NSButton *loadSphereCsvButton;
    IBOutlet NSButton *loadCylinderCsvButton;
    
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
    IBOutlet NSButton *showWireframeCheck;
    IBOutlet NSButton *showCollisionShapeCheck;
    IBOutlet NSButton *showCollisionDistanceCheck;
    IBOutlet NSButton *showReferenceBoxCheck;
    IBOutlet NSSlider *loadModelResolutionSlider;
    IBOutlet NSTextField *loadModelResolutionNBox;
    
    
    // under selector
    IBOutlet NSSlider *selectSpehreSlider;
    IBOutlet NSSlider *selectCylinderSlider;
    
    IBOutlet NSTextField *selectSphereNBox;
    IBOutlet NSTextField *selectCylinderNBox;
    
    IBOutlet NSButton *connetc1SpherePairButton;
    IBOutlet NSButton *removeSingleSphereButton;
    IBOutlet NSButton *removeSingleCylinderButton;
    IBOutlet NSButton *removeAllSpheresButton;
    IBOutlet NSButton *removeAllCylinderButton;
}

- (IBAction)changeSphereRadiusSlider:(NSSlider *)sender;
- (IBAction)changeSphereResolutionSlider:(NSSlider *)sender;
- (IBAction)changeSphereCollisionMarginSlider:(NSSlider *)sender;

- (IBAction)changeCylinderRadiusSlider:(NSSlider *)sender;
- (IBAction)changeCylinderResolutionSlider:(NSSlider *)sender;
- (IBAction)changeCylinderCollisionMarginSlider:(NSSlider *)sender;

- (IBAction)changeConnectRandomCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectRandomMaxDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupCylinderNumSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMinDistanceSlider:(NSSlider *)sender;
- (IBAction)changeConnectGroupMaxDistanceSlider:(NSSlider *)sender;

- (IBAction)changeRemoveGroupsMinNumSlider:(NSSlider *)sender;

- (IBAction)changeSphereRadiusNBox:(NSTextField *)sender;
- (IBAction)changeSphereResolutionNBox:(NSTextField *)sender;
- (IBAction)changeSphereCollisionMarginNBox:(NSTextField *)sender;

- (IBAction)changeCylinderRadiusNBox:(NSTextField *)sender;
- (IBAction)changeCylinderResolutionNBox:(NSTextField *)sender;
- (IBAction)changeCylinderCollisionMarginNBox:(NSTextField *)sender;

- (IBAction)changeConnectRandomCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectRandomMaxDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupCylinderNumNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMinDistanceNBox:(NSTextField *)sender;
- (IBAction)changeConnectGroupMaxDistanceNBox:(NSTextField *)sender;

- (IBAction)changeRemoveGroupsMinNumNBox:(NSTextField *)sender;

- (IBAction)pushResetInstanceShapeButton:(NSButton *)sender;
- (IBAction)pushConnectRandomButton:(NSButton *)sender;
- (IBAction)pushConnectGroupButton:(NSButton *)sender;
- (IBAction)pushCollisionTestButton:(NSButton *)sender;
- (IBAction)pushRemoveGroupsButton:(NSButton *)sender;
- (IBAction)pushRemoveDuplicateButton:(NSButton *)sender;

- (IBAction)pushExportToCsvButton:(NSButton *)sender;
- (IBAction)pushLoadCsvbutton:(NSButton *)sender;

- (IBAction)puchLoadModelDataButton:(NSButton *)sender;

- (IBAction)pushLoadSphereCsvButton:(NSButton *)sender;
- (IBAction)pushLoadCylinderCsvButton:(NSButton *)sender;


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

- (IBAction)changeLoadModelResolutionSlider:(NSSlider *)sender;
- (IBAction)changeLoadModelResolutionNBox:(NSTextField *)sender;

- (IBAction)pushNoiseFilterSegment:(NSSegmentedControl *)sender;
- (void) setupDefault;
- (void) initializeParameters;


// Bottom Control
- (IBAction)changeSelectSphereSlider:(NSSlider *)sender;
- (IBAction)changeSelectCylinderSlider:(NSSlider *)sender;
- (IBAction)changeSelectSphereNBox:(NSTextField *)sender;
- (IBAction)changeSelectCylinderNBox:(NSTextField *)sender;
- (IBAction)pushRemoveSphereButton:(NSButton *)sender;
- (IBAction)pushRemoveCylinderButton:(NSButton *)sender;

- (IBAction)pushRemoveAllSpheres:(NSButton *)sender;
- (IBAction)pushRemoveAllCylinderButton:(NSButton *)sender;


@end
