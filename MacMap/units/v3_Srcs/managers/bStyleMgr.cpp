//----------------------------------------------------------------------------
// File : bStyleMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Styles management class
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

#include "bStyleMgr.h"
#include "bStyleContainer.h"
#include "bDummyContainer.h"
#include "bMacMapApp.h"
#include "bMacMapDocument.h"
#include "bvDefMgr.h"

#include <mox_intf/ext_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/mm_messages.h>

#include <std_ext/bStdXMLBaseElement.h>

#include <MacMapSuite/bTrace.h>

#pragma mark -> bStdStyleMgr
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdStyleMgr::bStdStyleMgr(int *status)
			:_elts(sizeof(bStyleContainer*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdStyleMgr::~bStdStyleMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::load(){
	return(-1);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::unload(){
bStyleContainer*	c;
	
	for(int i=1;i<_elts.count();i++){
		if(_elts.get(i,&c)){
			break;
		}
		delete c;
	}
	_elts.reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::count(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::add(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::duplicate(int idx){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------				
bool bStdStyleMgr::remove(int idx){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleMgr::get_name(int idx, char* name){
_bTrace_("bStdStyleMgr::get_name",false);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return;
	}
	c->get_name(name);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::set_name(int idx, const char* name){
_bTrace_("bStdStyleMgr::set_name",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	if(index(name)>0){
_te_(name+" exists");
		return(false);
	}
char				bknm[256];
	c->get_name(bknm);
	if(strcmp(bknm,"default")==0){
_te_("Can't rename default style");
		return(false);
	}
	
	if(!c->set_name(name)){
_te_("set_name failed");
		return(false);
	}
// MAJ de la vue
bGenericStyle*		stl=NULL;
    for(int i=1;i<=_MMAPP_->layersMgr()->count();i++){
        stl=_MMAPP_->layersMgr()->get(i);
        if(!strcmp(bknm,stl->getname())){
            _MMAPP_->layersMgr()->change(i,idx);
        }
    }
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::signature(int idx){
_bTrace_("bStdStyleMgr::signature",false);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(_kNULLSign_);
	}
	return(c->signature());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::save(int idx){
_bTrace_("bStdStyleMgr::save",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	c->save();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bStdStyleMgr::get_root(int idx){
_bTrace_("bStdStyleMgr::get_root",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(NULL);
	}
	return(c->root());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleMgr::set_root(int idx, bGenericXMLBaseElement* root){
_bTrace_("bStdStyleMgr::set_root",true);
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return;
	}

bGenericXMLBaseElement*		bk=c->root();
	c->set_root(root);
	if(c->on_screen()){
bGenericStyle*	stl=NULL;		
        for(int i=1;i<=_MMAPP_->layersMgr()->count();i++){
            stl=_MMAPP_->layersMgr()->get(i);
            if(stl->root()==bk){
                _MMAPP_->layersMgr()->change(i,idx);
            }
        }
		_MMAPP_->mapIntf()->inval();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::index(const char* name){
bDummyContainer		dummy("",name,NULL);
bDummyContainer*	pdummy=&dummy;
	return(_elts.search(&pdummy,name_compare));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::index(const char* file_name, bool dummy){
bDummyContainer		foo(file_name,"",NULL);
bDummyContainer*	pdummy=&foo;
	return(_elts.search(&pdummy,fname_compare));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::index(bGenericXMLBaseElement* root){
bDummyContainer		dummy("","",root);
bDummyContainer*	pdummy=&dummy;
	return(_elts.search(&pdummy,root_compare));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::edit(int idx, int signature){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::create(int signature){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleMgr::set_on_screen(int idx, bool b){
_bTrace_("bStdStyleMgr::set_on_screen",true);
_tm_(idx+" set to "+b);
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return;
	}
	c->set_on_screen(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::get_on_screen(int idx){
_bTrace_("bStdStyleMgr::set_on_screen",true);
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	return(c->on_screen());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdStyleMgr::set_on_edit(int idx, bool b){
_bTrace_("bStdStyleMgr::set_on_edit",true);
_tm_(idx+" set to "+b);
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return;
	}
	c->set_on_edit(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::get_on_edit(int idx){
_bTrace_("bStdStyleMgr::get_on_edit",true);
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	return(c->on_edit());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdStyleMgr::make(){	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::name_compare(const void* a, const void* b){
bStdContainer*	ca=(*((bStdContainer**)a));
bStdContainer*	cb=(*((bStdContainer**)b));
char			na[FILENAME_MAX];
char			nb[FILENAME_MAX];
	ca->get_name(na);
	cb->get_name(nb);
	return(strcmp(nb,na));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdStyleMgr::fname_compare(const void* a, const void* b){
bStdContainer*	ca=(*((bStdContainer**)a));
bStdContainer*	cb=(*((bStdContainer**)b));
char			na[FILENAME_MAX];
char			nb[FILENAME_MAX];
	ca->file_name(na);
	cb->file_name(nb);
	return(strcmp(nb,na));
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bStdStyleMgr::root_compare(const void* a, const void* b){
bStdContainer*	ca=(*((bStdContainer**)a));
bStdContainer*	cb=(*((bStdContainer**)b));
	if(cb->root()>ca->root()){
		return(1);
	}
	if(cb->root()<ca->root()){
		return(-1);
	}
	return(0);
}

#pragma mark -
#pragma mark -> bStyleMgr
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyleMgr	::bStyleMgr(bGenericType* tp, int *status)
			:bStdStyleMgr(status){
	_tp=tp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStyleMgr::~bStyleMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStyleMgr::load(){
_bTrace_("bStyleMgr::load",true);
OSStatus	status=noErr;
int			n;

	n=_tp->fields()->count_param(kStyleFolderName);
	if(n<0){
_te_("n=="+n);
		return(-1);
	}		
	if(n==0){
_te_("n=="+n);
		return(-2);
	}
bStyleContainer*	c;
char				name[FILENAME_MAX];
	for(int i=1;i<=n;i++){
		_tp->fields()->get_param_name(kStyleFolderName,i,name);
		if(strstr(name,".xml")==NULL){
			continue;
		}
_tm_("len="+(unsigned int)strlen(name)+" for "+name);
		c=new bStyleContainer(name,_tp);
		if(c->status()){
			delete c;
			continue;
		}
		if(!_elts.add(&c)){
_te_("_elts.add==false");
			status=-3;
			return(status);
		}
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleMgr::add(bGenericXMLBaseElement* root){
_bTrace_("bStyleMgr::add",true);
char	name[FILENAME_MAX],fname[FILENAME_MAX];
char	buff[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgNewStyle,name,true);
	do{
		k++;
		sprintf(fname,"%s %d.xml",name,k);
		sprintf(buff,"%s %d",name,k);
		if(!index(fname,false)){
			break;
		}
	}while(true);
	
    message_string(kMsgNameNewStyle,name,true);
    if((GetAName(buff,name,false))&&(strlen(buff)>0)){
		sprintf(fname,"%s.xml",buff);
		if(index(fname,false)){
			k=0;
			do{
				k++;
				sprintf(fname,"%s %d.xml",buff,k);
				if(!index(fname,false)){
					break;
				}
			}while(true);		
		}
	}

_tm_("before new bStyleContainer _tp="+(void*)_tp+" root="+(void*)root);
bStyleContainer*	c=new bStyleContainer(fname,root,_tp);
_tm_("new bStyleContainer passed");
	if(c->status()!=noErr){
_te_("bStyleContainer status="+c->status());
		delete c;
		return(false);
	}
	if(!_elts.add(&c)){
_te_("_elts.add failed");
		delete c;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleMgr::duplicate(int idx){
_bTrace_("bStyleMgr::duplicate",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	
char	name[FILENAME_MAX],fname[FILENAME_MAX],buff[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgCopy,buff,true);
	c->get_name(fname);
	sprintf(name,"%s %s",fname,buff);
	
	do{
		k++;
		sprintf(fname,"%s-%d.xml",name,k);
		if(!index(fname,false)){
			break;
		}
	}while(true);
bStyleContainer*	nc=new bStyleContainer(fname,(*c));
	if(nc->status()!=noErr){
_te_("nc->status() == "+nc->status());
		delete nc;
		return(false);
	}
	if(!_elts.add(&nc)){
_te_("_elts.add failed");
		delete nc;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------				
bool bStyleMgr::remove(int idx){
_bTrace_("bStyleMgr::remove",true);
	if(count()<2){
_te_("can't remove last style");
		return(false);
	}
bStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	if(c->on_screen()){
_te_("style is on screen");
		return(false);	
	}
	if(c->on_edit()){
_te_("style is on edit");
		return(false);	
	}
	
char				bknm[256];
	c->get_name(bknm);
	if(strcmp(bknm,"default")==0){
_te_("Can't destroy default style");
		return(false);
	}
	
	if(!c->destroy()){
_te_("destroy failed");
		return(false);
	}
	delete c;
	_elts.rmv(idx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleMgr::edit(int idx, int signature){
_bTrace_("bStyleMgr::edit",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	if(c->on_edit()){
_te_("style is on edit");
		return(false);	
	}
bGenericXMLBaseElement* root=c->root();
bGenericXMLBaseElement* back=root;
vdef_edit				prm={root,_tp};
int						sign=c->signature();
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(int));
#endif

	if(signature==_kNULLSign_){
		sign=c->signature();
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(int));
#endif
		if(!vdefmgr->edit("",sign,&prm)){
_tw_("vdefmgr->edit failed");
			return(false);
		}
	}
	else{
		sign=signature;
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(int));
#endif
		if(!vdefmgr->edit("",signature,&prm)){
_tw_("vdefmgr->edit failed");
			return(false);
		}
	}
	
	if(back==prm.root){
_te_("back==root");
		return(false);
	}
	set_root(idx,prm.root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStyleMgr::create(int signature){
_bTrace_("bStyleMgr::create",true);
vdef_edit	prm={NULL,_tp};
	if(!vdefmgr->edit("",signature,&prm)){
_tm_("vdefmgr->edit == false");
		return(false);
	}
	if(prm.root==NULL){
_tm_("prm.root=NULL");
		return(false);
	}
	if(!add(prm.root)){
_te_("add == false");
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	//
// pas de ReleaseXMLInstance sur prm.root			//
// car le root est détruit lors de la libération	//
// du StdContainer									//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	//
		return(false);
	}
	return(true);
}

#pragma mark -
#pragma mark -> bVirtualStyleMgr
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVirtualStyleMgr::bVirtualStyleMgr(int *status)
				:bStdStyleMgr(status){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bVirtualStyleMgr::~bVirtualStyleMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bVirtualStyleMgr::load(){
_bTrace_("bVirtualStyleMgr::load",true);
OSStatus			status=noErr;
int					n;
bMacMapDocument*	doc=(bMacMapDocument*)map_doc;

	n=doc->countParams(kStyleFolderName);
	if(n<0){
_te_("n=="+n);
		return(-1);
	}		
	if(n==0){
_tm_("n=="+n);
		return(0);
	}
bVirtualStyleContainer*	c;
char					name[FILENAME_MAX];
	for(int i=1;i<=n;i++){
		doc->getParamName(kStyleFolderName,i,name);
		if(strstr(name,".xml")==NULL){
			continue;
		}
_tm_("len="+(unsigned int)strlen(name)+" for "+name);
		c=new bVirtualStyleContainer(name);
		if(c->status()){
			delete c;
			continue;
		}
		if(!_elts.add(&c)){
_te_("_elts.add==false");
			status=-3;
			return(status);
		}
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleMgr::add(bGenericXMLBaseElement* root){
_bTrace_("bVirtualStyleMgr::add",true);
char	name[FILENAME_MAX],fname[FILENAME_MAX];
char	buff[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgNewStyle,name,true);
	do{
		k++;
		sprintf(fname,"%s %d.xml",name,k);
		sprintf(buff,"%s %d",name,k);
		if(!index(fname,false)){
			break;
		}
	}while(true);
	
    message_string(kMsgNameNewStyle,name,true);
	if((GetAName(buff,name,false))&&(strlen(buff)>0)){
		sprintf(fname,"%s.xml",buff);
		if(index(fname,false)){
			k=0;
			do{
				k++;
				sprintf(fname,"%s %d.xml",buff,k);
				if(!index(fname,false)){
					break;
				}
			}while(true);		
		}
	}

bVirtualStyleContainer*	c=new bVirtualStyleContainer(fname,root);
	if(c->status()!=noErr){
		delete c;
		return(false);
	}
	if(!_elts.add(&c)){
		delete c;
		return(false);
	}

	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleMgr::duplicate(int idx){
_bTrace_("bVirtualStyleMgr::duplicate",true);
bVirtualStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	
char	name[FILENAME_MAX],fname[FILENAME_MAX],buff[FILENAME_MAX];
int		k=0;
	
	message_string(kMsgCopy,buff,true);
	c->get_name(fname);
	sprintf(name,"%s %s",fname,buff);
	
	do{
		k++;
		sprintf(fname,"%s-%d.xml",name,k);
		if(!index(fname,false)){
			break;
		}
	}while(true);
bVirtualStyleContainer*	nc=new bVirtualStyleContainer(fname,(*c));
	if(nc->status()!=noErr){
_te_("nc->status() == "+nc->status());
		delete nc;
		return(false);
	}
	if(!_elts.add(&nc)){
_te_("_elts.add failed");
		delete nc;
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------				
bool bVirtualStyleMgr::remove(int idx){
_bTrace_("bVirtualStyleMgr::remove",true);
	if(count()<2){
_te_("can't remove last style");
		return(false);
	}
bVirtualStyleContainer* c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	if(c->on_screen()){
_te_("style is on screen");
		return(false);	
	}
	if(c->on_edit()){
_te_("style is on edit");
		return(false);	
	}
	
char				bknm[256];
	c->get_name(bknm);
	if(strcmp(bknm,"default")==0){
_te_("Can't destroy default style");
		return(false);
	}
	
	if(!c->destroy()){
_te_("destroy failed");
		return(false);
	}
	delete c;
	_elts.rmv(idx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleMgr::edit(int idx, int signature){
_bTrace_("bVirtualStyleMgr::edit",true);
bStyleContainer*	c;
	if(!_elts.get(idx,&c)){
_te_("index "+idx+" out of range");
		return(false);
	}
	if(c->on_edit()){
_te_("style is on edit");
		return(false);	
	}
bGenericXMLBaseElement* root=c->root();
bGenericXMLBaseElement* back=root;
vdef_edit				prm={root,NULL};
int						sign=c->signature();
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sign,sizeof(int));
#endif

	if(signature==_kNULLSign_){
		sign=c->signature();
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(int));
#endif
		if(!vdefmgr->edit("",sign,&prm)){
_te_("vdefmgr->edit failed for"+&sign);
			return(false);
		}
	}
	else{
		sign=signature;
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(int));
#endif
		if(!vdefmgr->edit("",signature,&prm)){
_te_("vdefmgr->edit failed for"+&sign);
			return(false);
		}
	}
	
	if(back==prm.root){
_te_("back==root");
		return(false);
	}
	set_root(idx,prm.root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVirtualStyleMgr::create(int signature){
_bTrace_("bVirtualStyleMgr::create",true);
vdef_edit	prm={NULL,NULL};
	if(!vdefmgr->edit("",signature,&prm)){
_tm_("vdefmgr->edit==false");
		return(false);
	}
	if(prm.root==NULL){
_tm_("prm.root=NULL");
		return(false);
	}
	if(!add(prm.root)){
_te_("add==false");
		clssmgr->ReleaseXMLInstance(prm.root);
		return(false);
	}
	return(true);
}
