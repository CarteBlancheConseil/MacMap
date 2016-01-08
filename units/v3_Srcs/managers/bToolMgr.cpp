//----------------------------------------------------------------------------
// File : bToolMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Tools management class
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
// 18/11/2004 creation.
//----------------------------------------------------------------------------

#include "bToolMgr.h"
#include <mox_intf/bGenericTool.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolMgr::bToolMgr()
		:bStdExtMgr(kXMLSubClassExtTool,true,false,true,1){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolMgr::~bToolMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bToolMgr::load(){
int	err=bStdExtMgr::load();
	if(err){
		return(err);
	}
bGenericTool*	t;
	for(int i=1;i<=count();i++){
		t=(bGenericTool*)(void*)get(i);
		if(t){
			if(t->tool_class()==kToolNav){
				set_current(i);
				break;
			}
		}
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMgr::open(){
bGenericTool*	t;
	for(int i=1;i<=count();i++){
		t=(bGenericTool*)(void*)get(i);
		if(t){
			if(t->tool_class()==kToolNav){
				set_current(i);
				break;
			}
		}
	}
	bStdExtMgr::open();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMgr::close(){
	bStdExtMgr::close();
	set_current(0);
}

// ---------------------------------------------------------------------------
// Certains tools peuvent gérer une trace, même désactivé
// -----------
bool bToolMgr::event(EventRef evt){
	if(_closed){
		return(false);
	}
	if(	(GetEventClass(evt)==kEventClassWindow)			&&
		(GetEventKind(evt)==kEventWindowDrawContent)	){
bGenericTool*	ext;
		for(int i=1;i<=count();i++){
			ext=(bGenericTool*)(void*)get(i);
			if(ext){
				(void)ext->event(evt);
			}
		}
		return(true);
	}
	return(bStdExtMgr::event(evt));
}

// ---------------------------------------------------------------------------
// Certains tools peuvent gérer une trace, même désactivé
// -----------
bool bToolMgr::event(void* nsevt){
	if(_closed){
		return(false);
	}
//	if((GetEventClass(evt)==kEventClassWindow)		&&
//	   (GetEventKind(evt)==kEventWindowDrawContent)	){
//bGenericTool*	ext;
//		for(int i=1;i<=count();i++){
//			ext=(bGenericTool*)(void*)get(i);
//			if(ext){
//				(void)ext->event(evt);
//			}
//		}
//		return(true);
//	}A traiter
	
	return(bStdExtMgr::event(nsevt));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMgr::set_current(int index){
//_bTrace_("bToolMgr::set_current",true);

bGenericTool*	t=(bGenericTool*)(void*)get();

bGenericExt*    e=t;
//_tm_((void*)t);
char name[256];
    e->getclassname(name);
//_tm_(name);
   
    if(t){
//_tm_("deactivate");
		t->deactivate();
	}
	if(index<=0){
		return;
	}
	bStdExtMgr::set_current(index);
	t=(bGenericTool*)(void*)get();
	if(t){
		t->activate();
//_tm_("activate");
	}
	else{
//_te_("tool not found");
	}
	
}
