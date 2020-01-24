//----------------------------------------------------------------------------
// File : bStdNSXBoxWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Cocoa XMapBox base controller
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

#import "bStdNSXBoxWindowController.h"
#import "bStdNSXBoxCocoaStuff.h" 
#import "bStdNSXBox.h" 

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
@implementation bStdNSXBoxPanel

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[bStdNSXBoxPanel close]",true);
_tm_((__bridge void*)self);
//[window retain] and [window setReleasedWhenClosed:YES]
bStdNSXBoxWindowController*	mmc=(bStdNSXBoxWindowController*)[self windowController];
	if(mmc){
		[mmc prepareForClose];
	}
	[super close];
//NSUInteger k=[[NSApp windows] indexOfObject:self];
//_tm_((int)k);
}

// ---------------------------------------------------------------------------
// Just for debug
// ------------
/*-(void)dealloc{
_bTrace_("[bStdNSXBoxPanel dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}*/


/*
 
 s stated in the documentation, for non document base applications, you can simply:
 
 Keep a reference for your NSWindowController wherever your are calling it. (In the example below it's referenced by myWindowController;
 Make the class calling your NSWindowController implement the protocol NSWindowDelegate;
 Release your Window Controller by setting it to nil on windowWillClose: method
 To answer the question more precisely. When lazy instantiating your controller, set your class as the delegate:
 
 -(IBAction)showMyWindowAction:(id)sender
 {
 // If my window controller is not nil
 if (!myWindowController)
 {
 //instantiate it
 myWindowController = [[MyWindowController alloc] initWithWindowNibName:@"myWindow"];
 // set your class as delegate
 [myWindowController setDelegate:self];
 }
 
 [myWindowController.window orderFront:self];
 }
 And then implement the windowWillClose: method from the NSWindowDelegate protocol
 
 -(void)windowWillClose:(NSNotification *)notification
 {
 //Check if it's the right window that will close
 if ([notification.object isEqualTo:myWindowController.window])
 {
 //Set your controller to nil
 myWindowController = nil;
 }
 }
 That's it, your window controller will now dealloc and since we are verifying if it's controller is nil before showing the window, everything will work!
 
 I believe the reason why this is not implemented by default is because the initWithWindowNibName: is a somewhat heavy operation, and thus you have to think if dealloc'ing whatever is on your window will impact more or less than loading your window nib file.
 

 
 
*/

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bStdNSXBoxWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[bStdNSXBoxWindowController init]",true);
	self=[self initWithWindowNibName:@"Palette"];
	if(self){
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
/*-(void)dealloc{
_bTrace_("[bStdNSXBoxWindowController dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}*/

// ---------------------------------------------------------------------------
// 
// ------------
-(void)windowDidLoad {
_bTrace_("[bStdNSXBoxWindowController windowDidLoad]",true);
	[super windowDidLoad];
NSRect	frm=[[self window] frame];
_tm_("frame:"+frm.origin.x+";"+frm.origin.y+";"+frm.size.width+";"+frm.size.height);
    [[self window] retain];
    [[self window] setReleasedWhenClosed:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
	[super awakeFromNib];
//_bTrace_("[bStdNSXBoxWindowController awakeFromNib]",true);
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
_bTrace_("[bStdNSXBoxWindowController close]",true);
_tm_((__bridge void*)self);
	[super close];
}


#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)setExt:(bStdNSXBox*)ext{
	_ext=ext;
CGRect	bnds;
	
	_ext->get_bounds(&bnds);
	if(CGRectIsEmpty(bnds)||((bnds.size.width==100)&&(bnds.size.height==100))){
        bnds=NSRectToCGRect([[self window] frame]);
        _ext->set_bounds(&bnds);
	}
	else{
        [[self window] setFrame:NSRectFromCGRect(bnds) display:NO];
	}
	
	[[self window] makeKeyAndOrderFront:self];
	[[self window] makeFirstResponder:self];
	[[self window] makeKeyWindow];

char	name[256];
	GetName(_ext,name);
//  strcat(name," [Cocoa]");
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
//_bTrace_("[bStdNSXBoxWindowController checkMacMapEvent]",true);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)idle{
//_bTrace_("[bStdNSXBoxWindowController idle]",true);
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
void* CCWDXBOX_Init(const void* gapp, 
					const void* ext) {
bStdNSXBoxWindowController	*controller;
NSAutoreleasePool			*localPool;
	
    localPool=[[NSAutoreleasePool alloc] init];
    controller=[[bStdNSXBoxWindowController alloc] init];
    [controller setExt:(bStdNSXBox*)ext];
    [localPool release];
    return((__bridge void*)controller);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDXBOX_Dispose(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXBoxWindowController	*mmc=(__bridge bStdNSXBoxWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];
//NSWindow* wd=[mmc window];
    [mmc release];
//    [wd release];
    [localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void CCWDXBOX_Close(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXBoxWindowController	*mmc=(__bridge bStdNSXBoxWindowController*)instance;
	
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
void CCWDXBOX_CheckMMEvt(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXBoxWindowController	*mmc=(__bridge bStdNSXBoxWindowController*)instance;
	
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
void CCWDXBOX_Idle(const void* instance){
NSAutoreleasePool			*localPool;
bStdNSXBoxWindowController	*mmc=(__bridge bStdNSXBoxWindowController*)instance;
	
	if(mmc==NULL){
		return;
	}
	
    localPool=[[NSAutoreleasePool alloc] init];
	[mmc idle];
    [localPool release];
}

