//----------------------------------------------------------------------------
// File : BrowserWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base window controller classes for browsers
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
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#import "BrowserWindowController.h"
#import <MacMapSuite/bTrace.h>
#import <mox_intf/NSUIUtils.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserPanel

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[BrowserPanel close]",true);
_tm_((void*)self);
_tm_((void*)_hdr);
	[_hdr write_p];
BrowserWindowController*	mmc=(BrowserWindowController*)[self windowController];
	if(mmc){
		[mmc prepareForClose];
	}
	[super close];
}

// ---------------------------------------------------------------------------
// Just for debug
// ------------
-(void)dealloc{
_bTrace_("[BrowserPanel dealloc]",true);
_tm_((void*)self);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)performClose:(id)sender{
_bTrace_("[BrowserPanel performClose]",true);
BrowserWindowController*	mmc=(BrowserWindowController*)[self windowController];
	if(![mmc abort]){
		[super performClose:sender];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[BrowserWindowController init]",true);
	self=[super init];
    if(self){
        _ext=NULL;
    }
_tm_((void*)self);
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[BrowserWindowController awakeFromNib]",true);
	[super awakeFromNib];
	_ls=0;
	if(_ext){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)_ext->getapp();
		[_brw putExt:_ext];
		[_brw putLines:new bArray(*(gapp->selMgr()->elements()))];
		[self updateUI];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)checkMacMapEvent{
	[_brw checkMacMapEvent];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)idle{
bGenericMacMapApp*	gapp=[_brw app];
	if(_ls!=gapp->selMgr()->getState()){
		[self updateUI];
		_ls=gapp->selMgr()->getState();
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[BrowserWindowController close]",true);
_tm_((void*)self);
	[super close];
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_brw updateUI];
bGenericType* tp=[_brw type];
	if(tp){
char	str[256];
		tp->name(str);
		[_typ setCharValue:str];
		sprintf(str,"%ld",[_brw lines]->count());
		[_cnt setCharValue:str];
	}
}

#pragma mark ---- Intf Externe/Cocoa ----
// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericMacMapApp*)getApp{
	return((bGenericMacMapApp*)_ext->getapp());
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericType*)type{
	return [_brw type];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)abort{
	return [_brw abort];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end
