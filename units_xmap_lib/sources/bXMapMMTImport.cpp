//----------------------------------------------------------------------------
// File : bXMapMMTImport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, MacMap Text format import
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
// 23/06/2008 creation.
//----------------------------------------------------------------------------

#include "bXMapMMTImport.h"
#include "bXMapsGISIOUtils.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/MacMapSuiteCPP.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct fieldindex{
	int				tidx;
	int				bidx;
	void*			val;
}fieldindex;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapMMTImport::bXMapMMTImport(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("mmtimport");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMMTImport::~bXMapMMTImport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapMMTImport::create(bGenericXMLBaseElement* elt){
	return(new bXMapMMTImport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMMTImport::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			import();
			break;
		case kExtProcessCallWithParams:
			import((import_prm*)prm);
			break;
		case kExtProcessCallWithXMLTree:
			import(true);
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMMTImport::edit(void* prm){	
_bTrace_("bXMapMMTImport::edit",true);
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
char					exp[__MESSAGE_STRING_LENGTH_MAX__];
bGenericExt*			ext=_gapp->xmapMgr()->find('IGrd');
	if(!ext){
		b_message_string(kXMapMMTImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportIGridNotFoundExplID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
		return(true);
	}
	
const char*		nsft[1]={"MMT"};
const char**	pnsft=nsft;
bArray          paths(sizeof(char[PATH_MAX]));
void*           ctrlr=GISIOImport_getAccessoryViewController(false);

    if(NSGetFilePaths(pnsft,
                      1,
                      cNSUTF8StringEncoding,
                      false,
                      true,
                      GISIOImport_getAccessoryViewFromController(ctrlr),
                      paths)==cNSCancelButton){
        GISIOImport_releaseController(ctrlr);
_te_("user canceled");
        return(true);
    }
bool    ctin=GISIOImport_getCreateIfNeededFromController(ctrlr);
    GISIOImport_releaseController(ctrlr);
_tm_("createIfNeeded: "+ctin);
   
	ext->process(kExtProcessCallGetData,&_prm.grid);
	_prm.ctin=ctin;
    _prm.typErr=0;
    _prm.objErr=0;
    _prm.filErr=0;
	
bEventLog	log(_gapp,this);
char    path[PATH_MAX];
    for(long i=1;i<=paths.count();i++){
        paths.get(i,&path);
        splitPath(path,_prm.path,_prm.name);
        _prm.name[strlen(_prm.name)-4]=0;
_tm_(_prm.path);
_tm_(_prm.name);
        process(kExtProcessCallFromIntf,NULL);
    }
	log.close();

    if(_prm.typErr+_prm.objErr+_prm.filErr>0){
        b_message_string(kXMapMMTImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapMMTImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,_prm.typErr,_prm.objErr,_prm.filErr);
bAlertStop	alrt(msg,exp);
    }
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMMTImport::import(){	
	import(&_prm,false);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMMTImport::import(bool dummy){	
import_prm		prm;
bGenericExt*	ext=_gapp->xmapMgr()->find('IGrd');
char            msg[__MESSAGE_STRING_LENGTH_MAX__];
char            exp[__MESSAGE_STRING_LENGTH_MAX__];
	if(!ext){
		b_message_string(kXMapMMTImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportIGridNotFoundExplID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
		return(false);
	}
	ext->process(kExtProcessCallGetData,&prm.grid);
    prm.typErr=0;
    prm.objErr=0;
    prm.filErr=0;
	
bGenericXMLBaseElement* elt;
char					val[_values_length_max_];

	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(prm.path);
	elt=getelement(2);
	if(!elt){
		return(false);
	}
	elt->getvalue(prm.name);
	elt=getelement(3);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	prm.ctin=atoi(val);

	import(&prm,true);

    if(prm.typErr+prm.objErr+prm.filErr>0){
        b_message_string(kXMapMMTImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapMMTImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,prm.typErr,prm.objErr,prm.filErr);
bAlertStop	alrt(msg,exp,true);
    }
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMMTImport::import(import_prm* prm, bool silent){
_bTrace_("bXMapMMTImport::import",true);
int			status=LONG_MAX;
char		name[FILENAME_MAX];
char		msg[__MESSAGE_STRING_LENGTH_MAX__];
char		exp[__MESSAGE_STRING_LENGTH_MAX__];
int			tbl_kind;
double		reso=GetRawPrecisionCoef(_gapp);
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=prm->grid.srid;

	sprintf(name,"%s.mmt",prm->name);
bStdTable*		mmt=wtbl_falloc(kTableMMText,
								prm->path,
								name,
								false,
								&reso,
								&origin.x,
								&origin.y,
								&tsrid,
								&asrid,
								&status);			
	if(status<0){
		wtbl_free(mmt);
		b_message_string(kXMapMMTImporOpenErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("mmt table err "+status);
        prm->typErr++;
		return(false);
	}
	tbl_kind=status;

bGenericType*   tp=GISIOImport_getImportType(_gapp,tbl_kind,_prm);

/*	switch(tbl_kind){
		case kBaseKindPoint:
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_point));
			break;
		case kBaseKindPolyline:
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_line));
			break;
		case kBaseKindText:
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_text));
			break;
		case kBaseKindPolygon:
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_poly));
			break;
		case kBaseKindRaster:
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_raster));
			break;
	}

//new
	if((tp==NULL)&&(prm->ctin)){
		tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(prm->name));
		if(tp==NULL){
			strcpy(name,prm->name);
			UTF82MacRoman(name,FILENAME_MAX);			
			if(_gapp->typesMgr()->add(name,"",NULL,kBaseLocalDBID,tbl_kind)!=noErr){
_te_("_gapp->typesMgr()->add failed");
			}
			else{
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->count());
				_gapp->layersAccessCtx()->add(_gapp->typesMgr()->count(),1);
			}
		}
	}
//new*/
	
	if(tp==NULL){
		wtbl_free(mmt);
		b_message_string(kXMapMMTImportInvalidGridErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("no type for kind "+status);
        prm->typErr++;
		return(false);
	}
	
ivx_rect			bnds={-(__BOUNDS_MAX__-1),-(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1)};
bArray				a(sizeof(fieldindex));

	tp->iterator()->set_bounds(&bnds);

fieldindex			fx;
bGenericGeoElement*	o=NULL;
ivertices*			vxs;
int					kind,fvx=0;

	for(int i=1;i<=mmt->CountFields();i++){
		mmt->FieldSign(i,&kind);
		if(kind==_ivxs2){
			fvx=i;
			break;
		}
	}
	if(fvx==0){
		wtbl_free(mmt);
		b_message_string(kXMapMMTImportGeomNotFoundErrorID,msg,getbundle(),1);
bAlertStop	alrt(msg,"",silent);	
_te_("no geometry field ");
		return(false);
	}
	
	field_create(tp,mmt,a,silent);

int					err=0;
	b_message_string(kXMapMMTImportObjectCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,true,true,mmt->CountRecords());

// Attention, ne pas libŽrer les vxs car ils sortent direct de la table !!!
	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=mmt->CountRecords();i++){
		if(!wt.set_progress(i)){
			break;
		}
		mmt->ReadVal(i,fvx,&vxs);
		if(mmt->ReadVal(i,fvx,&vxs)){
_te_("read vertices err");
			err++;
            prm->objErr++;
			continue;
		}
		if(	(tp->kind()==kBaseKindPolygon)	||
			(tp->kind()==kBaseKindRaster)	){
			if(!ivs_closed(vxs)){
				vxs=ivx2_add(vxs,&vxs->vx.vx2[0]);
			}
		}

		if(!tp->new_object(&o)){
_te_("new_object failure for "+i);
			err++;
            prm->objErr++;
			break;
		}
		if(!o->setVertices(vxs)){
_te_("setVertices failure for at "+i);
			err++;
		}
		for(int j=1;j<=a.count();j++){
			a.get(j,&fx);
			if(!fx.bidx){
				err++;
                prm->filErr++;
				continue;
			}
			if(mmt->ReadVal(i,fx.tidx,fx.val)){
_te_("setVertices failure for "+i+";"+j);
				err++;
                prm->filErr++;
				continue;
			}
			if(!o->setValue(fx.bidx,fx.val)){
				err++;
                prm->filErr++;
_te_("setValue failure for "+i+";"+j);
			}
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(true);

	for(int i=1;i<=a.count();i++){
		a.get(i,&fx);
		if(fx.val){
			free(fx.val);
		}
	}
	a.reset();
	
	wtbl_free(mmt);
	if(err){
		b_message_string(kXMapMMTImportObjectCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapMMTImport::field_create(bGenericType* tp, bStdTable* mmt, bArray& f, bool silent){
_bTrace_("bXMapMMTImport::field_create",false);
bStdBaseAccessor*	bse;
int					x,k,fk,l,d;
char				name[256],to_name[256];
fieldindex			fx;
int					err=0;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				exp[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapMMTImportFieldCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,true,false,mmt->CountFields());

	bse=tp->fields();
	for(int i=1;i<=mmt->CountFields();i++){
		mmt->FieldName(i,name);
		mmt->FieldSign(i,&k);		
		mmt->FieldLength(i,&l);
		mmt->FieldDecimals(i,&d);
		
		if(	(k!=_char)	&&
			(k!=_bool)	&&
			(k!=_int)	&&
			(k!=_double)&&
			(k!=_date)	&&
			(k!=_time)	){
			continue;
		}
		
		x=bse->get_index(name);
		if(!x){
			if(bse->add(k,l,d,name)){
_te_("bse->add failed for "+name+" with "+x);
			}
			else{
				x=bse->get_index(name);
			}
		}
		else{
			bse->get_kind(x,&fk);
			if(fk!=k){
				sprintf(to_name,"%s:%.4s",name,&k);
				x=bse->get_index(to_name);
				if(!x){
					if(bse->add(k,l,d,to_name)){
_te_("bse->add failed for "+to_name+" with "+x);
					}
					else{
						x=bse->get_index(to_name);
					}
				}
			}
		}
		if(x==0){
			err++;
		}
		fx.bidx=x;
		fx.tidx=i;
		if(bse->get_size(x,&l)){
			l+=4;
			fx.val=malloc(l);
		}
		else{
_te_("bse->get_size failed for "+to_name+" with "+x);
			fx.val=NULL;
		}
		if(!f.add(&fx)){
_te_("f.add failed for "+x);
			err++;
		}
		wt.set_progress(i);
	}
	if(err){
		b_message_string(kXMapMMTImportFieldCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapMMTImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
}
