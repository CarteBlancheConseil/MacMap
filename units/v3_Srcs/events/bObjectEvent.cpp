//----------------------------------------------------------------------------
// File : bObjectEvent.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap object event class
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
// 22/08/2005 creation.
//----------------------------------------------------------------------------

#include "bObjectEvent.h"
#include "bMacMapGeoElement.h"
#include "bMacMapModifiedGeoElement.h"
#include "bMacMapApp.h"
#include <MacMapSuite/wtable.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bObjectEvent	::bObjectEvent(	char* msg, 
								int crt,
								int act)
				:bMacMapEvent(	msg,
								crt,
								kEventKindGeoElement,
								act,
								sizeof(bGenericGeoElement*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bObjectEvent::~bObjectEvent(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bObjectEvent::add(void* elt){
//bGenericGeoElement* oi=(*((bGenericGeoElement**)elt));
//	if(find(oi)){
//		return(true);
//	}
	return(bMacMapEvent::add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bObjectEvent::find(void* elt){
	return(find((bGenericGeoElement*)elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bObjectEvent::find(bGenericGeoElement* o){
	return(_elts.search(&o,compare));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bObjectEvent::compare(const void* a, const void* b){
bGenericGeoElement* oa=(*((bGenericGeoElement**)a));
bGenericGeoElement* ob=(*((bGenericGeoElement**)b));
	if(oa>ob){
		return(1);
	}
	if(oa<ob){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bObjectCreateEvent	::bObjectCreateEvent(	char* msg, 
											int crt)
					:bObjectEvent(	msg,
									crt,
									kEventActionCreate){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bObjectCreateEvent::~bObjectCreateEvent(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bObjectDeleteEvent	::bObjectDeleteEvent(	char* msg, 
											int crt)
					:bObjectEvent(	msg,
									crt,
									kEventActionDestroy){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bObjectDeleteEvent::~bObjectDeleteEvent(){
bMacMapGeoElement* o;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&o);
		if(o->killed()){
			delete o;
		}
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bObjectModifyEvent	::bObjectModifyEvent(	char* msg, 
											int crt)
					:bObjectEvent(	msg,
									crt,
									kEventActionModify){
//	_tbl=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bObjectModifyEvent::~bObjectModifyEvent(){
bMacMapModifiedGeoElement* o;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&o);
		delete o;
	}
/*	if(_tbl){
		wtbl_free(_tbl);
	}*/
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bObjectModifyEvent::add(void* elt, int fld){
bGenericGeoElement* oi=(*((bGenericGeoElement**)elt));
//	if(!_tbl){
//		clone_tbl(oi);
//	}
//	if(find(oi)){
//		return(true);
//	}
bGenericGeoElement* mi=new bMacMapModifiedGeoElement(oi,fld);
	return(bMacMapEvent::add(&mi));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bObjectModifyEvent::find(void* elt){
	return(find((bGenericGeoElement*)elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bObjectModifyEvent::find(bGenericGeoElement* o){
	return(_elts.search(&o,compare));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bObjectModifyEvent::compare(const void* a, const void* b){
bGenericGeoElement* oa=(*((bGenericGeoElement**)a));
bGenericGeoElement* ob=(*((bMacMapModifiedGeoElement**)b))->get_ref();
	if(oa>ob){
		return(1);
	}
	if(oa<ob){
		return(-1);
	}
	return(0);
}

