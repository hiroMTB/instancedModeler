//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#import "rnUIController.h"
#include "ofMain.h"
#include "testApp.h"

@interface rnUIController ()

@end

@implementation rnUIController


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
    
    [rnUIController initializeParameters];

    
}

+ (void) initializeParameters
{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
    
    //float fontSize          = [[defaults objectForKey:FONT_SIZE] floatValue];
}

//- (id)initWithWindow:(NSWindow *)window
//{
//    self = [super initWithWindow:window];
//    
//    // read the value of the user default with key aKey
//    // and return it in aColor
//    NSColor * bg=nil;
//    NSData *theData=[[NSUserDefaults standardUserDefaults] dataForKey:@"BackgroundColor"];
//    if (theData != nil){
//        bg =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
//        [backgroundColorPicker setColor:bg];
//        testApp::setBackgroundColor(bg.redComponent*255.0, bg.greenComponent*255.0, bg.blueComponent*255.0);
//
//    }
//        
//    return self;
//}


//- (void)viewDidLoad
//{
//    [super windowDidLoad];
//}


//
//- (IBAction)changeBackgroundColor:(NSColorWell *)sender {
//    float r = sender.color.redComponent;
//    float g = sender.color.greenComponent;
//    float b = sender.color.blueComponent;
//    testApp::setBackgroundColor(r*255.0, g*255.0, b*255.0);
//}


- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    // store the value in aColor in user defaults
    // as the value for key aKey
    NSData *theData=[NSArchiver archivedDataWithRootObject:backgroundColorPicker.color];
    [[NSUserDefaults standardUserDefaults] setObject:theData forKey:@"BackgroundColor"];
//    [super applicationSohuldTerminate:self];
    return true;
}

@end
