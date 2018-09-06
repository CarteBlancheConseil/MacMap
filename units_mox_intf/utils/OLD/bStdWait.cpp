//----------------------------------------------------------------------------
// File : bStdWait.cpp
// Project : MacMap
// Purpose : C++ source file : Wait windows utility classes
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
// 07/10/2003 creation.
//----------------------------------------------------------------------------

#include "bStdWait.h"
#include "Carb_Utils.h"
#include "ext_utils.h"
#include "mm_messages.h"
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// 
// ------------
//#define	kTimerMax	20
// Porté à 10000 pour SDK 10.5
#define	kTimerMax	10000

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdWait::bStdWait(	CFStringRef window,
					const char* title,
					const char* message, 
					bool autoclose, 
					bool couldbreak,
					bool show,
					int	 maximum){
	_autoclose=autoclose;
	_visible=false;
	_show=show;
	_couldbreak=couldbreak;
	_index=-1;
	_maximum=maximum;
	_timer=clock();
	_wd=NULL;
	_bar=NULL;
    
IBNibRef	nibs=NULL;
	if(CreateNibReferenceWithCFBundle(CFBundleGetMainBundle(),CFSTR("main"),&nibs)){
		NSBeep();
		return;
	}
	if(CreateWindowFromNib(nibs,window,&_wd)){
		NSBeep();
		DisposeNibReference(nibs);
		return;
	}
EventTypeSpec	specs[]={	{kEventClassCommand,kEventCommandProcess},
							{kEventClassWindow,kEventWindowClose},
							{kEventClassKeyboard,kEventRawKeyDown}	};
										
	InstallWindowEventHandler(_wd,evt_hdlr,GetEventTypeCount(specs),specs,(void*)this,NULL);
	
ControlID	cid={'wait',2};
ControlRef	c;
	
	GetControlByID(_wd,&cid,&_bar);
	SetControl32BitMinimum(_bar,1);
	SetControl32BitMaximum(_bar,maximum);
	SetControl32BitValue(_bar,1);
		
	if(title){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,title,kCFStringEncodingMacRoman);
		SetWindowTitleWithCFString(_wd,cfs);
		CFRelease(cfs);
	}
	if(message){		
		cid.id=3;
		GetControlByID(_wd,&cid,&c);		
		SetTextControlValue(c,message);
	}
	else{
char	mess[__MESSAGE_STRING_LENGTH_MAX__];
		message_string(kMsgProgress,mess,0);
		cid.id=3;
		GetControlByID(_wd,&cid,&c);		
		SetTextControlValue(c,mess);		
	}
	if(!couldbreak){
		cid.id=1;
		GetControlByID(_wd,&cid,&c);
		HideControl(c);
	}

	DisposeNibReference(nibs);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdWait::~bStdWait(){
	if(_wd){
		DisposeWindow(_wd);
	}
	_wd=NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdWait::set_progress(int index){

	if(index==0){
		return(true);
	}
	if(_show&&!_visible){
		ShowWindow(_wd);
		_visible=true;
	}
	
	if(clock()-_timer>kTimerMax){
// Plus de Idle avec SDK 10.5 ???
//		IdleApp();
		
		SetControl32BitValue(_bar,index);
		_timer=clock();
EventRef		evt;
EventTargetRef	tgt=GetEventDispatcherTarget();
OSStatus		status=ReceiveNextEvent(0,NULL,kEventDurationMillisecond,true,&evt);
		if(status==noErr){
			if(GetEventClass(evt)==kEventClassKeyboard){
				SendEventToEventTarget(evt,GetWindowEventTarget(_wd));
			}
		//	if(GetEventClass(evt)!=kEventClassCommand){
			SendEventToEventTarget(evt,tgt);
		//	}
	        ReleaseEvent(evt);
	    }
	}
	if(_index==0){
		return(false);
	}
	_index=index;
	if(_autoclose&&(_index==_maximum)){
		DisposeWindow(_wd);
		_wd=NULL;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdWait::get_progress(){
	return(_index);
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bStdWait::evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus	result=noErr;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);	
char		code;
bStdWait*	wt=(bStdWait*)up;
	
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandCancel:
				if(wt->_couldbreak){
					DisposeWindow(wt->_wd);
					wt->_wd=NULL;
					wt->_index=0;
fprintf(stderr,"WAIT PROC STOPPED BY USER\n");
				}
				break;
			
			default:
				result=eventNotHandledErr;
				break;
		}
	}
	else if(clss==kEventClassKeyboard){
		GetEventParameter(evt,kEventParamKeyMacCharCodes,typeChar,NULL,sizeof(char),NULL,&code);
		if(code==kEscapeCharCode){
			if(wt->_couldbreak){
				DisposeWindow(wt->_wd);
				wt->_wd=NULL;
				wt->_index=0;
fprintf(stderr,"WAIT PROC STOPPED BY USER\n");
			}		
		}
		else{
			result=eventNotHandledErr;
		}
	}
	else{
		result=eventNotHandledErr;
	}
	
	return(result);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bProgressWait	::bProgressWait(const char* title,
								const char* message, 
								bool autoclose, 
								bool couldbreak,
								int	 maximum)
				:bStdWait(	CFSTR("WaitProgress"),
							title,
							message,
							autoclose,
							couldbreak,
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
							bool autoclose, 
							bool couldbreak)
				:bStdWait(	CFSTR("WaitBarber"),
							title,
							message,
							autoclose,
							couldbreak,
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
bool bBarberWait::set_progress(int index){
int	idx=get_progress();
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
bCursWait	::bCursWait(	bool couldbreak)
			:bStdWait(		CFSTR("WaitBarber"),
							"",
							"",
							true,
							couldbreak,
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
bool bCursWait::set_progress(int index){
int	idx=get_progress();
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
