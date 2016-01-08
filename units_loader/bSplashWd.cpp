//----------------------------------------------------------------------------
// File : bSplashWd.cpp
// Project : MacMap
// Purpose : C++ source file : Splash screen class
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
// 17/09/2015 creation.
//----------------------------------------------------------------------------

#include "bSplashWd.h"
#include <mox_intf/Carb_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Globale
// ------------
bSplashWd* bSplashWd::_splashinst=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSplashWd::bSplashWd(bool about,
                     bGenericMacMapApp* gapp){
_bTrace_("bSplashWd::bSplashWd",false);
	_timer=clock();
	_wd=NULL;
	
CFBundleRef bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.loader.framework"));
IBNibRef	nibs=NULL;
	if(CreateNibReferenceWithCFBundle(bndl,CFSTR("menubar"),&nibs)){
        NSBeep();
		return;
	}
	if(CreateWindowFromNib(nibs,CFSTR("Splash"),&_wd)){
		NSBeep();
		DisposeNibReference(nibs);
		return;
	}
	
HIViewRef	vw;
HIViewID	vid={'Spls',1};

	HIViewFindByID(HIViewGetRoot(_wd),vid,&vw);
CGImageRef	img=LoadPNGImageFromBundle(bndl,CFSTR("SplashWD.png"));
	if(!img){
_te_("LoadPNGImageFromBundle");	
	}
	if(HIImageViewSetImage(vw,img)){
_te_("HIImageViewSetImage");	
	}
	if(img){
//		CFRelease(img);
	}
	HIViewSetNeedsDisplay(vw,true);

ControlRef	c;
char		sn[256];
    
CFStringRef vers=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(),CFSTR("CFBundleVersion"));
    CFStringGetCString(vers,sn,255,kCFStringEncodingMacRoman);
    vid.id=2;
    GetControlByID(_wd,&vid,&c);
    SetTextControlValue(c,sn);
    
	vid.id=3;
	GetControlByID(_wd,&vid,&c);
	SetTextControlValue(c,"");

	if(about){
		vid.id=5;
		GetControlByID(_wd,&vid,&c);
		SetTextControlValue(c,"");
	
		ShowWindow(_wd);
		
EventTypeSpec	specs[]={	{kEventClassKeyboard,kEventRawKeyDown},
							{kEventClassMouse,kEventMouseDown}		};
							
		InstallWindowEventHandler(_wd,evt_hdlr,GetEventTypeCount(specs),specs,(void*)this,NULL);
		(void)RunAppModalLoopForWindow(_wd);
	}
	else{
		ShowWindow(_wd);		
	}
	DisposeNibReference(nibs);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bSplashWd::~bSplashWd(){
	if(_wd){
		DisposeWindow(_wd);
	}
	_wd=NULL;
}

// ---------------------------------------------------------------------------
// Init globale
// ------------
bSplashWd* bSplashWd::instance(bool about,
                               bGenericMacMapApp* gapp){
	if(_splashinst==NULL){
		_splashinst=new bSplashWd(about,gapp);
	}
	return(_splashinst);
}

// ---------------------------------------------------------------------------
// Release globale
// ------------
void bSplashWd::release_instance(){
	if(_splashinst!=NULL){
		delete _splashinst;
		_splashinst=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSplashWd::set_progress(const char* str){
ControlRef	c;
ControlID	cid={'Spls',3};
	GetControlByID(_wd,&cid,&c);
	SetTextControlValue(c,str);
	
	if(clock()-_timer>5){
		_timer=clock();
EventRef		evt;
EventTargetRef	tgt=GetEventDispatcherTarget();
OSStatus		status=ReceiveNextEvent(0,NULL,kEventDurationMillisecond,true,&evt);
		if(status==noErr){
			if(GetEventClass(evt)==kEventClassKeyboard){
				SendEventToEventTarget(evt,GetWindowEventTarget(_wd));
			}
			SendEventToEventTarget(evt,tgt);
	        ReleaseEvent(evt);
	    }
	}

	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bSplashWd::evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus	result=noErr;
UInt32		clss=GetEventClass(evt);
char		code;
bSplashWd*	wt=(bSplashWd*)up;
	
	if(clss==kEventClassKeyboard){
		GetEventParameter(evt,kEventParamKeyMacCharCodes,typeChar,NULL,sizeof(char),NULL,&code);
		if(code==kEscapeCharCode){
			QuitAppModalLoopForWindow(wt->_wd);
		}
		else{
			result=eventNotHandledErr;
		}
	}
	else if(clss==kEventClassMouse){
		if(GetEventKind(evt)==kEventMouseDown){
			QuitAppModalLoopForWindow(wt->_wd);
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


