//----------------------------------------------------------------------------
// File : bXMapMgr.cpp
// Project : MacMap
// Purpose : C++ source file : XMaps management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 18/01/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapMgr.h"
#include <mox_intf/bGenericExtLib.h>
#include <std_ext/bStdXMap.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapMgr::bXMapMgr()
		:bStdExtMgr(kXMLSubClassExtXMap,false,true,false,0)
		,_evts(sizeof(bGenericExtLib*))
		,_idle(sizeof(bGenericExtLib*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMgr::~bXMapMgr(){
	_idle.reset();
	_evts.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapMgr::open(int idx){
_bTrace_("bXMapMgr::open",false);
//bGenericXMap*	ext=(bGenericXMap*)(void*)get(idx);
bStdXMap*	ext=(bStdXMap*)(void*)get(idx);
	if(!ext){
_te_("get failed for "+idx);
		return;
	}
int		flags;
char	name[_names_length_max_];
	ext->getclassname(name);
_tm_("opening "+name+":"+(void*)ext);
	ext->open(&flags);
	
	if(flags&kXMapNeedMenu){
		// bah quoi ?
	}
	
	if(flags&kXMapNeedIdle){
_tm_("XMap "+name+" need idle");
		_idle.add(&ext);
	}
	
	if(flags&kXMapNeedEvents){
_tm_("XMap "+name+" need events");
		_evts.add(&ext);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapMgr::close(){
	bStdExtMgr::close();
	_evts.reset();
	_idle.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapMgr::event(EventRef evt){
//_bTrace_("bXMapMgr::event",true);
	if(_evts.count()<=0){
//_tm_("_evts.count()<=0");
		return(false);
	}	
bool		b=false;
bStdXMap*	ext;
char		name[256];

	for(long i=1;i<=_evts.count();i++){
		_evts.get(i,&ext);
		if(ext){
			ext->getclassname(name);
//_tm_(name);
			b=ext->event(evt);
			if(b){
//_tm_(name+" break");
				break;
			}
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapMgr::event(void* nsevt){
	if(_evts.count()<=0){
		return(false);
	}	
bool		b=false;
bStdXMap*	ext;
char		name[256];
	
	for(long i=1;i<=_evts.count();i++){
		_evts.get(i,&ext);
		if(ext){
			ext->getclassname(name);
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
void bXMapMgr::idle(){
//_bTrace_("bXMapMgr::idle",true);
	if(_idle.count()<=0){
//_tm_("_idle.count = %d",_idle.count());
		return;
	}
	
bStdXMap*	ext;
	for(long i=1;i<=_idle.count();i++){
		_idle.get(i,&ext);
		if(ext){
			ext->idle(NULL);
		}
	}
}
