//----------------------------------------------------------------------------
// File : bStdvDef.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for vDefs (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSPalettevDef instead)
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
// 10/02/2005 creation.
//----------------------------------------------------------------------------

#include "bStdvDef.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
char * const vdef_op_list[]={	(char*)"",
								(char*)"equal",
								(char*)"notequal",
								(char*)"lower",
								(char*)"lowerorequal",
								(char*)"higher",
								(char*)"higherorequal",
								(char*)"contain",
								(char*)"beginwith",
								(char*)"endwith"};

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdvDef::bStdvDef(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		: bStdExt(elt,gapp,bndl){
	_subclss=kXMLSubClassExtvDef;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdvDef::~bStdvDef(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdvDef::open(int* flags){
	(*flags)=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdvDef::close(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::process(int msg, void* prm){
bool b=false;
	switch(msg){
		case kExtProcessCallWithParams:
			break;
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::test(void* prm){
	return(test((bGenericType*)prm));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::edit(void* prm){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::test(bGenericType* tp){
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::edit(bGenericXMLBaseElement** root, bGenericType* tp){
	_bTrace_("bStdvDef::edit",true);
OSStatus			status;
IBNibRef			nib=NULL;		
const EventTypeSpec	kWindowEvents[]={	{kEventClassCommand,kEventCommandProcess},
										{kEventClassFont,kEventFontSelection}};
	
	_hdlr=NULL;
	_wd=NULL;
	_run=false;
	_tp=tp;
	_root=(*root);
	for(;;){
		status=CreateNibReferenceWithCFBundle(getbundle(),CFSTR("main"),&nib);
		if(status){
			break;;
		}
		status=CreateWindowFromNib(nib,CFSTR("Window"),&_wd);
		if(status){
			break;
		}
		_hdlr=NewEventHandlerUPP(wd_evt_hdlr);
		if(!_hdlr){
			break;
		}
		InstallWindowEventHandler(_wd,_hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);
		SetPortWindowPort(_wd);		
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
	if(_hdlr){
		DisposeEventHandlerUPP(_hdlr);
		_hdlr=NULL;
	}
	if(_wd){
		DisposeWindow(_wd);
		_wd=NULL;
	}
	if(_run){
		(*root)=_root;
	}
//	_gapp->mapwd()->set_port();
	return(_run);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdvDef::make(){
	_root=NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdvDef::wd_event(EventRef evt, WindowRef wd){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdvDef::wd_init(WindowRef wd){
}

// ---------------------------------------------------------------------------
// 
// ------------
ControlRef bStdvDef::get_control(OSType sign, int id){
ControlID	cid={sign,id};
ControlRef	c;
	GetControlByID(_wd,&cid,&c);
	return(c);
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bStdvDef::wd_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus	result=eventNotHandledErr;
HICommand	cmd;
bStdvDef*	x=(bStdvDef*)up;
	
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

