//----------------------------------------------------------------------------
// File : bMacMapType.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap type class
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
// 13/01/2004 creation.
//----------------------------------------------------------------------------

#include "bMacMapType.h"
#include "bMacMapApp.h"
#include "bStyleMgr.h"
#include "bMacMapGeoElement.h"
#include "PathMgr.h"
#include <mox_intf/bStdWait.h>
#include <mox_intf/ext_utils.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/base_def.h>

// ---------------------------------------------------------------------------
//
// ------------
#ifdef __LITTLE_ENDIAN__
#define kLDASignature	'rAdL'
#else
#define kLDASignature	'LdAr'
#endif
#define kLDAParamName	"load_area"

// ---------------------------------------------------------------------------
// 
// ------------
static void SetFlag(void* o, bool b){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	geo->sethandled(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
static bool GetFlag(void* o){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	return(geo->handled());
}

// ---------------------------------------------------------------------------
// 
// ------------
static void GetBounds(void* o, ivx_rect* r){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	geo->getBounds(r);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int add_to_array(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
bArray*				arr=(bArray*)prm;
					
	arr->add(&geo);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int get_rect(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
ivx_rect*			vxr=(ivx_rect*)prm;
	if(	(vxr->top==0)		&&
		(vxr->bottom==0)	&&
		(vxr->left==0)		&&
		(vxr->right==0)		){
		geo->getBounds(vxr);
	}
	else{
ivx_rect			bnd;
		geo->getBounds(&bnd);
		ivr_union(vxr,&bnd,vxr);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static void read_name(char* path, char* name){
	name[0]=0;
char*	tmp=strrchr(path,'/');
	if(!tmp){
		return;
	}
	tmp--;
int	dot=-1,first=-1,len=strlen(path)-1;

	for(int i=len-1;i>-1;i--){
		if(path[i]=='/'){
			first=i+1;
			break;
		}
		else if((path[i]=='.')&&(dot==-1)){
			dot=i;
		}
	}
	if(first==-1){
		first=0;
	}
	if(dot==-1){
		dot=len;
	}
int x=dot-first;
    if(x>FILENAME_MAX){
        x=FILENAME_MAX;
    }
    else if(x<0){
        x=0;
    }
	strncpy(name,&path[first],x);
	name[dot-first]=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
static int get_sign(int k){
char s[5];
	sprintf(s,"%04d",k);
	return((*((int*)s)));
}

// ---------------------------------------------------------------------------
// Constructeur::ouverture
// ------------
bMacMapType::bMacMapType(int t, int *status)
			:_bse(this){
_bTrace_("bMacMapType::bMacMapType(int,int*)",true);
int		tbltype;
char	path[FILENAME_MAX+1],tname[FILENAME_MAX+1];
	
	_iter=NULL;
	_styles=NULL;
	_name[0]=0;
	_idx=_MMAPP_->typesMgr()->count()+1;
	_rmv=0;
    read_load_area();
                
	if(!PATH_get(t,&tbltype,path)){
		(*status)=-1;
_te_("status ="+(*status)+" at PATH_get");
		return;
	}
	
	tbltype=get_sign(tbltype);
_tm_("opening type with "+(UInt32*)&tbltype+" storage");
	*status=_bse.open(path,_MMAPP_->document()->srid(),GetRawPrecisionCoef(_MMAPP_));
	if(*status){
_te_("status = "+(*status)+" at new _bse.open "+path);
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_id_,&_id))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_id_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_state_,&_state))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_state_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_kind_,&_kind))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_kind_");
		return;
	}
	
	/*if((*status)=_bse.h_read(1,kHDR_bounds_,&_bounds)){
_te_("status = "+(*status)+" at _bse.h_read kHDR_bounds_");
		return;
	}*/
	_bounds.top=1-__BOUNDS_MAX__;
	_bounds.left=1-__BOUNDS_MAX__;
	_bounds.bottom=1+__BOUNDS_MAX__;
	_bounds.right=1+__BOUNDS_MAX__;
				
	if(((*status)=_bse.h_read(1,kHDR_name_,tname))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_name_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_precs_,&_precs))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_precs_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_unit2m_,&_unit2m))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_unit2m_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_srid_,&_srid))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_srid_");
		return;
	}
	
//	snprintf(_name,256,tname);
	strncpy(_name,tname,sizeof(_name)-1);
				
	read_name(path,tname);
	if((strlen(tname)>0)&&(strcmp(_name,tname))){
_tw_("replacing type name \""+_name+"\" to \""+tname+"\"");
//		snprintf(_name,256,tname);
		strncpy(_name,tname,sizeof(_name)-1);
		if(((*status)=_bse.h_write(1,kHDR_name_,tname))){
_te_("status = "+(*status)+" at _bse.h_write kHDR_name_");
			_name[0]=0;
			return;
		}
	}
	
