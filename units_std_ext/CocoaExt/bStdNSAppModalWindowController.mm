//----------------------------------------------------------------------------
// File : bStdNSAppModalWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base class for Cocoa modal window based XMap
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2015 Carte Blanche Conseil.
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
// 15/06/2011 creation.
//----------------------------------------------------------------------------

#import "bStdNSAppModalWindowController.h"
#import "bStdNSAppModalCocoaStuff.h" 
#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bStdNSAppModalWindowController

// ---------------------------------------------------------------------------
//  
// ------------
-(id)initWithExt:(bGenericExt*)ext{
_bTrace_("[bStdNSAppModalWindowController initWithExt]",true);
	self=[self initWithWindowNibName:@"Palette"];
	if(self){
		_ext=ext;
		_code=-1;
_tm_("self ok");
	}
	else{
_te_("pas de self");
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bStdNSAppModalWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)windowDidLoad {
_bTrace_("[bStdNSAppModalWindowController windowDidLoad]",true);
	[super windowDidLoad];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSAppModalWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Gestion Modal ----
// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)validDialog:(id)sender{
	_code=1;
	[NSApp stopModal];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(IBAction)cancelDialog:(id)sender{
	_code=0;
	[NSApp stopModal];
}

#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)runAppModal:(long*)code{
_bTrace_("[bStdNSAppModalWindowController runAppModal]",true);
	[[self window] makeKeyAndOrderFront:nil];
	[[self window] makeFirstResponder:nil];
    [NSApp runModalForWindow:[self window]];
	[[self window] setViewsNeedDisplay:NO];
	[[self window] orderOut:self];
	*code=_code;
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
