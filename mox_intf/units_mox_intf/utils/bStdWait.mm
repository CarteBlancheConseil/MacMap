//----------------------------------------------------------------------------
// File : bStdWait.mm
// Project : MacMap
// Purpose : C++/Objective-C++ source file : Wait windows utility classes
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
// 07/10/2003 creation.
// 30/11/2018 Cocoa mode.
// 09/12/2019 delay at open.
//----------------------------------------------------------------------------

#import "bStdWait.h"
#import "NSUIUtils.h"
#import <MacMapSuite/bTrace.h>
#import <Cocoa/Cocoa.h>
#import <stdlib.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define kTimerMax       1000
#define kProgressMax	INT_MAX

// ---------------------------------------------------------------------------
@interface MMWaitWindowController : NSWindowController{
    IBOutlet NSTextField*           _msg_lbl;
    IBOutlet NSProgressIndicator*   _bar_prg;
    IBOutlet NSButton*              _stp_btn;
    BOOL                            _end;
    long                            _index;
    NSTimer*                        _timer;
}

//----------------------------------------------------------------------------
-(void)setTitle:(const char*)title;
-(void)setMessage:(const char*)message;
-(void)hideStopButton;

-(void)setIndeterminate:(BOOL)flag;

-(long)maximum;
-(void)setMaximum:(double)max;

-(long)progress;
-(void)setProgress:(double)index;

-(BOOL)ended;

-(IBAction)stop:(id)sender;

-(void)update:(id)sender;
-(void)show;

//----------------------------------------------------------------------------
@end

// ---------------------------------------------------------------------------
//
// ------------
@implementation MMWaitWindowController : NSWindowController
// ---------------------------------------------------------------------------
//
// ------------
-(void)windowDidLoad {
    _end=NO;
    _index=0;
    [super windowDidLoad];
    
    [(NSPanel*)[self window] setFloatingPanel:YES];
    [(NSPanel*)[self window] setBecomesKeyOnlyIfNeeded:YES];
    [(NSPanel*)[self window] setWorksWhenModal:NO];
}

