//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#import "MainWindowController.h"
#include "ofMain.h"
#include "testApp.h"

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
NSString *const cylinderRadius      = @"cylinderRadius";
NSString *const cylinderResolution  = @"cylinderResolution";

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

@implementation MainWindowController

- (void) setupDefault
{
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

- (void) initializeParameters
{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];

    testApp::SPHERE_RADIUS                  = [[defaults objectForKey:sphereRadius] floatValue];
    testApp::SPHERE_RESOLUTION              = [[defaults objectForKey:sphereResolution] intValue];
    testApp::CYLINDER_RADIUS                = [[defaults objectForKey:cylinderRadius] floatValue];
    testApp::CYLINDER_RESOLUTION            = [[defaults objectForKey:cylinderResolution] intValue];
    testApp::CONNECT_RANDOM_CYLINDER_NUM    = [[defaults objectForKey:connectRandomCylinderNum] intValue];
    testApp::CONNECT_RANDOM_MIN_DIST        = [[defaults objectForKey:connectRandomMinDistance] floatValue];
    testApp::CONNECT_RANDOM_MAX_DIST        = [[defaults objectForKey:connectRandomMaxDistance] floatValue];
    testApp::CONNECT_GROUP_CYLINDER_NUM     = [[defaults objectForKey:connectGroupCylinderNum] intValue];
    testApp::CONNECT_GROUP_MIN_DIST         = [[defaults objectForKey:connectGroupMinDistance] floatValue];
    testApp::CONNECT_GROUP_MAX_DIST         = [[defaults objectForKey:connectGroupMaxDistance] floatValue];
    testApp::COLLISION_MARGIN               = [[defaults objectForKey:collisionMargin] floatValue];
    testApp::REMOVE_GROUPS_MIN_NUM          = [[defaults objectForKey:removeGroupsMinNum] intValue];
    testApp::bgType                         = [[defaults objectForKey:bgType] intValue];
    
    [sphereRadiusNBox setFloatValue:testApp::SPHERE_RADIUS];
    [sphereResolutionNBox setIntValue:testApp::SPHERE_RESOLUTION];
    [cylinderRadiusNBox setFloatValue:testApp::CYLINDER_RADIUS];
    [cylinderResolutionNBox setIntValue:testApp::CYLINDER_RESOLUTION];
    [connectRandomCylinderNumNBox setIntValue:testApp::CONNECT_RANDOM_CYLINDER_NUM];
    [connectRandomMinDistanceNBox setFloatValue:testApp::CONNECT_RANDOM_MIN_DIST];
    [connectRandomMaxDistanceNBox setFloatValue:testApp::CONNECT_RANDOM_MAX_DIST];
    [connectGroupCylinderNumNBox setIntValue:testApp::CONNECT_GROUP_CYLINDER_NUM];
    [connectGroupMinDistanceNBox setFloatValue:testApp::CONNECT_GROUP_MIN_DIST];
    [connectGroupMaxDistanceNBox setFloatValue:testApp::CONNECT_GROUP_MAX_DIST];
    [collisionMarginNBox setFloatValue:testApp::COLLISION_MARGIN];
    [removeGroupsMinNumNBox setIntValue:testApp::REMOVE_GROUPS_MIN_NUM];
    
    testApp::DRAW_WIREFRAME                 = [[defaults objectForKey:showWireframe] boolValue];
    testApp::DRAW_COLLISION_SHAPE           = [[defaults objectForKey:showCollisionShape] boolValue];
    testApp::DRAW_COLLISION_DISTANCE        = [[defaults objectForKey:showCollisionDistance] boolValue];
    
    NSColor * color = nil;
    NSData *theData = nil;

    // shape color
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:sphereColor];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [colorSphere setColor:color];
        testApp::colorSphere.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:cylinderColor];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [colorCylinder setColor:color];
        testApp::colorCylinder.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }

    // background color
    theData = [[NSUserDefaults standardUserDefaults] dataForKey:bgNormal];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerNormal setColor:color];
        testApp::bgNormal.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgLinear0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerLinear0 setColor:color];
        testApp::bgLinear0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgLinear1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerLinear1 setColor:color];
        testApp::bgLinear1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgCircular0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerCircular0 setColor:color];
        testApp::bgCircular0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgCircular1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerCircular1 setColor:color];
        testApp::bgCircular1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgBar0];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerBar0 setColor:color];
        testApp::bgBar0.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    theData=[[NSUserDefaults standardUserDefaults] dataForKey:bgBar1];
    if (theData != nil){
        color =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerBar1 setColor:color];
        testApp::bgBar1.set(color.redComponent*255.0, color.greenComponent*255.0, color.blueComponent*255.0);
    }
    


}

-(void)awakeFromNib{
    [self setupDefault];
    [self initializeParameters];
}

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    return self;
}


