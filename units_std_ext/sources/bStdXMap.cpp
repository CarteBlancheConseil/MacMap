//----------------------------------------------------------------------------
// File : bStdXMap.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for XMaps
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#include "bStdXMap.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMap::bStdXMap(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		: bStdExt(elt,gapp,bndl){
	_flgs=0;
	_subclss=kXMLSubClassExtXMap;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMap::~bStdXMap(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMap::open(int* flags){
	(*flags)=_flgs;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMap::close(){
}

// ---------------------------------------------------------------------------
// A supprimer
// -----------
bool bStdXMap::event(EventRef evt){	
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMap::nsevent(void* nsevt){	
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMap::idle(void* prm){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMap::process(int msg, void* prm){
bool b=false;
	switch(msg){
		case kExtProcessCallWithParams:
			break;
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMap::test(void* prm){
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMap::edit(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMap::set_flags(int flags){
	_flgs=flags;
}



