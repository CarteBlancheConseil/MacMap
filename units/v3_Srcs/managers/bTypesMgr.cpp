//----------------------------------------------------------------------------
// File : bTypesMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Types management class
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
// 16/01/2004 creation.
//----------------------------------------------------------------------------

#include "bTypesMgr.h"
#include "PathMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bStdFile.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTypesMgr	::bTypesMgr()
			:_elts(sizeof(bMacMapType*)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTypesMgr::~bTypesMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypesMgr::load(){
_bTrace_("bTypesMgr::load",true);
char			bsepath[PATH_MAX];
	GetBasePath(_MMAPP_,bsepath);	
bStdDirectory	acc(bsepath);	
int				n=PATH_count();
OSStatus		status;
	
	for(int i=1;i<=n;i++){
		if((status=load(i))){
			if(!PATH_rmv(i)){
				return(status);
			}
			i--;
			n--;
		}
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypesMgr::load(int idx){
_bTrace_("bTypesMgr::load",true);
_tm_("bTypesMgr::loading type number "+idx);

int				status=0;
bMacMapType*	tp=new bMacMapType(idx,&status);
	if(status){
_te_("status = "+status+" at new");
		delete tp;
		return(kTypMgrLoadTypeNewErr_);
	}
	
	if(!tp->load()){
_te_("tp->load() == false");
		delete tp;
		return(kTypMgrLoadTypeLoadErr_);
	}
	
	if(!_elts.add(&tp)){
_te_("_elts->add == false");
		delete tp;
		return(kTypMgrLoadAddElementErr_);
	}
	
char	c[256];
	tp->name(c);	
_tm_("Type name = "+c+", id="+tp->tid());
_tm_("NbAlive = "+tp->nb_live());
_tm_("NbKilled = "+tp->nb_killed());
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypesMgr::unload(){
_bTrace_("bTypesMgr::unload",true);
int				n=count();
bMacMapType*	tp;
	
/* NEW : POUR RETRAIT DES TYPES MARQUES */
	for(int i=n;i>0;i--){
		if(!_elts.get(i,&tp)){
_te_("_elts->get == false");
			return(kTypMgrUnloadGetElementErr_);
		}
		if(tp->is_remove()){
			if(PATH_rmv(i)==false){
_te_("PATH_rmv failed for "+i);
				return(-3);
			}
		}
	}
/* NEW : POUR RETRAIT DES TYPES MARQUES */
	
	for(int i=1;i<=n;i++){
		if(!_elts.get(i,&tp)){
_te_("_elts->get == false");
			return(kTypMgrUnloadGetElementErr_);
		}
		if(!tp){
_te_("tp==NULL");
			return(kTypMgrUnloadTypeNullErr_);
		}
		tp->save();
		delete tp;
	}
	_elts.reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// Création, relatif ou absolu
// -----------
int bTypesMgr::add(	const char* name,	// name
					const char* path,	// access path
					const char* data,	// additionnal data
					int tblkind,		// data storage signature
					int typkind){		// type kind
_bTrace_("bTypesMgr::add",true);
char			bsepath[PATH_MAX];
	GetBasePath(_MMAPP_,bsepath);	
bStdDirectory	acc(bsepath);	
	
	if(index(name)!=0){
_te_("type "+name+" exist !");
		return(kTypMgrAddOpenTypeExistErr_);
	}
int				status=noErr;
bMacMapType*	tp=new bMacMapType(	name,
									path,
									data,
									typkind,
									tblkind,
									&status);
	if(!tp){
_te_("tp == NULL at new type");
		return(kTypMgrAddCreateTypeNewErr_);
	}
	delete tp;
	if(status){
_te_("status == "+status+" at new type");
		return(kTypMgrAddCreateTypeNewErr_);
	}
char	fullpath[PATH_MAX];
	if(strlen(path)>0){
		sprintf(fullpath,"%s/%s.type/",path,name);
	}
	else{
		sprintf(fullpath,"%s.type/",name);
	}
int	idx=PATH_add(tblkind,fullpath);
	if(idx<=0){
_te_("PATH_add == "+idx);
		return(kTypMgrAddOpenPathAddErr_);
	}

	if(load(idx)){
_te_("load "+idx+" != noErr");
		PATH_rmv(idx);
		return(kTypMgrAddOpenLoadErr_);
	}
	tp=get(idx);
	
	eventmgr->create(tp);

	return(noErr);
}

// ---------------------------------------------------------------------------
// Ouverture, relatif ou absolu
// -----------
int bTypesMgr::add(const char* name, const char* path, int tblkind){
_bTrace_("bTypesMgr::add",true);
	if(index(name)!=0){
_te_("type "+name+" exist !");
		return(kTypMgrAddOpenTypeExistErr_);
	}
char			bsepath[PATH_MAX];
	GetBasePath(_MMAPP_,bsepath);	
bStdDirectory	acc(bsepath);	
	if(acc.status()!=0){
		_te_("acc.status()="+acc.status());
	}
	
_PUSH_
bStdDirectory	d(path);
bStdFile		f("Contents/Datas/storage.txt","r");
	if(!f.status()){
char	k[256];
		f.next_line(k,256);
		tblkind=atoi(k);
	}
_POP_
int	idx=PATH_add(tblkind,path);
	if(idx<=0){
_te_("PATH_add == "+idx);
		return(kTypMgrAddOpenPathAddErr_);
	}

	if(load(idx)){
_te_("load "+idx+" != noErr");
		PATH_rmv(idx);
		return(kTypMgrAddOpenLoadErr_);
	}
bGenericType* tp=get(idx);

	eventmgr->create(tp);

	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypesMgr::rmv(int idx){
_bTrace_("bTypesMgr::rmv",true);
bMacMapType*	tp=get(idx);
	if(!tp){
_te_("bad index :"+idx);
		return(-1);
	}
		
	tp->set_remove(true);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bTypesMgr::rename(int idx, const char* name){
_bTrace_("bTypesMgr::rename",true);
bMacMapType* tp=get(idx);
	if(tp->is_lock()){
_te_("locked type");
		return(-1);	
	}
int		db,i;
char	buff[PATH_MAX],oldpath[PATH_MAX],newpath[PATH_MAX];

	if(PATH_get(idx,&db,buff)==false){
		return(-1);
	}
	strcpy(oldpath,buff);
	for(i=strlen(buff)-2;i>=0;i--){
		if(buff[i]=='/'){
			buff[i+1]=0;
			break;
		}
	}
	if(i<0){
		buff[0]=0;
	}
	sprintf(newpath,"%s%s.type/",buff,name);
_tm_(newpath);	

char			bsepath[PATH_MAX];
	GetBasePath(_MMAPP_,bsepath);	
bStdDirectory	acc(bsepath);	
	
_PUSH_
bStdFile		f(oldpath,"r");

	if(f.status()){
_te_("f.status()=="+f.status());
		return(-2);
	}
	f.change_name(newpath);
_POP_
	
	if(PATH_change(idx,newpath)==false){
_te_("PATH_change failed");
		return(-3);
	}

	if(!tp){
_te_("ge");
		return(-4);
	}
	tp->set_name(name);
	eventmgr->modify(tp);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bTypesMgr::count(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// 
// ------------
bMacMapType* bTypesMgr::get(int idx){
bMacMapType*	tp;
	if(!_elts.get(idx,&tp)){
		return(NULL);
	}
	return(tp);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bTypesMgr::index(const char* tname){
	if(tname==NULL){
		return(0);
	}
bMacMapType*	tp;
int				n=count();
char			s1[256],s2[256];

	sprintf(s1,"%s",tname);
	strupper(s1);
	for(int i=1;i<=n;i++){
		if(!_elts.get(i,&tp)){
			return(0);
		}
		tp->name(s2);
		strupper(s2);
		if(strcmp(s1,s2)==0){
			return(i);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bTypesMgr::index(double tid){
bMacMapType*	tp;
int				n=count();

	for(int i=1;i<=n;i++){
		if(!_elts.get(i,&tp)){
			return(0);
		}
		if(tp->tid()==tid){
			return(i);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTypesMgr::make(){
_bTrace_("bTypesMgr::make",true);
	return(PATH_make());
}
