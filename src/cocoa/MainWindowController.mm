//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#include "ofMain.h"
#import "MainWindowController.h"
#include "rnApp.h"

NSString *const sphereColor         = @"sphereColor";
NSString *const cylinderColor		= @"cylinderColor";

NSString *const bgType            = @"bgType";
NSString *const bgNormal            = @"bgNormal";
NSString *const bgLinear0           = @"bgLinear0";
NSString *const bgLinear1           = @"bgLinear1";
NSString *const bgCircular0         = @"bgCircular0";
NSString *const bgCircular1         = @"bgCircular1";
NSString *const bgBar0              = @"bgBar0";
NSString *const bgBar1              = @"bgBar1";
NSString *const showWireframe       = @"showWireframe";
NSString *const showCollisionShape  = @"showCollisionShape";
NSString *const showCollisionDistance = @"showCollisionDistance";

NSString *const sphereRadius        = @"sphereRadius";
NSString *const sphereResolution    = @"sphereResolution";
NSString *const sphereCollisionMargin  = @"sphereCollisionMargin";

NSString *const cylinderRadius      = @"cylinderRadius";
NSString *const cylinderResolution  = @"cylinderResolution";
NSString *const cylinderCollisionMargin  = @"cylinderCollisionMargin";

NSString *const connectRandomCylinderNum = @"connectRandomCylinderNum";
NSString *const connectRandomMinDistance = @"connectRandomMinDistance";
NSString *const connectRandomMaxDistance = @"connectRandomMaxDistance";
NSString *const connectGroupCylinderNum  = @"connectGroupCylinderNum";
NSString *const connectGroupMinDistance  = @"connectGroupMinDistance";
NSString *const connectGroupMaxDistance  = @"connectGroupMaxDistance";

NSString *const collisionMargin          = @"collisionMargin";
NSString *const removeGroupsMinNum          = @"removeGroupsMinNum";

// button
NSString *const resetInstanceShape      = @"resetInstanceShape";
NSString *const connectRandom           = @"connectRandom";
NSString *const connectGroup            = @"connectGroup";
NSString *const removeGroups            = @"removeGroups";
NSString *const removeDuplicate         = @"removeDuplicate";
NSString *const removeAllCylinders       = @"removeAllCylinders";
NSString *const removeAllSpheres       = @"removeAllSpheres";

NSString *const loadModelResolution = @"loadModelResolution";


@implementation MainWindowController

- (void) setupDefault{
    NSString *userDefaultsValuesPath;
    NSDictionary *userDefaultsValuesDict;
    NSDictionary *initialValuesDict;
    NSArray *resettableUserDefaultsKeys;
    
    // load the default values for the user defaults
    userDefaultsValuesPath=[[NSBundle mainBundle] pathForResource:@"UserDefaults"
														   ofType:@"plist"];
    userDefaultsValuesDict=[NSDictionary dictionaryWithContentsOfFile:userDefaultsValuesPath];
    
    // set them in the standard user defaults
    [[NSUserDefaults standardUserDefaults] registerDefaults:userDefaultsValuesDict];
    
}

