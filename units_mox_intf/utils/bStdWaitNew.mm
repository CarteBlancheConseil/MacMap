//----------------------------------------------------------------------------
// File : bStdWait.cpp
// Project : MacMap
// Purpose : C++ source file : Wait windows utility classes
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
// 07/10/2003 creation.
// 03/01/2018 creation.
//----------------------------------------------------------------------------

#include "bStdWait.h"
#include "Carb_Utils.h"
#include "ext_utils.h"
#include "mm_messages.h"

#import "NSUIUtils.h"
#import <Cocoa/Cocoa.h>
#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define	kTimerMax	10000

// ---------------------------------------------------------------------------
@interface MMWaitWindowController : NSWindowController{
    IBOutlet NSTextField*            _msg_lbl;
    IBOutlet NSProgressIndicator*    _bar_prg;
    IBOutlet NSButton*               _stp_btn;
    BOOL                             _end;
}

//----------------------------------------------------------------------------
-(void)setTitle:(const char*)title;
-(void)setMessage:(const char*)message;
-(void)hideStopButton;
-(void)setIndeterminate:(BOOL)flag;
-(void)setMaximum:(double)max;
-(void)setProgress:(double)index;
-(long)getProgress;
-(IBAction)stop:(id)sender;

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
    
    [super windowDidLoad];
    
    [(NSPanel*)[self window] setFloatingPanel:YES];
    [(NSPanel*)[self window] setBecomesKeyOnlyIfNeeded:YES];
    [(NSPanel*)[self window] setWorksWhenModal:NO];
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
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)setIndeterminate:(BOOL)flag{
    [_bar_prg setIndeterminate:flag];
    if([_bar_prg isIndeterminate]){
        [_bar_prg startAnimation:self];// Utile ?
    }
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
-(void)setProgress:(double)index{
    if(!_end){
        [_bar_prg setDoubleValue:(double)index];
    }
}

// ---------------------------------------------------------------------------
//
// ------------
-(long)getProgress{
    if(_end){
        return 0;
    }
    return (long)[_bar_prg doubleValue];
}

// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)stop:(id)sender{
    [self close];
    _end=YES;
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
    _visible=false;
	_show=show;
    _canbreak=canbreak;
	_index=-1;
	_maximum=maximum;
	_timer=clock();
    
MMWaitWindowController	*controller=[[MMWaitWindowController alloc] initWithWindowNibName:@"MMWaitPalette"];
    [controller setTitle:title];
    [controller setMessage:message];
    [controller setIndeterminate:indeterminate];
    [controller setMaximum:maximum];
    if(!canbreak){
        [controller hideStopButton];
    }
    _ctrlr=controller;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdWait::~bStdWait(){
    if(_ctrlr){
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
        [controller close];
        [controller release];
        _ctrlr=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdWait::set_progress(long index){
	if(index==0){
		return true;
	}
MMWaitWindowController	*controller=(MMWaitWindowController*)_ctrlr;
    if(controller){
        if([controller getProgress]==0){
fprintf(stderr,"WAIT PROC STOPPED BY USER\n");
            [controller close];
            [controller release];
            _ctrlr=NULL;
            _index=0;
            return false;
        }
    }
    
	if(_show&&!_visible){
        if(controller){
            [[controller window] makeKeyAndOrderFront:nil];
            [[controller window] makeFirstResponder:nil];
        }
		_visible=true;
	}
	
	if(clock()-_timer>kTimerMax){
        _timer=clock();
        if(controller){
            [controller setProgress:index];
        }
        
        if(!_show&&_canbreak){// Traitement du cas CursWait pour l'escape
NSEvent*    nsevt=[NSApp currentEvent];
            if([nsevt type]==NSKeyDown){
                if([nsevt keyCode]==kVK_Escape){
                    if(controller){
                        [controller stop:nil];
                    }
                    else{
                        _index=0;
                    }
                }
            }
        }
        
/* Infame bidouille pour assurer le refresh de la fenêtre => trouver mieux */
EventRef		evt;
EventTargetRef	tgt=GetEventDispatcherTarget();
OSStatus		status=ReceiveNextEvent(0,NULL,kEventDurationMillisecond,true,&evt);
		if(status==noErr){
            SendEventToEventTarget(evt,tgt);
            ReleaseEvent(evt);
        }
    }
    
    if(_index==0){
        return true;
    }
    
	_index=index;
	if(_index==_maximum){
        if(controller){
            [controller stop:nil];
        }
	}
    return true;
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdWait::get_progress(){
	return(_index);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bProgressWait	::bProgressWait(const char* title,
								const char* message, 
								bool canbreak,
								long maximum)
				:bStdWait(	false,
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
bBarberWait	::bBarberWait(	const char* title,
							const char* message, 
							bool canbreak)
				:bStdWait(	true,
							title,
							message,
							canbreak,
							true,
							LONG_MAX){
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
long	idx=get_progress();
	if(idx==0){
		return(false);
	}
	if(idx==_maximum){
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
			:bStdWait(  true,
                        "",
                        "",
                        canbreak,
                        false,
                        LONG_MAX){
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
long	idx=get_progress();
	if(idx==0){
		return(false);
	}
	if(idx==_maximum){
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



/*            if( (GetEventClass(evt)==kEventClassKeyboard)   &&
 (GetEventKind(evt)==kEventRawKeyDown)      && _canbreak ){
 ByteCount       unilen = 2;
 UniChar         unicode[4];
 GetEventParameter(evt,
 kEventParamKeyUnicodes,
 typeUnicodeText,
 0,
 unilen,
 0,
 unicode);
 if(unicode[0]==27){
 if(controller){
 [controller stop:nil];
 }
 else{
 _index=0;
 }
 ReleaseEvent(evt);
 }
 else{
 SendEventToEventTarget(evt,tgt);
 ReleaseEvent(evt);
 }
 }
*/