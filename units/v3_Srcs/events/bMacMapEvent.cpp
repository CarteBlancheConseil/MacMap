//----------------------------------------------------------------------------
// File : bMacMapEvent.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap event management class
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
// 17/08/2005 creation.
//----------------------------------------------------------------------------

#include "bMacMapEvent.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
#ifdef __LITTLE_ENDIAN__
#define kUndoSignature 'odnu'
#define kRedoSignature 'oder'
#else
#define kUndoSignature kHICommandUndo
#define kRedoSignature kHICommandRedo
#endif

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapEvent::_last_id=0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapEvent	::bMacMapEvent(	char* msg, 
								int crt,
								int knd,
								int act,
								int esz)
				:_elts(esz){
	_last_id++;
	_id=_last_id;
	strncpy(_msg,msg,256);
	_creator=crt;
	_kind=knd;
	_action=act;
	_closed=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMacMapEvent::~bMacMapEvent(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEvent::eid(){
	return(_id);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEvent::kind(){
	return(_kind);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEvent::action(){
	return(_action);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEvent::creator(){
	return(_creator);
}

// ---------------------------------------------------------------------------
// 
// -----------
char* bMacMapEvent::message(){
	return(_msg);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEvent::is_undo(){
	return(_creator==kUndoSignature);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEvent::is_redo(){
	return(_creator==kRedoSignature);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEvent::add(void* elt){
	if(_closed){
		return(false);
	}
	return(_elts.add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEvent::add(void* elt, int prm){
	if(_closed){
		return(false);
	}
	return(_elts.add(elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEvent::find(void* elt){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEvent::close(){
	_closed=true;
}

// ---------------------------------------------------------------------------
// 
// -----------
bArray* bMacMapEvent::elements(){
	return(&_elts);
}
