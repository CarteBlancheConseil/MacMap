//----------------------------------------------------------------------------
// File : bXMapMiTabImport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, mif-mid-tab import
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

#include "bXMapMiTabImport.h"
#include "bXMapsGISIOUtils.h"
#include <mox_intf/bEventLog.h>
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
bXMapMiTabImport::bXMapMiTabImport(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("mitabimport");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMiTabImport::~bXMapMiTabImport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapMiTabImport::create(bGenericXMLBaseElement* elt){
	return(new bXMapMiTabImport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabImport::process(int msg, void* prm){
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
bool bXMapMiTabImport::edit(void* prm){	
_bTrace_("bXMapMiTabImport::edit",true);
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
char					exp[__MESSAGE_STRING_LENGTH_MAX__];
bGenericExt*			ext=_gapp->xmapMgr()->find('IGrd');
	if(!ext){
		b_message_string(kXMapMiTabImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportIGridNotFoundExplID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
		return(true);
	}

const char*		nsft[2]={"MIF","TAB"};
const char**	pnsft=nsft;
bArray          paths(sizeof(char[PATH_MAX]));
void*           ctrlr=GISIOImport_getAccessoryViewController(false);
    
    if(NSGetFilePaths(pnsft,
                      2,
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
_tm_("createIfNeeded: "+ctin);
    GISIOImport_releaseController(ctrlr);

    ext->process(kExtProcessCallGetData,&_prm.grid);
    _prm.ctin=ctin;
    _prm.typErr=0;
    _prm.objErr=0;
    _prm.filErr=0;

bEventLog	log(_gapp,this);
char        path[PATH_MAX];
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
        b_message_string(kXMapMiTabImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapMiTabImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,_prm.typErr,_prm.objErr,_prm.filErr);
bAlertStop	alrt(msg,exp);
    }
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabImport::import(){
	import(&_prm,false);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabImport::import(bool dummy){	
import_prm		prm;
bGenericExt*	ext=_gapp->xmapMgr()->find('IGrd');
char            msg[__MESSAGE_STRING_LENGTH_MAX__];
char            exp[__MESSAGE_STRING_LENGTH_MAX__];
	if(!ext){
		b_message_string(kXMapMiTabImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportIGridNotFoundExplID,exp,getbundle(),1);
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
        b_message_string(kXMapMiTabImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapMiTabImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,prm.typErr,prm.objErr,prm.filErr);
bAlertStop	alrt(msg,exp,true);
    }

	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabImport::import(import_prm* prm, bool silent){
_bTrace_("bXMapMiTabImport::import",true);
int			status=0;
char		name[FILENAME_MAX];
char		msg[__MESSAGE_STRING_LENGTH_MAX__];
char		exp[__MESSAGE_STRING_LENGTH_MAX__];
//double		reso=_gapp->document()->dist_resolution();
double		reso=GetRawPrecisionCoef(_gapp);
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=prm->grid.srid;

	sprintf(name,"%s.mif",prm->name);
bStdTable*		MiTab=wtbl_falloc(	kTableMITAB,
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
		wtbl_free(MiTab);
		sprintf(name,"%s.tab",prm->name);
		status=0;
		MiTab=wtbl_falloc(	kTableMITAB,
							prm->path,
							name,
							false,
							&reso,
							&origin.x,
							&origin.y,
							&tsrid,
							&asrid,
							&status);	
	}
	
	if(status<0){
		wtbl_free(MiTab);
		b_message_string(kXMapMiTabImporOpenMiTabErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("mitab table err "+status);
        prm->typErr++;
		return(false);
	}
	
bGenericType*	tp[5];
char            tname[FILENAME_MAX];
    strcpy(tname,_prm.name);
    
    sprintf(_prm.name,"%s_pt",tname);
    tp[kBaseKindPoint-1]=GISIOImport_getImportType(_gapp,kBaseKindPoint,_prm);
    sprintf(_prm.name,"%s_pl",tname);
    tp[kBaseKindPolyline-1]=GISIOImport_getImportType(_gapp,kBaseKindPolyline,_prm);
    sprintf(_prm.name,"%s_tx",tname);
    tp[kBaseKindText-1]=GISIOImport_getImportType(_gapp,kBaseKindText,_prm);
    sprintf(_prm.name,"%s_pg",tname);
    tp[kBaseKindPolygon-1]=GISIOImport_getImportType(_gapp,kBaseKindPolygon,_prm);

    strcpy(_prm.name,tname);
	
	if(	(tp[0]==NULL)	&&
		(tp[1]==NULL)	&&
		(tp[2]==NULL)	&&
		(tp[3]==NULL)	){
		wtbl_free(MiTab);
		b_message_string(kXMapMiTabImportInvalidGridErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
_te_("no type for kind "+status);
        prm->typErr++;
		return(false);
	}
	
ivx_rect			bnds={-(__BOUNDS_MAX__-1),-(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1),(__BOUNDS_MAX__-1)};
bArray*				a[4];//(sizeof(fieldindex));
	for(int i=0;i<4;i++){
		if(tp[i]){
			tp[i]->iterator()->set_bounds(&bnds);
			a[i]=new bArray(sizeof(fieldindex));
		}
	}

fieldindex			fx;
bGenericGeoElement*	o=NULL;
ivertices*			vxs;
int					kind,fvx=MiTab->CountFields()-1;

	field_create(tp,MiTab,a,silent);

int					err=0;
	b_message_string(kXMapMiTabImportObjectCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,true,MiTab->CountRecords());

	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=MiTab->CountRecords();i++){
		if(!wt.set_progress(i)){
			break;
		}
		kind=MiTab->ReadVal(i,fvx,&vxs);
		if(kind<0){
_te_("read vertices err");
			err++;
            prm->objErr++;
			continue;
		}
		if(tp[kind-1]==NULL){
_te_("no type for kind "+kind);
			ivs_free(vxs);
			err++;
            prm->objErr++;
			continue;
		}
		if(!tp[kind-1]->new_object(&o)){
_te_("new_object failure for "+i);
			ivs_free(vxs);
			err++;
            prm->objErr++;
			break;
		}
		if(!o->setVertices(vxs)){
_te_("setVertices failure for "+i);
			err++;
            prm->filErr++;
			continue;
		}
		ivs_free(vxs);
		for(int j=1;j<=MiTab->CountFields()-2;j++){
			a[kind-1]->get(j,&fx);
			if(!fx.idx){
				err++;
                prm->filErr++;
				continue;
			}
			if(MiTab->ReadVal(i,j,fx.val)){
_te_("setVertices failure for "+i+";"+j);
				err++;
                prm->filErr++;
				continue;
			}
			if(!o->setValue(fx.idx,fx.val)){
				err++;
                prm->filErr++;
_te_("setValue failure for "+i+";"+j);
			}
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(true);

	for(int j=0;j<4;j++){
		if(tp[j]==NULL){
			continue;
		}
		for(int i=1;i<=a[j]->count();i++){
			a[j]->get(i,&fx);
			if(fx.val){
				free(fx.val);
			}
		}
		delete(a[j]);
	}
	
	wtbl_free(MiTab);
	
	if(err){
		b_message_string(kXMapMiTabImportObjectCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapMiTabImport::field_create(bGenericType** tp, bStdTable* MiTab, bArray** f, bool silent){
_bTrace_("bXMapMiTabImport::field_create",false);
bStdBaseAccessor*	bse;
int					x,k,fk,l,d;
char				name[256],to_name[256];
fieldindex			fx;
int					err=0;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				exp[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapMiTabImportFieldCreateMessageID,msg,getbundle(),1);
bProgressWait wt("",msg,false,MiTab->CountFields()*5);

	for(int j=0;j<4;j++){
		if(tp[j]==NULL){
			continue;
		}
		bse=tp[j]->fields();
		for(int i=1;i<=MiTab->CountFields()-2;i++){
			MiTab->FieldName(i,name);
			MiTab->FieldSign(i,&k);		
			MiTab->FieldLength(i,&l);
			MiTab->FieldDecimals(i,&d);
			
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
			fx.idx=x;
			if(bse->get_size(x,&l)){
				fx.val=malloc(l);
			}
			else{
				fx.val=NULL;
			}
			if(!f[j]->add(&fx)){
_te_("f.add failed for "+x);
				err++;
			}
			wt.set_progress(j*(MiTab->CountFields()-2)+i);
		}
	}
	if(err){
		b_message_string(kXMapMiTabImportFieldCreateErrorID,msg,getbundle(),1);
		b_message_string(kXMapMiTabImportExplCreateErrorID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp,true);
	}
}