- (void) initializeParameters{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];

    rnApp::SPHERE_RADIUS                  = [[defaults objectForKey:sphereRadius] floatValue];
    rnApp::SPHERE_RESOLUTION              = [[defaults objectForKey:sphereResolution] intValue];
    rnApp::SPHERE_COLLISION_MARGIN        = [[defaults objectForKey:sphereCollisionMargin] floatValue];
    rnApp::CYLINDER_RADIUS                = [[defaults objectForKey:cylinderRadius] floatValue];
    rnApp::CYLINDER_RESOLUTION            = [[defaults objectForKey:cylinderResolution] intValue];
    rnApp::CYLINDER_COLLISION_MARGIN      = [[defaults objectForKey:cylinderCollisionMargin] floatValue];

    rnApp::CONNECT_RANDOM_CYLINDER_NUM    = [[defaults objectForKey:connectRandomCylinderNum] intValue];
    rnApp::CONNECT_RANDOM_MIN_DIST        = [[defaults objectForKey:connectRandomMinDistance] floatValue];
    rnApp::CONNECT_RANDOM_MAX_DIST        = [[defaults objectForKey:connectRandomMaxDistance] floatValue];
    rnApp::CONNECT_GROUP_CYLINDER_NUM     = [[defaults objectForKey:connectGroupCylinderNum] intValue];
    rnApp::CONNECT_GROUP_MIN_DIST         = [[defaults objectForKey:connectGroupMinDistance] floatValue];
    rnApp::CONNECT_GROUP_MAX_DIST         = [[defaults objectForKey:connectGroupMaxDistance] floatValue];
    rnApp::REMOVE_GROUPS_MIN_NUM          = [[defaults objectForKey:removeGroupsMinNum] intValue];
    rnApp::bgType                         = [[defaults objectForKey:bgType] intValue];
    rnApp::LOAD_MODEL_RESOLUTION          = [[defaults objectForKey:loadModelResolution] intValue];

    [sphereRadiusNBox setFloatValue:rnApp::SPHERE_RADIUS];
    [sphereResolutionNBox setIntValue:rnApp::SPHERE_RESOLUTION];
    [sphereCollisionMarginNBox setFloatValue:rnApp::SPHERE_COLLISION_MARGIN];

    [cylinderRadiusNBox setFloatValue:rnApp::CYLINDER_RADIUS];
    [cylinderResolutionNBox setIntValue:rnApp::CYLINDER_RESOLUTION];
    [cylinderCollisionMarginNBox setFloatValue:rnApp::CYLINDER_COLLISION_MARGIN];

    [connectRandomCylinderNumNBox setIntValue:rnApp::CONNECT_RANDOM_CYLINDER_NUM];
    [connectRandomMinDistanceNBox setFloatValue:rnApp::CONNECT_RANDOM_MIN_DIST];
    [connectRandomMaxDistanceNBox setFloatValue:rnApp::CONNECT_RANDOM_MAX_DIST];
    [connectGroupCylinderNumNBox setIntValue:rnApp::CONNECT_GROUP_CYLINDER_NUM];
    [connectGroupMinDistanceNBox setFloatValue:rnApp::CONNECT_GROUP_MIN_DIST];
    [connectGroupMaxDistanceNBox setFloatValue:rnApp::CONNECT_GROUP_MAX_DIST];
    [removeGroupsMinNumNBox setIntValue:rnApp::REMOVE_GROUPS_MIN_NUM];
    [loadModelResolutionNBox setIntValue:rnApp::LOAD_MODEL_RESOLUTION];
    
    rnApp::DRAW_WIREFRAME                 = [[defaults objectForKey:showWireframe] boolValue];
    rnApp::DRAW_COLLISION_SHAPE           = [[defaults objectForKey:showCollisionShape] boolValue];
    rnApp::DRAW_COLLISION_DISTANCE        = [[defaults objectForKey:showCollisionDistance] boolValue];
    
    NSColor * color = nil;
    NSData *theData = nil;

    // shape color
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:sphereColor];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [colorSphere setColor:color];
        rnApp::colorSphere.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:cylinderColor];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [colorCylinder setColor:color];
        rnApp::colorCylinder.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }

    // background color
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:bgNormal];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerNormal setColor:color];
        rnApp::bgNormal.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgLinear0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerLinear0 setColor:color];
        rnApp::bgLinear0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgLinear1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerLinear1 setColor:color];
        rnApp::bgLinear1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgCircular0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerCircular0 setColor:color];
        rnApp::bgCircular0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgCircular1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerCircular1 setColor:color];
        rnApp::bgCircular1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgBar0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerBar0 setColor:color];
        rnApp::bgBar0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgBar1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerBar1 setColor:color];
        rnApp::bgBar1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
}

-(void)awakeFromNib{
    [self setupDefault];
    [self initializeParameters];
}

- (id)initWithWindow:(NSWindow *)window{
    self = [super initWithWindow:window];
    return self;
}

