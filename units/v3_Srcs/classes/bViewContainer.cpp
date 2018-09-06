//----------------------------------------------------------------------------
// File : bViewContainer.cpp
// Project : MacMap
// Purpose : C++ source file : Views management classes
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
// 02/02/2005 creation.
//----------------------------------------------------------------------------

#include "bViewContainer.h"
#include "bMacMapApp.h"
#include "bMacMapDocument.h"
#include "bClassesMgr.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bViewContainer	::bViewContainer(const char* file_name)
				:bStdContainer(file_name){
_bTrace_("bViewContainer::bViewContainer(char*)",false);
_tm_("loading "+_file_name);
	load();
	if(_status){
_te_("failed with "+_status);
		make();
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bViewContainer	::bViewContainer(const char* file_name, bViewContainer& v)
				:bStdContainer(file_name,v){
_bTrace_("bViewContainer::bViewContainer(char*,bViewContainer&)",false);
_tm_("loading "+_file_name);
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
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(_root,"views",_file_name)){
_te_("writeXMLParam failed for "+_file_name);
		_status=-5;
		return;
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bViewContainer::~bViewContainer(){
	unload();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewContainer::dump(){
_bTrace_("bViewContainer::dump",false);
_tm_(_file_name);
	if(!_root){
_te_("no _root for file "+_file_name);
		return(false);
	}
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(_root,"views",_file_name)){
_te_("writeXMLParam failed for "+_file_name);
		_status=-5;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewContainer::set_name(const char* name){
_bTrace_("bViewContainer::set_name",false);
_tm_(name);
	if(!set_root_name(name)){
_te_("false for set_root_name :"+name);
		return(false);
	}
char	new_name[256];
	sprintf(new_name,"%s.xml",name);
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(_root,"views",new_name)){
_te_("writeXMLParam failed for "+new_name);
		_status=-1;
		return(false);
	}
	if(!doc->writeXMLParam(NULL,"views",_file_name)){
_te_("writeXMLParam failed for "+_file_name);
		_status=-1;
		return(false);
	}
	strcpy(_file_name,new_name);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewContainer::destroy(){
_bTrace_("bViewContainer::destroy",false);
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(NULL,"views",_file_name)){
_te_("writeXMLParam failed for "+_file_name);
		_status=-1;
		return(false);
	}
	clssmgr->ReleaseXMLInstance(_root);
	_root=NULL;
	_status=-1;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bViewContainer::load(){
_bTrace_("bViewContainer::load",false);
	_root=NULL;
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->readXMLParam(&_root,"views",_file_name)){
_tw_("readXMLParam failed for "+_file_name);
		_status=-1;
	}
	if(!_root){
_tw_("_root == NULL for "+_file_name);
		_status=-1;
	}
}
// ---------------------------------------------------------------------------
// 
// -----------
void bViewContainer::make(){
_bTrace_("bViewContainer::make",false);
char			name[256];
	
	sprintf(name,"%s",_file_name);
int				k=strlen(name);
	name[k-4]=0;

bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"view","");
	add_cdesc(arr,1,"version","3.0.0");
	add_cdesc(arr,1,"name",name);
	add_cdesc(arr,1,"layerarray","");

char			tname[256];
int				i,n=typesmgr->count();
bGenericType*	tp;
	
	for(i=1;i<=n;i++){
		add_cdesc(arr,2,"layer","");
		tp=typesmgr->get(i);
		if(!tp){
			break;
		}
		tp->name(tname);
		add_cdesc(arr,3,"type",tname);
		add_cdesc(arr,3,"style","default");
		add_idesc(arr,3,"visible",1);
		add_idesc(arr,3,"selectable",1);
	}
bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("ParseXMLDescriptors failed");
		return;
	}
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(root,"views",_file_name)){
_te_("writeXMLParam failed for "+_file_name);
		return;
	}
	_root=root;
	_status=noErr;
}
