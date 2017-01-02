//----------------------------------------------------------------------------
// File : bXMapSHPImport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, Shapefile import
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
// 22/03/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapSHPImport.h"
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
	int				idx;
	void*			val;
}fieldindex;


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSHPImport::bXMapSHPImport(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("shpimport");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSHPImport::~bXMapSHPImport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSHPImport::create(bGenericXMLBaseElement* elt){
	return(new bXMapSHPImport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSHPImport::process(int msg, void* prm){
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
bool bXMapSHPImport::edit(void* prm){	
_bTrace_("bXMapSHPImport::edit",true);
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
char					exp[__MESSAGE_STRING_LENGTH_MAX__];
bGenericExt*			ext=_gapp->xmapMgr()->find('IGrd');
	if(!ext){
		b_message_string(kXMapSHPImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportIGridNotFoundExplID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
		return(true);
	}
    
const char*		nsft[1]={"SHP"};
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
b_message_string(kXMapSHPImportGlobalErrorID,msg,getbundle(),1);
b_message_string(kXMapSHPImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,_prm.typErr,_prm.objErr,_prm.filErr);
bAlertStop	alrt(msg,exp);
    }
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSHPImport::import(){	
	import(&_prm,false);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSHPImport::import(bool dummy){	
import_prm		prm;
bGenericExt*	ext=_gapp->xmapMgr()->find('IGrd');
char            msg[__MESSAGE_STRING_LENGTH_MAX__];
char            exp[__MESSAGE_STRING_LENGTH_MAX__];
    if(!ext){
		b_message_string(kXMapSHPImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportIGridNotFoundExplID,exp,getbundle(),1);
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
        b_message_string(kXMapSHPImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapSHPImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,prm.typErr,prm.objErr,prm.filErr);
bAlertStop	alrt(msg,exp,true);
    }

    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSHPImport::import(import_prm* prm, bool silent){
_bTrace_("bXMapSHPImport::import",true);
int			status=0;
char		name[FILENAME_MAX];
char		msg[__MESSAGE_STRING_LENGTH_MAX__];
char		exp[__MESSAGE_STRING_LENGTH_MAX__];
double		reso=GetRawPrecisionCoef(_gapp);
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=prm->grid.srid;

	sprintf(name,"%s.shp",prm->name);
bStdTable*		shp=wtbl_falloc(kTableShape,
								prm->path,
								name,
								false,
								&reso,
								&origin.x,
								&origin.y,
								&tsrid,
								&asrid,
								&status);
	if(status<=0){
		wtbl_free(shp);
		b_message_string(kXMapSHPImporOpenSHPErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("shape table err "+status);
        prm->typErr++;
		return(false);
	}
    
bGenericType*	tp=GISIOImport_getImportType(_gapp,status,_prm);

/*    switch(status){
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
		default:
			tp=NULL;
			break;
	}

//new
	if((tp==NULL)&&(prm->ctin)){
		tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(prm->name));
		if(tp==NULL){
			strcpy(name,prm->name);
			UTF82MacRoman(name,FILENAME_MAX);			
			if(_gapp->typesMgr()->add(name,"",NULL,kBaseLocalDBID,status)!=noErr){
_te_("_gapp->typesMgr()->add failed");
			}
			else{
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->count());
				_gapp->layersAccessCtx()->add(_gapp->typesMgr()->count(),1);
			}
		}
	}
//new
	*/
	
	if(tp==NULL){
		wtbl_free(shp);
		b_message_string(kXMapSHPImportInvalidGridErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("no type for kind "+status);
        prm->typErr++;
		return(false);
	}
	
	status=0;
	sprintf(name,"%s.dbf",prm->name);
bStdTable*		dbf=wtbl_falloc(kTableDBF,
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
		wtbl_free(shp);
		wtbl_free(dbf);
		b_message_string(kXMapSHPImporOpenDBFErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("dbf table err "+status);
        prm->typErr++;
		return(false);
	}

ivx_rect			bnds={-(__BOUNDS_MAX__-1),-(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1)};
	
	tp->iterator()->set_bounds(&bnds);
	
bArray				a(sizeof(fieldindex));
fieldindex			fx;
bGenericGeoElement*	o=NULL;
ivertices*			vxs;

	field_create(tp,dbf,a,silent);

int					err=0;
	b_message_string(kXMapSHPImportObjectCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,true,true,shp->CountRecords());

	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=shp->CountRecords();i++){
		if(!wt.set_progress(i)){
			break;
		}
		if(!tp->new_object(&o)){
_te_("new_object failure for at "+i);
			err++;
            prm->objErr++;
			break;
		}
		if(shp->ReadVal(i,1,&vxs)){
_te_("shp->ReadVal failure for "+i);
			err++;
            prm->filErr++;
			continue;
		}
		if(!o->setVertices(vxs)){
_te_("setVertices failure for "+i);
			ivs_free(vxs);
			err++;
            prm->filErr++;
			continue;
		}
		ivs_free(vxs);
		for(int j=1;j<=dbf->CountFields();j++){
			a.get(j,&fx);
			if(!fx.idx){
				err++;
                prm->filErr++;
				continue;
			}
			if(dbf->ReadVal(i,j,fx.val)){
_te_("ReadVal failure at ("+i+";"+j+")");
				err++;
                prm->filErr++;
				continue;
			}
			if(!o->setValue(fx.idx,fx.val)){
				err++;
                prm->filErr++;
_te_("setValue failure at ("+i+";"+j+")"+"->("+fx.idx+")");
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
	
	wtbl_free(shp);
	wtbl_free(dbf);
	
	if(err){
		b_message_string(kXMapSHPImportObjectCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSHPImport::field_create(bGenericType* tp, bStdTable* dbf, bArray& f, bool silent){
_bTrace_("bXMapSHPImport::field_create",false);
bStdBaseAccessor*	bse=tp->fields();
int					x,k,fk,l,d;
char				name[256],to_name[256];
fieldindex			fx;
int					err=0;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				exp[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapSHPImportFieldCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,true,false,dbf->CountFields());

	for(int i=1;i<=dbf->CountFields();i++){
		dbf->FieldName(i,name);
		dbf->FieldSign(i,&k);		
		dbf->FieldLength(i,&l);
		dbf->FieldDecimals(i,&d);


		x=bse->get_index(name);
//_tm_(name+" index="+x);
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
//_tm_(name+" new index="+x);

		if(x==0){
			err++;
		}
		fx.idx=x;
		if(bse->get_size(x,&l)){
			fx.val=malloc(l);
		}
		else{
			fx.val=NULL;
		}
		if(!f.add(&fx)){
_te_("f.add failed for "+x);
			err++;
		}
		wt.set_progress(i);
	}
	if(err){
		b_message_string(kXMapSHPImportFieldCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapSHPImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
}
