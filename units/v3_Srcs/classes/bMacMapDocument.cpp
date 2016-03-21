//----------------------------------------------------------------------------
// File : bMacMapDocument.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap document class
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
// 01/12/2004 creation.
//----------------------------------------------------------------------------

#include "bMacMapDocument.h"
#include "bMacMapApp.h"
#include "bMacMapGeoElement.h"
#include "bStyleMgr.h"
#include "bDistUnitMgr.h"
#include <mox_intf/bXMLFile.h>
#include <unistd.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// -----------
#define _fpath	"Contents/Resources/docprefs.xml"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapDocument::bMacMapDocument(){
	_reso_coef=1;
	_ox=0;
	_oy=0;
	_reso_idx=10;
	_digits=6;
	_srid=-1;
int	status=noErr;
	_styles=new bVirtualStyleMgr(&status);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMacMapDocument::~bMacMapDocument(){
	if(_styles){
		delete _styles;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapDocument::name(char* name){
	strcpy(name,_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapDocument::location(char* path){
_bTrace_("bMacMapDocument::location(char*)",_DEVELOPMENT_VERS_);
	path[0]=0;
	strcpy(path,_path);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapDocument::srid(){
	return(_srid);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::set_srid(int srid){
_bTrace_("bMacMapDocument::set_srid",_DEVELOPMENT_VERS_);
	if(_srid!=-1){
_te_("srid alreay set to "+_srid);
		return(false);
	}

bGenericType*	tp;
int				tsrid;

	for(int i=1;i<=typesmgr->count();i++){
		tp=typesmgr->get(i);
		tp->fields()->h_read(1,kHDR_srid_,&tsrid);
		if((tsrid!=-1)&&(tsrid!=srid)){
_te_("srid alreay set to "+tsrid+" for type "+i);
		}
		else if(tp->fields()->h_write(1,kHDR_srid_,&srid)){
_te_("h_write failed type "+i);
		}
	}		
	_srid=srid;
	pref_unload();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
d2dvertex bMacMapDocument::get_origin(){
d2dvertex	vx={_ox,_oy};
	return(vx);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::set_origin(d2dvertex  vx){
	_ox=vx.x;
	_oy=vx.y;
	pref_unload();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bMacMapDocument::dist_resolution(){
	return(_reso_coef);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericUnit* bMacMapDocument::dist_resolution_unit(){
	return(distmgr->get(_reso_idx));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapDocument::dist_pref_digits(){
	return(_digits);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapDocument::set_dist_pref_digits(int digits){
	_digits=digits;
	pref_unload();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapDocument::make(const char* path, int unit, double reso, int srid){
_bTrace_("bMacMapDocument::make(const char*, int, double, int)",_DEVELOPMENT_VERS_);
	
	strcpy(_path,path);
	UTF82MacRoman(_path,PATH_MAX);
	read_name();
	
	_reso_idx=unit;
	_reso_coef=reso;
	_srid=srid;
		
	
char	bsepath[PATH_MAX];
	map_doc->location(bsepath);
_tm_(bsepath);
	if(bsepath[strlen(bsepath)-1]=='/'){
		bsepath[strlen(bsepath)-1]=0;
	}
char*	c=strrchr(bsepath,'/');
	if(c){
		c[0]=0;
	}
_tm_(bsepath);
bStdDirectory folder(bsepath);

	
char	name[FILENAME_MAX];
	sprintf(name,"%s.mmproject",_name);
	MacRoman2UTF8(name,FILENAME_MAX);// UTF8 File System

mode_t	msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
	if(mkdir(name,msk)){
_te_("mkdir \""+name+"\" failed");
		return(false);
	}
	
	UTF82MacRoman(name,FILENAME_MAX);// bStdFile/bStdDirectory MacRoman File System
bStdDirectory base(name);	
	if(!make_package()){
		_te_("make_package failed");
		return(false);
	}
	if(!pref_unload()){
		_te_("pref_unload failed");
		return(false);
	}
	if(!distmgr->make()){
		_te_("distmgr->make failed");
		return(false);
	}
	distmgr->set_current(_reso_idx);
	if(!scalemgr->make()){
		_te_("scalemgr->make failed");
		return(false);
	}
	if(!typesmgr->make()){
		_te_("typemgr->make failed");
		return(false);
	}
	if(!viewmgr->make()){
		_te_("viewmgr->make failed");
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapDocument::load(const char* path){
_bTrace_("bMacMapDocument::load(const char*)",_DEVELOPMENT_VERS_);
OSStatus	status=noErr;

_tm_("path:"+path);
	
	_state=0;
	if(strlen(path)>=PATH_MAX){
		return(-1);
	}
	strncpy(_path,path,PATH_MAX);	
	UTF82MacRoman(_path,PATH_MAX);
	read_name();

	for(;;){
		if(!pref_load()){
_te_("pref_load failed");
			status=-1;
			break;
		}
		_state++;// 1
		if((status=distmgr->load())){
_te_("distmgr->load()=="+status);
			break;
		}
		_state++;// 2
		if(distmgr->count()<=0){
_te_("distmgr->count()<=0");
			status=-1;
			break;
		}
		_state++;// 3
		if((status=scalemgr->load())){
_te_("scalemgr->load()=="+status);
			status=-1;
			break;
		}
		_state++;// 4
		if(scalemgr->count()<=0){
_te_("scalemgr->count()<=0");
			status=-1;
			break;
		}
		
		_state++;// 5
//		map_wd->awake(_name);// Ici car on ˆ besoin des Žchelles
		
		_state++;		
		if((status=typesmgr->load())){
_te_("typesmgr->load()=="+status);
			break;
		}
		if((status=_styles->load())){
_te_("_styles->load()=="+status);
			break;
		}
		
		_state++;// 6
		if((status=viewmgr->load())){
_te_("layersmgr->load()=="+status);
			break;
		}
		_state++;// 7
		if((status=layersmgr->load())){
_te_("layersmgr->load()=="+status);
			break;
		}
		_state++;// 8
		if((status=printmgr->load())){
_te_("printmgr->load()=="+status);
			break;
		}
		_state++;// 9
		eventmgr->enable();
		
		selmgr->incState();
		cntmgr->incState();
		
		calcmgr->open();
		varmgr->open();
		macromgr->open();
		scriptmgr->open();
		
		geogmgr->open();
		toolmgr->open();
		xmapmgr->open();
		vdefmgr->open();
		xboxmgr->open();
				
char msg[__MESSAGE_STRING_LENGTH_MAX__];
		message_string(kMsgNoSelection,msg,true);
		WriteMessage(_MMAPP_,msg);
		
_tm_("dist_resolution ="+dist_resolution());
bGenericUnit*	u;
		u=dist_resolution_unit();
_tm_("dist_resolution_unit = "+u->coef()+"/"+u->conv());
		u=distmgr->get();
_tm_("dist_resolution_pref = "+u->coef()+"/"+u->conv());
		
		break;
	}
		
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapDocument::unload(){
_bTrace_("bMacMapDocument::unload",_DEVELOPMENT_VERS_);

_tm_("unload with state at "+_state);
	if(_state>=9){		
		eventmgr->disable();
		
		scriptmgr->close();
		macromgr->close();
		xboxmgr->close();
		vdefmgr->close();
		xmapmgr->close();
		toolmgr->close();
		geogmgr->close();
		varmgr->close();
		calcmgr->close();
	}

	if(_state>=8){
		(void)layersmgr->unload();
	}
	
	if(_state>=9){
		(void)selmgr->unload();
		(void)cntmgr->unload();
	}	
	
	if(_state>=5){
		(void)typesmgr->unload();	
		(void)_styles->unload();	
	}
	
	if(_state>=7){
		(void)viewmgr->unload();
	}
	
	if(_state>=2){
		(void)distmgr->unload();
	}
	if(_state>=4){
		(void)scalemgr->unload();
	}
	
	if(_state>=9){
		(void)printmgr->unload();
	}
	
	if(_state>=9){
//		(void)map_wd->collapse();
	}	
//	(void)clssmgr->unload();
	
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	message_string(kMsgNoDocument,msg,true);
	WriteMessage(_MMAPP_,msg);
	
	_state=0;
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericStyleMgr* bMacMapDocument::styles(){
	return(_styles);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapDocument::set_save_objstate(bool prf){
	bMacMapGeoElement::set_save_objstate(prf);
	pref_unload();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::get_save_objstate(){
	return(bMacMapGeoElement::get_save_objstate());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::writeTree(bGenericXMLBaseElement* elt, int sign, const char* name){
char	fname[FILENAME_MAX];
char	path[PATH_MAX];
	sprintf(fname,"%s.xml",name);
	sprintf(path,"%.4s",(char*)&sign);
	return(writeXMLParam(elt,path,fname));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::readTree(bGenericXMLBaseElement** elt, int sign, const char* name){
char	fname[FILENAME_MAX];
char	path[PATH_MAX];
	sprintf(fname,"%s.xml",name);
	sprintf(path,"%.4s",(char*)&sign);
	return(readXMLParam(elt,path,fname));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::removeTree(int sign, const char* name){
_bTrace_("bMacMapDocument::removeTree",_DEVELOPMENT_VERS_);
char			path[256];
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
	sprintf(path,"%.4s",(char*)&sign);
	if(chdir(path)){
_tm_("folder "+path+" not found");
		return(false);
	}
	sprintf(path,"%s.xml",name);
bXMLFile	f(path,"r");
	if(f.status()){
_tm_("f.status()="+f.status());
		return(false);
	}
	f.erase();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapDocument::countParams(const char* sub){
_bTrace_("bMacMapDocument::countParams",_DEVELOPMENT_VERS_);
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(0);
	}
bStdDirectory	rsub(sub);
	rsub.mount();
	return(rsub.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::getParamName(const char* sub, int idx, char* name){
_bTrace_("bMacMapDocument::getParamName",_DEVELOPMENT_VERS_);
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
bStdDirectory	rsub(sub);
	rsub.mount();
	rsub.name(idx,name);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::writeXMLParam(bGenericXMLBaseElement* elt, const char* sub, const char* name){
_bTrace_("bMacMapDocument::writeXMLParam",_DEVELOPMENT_VERS_);
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
	if((sub[0]!=0)&&(chdir(sub))){
mode_t		msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
		if(mkdir(sub,msk)){
_te_("mkdir failed for "+sub);
			return(false);
		}
		if(chdir(sub)){
_te_("chdir failed for "+sub);
			return(false);
		}
	}
bStdFile	f(name,"w");
	if(f.status()){
_te_("f.status()="+f.status());
		return(false);
	}
	if(elt!=NULL){
		elt->encode(0,&f);
	}
	else{
		f.erase();
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::readXMLParam(bGenericXMLBaseElement** elt, const char* sub, const char* name){
_bTrace_("bMacMapDocument::readXMLParam",_DEVELOPMENT_VERS_);
	(*elt)=NULL;
	if(strstr(name,".xml")==NULL){
		return(false);
	}
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
bStdDirectory	rsub(sub);
	if(rsub.status()){
_tw_("folder "+sub+" not found");
// Pas de return pour pouvoir accŽder ˆ ce qui est ˆ la racine (types, scales,...)
	}
bXMLFile	f(name,"r");
	if(f.status()){
_tm_("f.status()="+f.status());
		return(false);
	}
	f.mount(clssmgr,elt);
	return((*elt)!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::writeParam(void* buffer, int sz, const char* sub, const char* name){
_bTrace_("bMacMapDocument::writeParam",_DEVELOPMENT_VERS_);
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
	if((sub[0]!=0)&&(chdir(sub))){
mode_t		msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
		if(mkdir(sub,msk)){
_te_("mkdir failed for "+sub);
			return(false);
		}
		if(chdir(sub)){
_te_("chdir failed for "+sub);
			return(false);
		}
	}
bStdFile	f(name,"w");
	if(f.status()){
_te_("f.status()="+f.status());
		return(false);
	}
	if(buffer!=NULL){
		f.write(buffer,sz);
	}
	else{
		f.erase();
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapDocument::readParam(void** buffer, int* sz, const char* sub, const char* name){
_bTrace_("bMacMapDocument::readParam",_DEVELOPMENT_VERS_);
	(*buffer)=NULL;
bStdDirectory	root(_path);
bStdDirectory	rsrc("Contents/Resources/");
	if(rsrc.status()){
_te_("rsrc.status()="+rsrc.status());
		return(false);
	}
bStdDirectory	rsub(sub);
	if(rsub.status()){
_tw_("folder "+sub+" not found");
// Pas de return pour pouvoir accŽder ˆ ce qui est ˆ la racine (types, scales,...)
	}
bStdFile	f(name,"r");
	if(f.status()){
_tm_("f.status()="+f.status());
		return(false);
	}
	f.mount((char**)buffer,sz);
	return(f.status()==0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapDocument::read_name(){
_bTrace_("bMacMapDocument::read_name",_DEVELOPMENT_VERS_);
int		i,n;
bool	on=false;
	
	n=strlen(_path);
	for(i=n-1;i>0;i--){
		if(_path[i]=='.'){
			on=true;
			n=i;
			continue;
		}
		if(on){
			if(_path[i]=='/'){
				break;
			}
		}
	}
	if(i==0){
		i=1;
	}
	else{
		i++;
	}
	memmove(_name,&_path[i],n-i);
	_name[n-i]=0;
	
_tm_("start=="+i+", length=="+(n-i));
_tm_("document name=="+_name);
}

// ---------------------------------------------------------------------------
// Implique d'avoir bien ajoutŽ l'extension
// ------------
bool bMacMapDocument::make_package(){
_bTrace_("bMacMapType::make_package",_DEVELOPMENT_VERS_);
mode_t	msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
	if(mkdir("Contents",msk)){
_te_("mkdir failed for Contents");
		return(false);
	}
bStdDirectory	cnt("Contents");
	if(mkdir("Datas",msk)){
_te_("mkdir failed for Datas");
		return(false);
	}
	if(mkdir("Resources",msk)){
_te_("mkdir failed for Resources");
		return(false);
	}
	if(mkdir("Resources disabled",msk)){
_te_("mkdir failed for Resources disabled");
		return(false);
	}
bStdFile	info("PkgInfo","w");
	info.write("projMapª");
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapDocument::pref_load(){
_bTrace_("bMacMapDocument::pref_load",_DEVELOPMENT_VERS_);
bStdDirectory			base(_path);
bXMLFile				f(_fpath,"r");
bGenericXMLBaseElement*	root;

	_reso_coef=1;
	_ox=0;
	_oy=0;
	_reso_idx=10;
	_digits=6;
	_srid=-1;

	f.mount(clssmgr,&root);
	if(!root){
_te_("root == NULL");
		return(false);
	}
_tm_("mount done");

bGenericXMLBaseElement*	elt;
char					val[256];
	
	elt=clssmgr->NthElement(root,1,"int");
	if(elt){
		elt->getvalue(val);
		_reso_idx=atoi(val);
	}
	elt=clssmgr->NthElement(root,2,"int");
	if(elt){
		elt->getvalue(val);
		_digits=atoi(val);
	}
	elt=clssmgr->NthElement(root,3,"int");
	if(elt){
		elt->getvalue(val);
		_srid=atoi(val);
	}
	
	elt=clssmgr->NthElement(root,1,"float");
	if(elt){
		elt->getvalue(val);
		_reso_coef=matof(val);
	}
	elt=clssmgr->NthElement(root,2,"float");
	if(elt){
		elt->getvalue(val);
		_ox=matof(val);
	}
	elt=clssmgr->NthElement(root,3,"float");
	if(elt){
		elt->getvalue(val);
		_oy=matof(val);
	}

	elt=clssmgr->NthElement(root,1,"bool");
	if(elt){
		elt->getvalue(val);
		bMacMapGeoElement::set_save_objstate(atoi(val));
_tm_(bMacMapGeoElement::get_save_objstate());
	}
_tm_("read done");
	clssmgr->ReleaseXMLInstance(root);
_tm_("reso idx ="+_reso_idx);
_tm_("reso coef ="+_reso_coef);
_tm_("origin x ="+_ox);
_tm_("origin y ="+_oy);
_tm_("reso coef ="+_reso_coef);
_tm_("nb digits ="+_digits);
_tm_("srid ="+_srid);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapDocument::pref_unload(){
_bTrace_("bMacMapDocument::pref_unload",_DEVELOPMENT_VERS_);
bStdDirectory	base(_path);
bXMLFile		f(_fpath,"w");
bArray			arr(sizeof(xmlelt));

	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0");
	add_idesc(arr,1,"int",_reso_idx);
	add_ddesc(arr,1,"float",_reso_coef,15);
	add_idesc(arr,1,"int",_digits);
	add_idesc(arr,1,"int",_srid);
	add_idesc(arr,1,"bool",bMacMapGeoElement::get_save_objstate());
	add_ddesc(arr,1,"float",_ox,15);
	add_ddesc(arr,1,"float",_oy,15);

bGenericXMLBaseElement* root=clssmgr->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return(false);
	}
	root->encode(0,&f);
	clssmgr->ReleaseXMLInstance(root);
	return(true);
}

