//----------------------------------------------------------------------------
// File : bStdNSPalettevDefWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Cocoa floating window vDef controller
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------

#import "bStdNSPalettevDefWindowController.h"
#import "bStdNSPalettevDefCocoaStuff.h" 
#import "bStdNSPalettevDef.h" 

#import <MacMapSuite/bTrace.h>

#import <mox_intf/bGenericMacMapApp.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/type_utils.h>
#import <mox_intf/CGUtils.h>
#import <mox_intf/NSUIUtils.h>
#import <mox_intf/type_utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bStdNSPalettevDefPanel

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSPalettevDefPanel close]",true);
_tm_((void*)self);
bStdNSPalettevDefWindowController*	mmc=(bStdNSPalettevDefWindowController*)[self windowController];
	if(mmc){
		[mmc prepareForClose];
	}
	[super close];
}

// ---------------------------------------------------------------------------
// Just for debug
// ------------
-(void)dealloc{
_bTrace_("[bStdNSPalettevDefPanel dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bStdNSPalettevDefWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bStdNSPalettevDefWindowController init]",true);
	self=[self initWithWindowNibName:@"Palette"];
	if(self){
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bStdNSPalettevDefWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)windowDidLoad {
_bTrace_("[bStdNSPalettevDefWindowController windowDidLoad]",true);
	[super windowDidLoad];
	
    [(NSPanel*)[self window] setFloatingPanel:YES];
    [(NSPanel*)[self window] setBecomesKeyOnlyIfNeeded:YES];
	[(NSPanel*)[self window] setWorksWhenModal:NO];
}

/*// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[bStdNSPalettevDefWindowController awakeFromNib]",true);
	[self read];
	[self updateUI];
}*/

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSPalettevDefWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)setExt:(bStdNSPalettevDef*)ext{
	_ext=ext;
CGRect	bnds;
NSRect	frm;
	
	_ext->get_bounds(&bnds);
	if(!CGRectIsEmpty(bnds)){
		frm=[[self window] frame];
		frm.origin.x=bnds.origin.x;
		frm.origin.y=bnds.origin.y;
		[[self window] setFrame:frm display:NO];
	}
	else{
		bnds=NSRectToCGRect([[self window] frame]);
		_ext->set_bounds(&bnds);
	}
	
	[[self window] makeKeyAndOrderFront:nil];
	[[self window] makeFirstResponder:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)prepareForClose{
CGRect	bnds=NSRectToCGRect([[self window] frame]);
	_ext->set_bounds(&bnds);
	_ext->wd_close();
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)checkMacMapEvent{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)idle{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)drawPreview:(CGRect)rect context:(CGContextRef)ctx{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)clickPreview:(CGPoint)point modifiers:(NSUInteger)mod{
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
void* CCWDvDef_Init(const void* gapp, 
					const void* ext) {
bStdNSPalettevDefWindowController	*controller;
NSAutoreleasePool					*localPool;
	
    localPool=[[NSAutoreleasePool alloc] init];   
    controller=[[bStdNSPalettevDefWindowController alloc] init];
    [controller setExt:(bStdNSPalettevDef*)ext];
    [localPool release];
    return((void*)controller);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDvDef_Dispose(const void* instance){
NSAutoreleasePool					*localPool;
bStdNSPalettevDefWindowController	*mmc=(bStdNSPalettevDefWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];        
	[mmc release];
    [localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDvDef_Close(const void* instance){
NSAutoreleasePool					*localPool;
bStdNSPalettevDefWindowController	*mmc=(bStdNSPalettevDefWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];        
	[mmc close];
    [localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDvDef_CheckMMEvt(const void* instance){
NSAutoreleasePool					*localPool;
bStdNSPalettevDefWindowController	*mmc=(bStdNSPalettevDefWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];        
	[mmc checkMacMapEvent];
    [localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDvDef_Idle(const void* instance){
NSAutoreleasePool					*localPool;
bStdNSPalettevDefWindowController	*mmc=(bStdNSPalettevDefWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];        
	[mmc idle];
    [localPool release];
}