- (IBAction)changeSphereRadiusSlider:(NSSlider *)sender{
    rnApp::SPHERE_RADIUS = sender.floatValue;
    [sphereRadiusNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeSphereResolutionSlider:(NSSlider *)sender{
    rnApp::SPHERE_RESOLUTION = sender.intValue;
    [sphereResolutionNBox setIntValue:sender.intValue];
}
- (IBAction)changeSphereCollisionMarginSlider:(NSSlider *)sender {
    rnApp::SPHERE_COLLISION_MARGIN = sender.floatValue;
    [sphereCollisionMarginNBox setFloatValue:sender.floatValue];
}

- (IBAction)changeCylinderRadiusSlider:(NSSlider *)sender{
    rnApp::CYLINDER_RADIUS = sender.floatValue;
    [cylinderRadiusNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeCylinderResolutionSlider:(NSSlider *)sender{
    rnApp::CYLINDER_RESOLUTION = sender.intValue;
    [cylinderResolutionNBox setIntValue:sender.intValue];
}
- (IBAction)changeCylinderCollisionMarginSlider:(NSSlider *)sender {
    rnApp::CYLINDER_COLLISION_MARGIN = sender.floatValue;
    [cylinderCollisionMarginNBox setFloatValue:sender.floatValue];
}

- (IBAction)changeConnectRandomCylinderNumSlider:(NSSlider *)sender{
    rnApp::CONNECT_RANDOM_CYLINDER_NUM = sender.intValue;
    [connectRandomCylinderNumNBox setIntValue:sender.intValue];
}
- (IBAction)changeConnectRandomMinDistanceSlider:(NSSlider *)sender{
    rnApp::CONNECT_RANDOM_MIN_DIST = sender.floatValue;
    [connectRandomMinDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectRandomMaxDistanceSlider:(NSSlider *)sender{
    rnApp::CONNECT_RANDOM_MAX_DIST = sender.floatValue;
    [connectRandomMaxDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectGroupCylinderNumSlider:(NSSlider *)sender{
    rnApp::CONNECT_GROUP_CYLINDER_NUM = sender.intValue;
    [connectGroupCylinderNumNBox setIntValue:sender.intValue];
}
- (IBAction)changeConnectGroupMinDistanceSlider:(NSSlider *)sender{
    rnApp::CONNECT_GROUP_MIN_DIST = sender.floatValue;
    [connectGroupMinDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectGroupMaxDistanceSlider:(NSSlider *)sender{
    rnApp::CONNECT_GROUP_MAX_DIST = sender.floatValue;
    [connectGroupMaxDistanceNBox setFloatValue:sender.floatValue];
}

- (IBAction)changeRemoveGroupsMinNumSlider:(NSSlider *)sender{
    rnApp::REMOVE_GROUPS_MIN_NUM = sender.intValue;
    [removeGroupsMinNumNBox setIntValue:sender.intValue];
}

//
//  NUMBER BOX
//
- (IBAction)changeSphereRadiusNBox:(NSTextField *)sender{
    rnApp::SPHERE_RADIUS = sender.floatValue;
    [sphereRadiusSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:sphereRadius];
}
- (IBAction)changeSphereResolutionNBox:(NSTextField *)sender{
    rnApp::SPHERE_RESOLUTION = sender.intValue;
    [sphereResolutionSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:sphereResolution];
}
- (IBAction)changeSphereCollisionMarginNBox:(NSTextField *)sender {
    rnApp::SPHERE_COLLISION_MARGIN = sender.floatValue;
    [sphereCollisionMarginSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:sphereCollisionMargin];
}

- (IBAction)changeCylinderRadiusNBox:(NSTextField *)sender{
    rnApp::CYLINDER_RADIUS = sender.floatValue;
    [cylinderRadiusSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:cylinderRadius];
}
- (IBAction)changeCylinderResolutionNBox:(NSTextField *)sender{
    rnApp::CYLINDER_RESOLUTION = sender.intValue;
    [cylinderResolutionSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:cylinderResolution];
}
- (IBAction)changeCylinderCollisionMarginNBox:(NSTextField *)sender {
    rnApp::CYLINDER_COLLISION_MARGIN = sender.floatValue;
    [cylinderCollisionMarginSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:cylinderCollisionMargin];
}

- (IBAction)changeConnectRandomCylinderNumNBox:(NSTextField *)sender{
    rnApp::CONNECT_RANDOM_CYLINDER_NUM = sender.intValue;
    [connectRandomCylinderNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:connectRandomCylinderNum];
}
- (IBAction)changeConnectRandomMinDistanceNBox:(NSTextField *)sender{
    rnApp::CONNECT_RANDOM_MIN_DIST = sender.floatValue;
    [connectRandomMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectRandomMinDistance];
}
- (IBAction)changeConnectRandomMaxDistanceNBox:(NSTextField *)sender{
    rnApp::CONNECT_RANDOM_MAX_DIST = sender.floatValue;
    [connectRandomMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectRandomMaxDistance];
}
- (IBAction)changeConnectGroupCylinderNumNBox:(NSTextField *)sender{
    rnApp::CONNECT_GROUP_CYLINDER_NUM  = sender.intValue;
    [connectGroupCylinderNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:connectGroupCylinderNum];
}
- (IBAction)changeConnectGroupMinDistanceNBox:(NSTextField *)sender{
    rnApp::CONNECT_GROUP_MIN_DIST = sender.floatValue;
    [connectGroupMinDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectGroupMinDistance];
}
- (IBAction)changeConnectGroupMaxDistanceNBox:(NSTextField *)sender{
    rnApp::CONNECT_GROUP_MAX_DIST = sender.floatValue;
    [connectGroupMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectGroupMaxDistance];
}

- (IBAction)changeManualConnectSphereASlider:(NSSlider *)sender {
    int n = sender.intValue;
    [manualConnectSphereANBox setIntegerValue:n];

    // change selector as well
    [selectSphereNBox setIntValue:n];
    rnApp::get()->spheres.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)changeManualConnectSphereBSlider:(NSSlider *)sender {
    int n = sender.intValue;
    [manualConnectSphereBNBox setIntegerValue:n];

    
    // change selector as well
    [selectSphereNBox setIntValue:n];
    rnApp::get()->spheres.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)changeManualConnectSphereANBox:(NSTextField *)sender {
    int n = sender.intValue;
    [manualConnectSphereASlider setIntValue:n];
}

- (IBAction)changeManualConnectSphereBNBox:(NSTextField *)sender {
    int n = sender.intValue;
    [manualConnectSphereBSlider setIntValue:n];
}

- (IBAction)pushManualConnectButton:(NSButton *)sender {
    int idA = manualConnectSphereANBox.intValue;
    int idB = manualConnectSphereBNBox.intValue;
    rnApp::get()->connectInstance(idA, idB);
    [self updateUIMaxInstance];
}

- (IBAction)changeRemoveGroupsMinNumNBox:(NSTextField *)sender{
    rnApp::REMOVE_GROUPS_MIN_NUM = sender.intValue;
    [removeGroupsMinNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:removeGroupsMinNum];
}

- (IBAction)pushResetInstanceShapeButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::RESET_INSTSANCE_SHAPE;
}

- (IBAction)pushConnectRandomButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::CONNECT_RANDOM;
}

- (IBAction)pushConnectGroupButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::CONNECT_GROUP;
}

- (IBAction)pushCollisionTestButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::COLLISION_TEST;
}

- (IBAction)pushRemoveGroupsButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::REMOVE_GROUPS;
}

- (IBAction)pushRemoveDuplicateButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::REMOVE_DUPLICATE;
}

- (IBAction)pushRemoveAllSpheres:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::REMOVE_ALL_SPHERES;
}

- (IBAction)pushRemoveAllCylinderButton:(NSButton *)sender {
    rnApp::CURRENT_PROCESS = rnApp::REMOVE_ALL_CYLINDERS;
}

- (IBAction)pushExportToCsvButton:(NSButton *)sender {
//    rnApp::CURRENT_PROCESS = rnApp::SAVE_CSV;
    
    NSSavePanel* savePanel = [NSSavePanel savePanel];
    
    [savePanel setMessage:@"Please input directory name (data will be stored in Sphere.csv and Cylidner.csv)"];
    [savePanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if(result == NSOKButton){
            string path = std::string([[[savePanel URL] path] UTF8String]);
            rnApp::get()->saveCsvData(path);
        }
    }];
    
}

- (IBAction)pushLoadCsvbutton:(NSButton *)sender {
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];

	NSArray* fileTypes = [NSArray arrayWithObjects:@"csv", nil];
	[openPanel setAllowsMultipleSelection:NO];
    [openPanel setAllowedFileTypes:fileTypes];
	[openPanel setMessage:@"Select .csv file"];
    [openPanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if (result == NSOKButton){
            if ([[openPanel URL] isFileURL]){
                string path = std::string([[[openPanel URL] path] UTF8String]);
                rnApp::get()->loadCsvData(path);
            }
        }
    }];
    
   [self updateUIMaxInstance];
}

