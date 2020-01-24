//----------------------------------------------------------------------------
// File : bXMapMiTabExport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, mif-mid-tab export
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

#include "bXMapMiTabExport.h"
#include "bXMapsGISIOUtils.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/MacMapSuiteCPP.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct fieldindex{
	int				bidx;
	int				didx;
	void*			val;
}fieldindex;

typedef struct extype{
	bGenericType*	tp;
	bStdTable*		MiTab;
	bArray*			flds;
}extype;

// ---------------------------------------------------------------------------
// 
// ------------
static void convert_string(char* str){
	if(strlen(str)==0){
		return;
	}
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman);
	CFStringGetCString(cfs,str,1024,kCFStringEncodingISOLatin1);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapMiTabExport::bXMapMiTabExport(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("mitabexport");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMiTabExport::~bXMapMiTabExport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapMiTabExport::create(bGenericXMLBaseElement* elt){
	return(new bXMapMiTabExport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabExport::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
		case kExtProcessCallWithParams:
			dump((char*)prm);
			break;
		case kExtProcessCallWithXMLTree:{
char					path[PATH_MAX];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(path);
				dump(path);
			}break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMiTabExport::edit(void* prm){
char	path[SHRT_MAX];
	if(choose_location(path)!=noErr){
		return(true);
	}
	return(process(kExtProcessCallFromIntf,path));
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bXMapMiTabExport::choose_location(char* path){
_bTrace_("bXMapMiTabExport::choose_location",true);
void*   ctrlr=GISIOExport_getAccessoryViewController();
    if(NSGetDirectoryPath(NULL,
                          0,
                          cNSUTF8StringEncoding,
                          true,
                          GISIOExport_getAccessoryViewFromController(ctrlr),
                          path)){
        GISIOExport_releaseController(ctrlr);
        if(path[strlen(path)-1]!='/'){
            strcat(path,"/");
        }
        return noErr;
    }
    GISIOExport_releaseController(ctrlr);
    return -1;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapMiTabExport::dump(char* path){
_bTrace_("bXMapMiTabExport::dump",true);
extype		et;
bArray		arr(sizeof(extype),_gapp->typesMgr()->count());
bArray		sel(*(_gapp->selMgr()->elements()));
int			status;
char		tname[FILENAME_MAX];
char		name[FILENAME_MAX];
CFStringRef	cfs;
//double		reso=_gapp->document()->dist_resolution();
double		reso=GetRawPrecisionCoef(_gapp);
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=_gapp->document()->srid();

bGenericExt*	fexp=_gapp->xmapMgr()->find('FExp');
	if(fexp){
fexp_prm		prm={0,NULL,NULL,tsrid};
		fexp->process(kExtProcessCallGetData,&prm);
		tsrid=prm.srid;
	}
	
	for(int i=1;i<=arr.count();i++){
		et.tp=_gapp->typesMgr()->get(i);
		if(TypeIsExportProtected(et.tp)){
_tw_("Can't process export protected type");
			continue;
		}
		if	(	(et.tp->nb_sel()>0)		||(
				(sel.count()==0)		&&
				(et.tp->nb_live()>0)	)){
			status=et.tp->kind();
			et.tp->name(tname);
			cfs=CFStringCreateWithCString(kCFAllocatorDefault,tname,kCFStringEncodingMacRoman);
			CFStringGetCString(cfs,tname,FILENAME_MAX,kCFStringEncodingUTF8);
			CFRelease(cfs);

			sprintf(name,"%s.mif",tname);
			et.MiTab=wtbl_falloc(	kTableMITAB,
									path,
									name,
									true,
									&reso,
									&origin.x,
									&origin.y,
									&tsrid,
									&asrid,
									&status);
			status=0;
			if(et.MiTab==NULL){
				et.tp=NULL;
			}
			else{
				et.flds=new bArray(sizeof(fieldindex));
				field_create(et.tp,et.MiTab,et.flds);
			}
		}
		else{
			et.tp=NULL;
			et.MiTab=NULL;
			et.flds=NULL;
		}
		arr.put(i,&et);
	}
	
	if(sel.count()>0){
		objs_dump(arr,sel);
	}
	else{
		objs_dump(arr);
	}
	
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&et);
		if(et.MiTab){
			wtbl_free(et.MiTab);
		}
		if(et.flds){
fieldindex	fx;
			for(int j=1;j<=et.flds->count();j++){
				et.flds->get(j,&fx);
				if(fx.val){
					free(fx.val);
				}
			}
			delete et.flds;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapMiTabExport::objs_dump(bArray& arr){
_bTrace_("bXMapMiTabExport::objs_dump",false);
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapMiTabExportProcessingID,msg,getbundle(),0);
bProgressWait	wt("",msg,true,arr.count());
extype			et;

	for(int i=1;i<=arr.count();i++){
		if(!wt.set_progress(i)){
_tm_("user break");
			break;
		}
		arr.get(i,&et);
		if(!et.tp){
			continue;
		}
		et.tp->iterator()->iterate(&et,obj_dump);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapMiTabExport::objs_dump(bArray& arr, bArray& sel){
_bTrace_("bXMapMiTabExport::objs_dump",false);
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapMiTabExportProcessingID,msg,getbundle(),0);
bProgressWait		wt("",msg,true,sel.count());
extype				et;
bGenericGeoElement*	o;
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
_tm_("user break");
			break;
		}
		sel.get(i,&o);
		arr.get(o->getType(),&et);
		if(et.tp){
			if(obj_dump(o,&et)){
_te_("obj_dump error");
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
#define __str_conversion__ 0
int bXMapMiTabExport::obj_dump(void *o, void *prm){
_bTrace_("bXMapMiTabExport::obj_dump",false);
bGenericGeoElement*	go=(bGenericGeoElement*)o;
extype*				et=(extype*)prm;
ivertices*			vxs;
int					k=et->MiTab->CountRecords()+1;
fieldindex			fx;
#if __str_conversion__==1	
int					kd;
#endif
	go->getVertices(&vxs);
	if(et->MiTab->WriteVal(k,et->MiTab->CountFields()-1,&vxs)){
	}
	for(int i=1;i<=et->flds->count();i++){
		et->flds->get(i,&fx);
		if(!go->getValue(fx.bidx,fx.val)){
_te_("go->getValue "+fx.bidx);
		}

#if __str_conversion__==1	
		et->MiTab->FieldSign(fx.didx,&kd);
		if(kd==_char){
			convert_string((char*)fx.val);
		}
#endif
		if(et->MiTab->WriteVal(k,fx.didx,fx.val)){
			if(fx.didx==1){
//_tm_("len=%d;%s",strlen((char*)fx.val),(char*)fx.val);
			}
//_te_("dbf->WriteVal %d;%d",fx.didx,fx.bidx);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapMiTabExport::field_create(bGenericType* tp, bStdTable* dbf, bArray* f){
_bTrace_("bXMapMiTabExport::field_create",false);
bStdBaseAccessor*	bse=tp->fields();
int					k,l,d,sz;
char				name[256];
fieldindex			fx;
int					idx=0,status=0;
bGenericExt*		fexp=_gapp->xmapMgr()->find('FExp');

	if(fexp){
bArray				arr(sizeof(fexp_field));
fexp_prm			prm;
int					/*fid,*/fidx;
fexp_field			fld;

		prm.tidx=tp->index();
		prm.name=NULL;
		prm.flds=&arr;
		fexp->process(kExtProcessCallGetData,&prm);
		for(int i=1;i<=arr.count();i++){
			arr.get(i,&fld);
_tm_("field id="+fld.fid);
			fidx=bse->get_index(fld.fid);
			if(!fidx){
_te_("field not found");
				continue;
			}
			if(strlen(fld.fname)>0){
				strcpy(name,fld.fname);
			}
			else{
				bse->get_name(fidx,name);
			}
			bse->get_kind(fidx,&k);
			bse->get_len(fidx,&l);
			bse->get_decs(fidx,&d);
			bse->get_size(fidx,&sz);
			if((status=dbf->AddField(name,k,l,d))){
_te_("dbf->add failed for "+name+"->"+status);
				continue;
			}
			else{
				idx++;
			}
			fx.bidx=fidx;
			fx.didx=idx;
			fx.val=malloc(sz);
			
_tm_(name+"->"+sz+":"+fx.bidx+":"+fx.didx);

			if(!f->add(&fx)){
_te_("f.add failed for "+i);
			}
		}		
	}
	else{
		for(int i=kOBJ_Name_;i<=bse->count();i++){
			bse->get_name(i,name);
			bse->get_kind(i,&k);
			bse->get_len(i,&l);
			bse->get_decs(i,&d);
			bse->get_size(i,&sz);
			if(dbf->AddField(name,k,l,d)){
_te_("dbf->add failed for "+name+"->"+status);
				continue;
			}
			else{
				idx++;
			}
			fx.bidx=i;
			fx.didx=idx;
			fx.val=malloc(sz);
			
_tm_(name+"->"+sz+":"+fx.bidx+":"+fx.didx);

			if(!f->add(&fx)){
_te_("f.add failed for "+i);
			}
		}
	}

}
