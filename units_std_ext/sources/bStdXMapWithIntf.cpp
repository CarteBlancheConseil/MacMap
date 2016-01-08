//----------------------------------------------------------------------------
// File : bStdXMapWithIntf.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for window based XMaps (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSXMap/bStdNSAppModalWindowController instead)
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#include "bStdXMapWithIntf.h"
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMapWithIntf::bStdXMapWithIntf(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				: bStdXMap(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMapWithIntf::~bStdXMapWithIntf(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMapWithIntf::open(int* flags){
	bStdXMap::open(flags);
	_wd=NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMapWithIntf::close(){
	bStdXMap::close();
	if(_wd){
		DisposeWindow(_wd);
		_wd=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMapWithIntf::edit(void* prm){
OSStatus			status;
IBNibRef			nib=NULL;
	
const EventTypeSpec	kWindowEvents[] = {	{kEventClassCommand,kEventCommandProcess},

										{kEventClassWindow,kEventWindowClose},
										{kEventClassWindow,kEventWindowResizeCompleted},
										{kEventClassWindow,kEventWindowZoomed},
										{kEventClassWindow,kEventWindowExpanded},
										{kEventClassWindow,kEventWindowCollapsed},
										{kEventClassWindow,kEventWindowDrawContent},
										{kEventClassWindow,kEventWindowActivated},
										{kEventClassWindow,kEventWindowDeactivated},
										{kEventClassWindow,kEventWindowBoundsChanging},
										{kEventClassWindow,kEventWindowBoundsChanged},
										{kEventClassWindow,kEventWindowHandleContentClick},
										{kEventClassWindow,kEventWindowCursorChange},
										
										{kEventClassMouse,kEventMouseDragged},
										{kEventClassMouse,kEventMouseEntered},
										{kEventClassMouse,kEventMouseExited},
										{kEventClassMouse,kEventMouseDown},
										{kEventClassMouse,kEventMouseUp},
										{kEventClassMouse,kEventMouseWheelMoved},
										 
										{kEventClassKeyboard,kEventRawKeyDown},
										{kEventClassKeyboard,kEventRawKeyUp},
										{kEventClassKeyboard,kEventRawKeyModifiersChanged},

										{kEventClassFont,kEventFontSelection}};
	
	_wd=NULL;
	for(;;){
//		use_fork();
		status=CreateNibReferenceWithCFBundle(getbundle(),CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("Window"),&_wd);
		if(status){
			break;
		}
EventHandlerUPP hdlr=wd_evt_hdlr;
		InstallWindowEventHandler(_wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);
		SetPortWindowPort(_wd);
		wd_init(_wd);
		ShowWindow(_wd);
//		restore_fork();
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
//	_gapp->mapwd()->set_port();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMapWithIntf::wd_event(EventRef evt, WindowRef wd){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMapWithIntf::wd_init(WindowRef wd){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bStdXMapWithIntf::wd_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus			result=eventNotHandledErr;
bStdXMapWithIntf*	x=(bStdXMapWithIntf*)up;
//bTrace				trc("bStdXMapWithIntf::wd_evt_hdlr",false);
//	x->use_fork();
	IdleApp();
	if(x->wd_event(evt,x->_wd)){
		result=noErr;
	}
//	x->restore_fork();
//_tm_("result=%d",result);
	return(result);
}

// ---------------------------------------------------------------------------
// 
// ------------
ControlRef bStdXMapWithIntf::get_control(OSType sign, int id){
ControlID	cid={sign,id};
ControlRef	c;
	
	GetControlByID(_wd,&cid,&c);
	return(c);
}