- (IBAction)changeSphereRadiusSlider:(NSSlider *)sender{
    testApp::SPHERE_RADIUS = sender.floatValue;
    [sphereRadiusNBox setFloatValue:sender.floatValue];
}

- (IBAction)changeSphereResolutionSlider:(NSSlider *)sender{
    testApp::SPHERE_RESOLUTION = sender.intValue;
    [sphereResolutionNBox setIntValue:sender.intValue];
}

- (IBAction)changeCylinderRadiusSlider:(NSSlider *)sender{
    testApp::CYLINDER_RADIUS = sender.floatValue;
    [cylinderRadiusNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeCylinderResolutionSlider:(NSSlider *)sender{
    testApp::CYLINDER_RESOLUTION = sender.intValue;
    [cylinderResolutionNBox setIntValue:sender.intValue];
}
- (IBAction)changeConnectRandomCylinderNumSlider:(NSSlider *)sender{
    testApp::CONNECT_RANDOM_CYLINDER_NUM = sender.intValue;
    [connectRandomCylinderNumNBox setIntValue:sender.intValue];
}

- (IBAction)changeConnectRandomMinDistanceSlider:(NSSlider *)sender{
    testApp::CONNECT_RANDOM_MIN_DIST = sender.floatValue;
    [connectRandomMinDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectRandomMaxDistanceSlider:(NSSlider *)sender{
    testApp::CONNECT_RANDOM_MAX_DIST = sender.floatValue;
    [connectRandomMaxDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectGroupCylinderNumSlider:(NSSlider *)sender{
    testApp::CONNECT_GROUP_CYLINDER_NUM = sender.intValue;
    [connectGroupCylinderNumNBox setIntValue:sender.intValue];
}
- (IBAction)changeConnectGroupMinDistanceSlider:(NSSlider *)sender{
    testApp::CONNECT_GROUP_MIN_DIST = sender.floatValue;
    [connectGroupMinDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeConnectGroupMaxDistanceSlider:(NSSlider *)sender{
    testApp::CONNECT_GROUP_MAX_DIST = sender.floatValue;
    [connectGroupMaxDistanceNBox setFloatValue:sender.floatValue];
}
- (IBAction)changeCollisionMarginSlider:(NSSlider *)sender{
    testApp::COLLISION_MARGIN = sender.floatValue;
    [collisionMarginNBox setFloatValue:sender.floatValue];
}

- (IBAction)changeRemoveGroupsMinNumSlider:(NSSlider *)sender{
    testApp::REMOVE_GROUPS_MIN_NUM = sender.intValue;
    [removeGroupsMinNumNBox setIntValue:sender.intValue];
}

//
//  NUMBER BOX
//
- (IBAction)changeSphereRadiusNBox:(NSTextField *)sender{
    testApp::SPHERE_RADIUS = sender.floatValue;
    [sphereRadiusSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:sphereRadius];
}
- (IBAction)changeSphereResolutionNBox:(NSTextField *)sender{
    testApp::SPHERE_RESOLUTION = sender.intValue;
    [sphereResolutionSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:sphereResolution];
}
- (IBAction)changeCylinderRadiusNBox:(NSTextField *)sender{
    testApp::CYLINDER_RADIUS = sender.floatValue;
    [cylinderRadiusSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:cylinderRadius];
}
- (IBAction)changeCylinderResolutionNBox:(NSTextField *)sender{
    testApp::CYLINDER_RESOLUTION = sender.intValue;
    [cylinderResolutionSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:cylinderResolution];
}
- (IBAction)changeConnectRandomCylinderNumNBox:(NSTextField *)sender{
    testApp::CONNECT_RANDOM_CYLINDER_NUM = sender.intValue;
    [connectRandomCylinderNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:connectRandomCylinderNum];
}
- (IBAction)changeConnectRandomMinDistanceNBox:(NSTextField *)sender{
    testApp::CONNECT_RANDOM_MIN_DIST = sender.floatValue;
    [connectRandomMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectRandomMinDistance];
}
- (IBAction)changeConnectRandomMaxDistanceNBox:(NSTextField *)sender{
    testApp::CONNECT_RANDOM_MAX_DIST = sender.floatValue;
    [connectRandomMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectRandomMaxDistance];
}
- (IBAction)changeConnectGroupCylinderNumNBox:(NSTextField *)sender{
    testApp::CONNECT_GROUP_CYLINDER_NUM  = sender.intValue;
    [connectGroupCylinderNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:connectGroupCylinderNum];
}
- (IBAction)changeConnectGroupMinDistanceNBox:(NSTextField *)sender{
    testApp::CONNECT_GROUP_MIN_DIST = sender.floatValue;
    [connectGroupMinDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectGroupMinDistance];
}
- (IBAction)changeConnectGroupMaxDistanceNBox:(NSTextField *)sender{
    testApp::CONNECT_GROUP_MAX_DIST = sender.floatValue;
    [connectGroupMaxDistanceSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:connectGroupMaxDistance];
}
- (IBAction)changeCollisionMarginNBox:(NSTextField *)sender{
    testApp::COLLISION_MARGIN = sender.floatValue;
    [collisionMarginSlider setFloatValue:sender.floatValue];
    [[NSUserDefaults standardUserDefaults] setFloat:sender.floatValue forKey:collisionMargin];
}

- (IBAction)changeRemoveGroupsMinNumNBox:(NSTextField *)sender{
    testApp::REMOVE_GROUPS_MIN_NUM = sender.intValue;
    [removeGroupsMinNumSlider setIntValue:sender.intValue];
    [[NSUserDefaults standardUserDefaults] setInteger:sender.intValue forKey:removeGroupsMinNum];
}

- (IBAction)pushResetInstanceShapeButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::RESET_INSTSANCE_SHAPE;
}

- (IBAction)pushConnectRandomButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::CONNECT_RANDOM;
}

- (IBAction)pushConnectGroupButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::CONNECT_GROUP;
}

- (IBAction)pushCollisionTestButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::COLLISION_TEST;
}

- (IBAction)pushRemoveGroupsButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::REMOVE_GROUPS;
}

- (IBAction)pushRemoveDuplicateButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::REMOVE_DUPLICATE;
}

- (IBAction)pushRemoveAllSpheres:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::REMOVE_ALL_SPHERES;
}

- (IBAction)pushRemoveAllCylinderButton:(NSButton *)sender {
    testApp::CURRENT_PROCESS = testApp::REMOVE_ALL_CYLINDERS;
}

- (IBAction)pushExportToCsvButton:(NSButton *)sender {
//    testApp::CURRENT_PROCESS = testApp::SAVE_CSV;
    
    NSSavePanel* savePanel = [NSSavePanel savePanel];
    
    [savePanel setMessage:@"Please input directory name (data will be stored in Sphere.csv and Cylidner.csv)"];
    [savePanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if(result == NSOKButton){
            string path = std::string([[[savePanel URL] path] UTF8String]);
            testApp::get()->saveCsvData(path);
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
                testApp::get()->loadCsvData(path);
            }
        }
    }];
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
                testApp::posModelPath_P = path;
                testApp::CURRENT_PROCESS = testApp::LOAD_MODEL_DATA;
                testApp::get()->loadModelData();
            }
        }
    }];
}

