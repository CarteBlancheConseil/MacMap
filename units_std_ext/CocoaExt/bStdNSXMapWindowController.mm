//----------------------------------------------------------------------------
// File : bStdNSXMapWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Cocoa floating window XMap controller
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
// 14/04/2014 creation.
//----------------------------------------------------------------------------

#import "bStdNSXMapWindowController.h"
#import "bStdNSXMapCocoaStuff.h" 
#import "bStdNSXMap.h" 

#import "bXMapStdIntf.h"

#import <MacMapSuite/bTrace.h>

#import <mox_intf/bGenericMacMapApp.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/type_utils.h>
#import <mox_intf/CGUtils.h>
#import <mox_intf/type_utils.h>

#import <mox_intf/NSUIUtils.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bStdNSXMapPanel

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSXMapPanel close]",true);
_tm_((void*)self);
bStdNSXMapWindowController*	mmc=(bStdNSXMapWindowController*)[self windowController];
	if(mmc){
		[mmc prepareForClose];
	}
	[super close];
}

// ---------------------------------------------------------------------------
// Just for debug
// ------------
-(void)dealloc{
_bTrace_("[bStdNSXMapPanel dealloc]",true);
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
@implementation bStdNSXMapWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bStdNSXMapWindowController init]",true);
	self=[self initWithWindowNibName:@"Palette"];
	if(self){
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bStdNSXMapWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)windowDidLoad {
_bTrace_("[bStdNSXMapWindowController windowDidLoad]",true);
	[super windowDidLoad];
NSRect	frm=[[self window] frame];
_tm_("frame:"+frm.origin.x+";"+frm.origin.y+";"+frm.size.width+";"+frm.size.height);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
	[super awakeFromNib];
//_bTrace_("[bStdNSXMapWindowController awakeFromNib]",true);
//NSButton *miniaturizeButton = [[self window]
//								standardWindowButton:NSWindowMiniaturizeButton];
//	[miniaturizeButton setHidden:YES];
//NSButton *zoomButton = [[self window]
//						standardWindowButton:NSWindowZoomButton];
//	[zoomButton setHidden:YES];	
//	[self read];
//	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSXMapWindowController close]",true);
_tm_((void*)self);
	[super close];
}


#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)setExt:(bStdNSXMap*)ext{
	_ext=ext;
CGRect	bnds;
	
	_ext->get_bounds(&bnds);
	if(!CGRectIsEmpty(bnds)){
		[[self window] setFrame:NSRectFromCGRect(bnds) display:NO];
	}
	else{
		bnds=NSRectToCGRect([[self window] frame]);
		_ext->set_bounds(&bnds);
	}
	
	[[self window] makeKeyAndOrderFront:self];

char	name[256];
	GetName(_ext,name);
//    strcat(name," [Cocoa]");
//	strrep(name,"…","");
//	strrep(name,"...","");
	[[self window] setTitle:[NSString stringWithCString:name encoding:NSMacOSRomanStringEncoding]];
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
//_bTrace_("[bStdNSXMapWindowController checkMacMapEvent]",true);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)idle{
//_bTrace_("[bStdNSXMapWindowController idle]",true);
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
void* CCWDXMap_Init(const void* gapp, 
					const void* ext) {
bStdNSXMapWindowController	*controller;
NSAutoreleasePool			*localPool;
	
    localPool=[[NSAutoreleasePool alloc] init];   
    controller=[[bStdNSXMapWindowController alloc] init];
    [controller setExt:(bStdNSXMap*)ext];
    [localPool release];
    return((void*)controller);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDXMap_Dispose(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXMapWindowController	*mmc=(bStdNSXMapWindowController*)instance;
	
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
void CCWDXMap_Close(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXMapWindowController	*mmc=(bStdNSXMapWindowController*)instance;
	
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
void CCWDXMap_CheckMMEvt(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXMapWindowController	*mmc=(bStdNSXMapWindowController*)instance;
	
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
void CCWDXMap_Idle(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXMapWindowController	*mmc=(bStdNSXMapWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];        
	[mmc idle];
    [localPool release];
}

