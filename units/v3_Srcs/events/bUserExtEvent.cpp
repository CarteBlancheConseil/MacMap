//----------------------------------------------------------------------------
// File : bUserExtEvent.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap user extern event class
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

#include "bUserExtEvent.h"
#include <mox_intf/bGenericExt.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bUserExtEvent	::bUserExtEvent(	char* msg, 
									int crt,
									int act)
				:bMacMapEvent(	msg,
								crt,
								kEventKindUserExt,
								act,
								sizeof(bGenericExt*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bUserExtEvent::~bUserExtEvent(){
    _elts.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bUserExtEvent::add(void* elt){
	return(bMacMapEvent::add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bUserExtEvent::find(void* elt){
	return(find((bGenericExt*)elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bUserExtEvent::find(bGenericExt* t){
    return(_elts.search(&t,compare));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bUserExtEvent::compare(const void* a, const void* b){
bGenericExt* ta=(*((bGenericExt**)a));
bGenericExt* tb=(*((bGenericExt**)b));
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
bUserExtCreateEvent	::bUserExtCreateEvent(	char* msg, 
											int crt)
					:bUserExtEvent(	msg,
									crt,
									kEventActionCreate){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bUserExtCreateEvent::~bUserExtCreateEvent(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bUserExtDeleteEvent	::bUserExtDeleteEvent(	char* msg, 
											int crt)
					:bUserExtEvent(	msg,
									crt,
									kEventActionDestroy){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bUserExtDeleteEvent::~bUserExtDeleteEvent(){
bGenericExt* t;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&t);
		delete t;
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bUserExtModifyEvent	::bUserExtModifyEvent(	char* msg, 
											int crt)
					:bUserExtEvent(	msg,
									crt,
									kEventActionModify){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bUserExtModifyEvent::~bUserExtModifyEvent(){
}

