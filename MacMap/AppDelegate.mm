//----------------------------------------------------------------------------
// File : AppDelegate.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base Application Class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// See the LICENSE.txt file for more information.
//
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
// 05/11/2018 creation.
//----------------------------------------------------------------------------
#import "AppDelegate.h"
#import "bMacMapApp.h"
#import <loader/loader.h>
#import <mox_intf/bStdAlert.h>
#import <MacMapSuite/bTrace.h>
#import <unistd.h>

// ---------------------------------------------------------------------------
//
// -----------
@implementation MacMapApplication
// ---------------------------------------------------------------------------
// 
// -----------
-(void)setMainMenu:(NSMenu*)aMenu{
    [super setMainMenu:aMenu];
    _willQuit=NO;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)setWillQuit{
    _willQuit=YES;
}

// ---------------------------------------------------------------------------
//
// -----------
-(BOOL)willQuit{
    return _willQuit;
}
// ---------------------------------------------------------------------------
//
// -----------
@end

// ---------------------------------------------------------------------------
//
// -----------
@implementation AppDelegate

// ---------------------------------------------------------------------------
//
// -----------
-(void)idle{
//_bTrace_("[AppDelegate idle]", false);
//_tm_("");
bMacMapApp*	gapp=(bMacMapApp*)bMacMapApp::instance();
    if(gapp->document()){
        if(gapp->mapIntf()){
            gapp->mapIntf()->idle();
        }
        gapp->xmapMgr()->idle();
        gapp->xboxMgr()->idle();
        gapp->toolMgr()->idle();
        gapp->vdefMgr()->idle();
        gapp->geogMgr()->idle();
        gapp->calcMgr()->idle();
        gapp->varMgr()->idle();
        gapp->macroMgr()->idle();
        gapp->scriptMgr()->idle();
    }
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)applicationDidFinishLaunching:(NSNotification *)aNotification {
_bTrace_("[AppDelegate applicationDidFinishLaunching]", true);
bMacMapApp* gapp=(bMacMapApp*)bMacMapApp::instance("MacMap");
OSStatus	status=gapp->run();
NSTimer*    tmr=[NSTimer scheduledTimerWithTimeInterval:0
                                                 target:self
                                               selector:@selector(idle)
                                               userInfo:nil
                                                repeats:YES];
#pragma unused(status)
#pragma unused(tmr)

// Dispatch NSApplicationDefined events for MacMap events
NSEvent*(^handler)(NSEvent*)=^(NSEvent *evt){
NSEvent*    res=evt;
        if([evt subtype]==NSEventSubtypeMacMapCommand){
            gapp->dispatch_cmd((UInt32)[evt data1]);
        }
        else{
            gapp->xmapMgr()->event(evt);
            gapp->xboxMgr()->event(evt);
            gapp->toolMgr()->event(evt);
            gapp->vdefMgr()->event(evt);
            gapp->geogMgr()->event(evt);
            gapp->calcMgr()->event(evt);
            gapp->varMgr()->event(evt);
            gapp->macroMgr()->event(evt);
            gapp->scriptMgr()->event(evt);
        }
        return res;
    };
    [NSEvent addLocalMonitorForEventsMatchingMask:NSApplicationDefinedMask handler:handler];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)applicationWillTerminate:(NSNotification *)aNotification {
MacMapApplication* a=NSApp;
    [a setWillQuit];
}

// ---------------------------------------------------------------------------
//
// -----------
-(BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender{
    return NO;
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)about:(id)sender{
_bTrace_("[AppDelegate about]", true);
bMacMapApp*	gapp=(bMacMapApp*)bMacMapApp::instance("MacMap");
    Loader_SplashAlloc(gapp);
    Loader_SplashRelease();
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)menuAction:(id)sender{
_bTrace_("[AppDelegate menuAction]", true);
UInt32  cmd=(UInt32)[((NSMenuItem*)sender) tag];
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd(cmd);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)undo:(id)sender{
_bTrace_("[AppDelegate undo]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)redo:(id)sender{
_bTrace_("[AppDelegate redo]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)cut:(id)sender{
_bTrace_("[AppDelegate cut]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)copy:(id)sender{
_bTrace_("[AppDelegate copy]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)paste:(id)sender{
_bTrace_("[AppDelegate paste]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)delete:(id)sender{
_bTrace_("[AppDelegate delete]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(IBAction)selectAll:(id)sender{
_bTrace_("[AppDelegate selectAll]", true);
bMacMapApp* app=(bMacMapApp*)bMacMapApp::instance();
    app->dispatch_cmd((UInt32)[sender tag]);
}

// ---------------------------------------------------------------------------
//
// -----------
-(BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)item{
bMacMapApp* gapp=(bMacMapApp*)bMacMapApp::instance();
    gapp->menuMgr()->check_menus();
    if([item tag]!=0){
UInt32  cmd=(UInt32)[item tag];
        return gapp->menuMgr()->is_enabled(cmd);
    }
    else{
SEL     act=[item action];
        if((act==@selector(newDocument:))           ||
           (act==@selector(openDocument:))          ||
           (act==@selector(_openRecentDocument:))   ){
            return (gapp->document()==NULL);
        }
        else if(act==@selector(clearRecentDocuments:)){
            return YES;
        }
        else if((act==@selector(closeAll:))||
                (act==@selector(performClose:))){
            return (gapp->document()!=NULL);
        }
        else if(act==@selector(about:)){
            return YES;
        }
        else if(act!=NULL){
NSLog(@"%@", NSStringFromSelector(act));
        }        
    }
    return NO;
}

// ---------------------------------------------------------------------------
//
// -----------
@end
