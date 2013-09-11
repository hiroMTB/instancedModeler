#import "AppDelegate.h"

@implementation AppDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)anotification
{
    if(mainWindowController == NULL){
        mainWindowController = [[MainWindowController alloc] initWithWindowNibName:@"MainWindow"];
    }

    [mainWindowController showWindow:self];
}

-(IBAction)showPreference:(id)sender
{
    if(prefController == NULL){
        prefController = [[PreferenceController alloc] initWithWindowNibName:@"Preference"];
    }
    
    [prefController showWindow:self];
}


@end
