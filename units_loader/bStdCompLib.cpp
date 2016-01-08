//----------------------------------------------------------------------------
// File : bStdCompLib.cpp
// Project : MacMap
// Purpose : C++ source file : Component plugin class
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

#include "bStdCompLib.h"
#include <mox_intf/mm_errors.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdCompLib::bStdCompLib(CFBundleRef bndl){
	_status=_bStdExtLibNoErr_;
	
	_bndl=bndl;
	_version=NULL;
	_name=NULL;
	_ialloc=NULL;
	_ifree=NULL;
	
	for(;;){
		_version=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("CFBundleVersion"));
		_name=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("CFBundleName"));
		
		if(_version==NULL){
			_status=_bStdExtLibNoVersErr_;
			break;
		}
		if(_name==NULL){
			break;
		}
		if(CFBundleIsExecutableLoaded(_bndl)){
			_status=_bStdExtLibBundleAlreadyLoadedErr_;
			break;
		}
		if(!CFBundleLoadExecutable(_bndl)){
			_status=_bStdExtLibBundleLoadErr_;
			break;
		}
		_ialloc=(iallocPtr)CFBundleGetFunctionPointerForName(bndl,CFSTR("lib_alloc"));
		if(!_ialloc){
			_status=_bStdExtLibNoInitProcErr_;
			break;
		}
		_ifree=(ifreePtr)CFBundleGetFunctionPointerForName(bndl,CFSTR("lib_free"));
		if(!_ifree){
			_status=_bStdExtLibNoDestroyProcErr_;
			break;
		}
		break;
	}
_bTrace_("bStdCompLib::bStdCompLib",false);
	if(_status){
		_status+=_bStdExtLibRootErr_;
_te_("bStdCompLib::bStdCompLib returns "+_status);
	}
	else{
char	c[256];
		
        name(c);
_tm_("application component "+c+" loaded");
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdCompLib::bStdCompLib(const char* name){
    _status=_bStdExtLibNoErr_;
    
    _bndl=NULL;
    _version=NULL;
    _name=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
    _ialloc=NULL;
    _ifree=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdCompLib::~bStdCompLib(){
	if(_bndl){
		CFRelease(_bndl);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCompLib::vers(char* c){
	CFStringGetCString(_version,c,256,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCompLib::name(char* c){
	CFStringGetCString(_name,c,256,kCFStringEncodingMacRoman);
}


// ---------------------------------------------------------------------------
// 
// -----------
void* bStdCompLib::i_allocate(int signature, void* app, void* prm){
	return(_ialloc(signature,app,prm));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCompLib::i_free(void* inst){
	_ifree(inst);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdCompLib::status(){
	if(_status){
		return(-(_bStdExtLibRootErr_+_status));
	}
	return(noErr);
}
