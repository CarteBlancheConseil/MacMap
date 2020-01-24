//----------------------------------------------------------------------------
// File : bStdExtMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for externs management
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004-2018 Carte Blanche Conseil.
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
// 14/12/2004 creation.
// 09/11/2018 64Bits
//----------------------------------------------------------------------------

#include "bStdExtMgr.h"
#include "bDummyExtLib.h"
#include "bMacMapApp.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdExtMgr	::bStdExtMgr(	UInt32 sign, 
							bool use_current, 
							bool need_idle, 
							bool need_events, 
							long index)
			:_elts(sizeof(bGenericExtLib*)){
	
	_sign=sign;
	_use_current=use_current;
	_need_idle=need_idle;
	_need_events=need_events;
	_index=(_use_current)?index:-1;
	
	_closed=true;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdExtMgr::~bStdExtMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtMgr::load(){
_bTrace_("bStdExtMgr::load",true);
_tm_("lib signature = "+(unsigned int*)&_sign);
OSStatus	err;

	if((err=extmgr->getlibs(_elts,_sign))){
_te_(err+" at getlibs");
		return(err);
	}
	_elts.sort(comp_signature);

_tm_((int)_elts.count()+" "+(UInt32*)&_sign);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtMgr::unload(){
_bTrace_("bStdExtMgr::unload",true);
_tm_("lib signature = "+(unsigned int*)&_sign);
bGenericExtLib*		ext;
	
	for(long i=1;i<=_elts.count();i++){
		_elts.get(i,&ext);
		delete ext;
	}

	_elts.reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::open(long idx){
_bTrace_("bStdExtMgr::open",false);
bGenericExt*	ext=get(idx);
	if(!ext){
_te_("get failed for "+idx);
		return;
	}
int		flags;
char	name[_names_length_max_];
	ext->getclassname(name);
_tm_("opening "+name+" "+(void*)ext);
	ext->open(&flags);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::open(){
_bTrace_("bStdExtMgr::open",true);
_tm_("open for manager "+(unsigned int*)&_sign);
	for(long i=1;i<=_elts.count();i++){
		open(i);
	}
	_closed=false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::close(){
_bTrace_("bStdExtMgr::close",true);
bGenericExt*	ext;
char			name[_names_length_max_];
	
	for(long i=1;i<=_elts.count();i++){
		ext=get(i);
		ext->getclassname(name);
_tm_("closing "+name);
		ext->close();
	}
	_closed=true;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::event(void* nsevt){
	if(_closed){
	}
	
	if(!_need_events){
		return(false);
	}
	if(_elts.count()<=0){
		return(false);
	}
	
bool			b=false;
long			i,n;
bGenericExt*	ext;
	
	if(_use_current){
		i=_index;
		n=_index;
	}
	else{
		i=1;
		n=_elts.count();
	}
	for(;i<=n;i++){
		ext=get(i);
		if(ext){
			b=ext->nsevent(nsevt);
			if(b){
				break;
			}
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::idle(){
//_bTrace_("bStdExtMgr::idle",true);
//_tm_((char*)&_sign);
	if(!_need_idle){
		return;
	}
	if(_elts.count()<=0){
//_tm_("no ext");
		return;
	}
	
long			i,n;
bGenericExt*	ext;
	
	if(_use_current){
		i=_index;
		n=_index;
	}
	else{
		i=1;
		n=_elts.count();
	}
//_tm_("idle from "+i+" to "+n);

	for(;i<=n;i++){
		ext=get(i);
		if(ext){
			ext->idle(NULL);
		}
	}
	IdleApp();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::edit(bGenericExt* ext, void* prm){
_bTrace_("bStdExtMgr::edit(bGenericExt* ext, void* prm)",true);
	return(ext->edit(prm));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::edit(const char* name, UInt32 signature, void* prm){
_bTrace_("bStdExtMgr::edit(char* name, UInt32 signature, void* prm)",true);
_tm_("edit "+(UInt32*)&signature);

	if(_elts.count()<=0){
_tm_("_elts.count()<=0");	
		return(false);
	}
bGenericExt*	ext=find(signature);
	if(!ext){
_tm_("find with signature == NULL");
		ext=find(name);
	}
	if(!ext){
_tm_("find with name == NULL");
		return(false);
	}
	return(edit(ext,prm));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::set_current(long index){
//_bTrace_("bStdExtMgr::set_current",true);
//_tm_("current set to "+index);
	if((_use_current)&&(index<=count())&&(index>0)){
		_index=index;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdExtMgr::get_current(){
	return(_index);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdExtMgr::count(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdExtMgr::get(){
	return((_use_current)?get(_index):NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdExtMgr::get(long idx){
bGenericExtLib*	ext;
	
	if(_elts.get(idx,&ext)){
		return(ext->inst());
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdExtMgr::find(const char* name){
bDummyExtLib	dummy(name,0);
bDummyExtLib*	pdummy=&dummy;
	_elts.setsorted(false);
long	k=_elts.search(&pdummy,comp_name);
	_elts.setsorted(true);
	if(k){
		return(get(k));
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdExtMgr::find(UInt32 signature){
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&signature,sizeof(UInt32));
#endif

bDummyExtLib	dummy("dummy",signature);
bDummyExtLib*	pdummy=&dummy;
long	k=_elts.search(&pdummy,comp_signature);
	if(k){
		return(get(k));
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdExtMgr::ext_name(long idx, char* name){
bGenericExtLib*	ext;
	
	if(_elts.get(idx,&ext)){
		ext->name(name);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 bStdExtMgr::ext_signature(long idx){
bGenericExtLib*	ext;
	
	if(_elts.get(idx,&ext)){
		return(ext->signature());
	}
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::add(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::remove(long idx){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdExtMgr::modify(long idx, bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdExtMgr::ext_index(const char* name){
bDummyExtLib	dummy(name,0);
bDummyExtLib*	pdummy=&dummy;

	_elts.setsorted(false);
long	k=_elts.search(&pdummy,comp_name);
	_elts.setsorted(true);
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdExtMgr::ext_index(UInt32 signature){
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&signature,sizeof(UInt32));
#endif
	
bDummyExtLib	dummy("dummy",signature);
bDummyExtLib*	pdummy=&dummy;
	return(_elts.search(&pdummy,comp_signature));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdExtMgr::comp_name(const void* a, const void* b){
bGenericExtLib* la=(*(bGenericExtLib**)a);
bGenericExtLib* lb=(*(bGenericExtLib**)b);
char ca[256];
char cb[256];
	
	lb->name(cb);
	la->name(ca);
	return(strcmp(cb,ca));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bStdExtMgr::comp_signature(const void* a, const void* b){
bGenericExtLib* la=(*(bGenericExtLib**)a);
bGenericExtLib* lb=(*(bGenericExtLib**)b);

UInt32		sa=la->signature();
UInt32		sb=lb->signature();
	
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sa,sizeof(UInt32));
	LBSwapWord(&sb,sizeof(UInt32));
#endif
	
	if(sa>sb){
		return(1);
	}
	if(sa<sb){
		return(-1);
	}
	return(0);
}