gi_init_p gp={&_bounds,status,SetFlag,GetFlag,GetBounds,ObjIDComp};
	_iter=(bGenericGeoIterator*)(extmgr->get_component(kComponentGeoIterator)->i_allocate(tbltype,_MMAPP_,&gp));
	if(!_iter){
_te_("no iterator");
		(*status)=-1;
		return;
	}

	_styles=new bStyleMgr(this,status);
	if((*status)){
_te_("status = "+(*status)+" at new bStyleMgr");
		return;
	}		
	
	_nbalive=0;
	_nbkilled=0;	
	_nsel=0;
	_ncnt=0;
		
	(*status)=noErr;
}

// ---------------------------------------------------------------------------
// Constructeur::création
// ------------
bMacMapType::bMacMapType(	const char* hname, 
							const char* hpath,
							const char* data,
							int hkind,
							int ttype, 
							int* status)
			:_bse(this){
_bTrace_("bMacMapType::bMacMapType(char*,char*,char*,int,int*)",true);
	_iter=NULL;
	_styles=NULL;
	_idx=_MMAPP_->typesMgr()->count()+1;
	_kind=hkind;
	_rmv=0;
    read_load_area();
                
int	xtype=get_sign(ttype);

_tm_("creating type with "+(UInt32*)&xtype+" storage");
_tm_("creating type hname="+hname);
_tm_("creating type hpath="+hpath);
//_tm_("creating type data="+data);
_tm_("creating type hkind="+hkind);
_tm_("creating type ttype="+ttype);
	
	_id=time(NULL);
	_state=0;
	_kind=hkind;
	_bounds.top=-__BOUNDS_MAX__;
	_bounds.left=-__BOUNDS_MAX__;
	_bounds.bottom=__BOUNDS_MAX__;
	_bounds.right=__BOUNDS_MAX__;
//	snprintf(_name,256,hname);
//_tm_("avant strncpy");
	strncpy(_name,hname,sizeof(_name)-1);

bGenericUnit*	u=map_doc->dist_resolution_unit();

	_precs=map_doc->dist_resolution();
	_unit2m=u->coef();
	_srid=map_doc->srid();
	_nbalive=0;
	_nbkilled=0;
	_nsel=0;
	_ncnt=0;

//_tm_("avant _bse.create");
	*status=_bse.create(hpath,xtype,_srid,_precs,_unit2m,_name,data,_kind);
	if(*status){
_te_("status = "+(*status)+" at _bse.create");
		return;
	}
		
//_tm_("avant make_style");
	if(!make_style()){
		(*status)=-1;
_te_("make_style == false");
		return;
	}

	if(((*status)=_bse.h_read(1,kHDR_id_,&_id))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_id_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_state_,&_state))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_state_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_kind_,&_kind))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_kind_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_bounds_,&_bounds))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_bounds_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_name_,_name))){
_te_("status = % atd _bse.h_read kHDR_name_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_precs_,&_precs))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_precs_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_unit2m_,&_unit2m))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_unit2m_");
		return;
	}
	if(((*status)=_bse.h_read(1,kHDR_srid_,&_srid))){
_te_("status = "+(*status)+" at _bse.h_read kHDR_srid_");
		return;
	}

_tm_("bounds are ("+_trrect_(_bounds));

