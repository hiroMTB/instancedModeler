//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#include "ofMain.h"

#import "PreferenceController.h"
#include "rnApp.h"

@interface PreferenceController ()

@end

@implementation PreferenceController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    [self setPreferencePaneNotification];
    
    return self;
}


- (void)windowDidLoad
{
    [super windowDidLoad];
}


- (void)setPreferencePaneNotification{
    if (self) {
    }
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context{
}


-(IBAction)closePreference:(id)sender
{
    [self close];
}
@end
