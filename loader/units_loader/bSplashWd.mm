//----------------------------------------------------------------------------
// File : bSplashWd.cpp
// Project : MacMap
// Purpose : C++ source file : Splash screen class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2019 Carte Blanche Conseil.
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
// 17/09/2015 creation.
// 06/08/2019 Cocoa.
//----------------------------------------------------------------------------

#include "bSplashWd.h"
#include <mox_intf/NSUIUtils.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
//
// ------------
#define	kTimerMax       10000

// ---------------------------------------------------------------------------
@interface MMSplashWindowController : NSWindowController{
    IBOutlet NSTextField*   _vrs_lbl;
    IBOutlet NSTextField*   _prg_lbl;
    IBOutlet NSTextField*   _plb_lbl;
    NSTimer*                _timer;
}

//----------------------------------------------------------------------------
-(void)setVers:(const char*)version;
-(void)setMessage:(const char*)message;
-(void)hideLoad;
-(void)update:(id)sender;
-(void)show;
-(IBAction)stop:(id)sender;

//----------------------------------------------------------------------------
@end


// ---------------------------------------------------------------------------
//
// ------------
@implementation MMSplashWindowController : NSWindowController
// ---------------------------------------------------------------------------
//
// ------------
-(void)setVers:(const char*)version{
    [_vrs_lbl setCharValue:version];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setMessage:(const char*)message{
    [_prg_lbl setCharValue:message];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)hideLoad{
    [_plb_lbl setHidden:YES];
    [_plb_lbl setEnabled:NO];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)update:(id)sender{
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)show{
    [(NSPanel*)[self window] setFloatingPanel:YES];
    [(NSPanel*)[self window] setBecomesKeyOnlyIfNeeded:YES];
    [(NSPanel*)[self window] setWorksWhenModal:NO];

    [[self window] makeKeyAndOrderFront:nil];
    [[self window] makeFirstResponder:nil];
    
/*NSWindowStyleMask msk=[[self window] styleMask];
    msk &= ~NSTitledWindowMask;
    [[self window] setStyleMask:msk];*/
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)startMainLoop{
    if(_timer){
        [_timer invalidate];
        _timer = nil;
    }
    _timer=[NSTimer scheduledTimerWithTimeInterval:0.25 target:self selector:@selector(update:) userInfo:nil repeats:YES];
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)stop:(id)sender{
    if(_timer){
        [_timer invalidate];
        _timer = nil;
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)mouseDown:(NSEvent*)event{
    [self stop:nil];
    [NSApp stopModal];
}

// ---------------------------------------------------------------------------
//
// ------------
@end

//----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Globale
// ------------
bSplashWd* bSplashWd::_splashinst=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSplashWd::bSplashWd(bool about,
                     bGenericMacMapApp* gapp){
//_bTrace_("bSplashWd::bSplashWd",true);
	_timer=clock();

CFStringRef vers=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(),CFSTR("CFBundleVersion"));
char		sn[256];
    CFStringGetCString(vers,sn,255,kCFStringEncodingMacRoman);
    
MMSplashWindowController	*controller=[[MMSplashWindowController alloc] initWithWindowNibName:@"MMSplashWindow"];
    _ctrlr=(void*)controller;
    
    if(about){
        [[controller window] makeKeyAndOrderFront:nil];
        [[controller window] makeFirstResponder:nil];
        [controller setVers:sn];
        [controller setMessage:""];
        [controller hideLoad];
        [NSApp runModalForWindow:[controller window]];
        [[controller window] setViewsNeedDisplay:NO];
        [[controller window] orderOut:controller];
    }
    else{
        [controller show];
        [controller setVers:sn];
        [controller setMessage:""];
        [controller startMainLoop];
    }
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bSplashWd::~bSplashWd(){
//_bTrace_("bSplashWd::~bSplashWd",true);
    if(_ctrlr){
MMSplashWindowController	*controller=(MMSplashWindowController*)_ctrlr;
        [controller stop:nil];
        [controller close];
        [controller release];
        _ctrlr=NULL;
    }
}

// ---------------------------------------------------------------------------
// Init globale
// ------------
bSplashWd* bSplashWd::instance(bool about,
                               bGenericMacMapApp* gapp){
	if(_splashinst==NULL){
		_splashinst=new bSplashWd(about,gapp);
	}
	return(_splashinst);
}

// ---------------------------------------------------------------------------
// Release globale
// ------------
void bSplashWd::release_instance(){
	if(_splashinst!=NULL){
		delete _splashinst;
		_splashinst=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSplashWd::set_progress(const char* str){
MMSplashWindowController    *controller=(MMSplashWindowController*)_ctrlr;
    if(controller==nil){
        return false;
    }
    if(clock()-_timer>kTimerMax){
        [controller setMessage:str];
        [[NSRunLoop mainRunLoop] runUntilDate:[NSDate date]];
        _timer=clock();
    }
	return(true);
}
