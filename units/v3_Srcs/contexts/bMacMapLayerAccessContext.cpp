//----------------------------------------------------------------------------
// File : bMacMapLayerAccessContext.cpp
// Project : MacMap
// Purpose : C++ source file : Layer context class (styles management in current view context)
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
// Classe de base pour dessin des objets
//----------------------------------------------------------------------------
// 20/05/2003 creation.
//----------------------------------------------------------------------------

#include <mox_intf/bGenericXMLLayerElement.h>
#include "bMacMapLayerAccessContext.h"
#include "bMacMapApp.h"
#include "bStyle.h"
#include "bVirtualStyle.h"
#include <mox_intf/xmldesc_utils.h>
#include <MacMapSuite/bTrace.h>
#include <iostream>

// ---------------------------------------------------------------------------
// 
// -----------
typedef struct containPrm{
	int						idx;
	int						n;
	bGenericXMLBaseElement* elt;
}containPrm;

// ---------------------------------------------------------------------------
// 
// -----------
static bool _getIndElementContainingLayer(bGenericXMLBaseElement *elt, void *prm, int indent){
containPrm	*p=(containPrm*)prm;	
char		clssname[_names_length_max_];
		
	elt->getclassname(clssname);
	if(!strcmp("layer",clssname)){
		p->n++;
		if(p->n==p->idx){
			p->elt=elt;
			return(false);
		}
	}
	else if(!strcmp("virtuallayer",clssname)){
		p->n++;
		if(p->n==p->idx){
			p->elt=elt;
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
static bGenericXMLBaseElement* GetIndlayer(bGenericXMLBaseElement* root, int idx){
containPrm	p;

	p.idx=idx;
	p.n=0;
	p.elt=NULL;
	(void)root->dotoall(&p,0,_getIndElementContainingLayer);
	return(p.elt);
}

// ---------------------------------------------------------------------------
// 
// -----------
static bool debugDump(bGenericXMLBaseElement *elt, void *prm, int indent){
char    clssname[_names_length_max_];
	elt->getclassname(clssname);
fprintf(stderr,"%s\n",clssname);
	return(true);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapLayerAccessContext::bMacMapLayerAccessContext(){
	_cursor=0;
	_elts=new bArray(sizeof(bStyle*));
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapLayerAccessContext::~bMacMapLayerAccessContext(){
	if(_elts){
		flush();
		delete _elts;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapLayerAccessContext::load(){
_bTrace_("bMacMapLayerAccessContext::load",true);

long                    margin=0;
bGenericXMLBaseElement	*root,*elt;
	if(map_doc->readTree(&root,0,"drawsearch")){
_tm_("found drawsearch.xml");
char	val[_values_length_max_];
		elt=clssmgr->NthElement(root,1,"int");
		if(elt){
			elt->getvalue(val);
			margin=atoi(val);
_tm_("margin="+(int)margin);
		}
		else{
_tw_("no margin");		
		}
		clssmgr->ReleaseXMLInstance(root);
		if(margin<0){
			margin=0;
		}
	}
	
	_curview=viewmgr->get_root();
    if(!_curview){
_te_("no _curview == NULL");
		return(-1);
    }
	_curview->init(this);
bStyle*	style;
	for(int i=1;i<=_elts->count();i++){
		if(!_elts->get(i,&style)){
_te_("_elts->get("+i+",&style)");
			return(-1);
		}
		if(	(style->gettype()==NULL)	&&
			(!style->is_virtual())		){
_te_("type not found for style "+i);
			remove(i);
			i--;
			continue;
		}
_tm_("* parsing "+i);
		if(!parse(i)){
		}
		style->setmargin(margin);
	}
	
	if(count()>0){
		set_current(1);
	}
	
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapLayerAccessContext::unload(){
_bTrace_("bMacMapLayerAccessContext::unload",true);
bStyle*	style;
	
	for(int i=1;i<=_elts->count();i++){
		if(!_elts->get(i,&style)){
			continue;
		}
		style->report();
	}
_tm_("* report ok");
	flush();
_tm_("* flush ok");
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::addlayer(void* elt){
_bTrace_("bMacMapLayerAccessContext::addlayer",true);
_tm_("addlayer");
bStyle*	style=new bStyle();
	if(!style){
		return(false);
	}
	if(!_elts->add(&style)){
		return(false);
	}
	style->setlayer(elt);
	_cursor=count();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::addvirtuallayer(void* elt){
_bTrace_("bMacMapLayerAccessContext::addvirtuallayer",true);
_tm_("addlayer");
bStyle*	style=new bVirtualStyle();
	if(!style){
		return(false);
	}
	if(!_elts->add(&style)){
		return(false);
	}
	style->setlayer(elt);
	_cursor=count();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::settype(const char *name){
_bTrace_("bMacMapLayerAccessContext::settype",true);
bGenericStyle*	style;
	if(!_elts->get(_cursor,&style)){
		return;
	}
	style->settype(name);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setstyle(const char *name){
bStyle*	style;
	if(!_elts->get(_cursor,&style)){
		return;
	}
	style->setname(name);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setvisible(bool vis){
bStyle*	style;
	if(!_elts->get(_cursor,&style)){
		return;
	}
	style->setvisible(vis);
bGenericXMLBaseElement	*elt=clssmgr->NthElement(_array,_cursor,"visible");
	if(elt){
		elt->setvalue(vis?(char*)"1":(char*)"0");
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setselectable(bool sel){
bStyle*	style;
	if(!_elts->get(_cursor,&style)){
		return;
	}
	style->setselectable(sel);
bGenericXMLBaseElement	*elt=clssmgr->NthElement(_array,_cursor,"selectable");
	if(elt){
		elt->setvalue(sel?(char*)"1":(char*)"0");
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setroot(void* elt){
bStyle*	style;
	if(!_elts->get(_cursor,&style)){
		return;
	}
	style->setroot(elt);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setarray(void* elt){
_bTrace_("bMacMapLayerAccessContext::setarray",true);
	_array=(bGenericXMLBaseElement*)elt;
_tm_("-> done");
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::setidentification(void* elt){
_bTrace_("bMacMapLayerAccessContext::setidentification",true);
	_identifier=(bGenericXMLBaseElement*)elt;
_tm_("-> done");
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::flush(){
bStyle*	style;
	
	for(int i=1;i<=_elts->count();i++){
		if(_elts->get(i,&style)){
			delete style;
		}
	}
	_elts->reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::parse(int idx){
bStyle*	style;
	if(_elts->get(idx,&style)){
		return(style->load());
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapLayerAccessContext::count(){
	return(_elts->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericStyle* bMacMapLayerAccessContext::get(int idx){
bGenericStyle*	style;
	if(!_elts->get(idx,&style)){
		return(NULL);
	}
	return(style);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::set_current(int idx){
	if((idx<1)||(idx>count())){
		return(false);
	}
	_cursor=idx;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapLayerAccessContext::get_current(){
	return(_cursor);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::move(int idx, int offset){
_bTrace_("bMacMapLayerAccessContext::move",true);
	if(!_elts->push(idx,offset)){
		return(false);
	}
	_array->pushelement(idx,offset);
	if(idx==_cursor){
		_cursor+=offset;
	}
//	map_wd->inval();
	_MMAPP_->mapIntf()->inval();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::remove(int idx){
_bTrace_("bMacMapLayerAccessContext::remove",true);
bStyle*	style;
	if(!_elts->get(idx,&style)){
		return(false);
	}
	delete style;
	if(!_elts->rmv(idx)){
		return(false);
	}
bGenericXMLBaseElement*	elt=_array->getelement(idx);
	_array->rmvelement(idx);
	if(elt){
bGenericXMLBaseElement*	k=elt->instance();
		k->kill(elt);
	}
	if((idx==_cursor)&&(idx==count()+1)){
		_cursor=count();
	}
	//	map_wd->inval();
	_MMAPP_->mapIntf()->inval();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::add(int tidx, int sidx){
_bTrace_("bMacMapLayerAccessContext::add",true);
bGenericType*	tp=typesmgr->get(tidx);
bStyle*			style;

	if(tp){
		style=new bStyle();
	}
	else{
		style=new bVirtualStyle();
	}
	
	if(!style){
		return(false);
	}
	if(!_elts->add(&style)){
		delete style;
		return(false);
	}
	
bArray  arr(sizeof(xmlelt));

	if(tp){
		add_cdesc(arr,0,"layer","");
	}
	else{
		add_cdesc(arr,0,"virtuallayer","");
	}

char name[256];

	if(tp){
		tp->name(name);
	}
	else{
		message_string(kMsgVirtual,name,1);
	}
	style->settype(name);
	if(tp){
		add_cdesc(arr,1,"type",name);
	}
	
	if(tp){
		tp->styles()->get_name(sidx,name);
	}
	else{
		map_doc->styles()->get_name(sidx,name);
	}
	style->setname(name);
	add_cdesc(arr,1,"style",name);
	style->setvisible(true);
	add_idesc(arr,1,"visible",1);
	style->setselectable(true);
	add_idesc(arr,1,"selectable",(tp!=0));
	
bGenericXMLBaseElement	*root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
		delete style;
		_elts->rmv(_elts->count());
		return(false);
	}
	style->setlayer(root);
	_array->addelement(root);
	parse(_elts->count());
	//	map_wd->inval();
	_MMAPP_->mapIntf()->inval();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapLayerAccessContext::change(int idx, int sidx){
_bTrace_("bMacMapLayerAccessContext::change",true);
bStyle*	style;
	if(!_elts->get(idx,&style)){
_te_("_elts->get failed for "+idx);
		return(false);
	}
bool					v=style->visible();
bool					s=style->selectable();
bGenericXMLBaseElement*	root;
char					name[256];

	if(style->is_virtual()){
		delete style;
		style=new bVirtualStyle();
		if(!_elts->put(idx,&style)){
_te_("_elts->put failed for virtualstyle "+idx);
			return(false);
		}
		map_doc->styles()->get_name(sidx,name);
_tm_("changing "+name+" (virtual)");
	}
	else{
bGenericType*			tp=style->gettype();
		if(tp==NULL){
_te_("NULL type");
			return(false);
		}
		delete style;
		style=new bStyle();
		if(!_elts->put(idx,&style)){
_te_("_elts->put failed for style "+idx);
			return(false);
		}
		tp->name(name);
		style->settype(name);
		tp->styles()->get_name(sidx,name);
_tm_("changing "+name);
	}
	
	style->setname(name);
	style->setvisible(v);
	style->setselectable(s);
	root=GetIndlayer(_array,idx);
	if(root==NULL){
_te_("NULL root");
		return(false);
	}
	
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,1,"style");
	if(elt==NULL){
_te_("NULL style element");
		_array->dotoall(NULL,0,debugDump);		
		return(false);
	}
	elt->setvalue(name);
	style->setlayer(root);	
	parse(idx);
	_MMAPP_->mapIntf()->inval();
	check_on_screen();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapLayerAccessContext::check_on_screen(){
_bTrace_("bMacMapLayerAccessContext::check_on_screen",true);
bStyle*					style;
int						sidx;
bGenericXMLBaseElement*	root;
bGenericType*			tp;

	for(int i=1;i<=_elts->count();i++){
		if(!_elts->get(i,&style)){
		}
		root=style->root();
		if(!root){
			continue;
		}
		tp=style->gettype();
		if(tp){
			sidx=tp->styles()->index(root);
			if(sidx==0){
				continue;
			}
			tp->styles()->set_on_screen(sidx,true);
		}
		else{
			sidx=map_doc->styles()->index(root);
			if(sidx==0){
				continue;
			}
			map_doc->styles()->set_on_screen(sidx,true);
		}
	}
}

