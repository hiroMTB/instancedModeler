#include "ofMain.h"

#import "AppDelegate.h"

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)anotification
{
    if(mainWindowController == NULL){
        mainWindowController = [[MainWindowController alloc] initWithWindowNibName:@"MainWindow"];
    }

    [mainWindowController showWindow:self];
}


@end
