//----------------------------------------------------------------------------
// File : bStyleContainer.cpp
// Project : MacMap
// Purpose : C++ source file : Styles management classes
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
// 07/02/2005 creation.
//----------------------------------------------------------------------------

#include "bStyleContainer.h"
#include "bMacMapApp.h"
#include "bClassesMgr.h"
#include "bMacMapDocument.h"

#include <mox_intf/endian.h>
#include <mox_intf/Type_Utils.h>

#include <MacMapSuite/bTrace.h>

#pragma mark -> bStdStyleContainer
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdStyleContainer	::bStdStyleContainer(const char* file_name)
					:bStdContainer(file_name){
_bTrace_("bStdStyleContainer::bStdStyleContainer(char*)",false);
	_on_screen=false;
	_on_edit=false;
_tm_("loading "+_file_name);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdStyleContainer	::bStdStyleContainer(	const char* file_name, 
											bStdStyleContainer& v)
					:bStdContainer(file_name,v){
_bTrace_("bStdStyleContainer::bStdStyleContainer(char*,bStdStyleContainer&)",false);
	_on_screen=false;
	_on_edit=false;
_tm_("loading "+_file_name);	
}


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdStyleContainer	::bStdStyleContainer(	const char* file_name, 
											bGenericXMLBaseElement* root)
					:bStdContainer(file_name,root){
_bTrace_("bStdStyleContainer::bStdStyleContainer(char*,bGenericXMLBaseElement*)",false);
_tm_("_root="+(void*)_root);
_tm_(file_name);
	_on_screen=false;
	_on_edit=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdStyleContainer::~bStdStyleContainer(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::set_name(const char* name){
_bTrace_("bStdStyleContainer::set_name",false);
_tm_(name);
	if(!set_root_name(name)){
_te_("false for set_root_name :"+name);
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::dump(){
_bTrace_("bStdStyleContainer::dump",false);
_tm_(_file_name);
	if(!_root){
_te_("no _root for file "+_file_name);
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::destroy(){
_bTrace_("bStdStyleContainer::destroy",false);
	clssmgr->ReleaseXMLInstance(_root);
	_root=NULL;
	_status=-1;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleContainer::set_root(bGenericXMLBaseElement* root){
	clssmgr->ReleaseXMLInstance(_root);
	_root=root;
	if(!dump()){
	}
}
		
// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleContainer::signature(){
_bTrace_("bStdStyleContainer::signature",false);
	if(_root){
bGenericXMLBaseElement* elt=clssmgr->NthElement(_root,1,"sign");
		if(!elt){
			return(_kNULLSign_);
		}
char	value[_values_length_max_];
		elt->getvalue(value);
		if(value[0]==0){	
			return(_kNULLSign_);
		}
int		sgn=((*((int*)value)));
		return(sgn);
	}
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::save(){
_bTrace_("bStdStyleContainer::signature",false);
_tw_("DEPRECATED");
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::on_screen(){
	return(_on_screen);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleContainer::set_on_screen(bool b){
	_on_screen=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleContainer::on_edit(){
	return(_on_edit);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleContainer::set_on_edit(bool b){
	_on_edit=b;
}

#pragma mark -
#pragma mark -> bStyleContainer
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyleContainer	::bStyleContainer(	const char* file_name,
									bGenericType* tp)
				:bStdStyleContainer(file_name){
_bTrace_("bStyleContainer::bStyleContainer(char*,bGenericType*)",false);
	_tp=tp;
	load();
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyleContainer	::bStyleContainer(const char* file_name, bStdStyleContainer& v)
				:bStdStyleContainer(file_name,v){
_bTrace_("bStyleContainer::bStyleContainer(char*,bStdStyleContainer&)",false);
	_tp=((bStyleContainer*)&v)->_tp;
	load();
	strcpy(_file_name,file_name);
	if(_status){
		return;
	}
char	name[FILENAME_MAX];
	strcpy(name,file_name);
int k=strlen(name);
	name[k-4]=0;
	if(!set_root_name(name)){
		_status=-2;
		return;
	}
	if(!SetXMLParamToType(_tp,kStyleFolderName,_file_name,_root)){
_te_("SetXMLParamToType failed for "+_file_name);
		_status=-5;
	}
}


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyleContainer	::bStyleContainer(const char* file_name, bGenericXMLBaseElement* root, bGenericType* tp)
				:bStdStyleContainer(file_name,root){
_bTrace_("bStyleContainer::bStyleContainer(char*,bGenericXMLBaseElement*,bGenericType*)",false);
char	name[FILENAME_MAX];
	_tp=tp;
	strcpy(name,file_name);
int k=strlen(name);
	name[k-4]=0;
	if(!set_root_name(name)){
_te_("set_root_name");
		_status=-2;
		return;
	}
	if(!SetXMLParamToType(_tp,kStyleFolderName,_file_name,_root)){
_te_("SetXMLParamToType failed for "+_file_name);
		_status=-5;
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStyleContainer::~bStyleContainer(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleContainer::set_name(const char* name){
_bTrace_("bStyleContainer::set_name",false);
	if(!bStdStyleContainer::set_name(name)){
		return(false);
	}
char	new_name[256];
	sprintf(new_name,"%s.xml",name);
	if(!SetXMLParamToType(_tp,kStyleFolderName,new_name,_root)){
_te_("SetXMLParamToType failed for "+new_name);
		_status=-5;
		return(false);
	}
	if(!SetXMLParamToType(_tp,kStyleFolderName,_file_name,NULL)){
_te_("SetXMLParamToType failed for "+_file_name);
		_status=-5;
		return(false);
	}
	strcpy(_file_name,new_name);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleContainer::dump(){
_bTrace_("bStyleContainer::dump",false);
	if(!bStdStyleContainer::dump()){
		return(false);
	}
	if(!SetXMLParamToType(_tp,kStyleFolderName,_file_name,_root)){
_te_("SetXMLParamToType failed for "+_file_name);
		_status=-5;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleContainer::destroy(){
_bTrace_("bStyleContainer::destroy",false);
	if(!SetXMLParamToType(_tp,kStyleFolderName,_file_name,NULL)){
_te_("SetXMLParamToType(NULL) failed for "+_file_name);
		return(false);
	}
	return(bStdStyleContainer::destroy());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStyleContainer::load(){
_bTrace_("bStyleContainer::load",false);
	_root=NULL;
	_root=GetXMLParamFromType(_MMAPP_,_tp,kStyleFolderName,_file_name);
	if(!_root){
_te_("_root == NULL for "+_file_name);
		_status=-1;
	}
}

#pragma mark -
#pragma mark -> bVirtualStyleContainer
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVirtualStyleContainer	::bVirtualStyleContainer(const char* file_name)
						:bStdStyleContainer(file_name){	
_bTrace_("bVirtualStyleContainer::bVirtualStyleContainer(char*)",false);
	load();
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVirtualStyleContainer	::bVirtualStyleContainer(const char* file_name, bStdStyleContainer& v)
						:bStdStyleContainer(file_name,v){
_bTrace_("bVirtualStyleContainer::bVirtualStyleContainer(char*,bStdStyleContainer&)",false);
	load();
	strcpy(_file_name,file_name);
	if(_status){
		return;
	}
char	name[FILENAME_MAX];
	strcpy(name,file_name);
int k=strlen(name);
	name[k-4]=0;
	if(!set_root_name(name)){
		_status=-2;
		return;
	}
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	if(!doc->writeXMLParam(_root,kStyleFolderName,_file_name)){
_te_("map_doc->writeXMLParam failed for "+_file_name);
		_status=-5;
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVirtualStyleContainer	::bVirtualStyleContainer(const char* file_name, bGenericXMLBaseElement* root)
						:bStdStyleContainer(file_name,root){
_bTrace_("bVirtualStyleContainer::bVirtualStyleContainer(char*,bGenericXMLBaseElement*)",false);
char	name[FILENAME_MAX];
	strcpy(name,file_name);
int k=strlen(name);
	name[k-4]=0;
	if(!set_root_name(name)){
_te_("set_root_name");
		_status=-2;
		return;
	}
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	if(!doc->writeXMLParam(_root,kStyleFolderName,_file_name)){
_te_("map_doc->writeXMLParam failed for "+_file_name);
		_status=-5;
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bVirtualStyleContainer::~bVirtualStyleContainer(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleContainer::set_name(const char* name){
_bTrace_("bVirtualStyleContainer::set_name",false);
	if(!bStdStyleContainer::set_name(name)){
		return(false);
	}
char	new_name[256];
	sprintf(new_name,"%s.xml",name);
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	if(!doc->writeXMLParam(_root,kStyleFolderName,new_name)){
_te_("map_doc->writeXMLParam failed for "+new_name);
		_status=-5;
		return(false);
	}
	if(!doc->writeXMLParam(NULL,kStyleFolderName,_file_name)){
_te_("map_doc->writeXMLParam failed for "+_file_name);
		_status=-5;
		return(false);
	}
	strcpy(_file_name,new_name);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleContainer::dump(){
_bTrace_("bVirtualStyleContainer::dump",false);
	if(!bStdStyleContainer::dump()){
		return(false);
	}
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	if(!doc->writeXMLParam(_root,kStyleFolderName,_file_name)){
_te_("map_doc->writeXMLParam failed for "+_file_name);
		_status=-5;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleContainer::destroy(){
_bTrace_("bStyleContainer::destroy",false);
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	if(!doc->writeXMLParam(NULL,kStyleFolderName,_file_name)){
_te_("map_doc->writeXMLParam failed for "+_file_name);
		return(false);
	}
	return(bStdStyleContainer::destroy());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bVirtualStyleContainer::load(){
_bTrace_("bStyleContainer::load",false);
	_root=NULL;
bMacMapDocument* doc=(bMacMapDocument*)map_doc;
	doc->readXMLParam(&_root,kStyleFolderName,_file_name);
	if(!_root){
_te_("_root == NULL for "+_file_name);
		_status=-1;
	}
}
