//----------------------------------------------------------------------------
// File : bSDKXMap.cpp
// Project : MacMap
// Purpose : C++ source file : standard MacMapSDK XMap 
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
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#include "bSDKXMap.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/MacMapSuiteC.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSDKXMap::bSDKXMap(	bGenericXMLBaseElement* elt, 
					bGenericMacMapApp* gapp, 
					CFBundleRef bndl,
					CWXMap_OpenPtr open,
					CWXMap_ClosePtr close,
					CWXMap_EventPtr event,
					CWXMap_IdlePtr idle,
					CWXMap_ProcessPtr process,
					CWXMap_TestPtr test,
					CWXMap_EditPtr edit,
					char* name)
		: bStdXMap(elt,gapp,bndl){
_bTrace_("bSDKXMap::bSDKXMap",_VERBOSE_);
_tm_((void*)this);
	_subclss=kXMLSubClassExtXMap;
	setclassname(name);
	_open=open;
	_close=close;
	_event=event;
	_idle=idle;
	_process=process;
	_test=test;
	_edit=edit;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bSDKXMap::~bSDKXMap(){
_bTrace_("bSDKXMap::~bSDKXMap",_VERBOSE_);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bSDKXMap::create(bGenericXMLBaseElement* elt){
_bTrace_("bSDKXMap::create",_VERBOSE_);
char	name[_names_length_max_];
	getclassname(name);
	return(new bSDKXMap(elt,
						_gapp,
						elt->getbundle(),
						_open,
						_close,
						_event,
						_idle,
						_process,
						_test,
						_edit,
						name));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSDKXMap::open(int* flags){
_bTrace_("bSDKXMap::open",_VERBOSE_);
	if(_open){
		_open(this,flags);
	}
	else{
		(*flags)=kCWXMapNeedNothing;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSDKXMap::close(){
_bTrace_("bSDKXMap::close",_VERBOSE_);
	if(_close){
		_close(this);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSDKXMap::event(EventRef evt){
_bTrace_("bSDKXMap::event",_VERBOSE_);
	if(_event){
		return(_event(this,evt));
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSDKXMap::idle(void* prm){
_bTrace_("bSDKXMap::idle",_VERBOSE_);
	if(_idle){
		return(_idle(this,prm));
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSDKXMap::process(int msg, void* prm){
_bTrace_("bSDKXMap::process",_VERBOSE_);
	if(_process){
		return(_process(this,msg,prm));
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSDKXMap::test(void* prm){
_bTrace_("bSDKXMap::test",_VERBOSE_);
	if(_test){
		return(_test(this,prm));
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSDKXMap::edit(void* prm){
_bTrace_("bSDKXMap::edit",_VERBOSE_);
	if(_edit){
		return(_edit(this,prm));
	}
	return(false);
}

