//----------------------------------------------------------------------------
// File : bStdXMapWithModalIntf.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for modal window based XMaps (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSAppModalWindowController instead)
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

#include "bStdXMapWithModalIntf.h"
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMapWithModalIntf	::bStdXMapWithModalIntf(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						: bStdXMapWithIntf(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMapWithModalIntf::~bStdXMapWithModalIntf(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMapWithModalIntf::edit(void* prm){
OSStatus			status;
IBNibRef			nib=NULL;
		
	const EventTypeSpec	kWindowEvents[]={	{kEventClassCommand,kEventCommandProcess},
											{kEventClassFont,kEventFontSelection}};
	
	_wd=NULL;
	_run=false;
	
	for(;;){
		status=CreateNibReferenceWithCFBundle(getbundle(),CFSTR("main"),&nib);
		if(status){
			break;;
		}
		status=CreateWindowFromNib(nib,CFSTR("Window"),&_wd);
		if(status){
			break;
		}
EventHandlerUPP hdlr=wd_evt_hdlr;
		InstallWindowEventHandler(_wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);
//		SetPortWindowPort(_wd);
		wd_init(_wd);
		ShowWindow(_wd);		
		status=RunAppModalLoopForWindow(_wd);
		if(status){
			break;
		}
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(_wd){
		DisposeWindow(_wd);
		_wd=NULL;
	}
	if(_run){
		_run=process(kExtProcessCallFromIntf,prm);
	}
//	_gapp->mapwd()->set_port();
	return(_run);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bStdXMapWithModalIntf::wd_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus				result=eventNotHandledErr;
HICommand				cmd;
bStdXMapWithModalIntf*	x=(bStdXMapWithModalIntf*)up;
	
	IdleApp();
	if(x->wd_event(evt,x->_wd)){
		result=noErr;
	}
	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				QuitAppModalLoopForWindow(x->_wd);
				x->_run=true;
				result=noErr;
				break;
			case kHICommandCancel:
				QuitAppModalLoopForWindow(x->_wd);
				x->_run=false;
				result=noErr;
				break;
		}
	}
	return(result);
}

