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


@implementation MainWindowController

+ (void) setupDefault
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
    
    [MainWindowController initializeParameters];
}

+ (void) initializeParameters
{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
    
    //float fontSize          = [[defaults objectForKey:FONT_SIZE] floatValue];
}


- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    
    // read the value of the user default with key aKey
    // and return it in aColor
    NSColor * bg=nil;
    NSData *theData=[[NSUserDefaults standardUserDefaults] dataForKey:@"BackgroundColor"];
    if (theData != nil){
        bg =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [bgPickerNormal setColor:bg];
        testApp::bgNormal.set(bg.redComponent*255.0, bg.greenComponent*255.0, bg.blueComponent*255.0);

    }
        
    return self;
}



//- (IBAction)changeBackgroundColor:(NSColorWell *)sender {
//}

- (IBAction)changeSphereRadius:(NSSlider *)sender {
    
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


- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    // store Color data
    NSData *theData=[NSArchiver archivedDataWithRootObject:bgPickerNormal.color];
    [[NSUserDefaults standardUserDefaults] setObject:theData forKey:@"BackgroundColor"];
    return true;
}

@end
