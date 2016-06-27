//----------------------------------------------------------------------------
// File : bViewMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Views management class
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
// 02/02/2005 creation.
//----------------------------------------------------------------------------

#include "bViewMgr.h"
#include "bMacMapDocument.h"
#include "bViewContainer.h"
#include "bDummyContainer.h"
#include "bMacMapApp.h"
#include "bClassesMgr.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define _dirname	"views"
#define _fname		"curview.xml"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bViewMgr::bViewMgr()
		:_elts(sizeof(bViewContainer*)){
	_cur=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bViewMgr::~bViewMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::load(){
_bTrace_("bViewMgr::load",true);
	_cur=1;
OSStatus			status=noErr;
int					i,n;
bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	n=doc->countParams(_dirname);
	if(n<0){
_te_("n=="+n);
		status=-1;
		return(status);
	}
	if(n==0){
_te_("n=="+n);
		status=-2;
		return(status);
	}
	
bViewContainer*	v;	
char			name[FILENAME_MAX];
	
	for(i=1;i<=n;i++){
		doc->getParamName(_dirname,i,name);
		if(strstr(name,".xml")==NULL){
			continue;
		}
        v=new bViewContainer(name);
		if(v->status()){
			delete v;
			continue;
		}
		if(!_elts.add(&v)){
_te_("_elts.add==false");
			status=-3;
			break;
		}
	}
	
	if(status){
	}
		
bGenericXMLBaseElement* elt;
	if(!doc->readXMLParam(&elt,"",_fname)){
_tw_("readXMLParam failed for "+_fname);
		status=-4;
		return(status);
	}

bGenericXMLBaseElement* chld=clssmgr->NthElement(elt,1,"name");
	if(!chld){
_te_("clssmgr->NthElement == NULL for \"name\"");
		status=-5;
		return(status);
	}
char	value[_values_length_max_];
	chld->getvalue(value);
	clssmgr->ReleaseXMLInstance(elt);
	_cur=find(value);
_tm_("_cur=="+_cur+" ("+value+")");
	if(_cur<0||_cur>count()){
		_cur=1;
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::unload(){
int		i;
bViewContainer*	v;
	
	for(i=1;i<_elts.count();i++){
		if(_elts.get(i,&v)){
			break;
		}
		delete v;
	}
	_elts.reset();
	_cur=0;
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::count(){
	return(_elts.count());
}
		
// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::get_current(){
	return(_cur);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::set_current(int idx){
_bTrace_("bViewMgr::set_current",false);
	if(idx<1||idx>count()){
_te_("index out of range :"+idx);
		return(false);
	}
	
	layersmgr->StopDraw();
	//usleep(240);
	
	
int bk=_cur;
	
	_cur=idx;
	if(!dump()){
		_cur=bk;
		return(false);
	}
	
// On fait un reload pour ne pas retrouver la vue modifiée
// 
bViewContainer*	vo;	
bViewContainer*	vn;	
	if(_elts.get(bk,&vo)){
char			name[FILENAME_MAX];
		vo->file_name(name);
		vn=new bViewContainer(name);
		if(vn->status()==noErr){
			delete vo;
			_elts.put(bk,&vn);
		}
	}

	layersmgr->ViewChanged();

	return(true);
}
		
// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::append(){	
char	name[FILENAME_MAX],fname[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgNewView,name,true);
	do{
		k++;
		sprintf(fname,"%s %d.xml",name,k);
		if(!find(fname)){
			break;
		}
	}while(true);
	
bViewContainer*		v=new bViewContainer(fname);
	if(v->status()!=noErr){
		delete v;
		return(false);
	}
	if(!_elts.add(&v)){
		delete v;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::duplicate(int idx){
_bTrace_("bViewMgr::duplicate",true);
_tm_("for :"+idx);
bViewContainer*	v;
	if(!_elts.get(idx,&v)){
_te_("bad index");
		return(false);
	}

char	name[FILENAME_MAX],fname[FILENAME_MAX],buff[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgCopy,buff,true);
	v->get_name(fname);
	sprintf(name,"%s %s",fname,buff);
	
	do{
		k++;
		sprintf(fname,"%s-%d.xml",name,k);
		if(!find(fname)){
			break;
		}
	}while(true);

_tm_("name set to :"+fname);

bViewContainer*		nv=new bViewContainer(fname,(*v));
	if(nv->status()!=noErr){
_te_("bad status:"+nv->status());
		delete nv;
		return(false);
	}
	if(!_elts.add(&nv)){
		delete nv;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::remove(int idx){
_bTrace_("bViewMgr::remove",true);
	if(count()<2){
_te_("can't remove last view");
		return(false);
	}
	if(idx==_cur){
_te_("can't remove current view");
		return(false);
	}
bViewContainer* v;
	if(!_elts.get(idx,&v)){
_te_("can't remove view (index out of range) :"+idx);
		return(false);
	}
	
	if(!v->destroy()){
_te_("can't remove view (destroy error "+v->status()+")");
		return(false);
	}
	delete v;
	_elts.rmv(idx);
	if(_cur>idx){
		_cur--;
	}
	return(true);
}
		
// ---------------------------------------------------------------------------
// 
// -----------
void bViewMgr::get_name(int idx, char* name){
bViewContainer* v;
	if(!_elts.get(idx,&v)){
		return;
	}
	v->get_name(name);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bViewMgr::get_name(char* name){
	get_name(_cur,name);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::set_name(int idx, const char* name){
_bTrace_("bViewMgr::set_name",true);
bViewContainer* v;
	if(!_elts.get(idx,&v)){
		return(false);
	}
char	old[256];
	get_name(idx,old);
	if(strcmp(old,name)==0){
		return(true);
	}

	sprintf(old,"%s.xml",name);
	if(find(old)){
_te_("duplicate name "+old);
		return(false);
	}
	
	if(!v->set_name(name)){
_te_("container can't set name");
		return(false);
	}
	if(_cur==idx){
		if(!dump()){
_te_("returns false at dump");
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::set_name(const char* name){
	return(set_name(_cur,name));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::save(int idx){
bViewContainer*		v;
	if(!_elts.get(idx,&v)){
		return(false);
	}
	v->dump();
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::save(){
	return(save(_cur));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bViewMgr::get_root(int idx){
bViewContainer* v;
	if(!_elts.get(idx,&v)){
		return(NULL);
	}
	return(v->root());
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bViewMgr::get_root(){
	return(get_root(_cur));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::make(){
_bTrace_("bViewMgr::make",true);
char			name[FILENAME_MAX],fname[FILENAME_MAX];	
	message_string(kMsgNewView,name,true);
	sprintf(fname,"%s.xml",name);

bGenericXMLBaseElement* root;

_PUSH_
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0");
	add_cdesc(arr,1,"name",fname);
	root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
_POP_
	if(!root){
_te_("root == NULL");
		return(false);
	}

bMacMapDocument*	doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->writeXMLParam(root,"",_fname)){
_tw_("writeXMLParam failed for "+_fname);
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	clssmgr->ReleaseXMLInstance(root);
	
_PUSH_
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"view","");
	add_cdesc(arr,1,"version","3.0");
	add_cdesc(arr,1,"name",name);
	add_cdesc(arr,1,"layerarray","");
	root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
_POP_
	if(!root){
		return(false);
	}
	if(!doc->writeXMLParam(root,_dirname,fname)){
_tw_("writeXMLParam failed for "+_fname);
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bViewMgr::dump(){
_bTrace_("bViewMgr::dump",true);
bViewContainer*	v;
	_elts.get(_cur,&v);
bGenericXMLBaseElement* root;	
bMacMapDocument*		doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	if(!doc->readXMLParam(&root,"",_fname)){
_tw_("readXMLParam failed for "+_fname);
		return(false);	
	}
	if(root==NULL){
_tw_("root==NULL");
		return(false);	
	}
	
char	name[FILENAME_MAX];
	v->file_name(name);
	
bGenericXMLBaseElement* elt=clssmgr->NthElement(root,1,"name");
	if(!elt){
_te_("clssmgr->NthElement == NULL for \"name\"");
		clssmgr->ReleaseXMLInstance(root);
		return(false);
	}
	elt->setvalue(name);

	if(!doc->writeXMLParam(root,"",_fname)){
_tw_("readXMLParam failed for "+_fname);
		return(false);	
	}
	
	clssmgr->ReleaseXMLInstance(root);
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::find(const char* name){
bDummyContainer		dummy(name,"",NULL);
bDummyContainer*	pdummy=&dummy;
	return(_elts.search(&pdummy,name_compare));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bViewMgr::name_compare(const void* a, const void* b){
bViewContainer*	va=(*((bViewContainer**)a));
bViewContainer*	vb=(*((bViewContainer**)b));
char	na[FILENAME_MAX];
char	nb[FILENAME_MAX];
	va->file_name(na);
	vb->file_name(nb);
	return(strcmp(na,nb));
}