- (IBAction)puchLoadModelDataButton:(NSButton *)sender {
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
	
	NSArray* fileTypes = [NSArray arrayWithObjects:@"ply", nil];
	[openPanel setAllowsMultipleSelection:NO];
	[openPanel setMessage:@"Select .ply file for original particle position"];
    [openPanel setAllowedFileTypes:fileTypes];
//    [openPanel setDirectoryURL:[NSURL fileURLWithPath:@"/Library/Desktop Pictures/"]];
    [openPanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        
        if (result == NSOKButton)
        {
            if ([[openPanel URL] isFileURL]){
                //[self configureImage:[[openPanel URL] path]];
                string path = std::string([[[openPanel URL] path] UTF8String]);
                rnApp::posModelPath_P = path;
                rnApp::CURRENT_PROCESS = rnApp::LOAD_MODEL_DATA;
                rnApp::get()->loadModelData();
            }
        }
    }];
}

- (IBAction)changeColorSphere:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::colorSphere.set(r*255.0, g*255.0, b*255.0);
}

- (IBAction)changeColorCylinder:(NSColorWell *)sender{
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::colorCylinder.set(r*255.0, g*255.0, b*255.0);
}

- (IBAction)changeBgType:(NSMatrix *)sender {
    rnApp::bgType = sender.selectedTag;
}

