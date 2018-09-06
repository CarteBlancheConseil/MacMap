//----------------------------------------------------------------------------
// File : bStdExtLib.cpp
// Project : MacMap
// Purpose : C++ source file : Extern plugin class
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

#include "bStdExtLib.h"
#include "bSplashWd.h"
#include <mox_intf/endian.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_errors.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdExtLib::bStdExtLib(CFBundleRef bndl,
                       bGenericMacMapApp* gapp){
_bTrace_("bStdExtLib::bStdExtLib",false);
char	path[PATH_MAX];
UInt32	lsign=_kNULLSign_;

	_status=_bStdExtLibNoErr_;
	_bndl=bndl;
	_family=_kNULLSign_;
	_creator=_kNULLSign_;
	_version=NULL;
	_name=NULL;
	_sign=_kNULLSign_;
	_mmvers=NULL;
	_init=NULL;
	_destroy=NULL;
	_inst=NULL;
	
	for(;;){
		CFBundleGetPackageInfo(_bndl,&_family,&_creator);
		_version=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("CFBundleVersion"));
		_name=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("CFBundleName"));
				
CFStringRef	cfs;
		
		cfs=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("MacMapLibSign"));
		_sign=(cfs)?(*((int*)CFStringGetCStringPtr(cfs,0))):_kNULLSign_;
		_mmvers=(CFStringRef)CFBundleGetValueForInfoDictionaryKey(_bndl,CFSTR("MacMapMinVers"));

CFURLRef	url=CFBundleCopyBundleURL(_bndl);

		cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
		CFStringGetCString(cfs,path,PATH_MAX,kCFStringEncodingUTF8);
		CFRelease(cfs);
		
		if(_family==_kNULLSign_){
			_status=_bStdExtLibNoFamilyErr_;
			break;
		}
		if(_creator==_kNULLSign_){
			_status=_bStdExtLibNoCreatorErr_;
			break;
		}
		if(_version==NULL){
			_status=_bStdExtLibNoVersErr_;
			break;
		}
		if(_name==NULL){
			break;
		}
		if(_sign==_kNULLSign_){
			_status=_bStdExtLibNoSignatureErr_;
			break;
		}
		if(_mmvers==NULL){
			_status=_bStdExtLibNoMacMapVersErr_;
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
		
		_init=(initPtr)CFBundleGetFunctionPointerForName(bndl,CFSTR("init"));
		if(!_init){
			_status=_bStdExtLibNoInitProcErr_;
			break;
		}
		_destroy=(destroyPtr)CFBundleGetFunctionPointerForName(bndl,CFSTR("destroy"));
		if(!_destroy){
			_status=_bStdExtLibNoDestroyProcErr_;
			break;
		}
		_init(&_inst,gapp,_bndl);
		if(!_inst){
			_status=_bStdExtLibNoInstanceErr_;
			break;
		}
		break;
	}
	
	if(_status){
		_status+=_bStdExtLibRootErr_;
_te_("bStdExtLib::bStdExtLib returns "+_status);
	}
	else{
char	c[256];
		name(c);
        (void)bSplashWd::instance(false,gapp)->set_progress(c);
// DEBUG<
        MacRoman2UTF8(c,256);
		lsign=_sign;
#ifdef __LITTLE_ENDIAN__
        LBSwapWord(&lsign,sizeof(int));
#endif
_tm_(c+" "+(unsigned int*)(&_family)+"\t"+(unsigned int*)(&lsign)+" loaded");
// DEBUG>
	}
	
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdExtLib::~bStdExtLib(){
	if(_destroy&&_inst){
		_destroy(_inst);
	}
	if(_bndl){
		CFRelease(_bndl);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtLib::status(){
	if(_status){
		return(-(_bStdExtLibRootErr_+_status));
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtLib::family(){
	return(_family);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtLib::creator(){
	return(_creator);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtLib::vers(char* c){
	CFStringGetCString(_version,c,256,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtLib::name(char* c){
	CFStringGetCString(_name,c,256,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtLib::signature(){
	return(_sign);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtLib::macmap_vers(char* c){
	CFStringGetCString(_mmvers,c,256,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdExtLib::inst(){
	return(_inst);
}
