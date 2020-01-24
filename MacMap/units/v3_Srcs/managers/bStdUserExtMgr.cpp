//----------------------------------------------------------------------------
// File : bStdUserExtMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for user externs management
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2018 Carte Blanche Conseil.
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
// 05/12/2005 creation.
// 09/11/2018 64Bits
//----------------------------------------------------------------------------

#include "bStdUserExtMgr.h"
#include "bMacMapApp.h"
#include "bMacMapEventMgr.h"
#include "bDummyExtLib.h"
#include <mox_intf/bXMLFile.h>
#include <std_ext/bStdUserExt.h>
#include <std_ext/bStdUserDummy.h>
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserExtMgr	::bStdUserExtMgr(	UInt32 sign, 
									bool use_current, 
									bool need_idle, 
									bool need_events, 
									long index,
									const char* container)
				:bStdExtMgr(sign,use_current,need_idle,need_events,index)
				,_uelts(sizeof(bGenericExt*)){
	strncpy(_container,container,_names_length_max_);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserExtMgr::~bStdUserExtMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUserExtMgr::load(){
int	status=bStdExtMgr::load();
	if(status){
		return(status);
	}
int		sign=_sign;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(UInt32));
#endif

bDummyExtLib	dummy("dummy",sign);
#pragma unused(dummy)
bDummyExtLib*	pdummy=&dummy;
long	k=_elts.search(&pdummy,comp_signature);
	if(k){
		_elts.rmv(k);
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserExtMgr::open(){
	bStdExtMgr::open();
	if(int status=usr_load()){
#pragma unused(status)
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserExtMgr::close(){
	if(int status=usr_unload()){
#pragma unused(status)
	}
	bStdExtMgr::close();
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdUserExtMgr::count(){
	return(bStdExtMgr::count()+_uelts.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdUserExtMgr::get(long idx){
bGenericExt*	ext=bStdExtMgr::get(idx);
	if(ext){
		return(ext);
	}
	if(_uelts.get(idx-_elts.count(),&ext)){
		return(ext);
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUserExtMgr::usr_load(){
_bTrace_("bStdUserExtMgr::usr_load",true);
_tm_("usr_load for manager "+(UInt32*)&_sign);
char			pth[FILENAME_MAX];
char			name[FILENAME_MAX];
int				sign=_sign;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(UInt32));
#endif
	sprintf(pth,"%.4s",(char*)&sign);

bGenericExt*	ext;	
int				flg=0;
int				n=map_doc->countParams(pth);

	for(int i=1;i<=n;i++){
		map_doc->getParamName(pth,i,name);
		if(!strstr(name,".xml")){
			continue;
		}
		if(!map_doc->readXMLParam((bGenericXMLBaseElement**)&ext,pth,name)){
_te_("map_doc->readXMLParam failed");
			continue;
		}		
		ext->open(&flg);
		_uelts.add(&ext);
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUserExtMgr::usr_unload(){
_bTrace_("bStdExtMgr::usr_unload",true);
_tm_("lib signature = "+(UInt32*)&_sign);
bGenericExt*	ext;
	
	for(int i=1;i<=_uelts.count();i++){
		_uelts.get(i,&ext);
		delete ext;
	}

	_uelts.reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdUserExtMgr::find(const char* name){
bGenericExt*	ext=bStdExtMgr::find(name);
	if(ext){
		return(ext);
	}
bStdUserDummy	dummy(_MMAPP_,name);
bStdUserDummy*	pdummy=&dummy;
long			k=_uelts.search(&pdummy,comp_uname);
	if(k){
		if(_uelts.get(k,&ext)){
			return(ext);
		}
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bStdUserExtMgr::find(UInt32 signature){
bGenericExt*	ext=bStdExtMgr::find(signature);
	if(ext){
		return(ext);
	}
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&signature,sizeof(UInt32));
#endif

bStdUserDummy	dummy(_MMAPP_,signature);
bStdUserDummy*	pdummy=&dummy;
long			k=_uelts.search(&pdummy,comp_usignature);
	if(k){
		if(_uelts.get(k,&ext)){
			return(ext);
		}
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserExtMgr::ext_name(long idx, char* name){
	if(idx<=_elts.count()){
		bStdExtMgr::ext_name(idx,name);
		return;
	}
bGenericExt*	ext=get(idx);
	if(!ext){
		return;
	}
user_ext_data	p;
	ext->process(kExtProcessCallGetData,&p);
	strcpy(name,p.name);
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 bStdUserExtMgr::ext_signature(long idx){
	if(idx<=_elts.count()){
		return(bStdExtMgr::ext_signature(idx));
	}
bGenericExt*	ext=get(idx);
	if(!ext){
		return(_kNULLSign_);
	}
user_ext_data	p;
	ext->process(kExtProcessCallGetData,&p);
	return(p.sign);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExtMgr::add(bGenericXMLBaseElement* root){
_bTrace_("bStdUserExtMgr::add",true);
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,1,"name");
	if(!elt){
_te_("no name");
		return(false);
	}
char					name[_values_length_max_];
	elt->getvalue(name);
int	sign=_sign;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(UInt32));
#endif	
	if(!map_doc->writeTree(root,sign,name)){
_te_("writeTree failed");
		return(false);
	}
bGenericExt*	ext=(bGenericExt*)(void*)root;
int				flg;
	ext->open(&flg);
	_uelts.add(&ext);
	eventmgr->create(ext);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExtMgr::remove(long idx){
_bTrace_("bStdUserExtMgr::remove",true);
bGenericExt*	ext;
	if(!_uelts.get(idx-_elts.count(),&ext)){
_te_("range error "+(idx-_elts.count())+"->[1.."+_uelts.count()+"]");
		return(false);
	}
char	name[FILENAME_MAX];
	ext_name(idx,name);
int	sign=_sign;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(UInt32));
#endif	
	if(!map_doc->removeTree(sign,name)){
_te_("removeTree failed");
		return(false);
	}
	_uelts.rmv(idx-_elts.count());
	ext->close();
	eventmgr->kill(ext);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExtMgr::modify(long idx, bGenericXMLBaseElement* root){
_bTrace_("bStdUserExtMgr::modify",true);
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,1,"name");
	if(!elt){
_te_("no name");
		return(false);
	}

bGenericExt*	ext;
	if(!_uelts.get(idx-_elts.count(),&ext)){
_te_("range error "+(idx-_elts.count())+"->[1.."+_uelts.count()+"]");
		return(false);
	}
char	name[_values_length_max_];
	ext_name(idx,name);
int	sign=_sign;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(UInt32));
#endif	
	if(!map_doc->removeTree(sign,name)){
_te_("removeTree failed");
		return(false);
	}
	
	elt->getvalue(name);
	if(!map_doc->writeTree(root,sign,name)){
_te_("writeTree failed");
		return(false);
	}
	ext=(bGenericExt*)(void*)root;
int				flg;
	ext->open(&flg);
	_uelts.put(idx-_elts.count(),&ext);
	eventmgr->modify(ext);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdUserExtMgr::ext_index(const char* name){
long	k=bStdExtMgr::ext_index(name);
	if(k){
		return(k);
	}
bStdUserDummy	dummy(_MMAPP_,name);
bStdUserDummy*	pdummy=&dummy;
	k=_uelts.search(&pdummy,comp_uname);
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bStdUserExtMgr::ext_index(UInt32 signature){
long	k=bStdExtMgr::ext_index(signature);
	if(k){
		return(k);
	}
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&signature,sizeof(UInt32));
#endif
	
bStdUserDummy	dummy(_MMAPP_,signature);
bStdUserDummy*	pdummy=&dummy;
	k=_uelts.search(&pdummy,comp_usignature);
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUserExtMgr::comp_uname(const void* a, const void* b){
bStdUserExt* la=(*(bStdUserExt**)a);
bStdUserExt* lb=(*(bStdUserExt**)b);
user_ext_data	xa;
user_ext_data	xb;
	la->process(kExtProcessCallGetData,&xa);
	lb->process(kExtProcessCallGetData,&xb);
	return(strcmp(xa.name,xb.name));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bStdUserExtMgr::comp_usignature(const void* a, const void* b){
bStdUserExt* la=(*(bStdUserExt**)a);
bStdUserExt* lb=(*(bStdUserExt**)b);	
user_ext_data	xa;
user_ext_data	xb;
	la->process(kExtProcessCallGetData,&xa);
	lb->process(kExtProcessCallGetData,&xb);

	if(xa.sign>xb.sign){
		return(1);
	}
	if(xa.sign<xb.sign){
		return(-1);
	}
	return(0);
}

