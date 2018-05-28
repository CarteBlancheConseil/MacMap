//----------------------------------------------------------------------------
// File : bStdUnitMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for units manager
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 24/11/2004 creation.
//----------------------------------------------------------------------------

#include "bStdUnitMgr.h"
#include "bStdUnit.h"
#include "bClassesMgr.h"
#include "bMacMapApp.h"
#include "bMacMapDocument.h"
#include <mox_intf/bXMLFile.h>
#include <mox_intf/xmldesc_utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUnitMgr	::bStdUnitMgr()
			: _elts(sizeof(bStdUnit*)){
	_cur=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUnitMgr::~bStdUnitMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdUnit* bStdUnitMgr::allocator(){
	return(new bStdUnit());
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::load(){
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::unload(){
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::app_load(){
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::app_unload(){
_bTrace_("bStdUnitMgr::app_unload",_DEVELOPMENT_VERS_);
	reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::get_current(){
	return(_cur);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUnitMgr::set_current(int idx){
	if((idx>0)&&(idx<=count())){
		_cur=idx;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::count(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericUnit* bStdUnitMgr::get(){
	return(get(_cur));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericUnit* bStdUnitMgr::get(int idx){
bStdUnit* u;
	if(!_elts.get(idx,&u)){
		u=NULL;
	}
	return(u);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnitMgr::set(int idx, const char* name, double coef){
bGenericUnit* u;
	if(!_elts.get(idx,&u)){
		return;
	}
	u->set(name,coef);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUnitMgr::add(const char* name, double coef){
bGenericUnit* u=allocator();
	if(!u){
		return(false);
	}
	if(!_elts.add(&u)){
		delete u;
		return(false);
	}
	u->set(name,coef);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUnitMgr::rmv(int idx){
bGenericUnit* u;
	if(!_elts.get(idx,&u)){
		return(false);
	}
	if(!_elts.rmv(idx)){
		return(false);
	}
	delete u;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUnitMgr::make(){	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnitMgr::reset(){	
int			i;
bStdUnit*	u;
	
	for(i=1;i<=count();i++){
		if(_elts.get(i,&u)){
			delete u;
		}
	}
	_elts.reset();
	_cur=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::pth_load(const char* path){
_bTrace_("bStdUnitMgr::pth_load",_DEVELOPMENT_VERS_);
bGenericXMLBaseElement*	root=NULL;
bMacMapDocument*		doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	doc->readXMLParam(&root,"",path);
	if(!root){
_te_("root == NULL");
		return(-1);
	}	
	(void)root->dotoall(this,0,_load);
	if(count()==0){
		clssmgr->ReleaseXMLInstance(root);
_te_("count==0");
		return(-1);
	}
	(void)root->dotoall(this,0,_current);
	if(_cur==0){
		_cur=1;
	}	
	clssmgr->ReleaseXMLInstance(root);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::apppth_load(const char* name){
_bTrace_("bStdUnitMgr::apppth_load",_DEVELOPMENT_VERS_);
_tm_("name="+name);
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
	if(!cfs){
_te_("cfs == NULL at CFStringCreateWithCString");
		return(-1);
	}
CFURLRef url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),cfs,CFSTR("xml"),NULL);
	CFRelease(cfs);
	if(!url){
_te_("url == NULL");
		return(-1);
	}
char	fpath[PATH_MAX];
	cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
	CFRelease(url);
	if(!cfs){
_te_("cfs == NULL at CFURLCopyFileSystemPath");
		return(-1);
	}
	CFStringGetCString(cfs,fpath,PATH_MAX,kCFStringEncodingMacRoman);
_tm_("fpath="+fpath);
	CFRelease(cfs);


bXMLFile f(fpath,"r");
bGenericXMLBaseElement*	root;
	f.mount(clssmgr,&root);
	if(!root){
_te_("root == NULL");
		return(-1);
	}
	(void)root->dotoall(this,0,_load);
	if(count()==0){
		clssmgr->ReleaseXMLInstance(root);
_te_("count==0");
		return(-1);
	}
	(void)root->dotoall(this,0,_current);
	if(_cur==0){
		_cur=1;
	}
	clssmgr->ReleaseXMLInstance(root);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUnitMgr::pth_unload(const char* path, bool dispose){
_bTrace_("bStdUnitMgr::pth_unload",_DEVELOPMENT_VERS_);
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	add_idesc(arr,1,"int",_cur);

bGenericUnit*	u;
char			c[256];

	for(int i=1;i<=count();i++){
		u=get(i);
		u->name_id(c);
		add_ddesc(arr,1,"float",u->coef(),15);
		add_cdesc(arr,1,"name",c);
_tm_(c+" "+u->coef());
	}

bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return(false);
	}
bMacMapDocument*		doc=(bMacMapDocument*)bMacMapApp::instance()->document();	
	doc->writeXMLParam(root,"",path);
	clssmgr->ReleaseXMLInstance(root);

	if(dispose){
		reset();
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdUnitMgr::_load(bGenericXMLBaseElement *elt, void *prm, int indent){
_bTrace_("bStdUnitMgr::_load",false);
bStdUnitMgr*	mgr=(bStdUnitMgr*)prm;
char			clssname[256];
char			value[256];
bStdUnit*		u;
	
	elt->getclassname(clssname);
	elt->getvalue(value);
	if(!strcmp("float",clssname)){
		u=mgr->allocator();
		u->set(NULL,matof(value));
		return(mgr->_elts.add(&u));

	}
	else if(!strcmp("name",clssname)){
		if(!mgr->_elts.get(mgr->_elts.count(),&u)){
			return(false);
		}
		u->set(value,u->coef());	

char	sv[256];
char	lv[256];		
u->short_name(sv);	
u->long_name(lv);			
_tm_(mgr->count()+" -> "+u->coef()+" : "+sv+" : "+lv);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdUnitMgr::_current(bGenericXMLBaseElement *elt, void *prm, int indent){
_bTrace_("bStdUnitMgr::_current",false);
bStdUnitMgr*	mgr=(bStdUnitMgr*)prm;
char			clssname[256];
char			value[256];
	
	elt->getclassname(clssname);
	if(!strcmp("int",clssname)){
		elt->getvalue(value);
		mgr->set_current(atoi(value));
_tm_("cur : "+value);
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUnitMgr::make(const char* fpath, const char* fname){
_bTrace_("bStdUnitMgr::make",_DEVELOPMENT_VERS_);
_tm_("fpath="+fpath);
_tm_("fname="+fname);
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,fname,kCFStringEncodingMacRoman);
	if(!cfs){
_te_("cfs == NULL at CFStringCreateWithCString");
		return(false);
	}
CFURLRef url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),cfs,CFSTR("xml"),NULL);
	CFRelease(cfs);
	if(!url){
_te_("url == NULL");
		return(false);
	}
char	path[PATH_MAX];
	cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
	CFRelease(url);
	if(!cfs){
_te_("cfs == NULL at CFURLCopyFileSystemPath");
		return(false);
	}
	CFStringGetCString(cfs,path,SHRT_MAX,kCFStringEncodingMacRoman);
_tm_("path="+path);
	CFRelease(cfs);
bStdFile	rsrc(path,"r");
	if(rsrc.status()){
_te_("rsrc file error "+rsrc.status());
	}
void*		buffer;
int			sz;
	rsrc.mount((char**)&buffer,&sz);
	if(rsrc.status()){
_te_("rsrc file mount error "+rsrc.status());
	}
bMacMapDocument*		doc=(bMacMapDocument*)bMacMapApp::instance()->document();
	doc->writeParam(buffer,sz,"",fpath);
	free(buffer);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnitMgr::sort(int(*compare)(const void*,const void*)){
	_elts.sort(compare);
}


// ---------------------------------------------------------------------------
// OLD PTH_LOAD
// -----------
/*bXMLFile f(path,"r");
bGenericXMLBaseElement*	root;
	f.mount(&root);
	if(!root){
_te_("root == NULL");
		return(-1);
	}
	(void)root->dotoall(this,0,_load);
	if(count()==0){
		clssmgr->ReleaseXMLInstance(root);
_te_("count==0");
		return(-1);
	}
	(void)root->dotoall(this,0,_current);
	if(_cur==0){
		_cur=1;
	}*/

// ---------------------------------------------------------------------------
// OLD PTH_UNLOAD
// -----------
/*bXMLFile f(path,"w");
	if(!f.status()){
bArray	arr(sizeof(xmlelt));
		add_cdesc(arr,0,"param","");
		add_cdesc(arr,1,"version","3.0.0");
		add_idesc(arr,1,"int",_cur);
	
bGenericUnit*	u;
char			c[256];

		for(int i=1;i<=count();i++){
			u=get(i);
			u->name_id(c);
			add_ddesc(arr,1,"float",u->coef(),15);
			add_cdesc(arr,1,"name",c);
_tm_(c+" "+u->coef());
		}
	
bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
		free_descs(arr);
		if(!root){
_te_("root == NULL");
			return(false);
		}
		root->encode(0,&f);
	
		clssmgr->ReleaseXMLInstance(root);
	}*/