- (IBAction)pushLoadSphereCsvButton:(NSButton *)sender {
}

- (IBAction)pushLoadCylinderCsvButton:(NSButton *)sender {
}


- (IBAction)changeColorSphere:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::colorSphere.set(r*255.0, g*255.0, b*255.0);
}

- (IBAction)changeColorCylinder:(NSColorWell *)sender{
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::colorCylinder.set(r*255.0, g*255.0, b*255.0);
}

- (IBAction)changeBgType:(NSMatrix *)sender {
    testApp::bgType = sender.selectedTag;
}

- (IBAction)changeBgNormal:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgNormal.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 0;
    [bgTypeMatrix selectCellWithTag:0];
}

- (IBAction)changeBgLinear0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgLinear0.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 1;
    [bgTypeMatrix selectCellWithTag:1];
}

- (IBAction)changeBgLinear1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgLinear1.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 1;
    [bgTypeMatrix selectCellWithTag:1];
}

- (IBAction)changeBgCircular0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgCircular0.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 2;
    [bgTypeMatrix selectCellWithTag:2];
}

- (IBAction)changeBgCircular1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgCircular1.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 2;
    [bgTypeMatrix selectCellWithTag:2];
}

- (IBAction)changeBgBar0:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgBar0.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 3;
    [bgTypeMatrix selectCellWithTag:3];
}

- (IBAction)changeBgBar1:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::bgBar1.set(r*255.0, g*255.0, b*255.0);
    testApp::bgType = 3;
    [bgTypeMatrix selectCellWithTag:3];
}

- (IBAction)changeShowWireframeCheck:(NSButton *)sender {
    testApp::DRAW_WIREFRAME = (bool)sender.state;
}

- (IBAction)changeShowCollisionShapeCheck:(NSButton *)sender {
    testApp::DRAW_COLLISION_SHAPE = (bool)sender.state;
}

- (IBAction)changeShowCollisionDistanceCheck:(NSButton *)sender {
    testApp::DRAW_COLLISION_DISTANCE = (bool)sender.state;
}

- (IBAction)changeShowReferenceBoxCheck:(NSButton *)sender {
    testApp::DRAW_REFERENCE_BOX = (bool)sender.state;
}


// ??? can not call this func.
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    cout << "applicationShouldTerminate" << endl;
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    
    [prefs setFloat:sphereRadiusSlider.floatValue forKey:sphereRadius];
    [prefs synchronize];
    return true;
}

@end
