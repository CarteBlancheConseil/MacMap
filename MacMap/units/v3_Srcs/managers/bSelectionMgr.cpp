//----------------------------------------------------------------------------
// File : bSelectionMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Class for management of selection object list 
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 22/01/2004 creation.
//----------------------------------------------------------------------------

#include "bSelectionMgr.h"
#include "bLayersMgr.h"
#include "bMacMapApp.h"
#include "bMacMapGeoElement.h"
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSelectionMgr::bSelectionMgr():bObjListMgr(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bSelectionMgr::~bSelectionMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectionMgr::unload(){
//	layersmgr->DrawSelection(NULL);
	bObjListMgr::flush();
	report();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectionMgr::add(bGenericGeoElement* o){
	if(bObjListMgr::add(o)!=noErr){
		report();
		return(-1);
	}
	report();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectionMgr::add(bArray* l){
bGenericGeoElement*	o;
OSStatus			status=noErr;
bArray				b(*l);
	
//	layersmgr->DrawSelection(NULL);
	for(long i=b.count();i>0;i--){
		if(b.get(i,&o)){
			if(o->selected()){
				if(!b.rmv(i)){
					status=-1;
					break;
				}
			}
			else{
				o->setselect(true);
			}
		}
		else{
			status=-2;
			break;
		}
	}
	if(status){
		report();
		return(status);
	}
	status=bObjListMgr::add(&b);
//	layersmgr->DrawSelection(NULL);
	report();
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectionMgr::rmv(bGenericGeoElement* o){
	if(bObjListMgr::rmv(o)!=noErr){
		report();
		return(-1);
	}	
	report();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectionMgr::flush(){
long                n=count();
bGenericGeoElement*	o;
	
//	layersmgr->DrawSelection(NULL); 
	for(long i=n;i>0;i--){
		if(_elts->get(i,&o)){
			o->setselect(false);
		}
	}
	bObjListMgr::flush();
	
	report();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectionMgr::report(){
char				c1[256],c2[256],c3[256];
bGenericType*		tp;
bGenericGeoElement* o;

	if(bMacMapGeoElement::get_inval()==false){
		return;
	}
	
	switch(count()){
		case 0:
			message_string(kMsgNoSelection,c1,1);
			WriteMessage(_MMAPP_,c1);
			break;
		case 1:
			_elts->get(1,&o);
			o->getName(c1);
			tp=typesmgr->get(o->getType());
			if(!tp){
				WriteMessage(_MMAPP_,"");
				break;
			}
			if(tp->fields()->get_constraint(kOBJ_SubType_,o->getSubType(),c2)==false){
				strcpy(c2,"????");
			}
			sprintf(c3,"%s %s",c2,c1);
			WriteMessage(_MMAPP_,c3);
			break;
		default:			
			if(type_unique()){
				_elts->get(1,&o);
				tp=typesmgr->get(o->getType());
				if(!tp){
					WriteMessage(_MMAPP_,"");
					break;
				}
				tp->name(c1);
				message_string(kMsgPlural,c2,0);
				sprintf(c3,"%ld %s %s",count(),c2,c1);
				WriteMessage(_MMAPP_,c3);
			}
			else{				
				message_string(kMsgObjects,c1,0);
				message_string(kMsgTypes,c2,0);
				sprintf(c3,"%ld %s, %d %s",count(),c1,type_count(),c2);
				WriteMessage(_MMAPP_,c3);
			}
			break;
	}
//    _MMAPP_->menuMgr()->check_menus();
}

