//----------------------------------------------------------------------------
// File : bDelayedAE.cpp
// Project : MacMap
// Purpose : C++ source file : System events management (close, quit, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 28/06/2007 creation.
//----------------------------------------------------------------------------

#include "bDelayedAE.h"
#include <mox_intf/Carb_Utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAE::bDelayedAE(	const AppleEvent* ae,
						bMacMapApp* zapp){
	_zapp=zapp;
	_status=noErr;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAE::~bDelayedAE(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAE::exec(AppleEvent* out_ae){
	return(_status);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAE::exec(){
	return(_status);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAEOpenWithPath	::bDelayedAEOpenWithPath(const AppleEvent* ae,
												 bMacMapApp* zapp)
						:bDelayedAE(ae,zapp){
AEDescList	docList;
long		items;
long		actualSize;
AEKeyword	keywd;
DescType	returnedType;
	
	_status=AEGetParamDesc(ae,keyDirectObject,typeAEList,&docList);
	if(_status!=noErr){
		return;
	}
	_status=AECountItems(&docList,&items);
	if(_status!=noErr){
		return;
	}
FSRef	ref;
	_status=AEGetNthPtr(&docList,1,typeFSRef,&keywd,&returnedType,&ref,sizeof(FSRef),&actualSize);
	if(_status!=noErr){
		return;
	}
UInt8	path[PATH_MAX*3];
	FSRefMakePath(&ref,path,PATH_MAX*3);
	strcpy(_path,(char*)path);
	strcat(_path,"/");
	_status=AEDisposeDesc(&docList);
	if(_status!=noErr){
		return;
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAEOpenWithPath::~bDelayedAEOpenWithPath(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEOpenWithPath::exec(AppleEvent* out_ae){
	return(exec());
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEOpenWithPath::exec(){
	if(_status){
		return(_status);
	}
	if(_zapp->document()){
		_status=-1;
		return(_status);
	}
	return(_zapp->open_doc(_path));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAEClose	::bDelayedAEClose(	const AppleEvent* ae,
									bMacMapApp* zapp)
				:bDelayedAE(ae,zapp){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAEClose::~bDelayedAEClose(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEClose::exec(AppleEvent* out_ae){
	return(exec());
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEClose::exec(){
	if(_status){
		return(_status);
	}
	if(!_zapp->document()){
		_status=-1;
		return(_status);
	}	
	SendCommandToApplication(kHICommandClose);
	return(_status);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAEQuit	::bDelayedAEQuit(	const AppleEvent* ae,
									bMacMapApp* zapp)
				:bDelayedAE(ae,zapp){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAEQuit::~bDelayedAEQuit(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEQuit::exec(AppleEvent* out_ae){
	return(exec());
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAEQuit::exec(){
	if(_status){
		return(_status);
	}
	SendCommandToApplication(kHICommandQuit);
	return(_status);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAECmd	::bDelayedAECmd(	const AppleEvent* ae,
									bMacMapApp* zapp)
				:bDelayedAE(ae,zapp){
AEDesc		d,r;

	d.dataHandle=NULL;
	r.dataHandle=NULL;

	if((_status=AEGetParamDesc(ae,keyDirectObject,typeType,&d))){
		if(d.dataHandle){
			AEDisposeDesc(&d);
		}
		if(r.dataHandle){
			AEDisposeDesc(&r);
		}
		return;
	}
	if((_status=AECoerceDesc(&d,typeType,&r))){
		if(d.dataHandle){
			AEDisposeDesc(&d);
		}
		if(r.dataHandle){
			AEDisposeDesc(&r);
		}
		return;
	}	
	if((_status=AEGetDescData(&r,&_cmd,sizeof(UInt32)))){
		if(d.dataHandle){
			AEDisposeDesc(&d);
		}
		if(r.dataHandle){
			AEDisposeDesc(&r);
		}
		return;
	}
	if(d.dataHandle){
		AEDisposeDesc(&d);
	}
	if(r.dataHandle){
		AEDisposeDesc(&r);
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAECmd::~bDelayedAECmd(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAECmd::exec(AppleEvent* out_ae){
	return(exec());
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bDelayedAECmd::exec(){
	if(_status){
		return(_status);
	}
	SendCommandToApplication(_cmd);
	return(_status);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDelayedAECmdParam	::bDelayedAECmdParam(	const AppleEvent* ae,
											bMacMapApp* zapp)
					:bDelayedAECmd(ae,zapp){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDelayedAECmdParam::~bDelayedAECmdParam(){
}

