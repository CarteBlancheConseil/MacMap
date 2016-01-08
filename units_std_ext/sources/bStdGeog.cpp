//----------------------------------------------------------------------------
// File : bStdGeog.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Geogs
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
// 28/01/2005 creation.
//----------------------------------------------------------------------------

#include "bStdGeog.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdGeog::	bStdGeog(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		:	bStdExt(elt,gapp,bndl){	
	_subclss=kXMLSubClassExtGeog;
	_root=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdGeog::~bStdGeog(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::open(int* flags){
	(*flags)=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::close(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdGeog::event(EventRef evt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdGeog::nsevent(void* nsevt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::idle(void* prm){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdGeog::process(int msg, void* prm){
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
bool bStdGeog::test(void* prm){
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdGeog::edit(void* prm){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdGeog::edit(WindowRef wd, Rect* r){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::end_edit(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::hide(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::show(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::get_params(bArray* arr, int indent){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdGeog::put_params(bGenericXMLBaseElement* root){
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLFormulaElement* bStdGeog::root(){
	return(_root);
}