gi_init_p gp={&_bounds,status,SetFlag,GetFlag,GetBounds,ObjIDComp};
	_iter=(bGenericGeoIterator*)(extmgr->get_component(kComponentGeoIterator)->i_allocate(xtype,_MMAPP_,&gp));
	if(!_iter){
		(*status)=-1;
		return;
	}
		
	_styles=new bStyleMgr(this,status);
	if((*status)){
_te_("status = "+(*status)+" at new bStyleMgr");
		return;
	}
	
	(*status)=noErr;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMacMapType::~bMacMapType(){
_bTrace_("bMacMapType::~bMacMapType",true);

_tm_("deleting "+_idx+" "+_name);

	if(_iter){
bMacMapGeoElement*	geo;
bArray				arr(sizeof(bMacMapGeoElement*));
char				cftitle[__MESSAGE_STRING_LENGTH_MAX__];
char				cfprompt[__MESSAGE_STRING_LENGTH_MAX__];
		message_string(kMsgClose,cftitle,true);
		strcpy(cfprompt,_name);

_tm_("iterate");
		_iter->iterate(&arr,add_to_array);
_tm_("flush iterator");
		_iter->flush();
_tm_((int)arr.count()+" objects to delete");

bProgressWait	wait(cftitle,cfprompt,true,false,arr.count());
		for(long i=1;i<=arr.count();i++){
//_tm_(i);
			wait.set_progress(i);
			if(arr.get(i,&geo)){
				delete geo;
			}
		}
		arr.reset();
_tm_("deleting _iter");
		extmgr->get_component(kComponentGeoIterator)->i_free(_iter);
	}
	
	if(_styles){
_tm_("deleting _styles");
		_styles->unload();
		delete _styles;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::load(){
_bTrace_("bMacMapType::load",true);
bool		k,b;
long		i,n;
char		cftitle[__MESSAGE_STRING_LENGTH_MAX__],cfprompt[__MESSAGE_STRING_LENGTH_MAX__];
	
_tm_(_idx+" "+_name);

	message_string(kMsgOpen,cftitle,true);
	strcpy(cfprompt,_name);
	
	k=true;
	n=_bse.count_records();

bProgressWait	wait(cftitle,cfprompt,true,true,n);
	
	for(i=1;i<=n;i++){
		if(!wait.set_progress(i)){
			_iter->flush();
			break;
		}
		if(!load_object(i,&b)){
			k=false;
_te_("false at load_object "+i);
			break;
		}
		if(!b){
			continue;
		}
	}
	
	if(k){
		_iter->start();
		
		_bounds.top=_bounds.left=_bounds.bottom=_bounds.right;
		_iter->iterate(&_bounds,get_rect);
		_iter->set_bounds(&_bounds);
	}
	
	
	(void)_styles->load();
		
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::save(){
_bTrace_("bMacMapType::save",true);
	if(is_lock()){
		return;
	}
	(void)_bse.h_write(1,kHDR_state_,&_state);
	// le genre ne change pas
	if(_iter){
		_iter->bounds(&_bounds);
	}
	(void)_bse.h_write(1,kHDR_bounds_,&_bounds);
	(void)_bse.h_write(1,kHDR_name_,_name);
	// la précision ne change pas
	// l'unité ne change pas
	// la précision ne change pas
	// le libellé court ne change pas
	// le libellé long ne change pas
	(void)_bse.h_write(1,kHDR_alive_,&_nbalive);
	(void)_bse.h_write(1,kHDR_killed_,&_nbkilled);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::pack(){
	if(is_lock()){
		return;
	}
	//	_bse.pack();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::name(char *tname){	
	sprintf(tname,"%s",_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::set_name(const char *tname){
	if(is_lock()){
		return;
	}
	strcpy(_name,tname);	
	(void)_bse.h_write(1,kHDR_name_,_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bMacMapType::tid(){
	return(_id);
}

// ---------------------------------------------------------------------------
// 
int bMacMapType::index(){	
	return(_idx);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::kind(){	
	return(_kind);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::nb_rec(){	
	return(_nbalive+_nbalive);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::nb_live(){	
	return(_nbalive);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::nb_killed(){	
	return(_nbkilled);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::load_object(int o, bGenericGeoElement** f){
_bTrace_("bMacMapType::load_object",false);
int					status;
bMacMapGeoElement*	obj=new bMacMapGeoElement(this,o,&status);
	if(status<0){// Error
_te_("status = "+status);
		delete obj;
		(*f)=NULL;
		return(false);
	}
	if(status==1){// Killed object
		delete obj;
		_nbkilled++;
		(*f)=NULL;
		return(true);
	}
    if(status==2){// Out of load area
        delete obj;
        (*f)=NULL;
        _nbalive++;
        return(true);
    }
	_nbalive++;
	(*f)=obj;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::load_object(int o, bool *b){
bGenericGeoElement	*f;
	if(!load_object(o,&f)){
		return(false);
	}
	(*b)=(f!=NULL);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::new_object(bGenericGeoElement	**f){
_bTrace_("bMacMapType::new_object",false);
	(*f)=NULL;
	if(is_lock()){
		return(false);
	}
int					status;
bMacMapGeoElement*	obj=new bMacMapGeoElement(this,&status);
	if(status<0){
_te_("status = "+status);
		_nbalive++;// Décrémenté dans kill object
		kill_object(obj);
		delete obj;
		(*f)=NULL;
		return(false);
	}
	_nbalive++;
	(*f)=obj;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::clone_object(bGenericGeoElement	*fin,	bGenericGeoElement	**fout){
_bTrace_("bMacMapType::clone_object",false);
	(*fout)=NULL;
	if(is_lock()){
		return(false);
	}
int					status;
bMacMapGeoElement*	obj=new bMacMapGeoElement((bMacMapGeoElement*)fin,&status);
	if(status<0){
_te_("status = "+status);
		_nbalive++;// Décrémenté dans kill object
		kill_object(obj);
		delete obj;
		(*fout)=NULL;
		return(false);
	}
	_nbalive++;
	(*fout)=obj;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::append(){
	if(is_lock()){
		return(-1);
	}
int k=_bse.count_records();
	if(k<0){
		return(-1);
	}
int	id=0;
	if(_bse.write(k+1,kOBJ_ID_,&id)!=0){		
		return(-1);
	}
	return(k+1);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::kill_object(bGenericGeoElement *f){
_bTrace_("bMacMapType::kill_object",false);
	if((is_lock())||(f->is_lock())){
_tm_("object lock");
		return(false);
	}
	f->kill();
	if(f->killed()){
		_nbalive--;
		_nbkilled++;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapType::unkill_object(bGenericGeoElement *f){
	f->unkill();
	if(!f->killed()){
		_nbalive++;
		_nbkilled--;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapType::nb_sel(){
	return(_nsel);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::inc_sel(int n){
	_nsel+=n;
	if(_nsel<0){
		_nsel=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------		
int bMacMapType::nb_cont(){
	return(_ncnt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapType::inc_cont(int n){
	_ncnt+=n;
	if(_ncnt<0){
		_ncnt=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------	
void bMacMapType::location(char* path){
int dbt;
	(void)PATH_get(_idx,&dbt,path);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdBaseAccessor* bMacMapType::fields(){
	return(&_bse);
}

// ---------------------------------------------------------------------------
// 
// -----------		
bGenericGeoIterator* bMacMapType::iterator(){
	return(_iter);
}

// ---------------------------------------------------------------------------
// 
// -----------		
bGenericStyleMgr* bMacMapType::styles(){
	return(_styles);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapType::is_lock(){
	return((_state&_kBaseLock_)==_kBaseLock_);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapType::set_lock(bool b){
	if((b)&&(!is_lock())){
		_state|=_kBaseLock_;
	}
	else if((!b)&&(is_lock())){
		_state^=_kBaseLock_;
	}
	(void)_bse.h_write(1,kHDR_state_,&_state);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapType::is_remove(){
	return(_rmv);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapType::set_remove(bool b){
	_rmv=b;
}

// ---------------------------------------------------------------------------
//
// ------------
void bMacMapType::read_load_area(){
_bTrace_("bMacMapType::read_load_area",false);
bGenericXMLBaseElement*	root;
    if(map_doc->readTree(&root,kLDASignature,kLDAParamName)){
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
        elt=clssmgr->NthElement(root,1,"int");
        if(elt){
            elt->getvalue(val);
            _lda.left=atoi(val);
        }
        elt=clssmgr->NthElement(root,2,"int");
        if(elt){
            elt->getvalue(val);
            _lda.top=atoi(val);
        }
        elt=clssmgr->NthElement(root,3,"int");
        if(elt){
            elt->getvalue(val);
            _lda.right=atoi(val);
        }
        elt=clssmgr->NthElement(root,4,"int");
        if(elt){
            elt->getvalue(val);
            _lda.bottom=atoi(val);
        }
        if(!ivr_ok(&_lda)){
_te_("!ivr_ok(&_lda)");
            _lda.left=_lda.top=LONG_MIN;
            _lda.right=_lda.bottom=LONG_MAX;
        }
        clssmgr->ReleaseXMLInstance(root);
    }
    else{
        _lda.left=_lda.top=LONG_MIN;
        _lda.right=_lda.bottom=LONG_MAX;
    }
_tm_(_trrect_(_lda));
}

// ---------------------------------------------------------------------------
// A PLACER DANS STYLEMGR
// ------------
bool bMacMapType::make_style(){
_bTrace_("bMacMapType::make_style",false);
char		path[PATH_MAX];
CFURLRef	url;
	switch(_kind){
		case kBaseKindPoint:
			url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("default_point_style"),CFSTR("xml"),NULL);
			break;
		case kBaseKindPolyline:
			url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("default_line_style"),CFSTR("xml"),NULL);
			break;
		case kBaseKindText:
			url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("default_text_style"),CFSTR("xml"),NULL);
			break;
		case kBaseKindPolygon:
			url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("default_surf_style"),CFSTR("xml"),NULL);
			break;
		case kBaseKindRaster:
			url=CFBundleCopyResourceURL(CFBundleGetMainBundle(),CFSTR("default_raster_style"),CFSTR("xml"),NULL);
			break;
		default:
_te_("bad type kind "+_kind);
			return(false);
			break;
	}	
	if(!url){
_te_("url == NULL");
		return(false);
	}
	
CFStringRef	cfs=CFURLCopyFileSystemPath(url,kCFURLPOSIXPathStyle);
	CFRelease(url);
	CFStringGetCString(cfs,path,PATH_MAX,kCFStringEncodingMacRoman);
	CFRelease(cfs);

void*		buffer;
int			sz;
bStdFile	stl(path,"r");
	if(stl.status()){
_te_("stl.status()="+stl.status());
		return(false);
	}
	
	stl.mount((char**)&buffer,&sz);
	_bse.set_param("styles","default.xml",buffer,sz);
	free(buffer);
	
	return(true);
}
