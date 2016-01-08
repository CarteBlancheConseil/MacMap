//----------------------------------------------------------------------------
// File : bFieldEvent.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap field (database) event class
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

#include "bFieldEvent.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bFieldEvent	::bFieldEvent(	char* msg, 
							int crt,
							int act)
			:bMacMapEvent(	msg,
							crt,
							kEventKindDataStruct,
							act,
							sizeof(field_info)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bFieldEvent::~bFieldEvent(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bFieldEvent::add(void* elt){
/*field_info* fi=(field_info*)elt;
	if(find(fi)){
		return(true);
	}*/
	return(bMacMapEvent::add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bFieldEvent::find(void* elt){
	return(find((field_info*)elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bFieldEvent::find(field_info* f){
	return(_elts.search(f,compare));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bFieldEvent::compare(const void* a, const void* b){
field_info* fa=(field_info*)a;
field_info* fb=(field_info*)b;
	if(fa->tp>fb->tp){
		return(1);
	}
	if(fa->tp<fb->tp){
		return(-1);
	}
	if(fa->fid>fb->fid){
		return(1);
	}
	if(fa->fid<fb->fid){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bFieldCreateEvent	::bFieldCreateEvent(	char* msg, 
											int crt)
					:bFieldEvent(	msg,
									crt,
									kEventActionCreate){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bFieldCreateEvent::~bFieldCreateEvent(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bFieldDeleteEvent	::bFieldDeleteEvent(	char* msg, 
											int crt)
					:bFieldEvent(	msg,
									crt,
									kEventActionDestroy){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bFieldDeleteEvent::~bFieldDeleteEvent(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bFieldModifyEvent	::bFieldModifyEvent(	char* msg, 
											int crt)
					:bFieldEvent(	msg,
									crt,
									kEventActionModify){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bFieldModifyEvent::~bFieldModifyEvent(){
}