// ---------------------------------------------------------------------------
//
// ------------
-(BOOL)shouldCascadeWindows{
    return NO;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setTitle:(const char*)title{
   [[self window] setTitle:[NSString stringWithCString:title encoding:NSMacOSRomanStringEncoding]];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setMessage:(const char*)message{
    [_msg_lbl setCharValue:message];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)hideStopButton{
    [_stp_btn setHidden:YES];
    [_stp_btn setEnabled:NO];
    [_stp_btn setAction:nil];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setIndeterminate:(BOOL)flag{
    [_bar_prg setIndeterminate:flag];
    [_bar_prg setHidden:NO];
}

// ---------------------------------------------------------------------------
//
// ------------
-(long)maximum{
    return (long)[_bar_prg maxValue];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setMaximum:(double)max{
    [_bar_prg setMaxValue:max];
}

// ---------------------------------------------------------------------------
//
// ------------
-(BOOL)ended{
    return _end;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)update:(id)sender{
    if(!_timer){
        return;
    }
    if(_index!=(long)[_bar_prg doubleValue]){
        [_bar_prg setDoubleValue:(double)_index];
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setProgress:(double)index{
    _index=index;
}

// ---------------------------------------------------------------------------
//
// ------------
-(long)progress{
    if(_end){
        return 0;
    }
    return (long)[_bar_prg doubleValue];
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)show{
    [[self window] makeKeyAndOrderFront:nil];
    [[self window] makeFirstResponder:nil];
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)stop:(id)sender{
    if(_timer){
        [_timer invalidate];
        _timer = nil;
    }
    [_bar_prg stopAnimation:nil];
    _end=YES;
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)startMainLoop{
    _end=NO;
    if(_timer){
        [_timer invalidate];
        _timer = nil;
    }
    [_bar_prg startAnimation:nil];
    [_bar_prg setDoubleValue:0];
    _timer=[NSTimer scheduledTimerWithTimeInterval:0.25 target:self selector:@selector(update:) userInfo:nil repeats:YES];
}

// ---------------------------------------------------------------------------
//
// ------------
@end

//----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdWait::bStdWait(	bool indeterminate,
					const char* title,
					const char* message, 
					bool canbreak,
					bool show,
					long maximum){
//_bTrace_("bStdWait::bStdWait", true);
    
    _canbreak=canbreak;
    _show=show;
    
MMWaitWindowController	*controller=[[MMWaitWindowController alloc] initWithWindowNibName:@"MMWaitPalette"];
    [controller setTitle:title];
    [controller setMessage:message];
    [controller setIndeterminate:indeterminate];
    [controller setMaximum:maximum];
    if(!canbreak){
        [controller hideStopButton];
    }
    _shown=false;
    /*if(show){
        [controller show];
    }*/
    _ctrlr=(void*)controller;
    _timer=clock();
    [controller startMainLoop];
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdWait::~bStdWait(){
//_bTrace_("bStdWait::~bStdWait", true);
    if(_ctrlr){
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
        if(![controller ended]){
            [controller stop:nil];
        }
        [controller close];
        [controller release];
        _ctrlr=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdWait::set_progress(long index){
_bTrace_("bStdWait::set_progress",false);
    if(index==0){
		return true;
	}
    
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
    if(controller==nil){
        return false;
    }
    
    if([controller ended]==YES){
_tw_("WAIT PROC STOPPED BY USER");
        return false;
    }
    
    if(clock()-_timer>kTimerMax){
        if(_show&&!_shown){
            [controller show];
            _shown=true;
        }

        [[NSRunLoop mainRunLoop] runUntilDate:[NSDate date]];
        if(_canbreak){
NSEvent*    nsevt=[NSApp nextEventMatchingMask:NSEventMaskLeftMouseDown+NSEventMaskKeyDown
                                     untilDate:nil
                                        inMode:NSEventTrackingRunLoopMode
                                       dequeue:YES];
            if(nsevt){
                if(([nsevt type]==NSKeyDown)    &&
                   ([nsevt keyCode]==kVK_Escape)){
                    [controller stop:nil];
                }
                /*if([[controller window] isVisible]==NO){
                    _tm_("Window is not visible");
                    if(([nsevt type]==NSKeyDown)    &&
                       ([nsevt keyCode]==kVK_Escape)){
                        [controller stop:nil];
                    }
                }*/
                else{
                    if([nsevt window]==[controller window]){
                        [[controller window] sendEvent:nsevt];
                    }
                }
            }
        }
        _timer=clock();
    }

    [controller setProgress:index];
    return true;
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdWait::get_progress(){
_bTrace_("bStdWait::get_progress",false);
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
    if(controller==nil){
        return 0;
    }
    if([controller ended]==YES){
_tw_("WAIT PROC STOPPED BY USER");
        return 0;
    }
    return [controller progress];
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bProgressWait	::bProgressWait(const char* title,
								const char* message, 
								bool canbreak,
								long maximum)
				:bStdWait(false,
                          title,
                          message,
                          canbreak,
                          true,
                          maximum){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bProgressWait::~bProgressWait(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBarberWait	::bBarberWait(const char* title,
                          const char* message,
                          bool canbreak)
				:bStdWait(true,
                          title,
                          message,
                          canbreak,
                          true,
                          kProgressMax){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBarberWait::~bBarberWait(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBarberWait::set_progress(long index){
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
    if(controller==nil){
        return 0;
    }
long	idx=[controller progress];
	if(idx==0){
		return(false);
	}
	if(idx==[controller maximum]){
		idx=1;
	}
	else if(idx==-1){
		idx=1;
	}
	else{
		idx++;
	}
	return(bStdWait::set_progress(idx));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCursWait	::bCursWait(bool canbreak)
			:bStdWait(true,
                      "",
                      "",
                      canbreak,
                      false,
                      kProgressMax){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bCursWait::~bCursWait(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCursWait::set_progress(long index){
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
    if(controller==nil){
        return 0;
    }
long	idx=[controller progress];
    if(idx==0){
        return(false);
    }
    if(idx==[controller maximum]){
        idx=1;
    }
    else if(idx==-1){
        idx=1;
    }
    else{
        idx++;
    }
    return(bStdWait::set_progress(idx));
}
