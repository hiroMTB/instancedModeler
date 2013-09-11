#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window;
@synthesize prefController = _prefController;


- (void)applicationDidFinishLaunching:(NSNotification *)anotification
{
}

-(IBAction)showPreference:(id)sender
{
    if(!self.prefController)
        self.prefController = [[PreferenceController alloc] initWithWindowNibName:@"Preference"];
    
    [self.prefController showWindow:self];
}



- (void)dealloc
{
    [super dealloc];
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification{
    cout << "AppDelegate" << endl;
}

@end
