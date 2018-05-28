//----------------------------------------------------------------------------
// File : bTypeEvent.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap type event class
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
// 22/08/2005 creation.
//----------------------------------------------------------------------------

#include "bTypeEvent.h"
#include "bMacMapType.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTypeEvent	::bTypeEvent(	char* msg, 
							int crt,
							int act)
			:bMacMapEvent(	msg,
							crt,
							kEventKindTypeElement,
							act,
							sizeof(bGenericType*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTypeEvent::~bTypeEvent(){
    _elts.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTypeEvent::add(void* elt){
	return(bMacMapEvent::add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypeEvent::find(void* elt){
	return(find((bGenericType*)elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypeEvent::find(bGenericType* t){
    return(_elts.search(&t,compare));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bTypeEvent::compare(const void* a, const void* b){
bGenericType* ta=(*((bGenericType**)a));
bGenericType* tb=(*((bGenericType**)b));
	if(ta>tb){
		return(1);
	}
	if(ta<tb){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTypeCreateEvent	::bTypeCreateEvent(	char* msg, 
										int crt)
					:bTypeEvent(	msg,
									crt,
									kEventActionCreate){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTypeCreateEvent::~bTypeCreateEvent(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTypeDeleteEvent	::bTypeDeleteEvent(	char* msg, 
										int crt)
					:bTypeEvent(	msg,
									crt,
									kEventActionDestroy){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTypeDeleteEvent::~bTypeDeleteEvent(){
bMacMapType* t;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&t);
		delete t;
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTypeModifyEvent	::bTypeModifyEvent(	char* msg, 
										int crt)
					:bTypeEvent(	msg,
									crt,
									kEventActionModify){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTypeModifyEvent::~bTypeModifyEvent(){
}

