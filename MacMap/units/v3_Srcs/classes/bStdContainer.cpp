//----------------------------------------------------------------------------
// File : bStdContainer.cpp
// Project : MacMap
// Purpose : C++ source file : Standard container base class for styles and views management
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
// 08/02/2005 creation.
//----------------------------------------------------------------------------

#include "bStdContainer.h"
#include "bMacMapApp.h"
#include "bClassesMgr.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdContainer::bStdContainer(const char* file_name){
	_root=NULL;
	_status=noErr;
	strcpy(_file_name,file_name);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdContainer::bStdContainer(const char* file_name, bStdContainer& v){
	_root=NULL;
	_status=noErr;
	strcpy(_file_name,v._file_name);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdContainer::bStdContainer(const char* file_name, bGenericXMLBaseElement* root){
	_root=root;
	_status=noErr;
	strcpy(_file_name,file_name);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdContainer::bStdContainer(const char* file_name, bool dummy){
	_root=NULL;
	_status=noErr;
	strcpy(_file_name,file_name);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdContainer::~bStdContainer(){
	unload();
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdContainer::status(){
	return(_status);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdContainer::get_name(char* name){
	if(_root){
bGenericXMLBaseElement* elt=clssmgr->NthElement(_root,1,"name");
		if(!elt){
			sprintf(name,"????");
			return;
		}
		elt->getvalue(name);
		if(name[0]==0){	
			sprintf(name,"????");
		}
	}
	else{
		sprintf(name,"????");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdContainer::set_name(const char* name){
_bTrace_("bStdContainer::set_name",false);
_tw_("DEPRECATED");
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdContainer::file_name(char* name){
	strcpy(name,_file_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bStdContainer::root(){
	return(_root);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdContainer::dump(){
_bTrace_("bStdContainer::dump",false);
_tw_("DEPRECATED");
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdContainer::destroy(){
_bTrace_("bStdContainer::destroy",false);
_tw_("DEPRECATED");
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdContainer::load(){
_bTrace_("bStdContainer::load",true);
_tw_("DEPRECATED");
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdContainer::unload(){
	if(_root){
		clssmgr->ReleaseXMLInstance(_root);
		_root=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdContainer::set_root_name(const char* name){
_bTrace_("bStdContainer::set_root_name",false);
	if(!_root){
		return(false);
	}
	if(strlen(name)>252){
		return(false);
	}
	
bGenericXMLBaseElement* elt=clssmgr->NthElement(_root,1,"name");
	if(!elt){
_te_("clssmgr->NthElement == NULL for \"name\"");
		return(false);
	}
	elt->setvalue(name);
	return(true);
}