- (IBAction)changeBgNormal:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgNormal.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 0;
    [bgTypeMatrix selectCellWithTag:0];
}

- (IBAction)changeBgLinear0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgLinear0.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 1;
    [bgTypeMatrix selectCellWithTag:1];
}

- (IBAction)changeBgLinear1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgLinear1.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 1;
    [bgTypeMatrix selectCellWithTag:1];
}

- (IBAction)changeBgCircular0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgCircular0.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 2;
    [bgTypeMatrix selectCellWithTag:2];
}

- (IBAction)changeBgCircular1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgCircular1.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 2;
    [bgTypeMatrix selectCellWithTag:2];
}

- (IBAction)changeBgBar0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgBar0.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 3;
    [bgTypeMatrix selectCellWithTag:3];
}

- (IBAction)changeBgBar1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    rnApp::bgBar1.set(r*255.0, g*255.0, b*255.0);
    rnApp::bgType = 3;
    [bgTypeMatrix selectCellWithTag:3];
}

- (IBAction)changeShowWireframeCheck:(NSButton *)sender {
    rnApp::DRAW_WIREFRAME = (bool)sender.state;
}

- (IBAction)changeShowCollisionShapeCheck:(NSButton *)sender {
    rnApp::DRAW_COLLISION_SHAPE = (bool)sender.state;
}

- (IBAction)changeShowCollisionDistanceCheck:(NSButton *)sender {
    rnApp::DRAW_COLLISION_DISTANCE = (bool)sender.state;
}

- (IBAction)changeShowReferenceBoxCheck:(NSButton *)sender {
    rnApp::DRAW_REFERENCE_BOX = (bool)sender.state;
}

- (IBAction)changeLoadModelResolutionSlider:(NSSlider *)sender {
    rnApp::LOAD_MODEL_RESOLUTION = sender.intValue;
    [loadModelResolutionNBox setIntValue:sender.intValue];
}

- (IBAction)changeLoadModelResolutionNBox:(NSTextField *)sender {
    rnApp::LOAD_MODEL_RESOLUTION = sender.intValue;
    [loadModelResolutionSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:loadModelResolution];
}

- (IBAction)changeSelectSphereSlider:(NSSlider *)sender {
    int n = sender.intValue;
    [selectSphereNBox setIntValue:n];
    rnApp::get()->spheres.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)changeSelectCylinderSlider:(NSSlider *)sender {
    int n = sender.intValue;
    [selectCylinderNBox setIntValue:n];
    rnApp::get()->cylinders.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)changeSelectSphereNBox:(NSTextField *)sender {
    int n = sender.intValue;
    [selectSpehreSlider setIntValue:n];
    rnApp::get()->spheres.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)changeSelectCylinderNBox:(NSTextField *)sender {
    int n = sender.intValue;
    [selectCylinderSlider setIntValue:n];
    rnApp::get()->cylinders.selectInstance(n);
    [self updateUIMaxInstance];
}

- (IBAction)pushRemoveSphereButton:(NSButton *)sender {
    rnApp::get()->spheres.removeSelectedInstance();
}

- (IBAction)pushRemoveCylinderButton:(NSButton *)sender {
    rnApp::get()->cylinders.removeSelectedInstance();
}

- (IBAction)pushNoiseFilterSegment:(NSSegmentedControl *)sender {
    rnApp::LOAD_MODEL_WITH_NOISE_FILTER = (bool)sender.selectedSegment;
    cout << rnApp::LOAD_MODEL_WITH_NOISE_FILTER << endl;
}


// ??? can not call this func.
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    cout << "applicationShouldTerminate" << endl;
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    
    [prefs setFloat:sphereRadiusSlider.floatValue forKey:sphereRadius];
    [prefs synchronize];
    return true;
}

- (void) updateUIMaxInstance{
    int maxS = rnApp::get()->spheres.getInstanceNum() - 1;
    int maxC = rnApp::get()->cylinders.getInstanceNum() - 1;

    [selectSpehreSlider setMaxValue:maxS];
    [selectCylinderSlider setMaxValue:maxC];
    [manualConnectSphereASlider setMaxValue:maxS];
    [manualConnectSphereBSlider setMaxValue:maxS];
    
}

@end
