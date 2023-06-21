//----------------------------------------------------------------------------
// File : bClassesMgr.cpp
// Project : MacMap
// Purpose : C++ source file : XML classes management and parse class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2003-2018 Carte Blanche Conseil.
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
// Classe pour gestion des classes xml externes
//----------------------------------------------------------------------------
// 16/05/2003 creation.
// 07/11/2018 64bits
//----------------------------------------------------------------------------

#include "bClassesMgr.h"
#include "bMacMapApp.h"

#include <mox_intf/bGenericExtLib.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/mm_messages.h>

#include <std_ext/bXMLDummyElement.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bStdFile.h>
#include <MacMapSuite/bStdDirectory.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

//----------------------------------------------------------------------------

typedef struct cewnPrm{
	long	n;
	char	name[_names_length_max_];
}cewnPrm;

typedef struct giewnPrm{
	long					idx;
	long					n;
	char					name[_names_length_max_];
	bGenericXMLBaseElement* elt;
}giewnPrm;

// ---------------------------------------------------------------------------
// 
// -----------
static bool _countElementsWithName(bGenericXMLBaseElement *elt, void *prm, int indent){
cewnPrm	*p=(cewnPrm*)prm;
char	clssname[_names_length_max_];
	
	elt->getclassname(clssname);
	if(!strcmp(p->name,clssname)){
		p->n++;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
static bool _getIndElementWithName(bGenericXMLBaseElement *elt, void *prm, int indent){
giewnPrm	*p=(giewnPrm*)prm;	
char		clssname[_names_length_max_];
		
	elt->getclassname(clssname);
	if(!strcmp(p->name,clssname)){
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
static int comp(const void *a, const void *b){
bGenericXMLBaseElement*	ea=(*((bGenericXMLBaseElement**)a));
bGenericXMLBaseElement*	eb=(*((bGenericXMLBaseElement**)b));
char					aname[_names_length_max_];
char					bname[_names_length_max_];
	
	ea->getclassname(aname);
	eb->getclassname(bname);
	return(strcmp(aname,bname));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bClassesMgr::bClassesMgr()
			:_elts(sizeof(bGenericXMLBaseElement*))
			,_extelts(sizeof(bGenericExtLib*)){
	_loaded=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bClassesMgr::~bClassesMgr(){
}

// ---------------------------------------------------------------------------
// Chargement classes XML externes
// -----------
int bClassesMgr::load(){
_bTrace_("bClassesMgr::load",true);
long					i,n=0;
OSStatus				err;
bGenericExtLib*			ext;
bGenericXMLBaseElement*	elt;

	err=((bGenericExtLibMgr*)extmgr)->getlibs(_extelts,_kNULLSign_);
	if(err){
_te_(err+" at getlibs");
		return(err);
	}
	n=_extelts.count();
	for(i=1;i<=n;i++){
		_extelts.get(i,&ext);
		elt=(bGenericXMLBaseElement*)ext->inst();
		_elts.add(&elt);
	}
	
	_elts.sort(comp);
	
	if(_extelts.count()<=0){
_te_("nb classes<=0");
char	msg[__ERROR_STRING_LENGTH_MAX__];
		error_string(error_num(_bClassesMgrRootErr_,_bClassesMgrClassNotLoadedErr_),msg);
bAlertStop	alrt(msg,"");
		exit(0);
	}

_tm_(_extelts.count());
	_loaded=true;

	return(noErr);
}

// ---------------------------------------------------------------------------
// Déchargement classes XML externes
// -----------
int bClassesMgr::unload(){
/*_bTrace_("bClassesMgr::unload",true);
int			i;
bGenericExtLib*	ext;
	
	for(i=1;i<=_extelts.count();i++){
		_extelts.get(i,&ext);
		delete ext;
	}*/
	
// Suppression des classes avec créateur,
// car elles sont ajoutées en direct par les externes
// Les autres sont virées par bGenericExtLib
bGenericXMLBaseElement*	elt;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&elt);
		ReleaseXMLInstance(elt);
	}

	_elts.reset();
	_extelts.reset();
	
	_loaded=false;

	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::CreateXMLInstance(const char* name){
_bTrace_("bClassesMgr::CreateXMLInstance",false);
bXMLDummyElement	dummy((char*)name,_MMAPP_);
bXMLDummyElement*	pdummy=&dummy;
long				idx=_elts.search(&pdummy,comp);
	if(idx<=0){
_te_("not found :"+name);
		return(NULL);
	}

bGenericXMLBaseElement* res;
	if(!_elts.get(idx,&res)){
_te_("get failed");
		return(NULL);
	}
	return(res->create(res));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::CreateXMLInstance(	const char* name, 
														const char* value, 
														bGenericXMLBaseElement* parent){
_bTrace_("bClassesMgr::search",false);
bGenericXMLBaseElement*	elt=(bGenericXMLBaseElement*)CreateXMLInstance(name);
	if(!elt){
_te_("not found :"+name);
		return(NULL);
	}
	if(value){
		elt->setvalue((char*)value);
	}
	if(parent){
		if(!((bGenericXMLBaseElement*)parent)->addelement(elt)){
_te_("parent->addChild returns NULL for "+name);
		}
	}
	return(elt);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::FindXMLInstance(int sign){
long			i;
bGenericExtLib*	ext;
	
	for(i=1;i<=_extelts.count();i++){
		if(!_extelts.get(i,&ext)){
			continue;
		}
		if(ext->signature()==sign){
			return(ext->inst());
		}
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bClassesMgr::GetXMLInstanceName(int sign, char *name){
long			i;
bGenericExtLib*	ext;

	for(i=1;i<=_extelts.count();i++){
		if(!_extelts.get(i,&ext)){
			continue;
		}
		if(ext->signature()==sign){
			ext->inst()->getclassname(name);
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bClassesMgr::FindXMLInstanceByClass(bArray& arr, int sign){
long					i;
bGenericXMLBaseElement*	elt;
	
	for(i=1;i<=_elts.count();i++){
		if(!_elts.get(i,&elt)){
			continue;
		}
		if(elt->getclass()==sign){
			arr.add(&elt);
		}
	}
}


// ---------------------------------------------------------------------------
// 
// -----------
void bClassesMgr::FindXMLInstanceBySubClass(bArray& arr, int sign){
long					i;
bGenericXMLBaseElement*	elt;
	
	for(i=1;i<=_elts.count();i++){
		if(!_elts.get(i,&elt)){
			continue;
		}
		if(elt->getsubclass()==sign){
			arr.add(&elt);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bClassesMgr::ReleaseXMLInstance(bGenericXMLBaseElement* elt){
//_bTrace_("bClassesMgr::ReleaseXMLInstance",false);
//_tm_((void*)elt);
	if(!elt){
//_te_("elt==NULL");
		return;
	}	
bGenericXMLBaseElement* killer=((bGenericXMLBaseElement*)elt)->instance();
	if(!killer){
//_te_("killer==NULL");
		return;
	}
	killer->kill(elt);
}

// ---------------------------------------------------------------------------
//
// -----------
bGenericXMLBaseElement* bClassesMgr::ParseXMLData(void* data, size_t datasz){
_bTrace_("bClassesMgr::ParseXMLData(void*,size_t)",false);
xmlDoc *doc=xmlReadMemory((const char*)data,datasz,"noname.xml",NULL,XML_PARSE_NOBLANKS);
    if(doc==NULL){
_te_("xmlReadMemory failed");
        return NULL;
    }
xmlNode *root=xmlDocGetRootElement(doc);
    if(root==NULL){
_te_("xmlDocGetRootElement returned NULL");
        xmlFreeDoc(doc);
        return NULL;
    }
    if(root->type!=XML_ELEMENT_NODE) {
_te_("root->type!=XML_ELEMENT_NODE");
        xmlFreeDoc(doc);
        return NULL;
    }
bGenericXMLBaseElement* elt=(bGenericXMLBaseElement*)CreateXMLInstance((const char*)root->name);
    if(elt){
        elt->decode(root);
bGenericXMLBaseElement* tmp=elt->getelement(1);
        elt->rmvelement(1);
        ReleaseXMLInstance(elt);
        elt=tmp;
    }
    else{
_te_("class not found");
    }
    xmlFreeDoc(doc);
    return(elt);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::ParseXMLDescriptors(bArray* xml){
_bTrace_("bClassesMgr::ParseXMLDescriptors",true);
long					i,n;
xmlelt					xelt;
bGenericXMLBaseElement	*elt,*parent;
	
	n=xml->count();
	if(n<1){
_te_("empty array");
		return(NULL);
	}
	for(i=1;i<=n;i++){
		if(!xml->get(i,&xelt)){
_te_("xml->get failed");
			return(NULL);
		}
		if((i==1)&&(xelt.indent!=0)){
_te_("(i==1)&&(elt->indent!=0)");
			return(NULL);
		}
		elt=(bGenericXMLBaseElement*)CreateXMLInstance(xelt.name);
		if(!elt){
_te_("instance not found :"+xelt.name);
			return(NULL);
		}
		xelt.element=elt;
		if(!xml->put(i,&xelt)){
_te_("xml->put failed");
			return(NULL);
		}
		elt->setvalue(xelt.value);
		
		if(i>1){
			parent=(bGenericXMLBaseElement*)findParent(xml,i,xelt.indent);
			if(!parent){
_te_("no parent");
				return(NULL);
			}
			if(!parent->addelement(elt)){
_te_("parent->addelement(elt) returns false");
				return(NULL);
			}
		}
	}
	xml->get(1,&xelt);
	return(xelt.element);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bClassesMgr::AddXMLClass(bGenericXMLBaseElement* elt){
_bTrace_("bClassesMgr::AddXMLClass",false);
char	nm[_names_length_max_];
	elt->getclassname(nm);
	if(_loaded){
_te_("classes already loaded, can't add "+nm);
		return(false);
	}
	if(!_elts.add(&elt)){
_te_("add error, can't add "+nm);
		return(false);
	}
_tm_(nm);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::NthElement(bGenericXMLBaseElement* root, long idx, const char *name){
giewnPrm	p;
	strcpy(p.name,name);
	p.idx=idx;
	p.n=0;
	p.elt=NULL;
    (void)root->dotoall(&p,0,_getIndElementWithName);
	if(!p.elt){
		return(NULL);
	}
	return(p.elt);
}

// ---------------------------------------------------------------------------
// 
// -----------
long bClassesMgr::CountElements(bGenericXMLBaseElement* root, const char *name){
cewnPrm	p;
	strcpy(p.name,name);
	p.n=0;
	(void)root->dotoall(&p,0,_countElementsWithName);
	return(p.n);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bClassesMgr::findParent(bArray* xml, int idx, int indent){
long	i;
xmlelt	xelt;
	
	for(i=idx;i>=1;i--){
		if(!xml->get(i,&xelt)){
			return(NULL);
		}
		if(xelt.indent==(indent-1)){
			return(xelt.element);
		}
	}
	return(NULL);
}

