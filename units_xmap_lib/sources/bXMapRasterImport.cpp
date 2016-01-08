//----------------------------------------------------------------------------
// File : bXMapRasterImport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, raster imports
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
// 23/11/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapRasterImport.h"
#include "bXMapsGISIOUtils.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/CGUtils.h>
#include <MacMapSuite/MacMapSuiteCPP.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bStdFile.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapRasterImport	::bXMapRasterImport(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("rasterimport");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapRasterImport::~bXMapRasterImport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapRasterImport::create(bGenericXMLBaseElement* elt){
	return(new bXMapRasterImport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			import();
			break;
		case kExtProcessCallWithParams:{
bStdDirectory	dr(((import_prm*)prm)->path);
			if(dr.status()){
				return(false);
			}
			import((import_prm*)prm);
			}
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::edit(void* prm){	
_bTrace_("bXMapRasterImport::edit",true);
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
char					exp[__MESSAGE_STRING_LENGTH_MAX__];
char					name[FILENAME_MAX];
char					fname[FILENAME_MAX];
bGenericExt*			ext=_gapp->xmapMgr()->find('IGrd');

	if(!ext){
		b_message_string(kXMapRasterImportIGridNotFoundErrorID,msg,getbundle(),1);
		b_message_string(kXMapRasterImportIGridNotFoundExplID,exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
		return(true);
	}

const char*		nsft[5]={"PNG","TIF","TIFF","JPG","JPEG"};
const char**	pnsft=nsft;
bArray          paths(sizeof(char[PATH_MAX]));
void*           ctrlr=GISIOImport_getAccessoryViewController(true);
    
    if(NSGetFilePaths(pnsft,
                      5,
                      cNSMacOSRomanStringEncoding,
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

	_gapp->layersMgr()->SetObjInvalidation(false);
b_message_string(kXMapRasterImportMessageID,msg,getbundle(),1);
bProgressWait	wt("",msg,true,false,paths.count());
bEventLog       log(_gapp,this);
char            path[PATH_MAX];
    for(long i=1;i<=paths.count();i++){
        if(!wt.set_progress(i)){
            break;
        }
        
        paths.get(i,&path);
        splitPath(path,_prm.path,_prm.name);
        
		sprintf(name,"%s",_prm.name);
		for(int j=strlen(name)-1;j>=0;j--){
			if(name[j]=='.'){
				name[j]=0;
			}
		}
		
bStdDirectory	dr(_prm.path);
		if(dr.status()){
_te_("dr.status()=="+dr.status());
            _prm.objErr++;
			break;
		}
		
{
			sprintf(fname,"%s.TFW",name);
bStdFile	f(fname,"r");
			if((f.status()==noErr)&&(parse_tfw(f))){
				import();
				continue;
			}
}	
{
			sprintf(fname,"%s.TAB",name);
bStdFile	f(fname,"r");
			if((f.status()==noErr)&&(parse_tab(f))){
				import();
				continue;
			}
}
		
		bStdXMapWithModalIntf::edit(prm);
	}
	log.close();
    if(_prm.typErr+_prm.objErr+_prm.filErr>0){
        b_message_string(kXMapRasterImportGlobalErrorID,msg,getbundle(),1);
        b_message_string(kXMapRasterImportGlobalExplID,exp,getbundle(),1);
        sprintf(exp,exp,_prm.typErr,_prm.objErr,_prm.filErr);
bAlertStop	alrt(msg,exp);
    }
    _gapp->layersMgr()->SetObjInvalidation(true);
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::import(){	
	import(&_prm);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::import(import_prm* prm){
_bTrace_("bXMapRasterImport::import",false);
bGenericType*	tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(_prm.grid.t_raster));
	if(tp==NULL){
_te_("tp==NULL");
        prm->typErr++;
		return(false);
	}

double	hres,vres;
int		hsz,vsz;
void*	data;
int		sz;
	
{
bStdFile	img(prm->name,"rb");
	if(img.status()){
_te_("img.status()=="+img.status()+" at open");
        _prm.objErr++;
		return(false);
	}
	img.mount((char**)&data,&sz);
	if(img.status()){
_te_("img.status()=="+img.status()+" at mount");
        _prm.objErr++;
		return(false);
	}
}
		
	if(GetImageInfoFromData(data,sz,GetImageKind(prm->name),&hsz,&vsz,&hres,&vres)){
_te_("GetImageInfoFromData failed");
		free(data);
        _prm.objErr++;
		return(false);
	}

	if(!tp->fields()->set_param("rasters",prm->name,data,sz)){
_te_("tp->fields()->set_param failed");
		free(data);
        _prm.objErr++;
		return(false);
	}
	
	free(data);

ivx_rect	vr;
i2dvertex	vx;
double		w=hsz,h=vsz;

	Convert(&vx,&_tl);
	vr.top=vx.v;
	vr.left=vx.h;

bGenericUnit*	u=_gapp->distMgr()->get();
	switch(_ik){
		case kRasterImportKindScale:
			w*=(72.0/hres);
			h*=(72.0/vres);
			w*=((0.0254*u->coef())/72.0);
			h*=((0.0254*u->coef())/72.0);
			w*=_scl;
			h*=_scl;
			_br=_tl;
			_br.x+=w;
			_br.y-=h;
			Convert(&vx,&_br);
			vr.bottom=vx.v;
			vr.right=vx.h;
			break;
		case kRasterImportKindTFW:
		case kRasterImportKindTAB:
			_br.x=_tl.x+(w*_xsz);
			_br.y=_tl.y-(h*_ysz);
			Convert(&vx,&_br);
			vr.bottom=vx.v;
			vr.right=vx.h;
			break;
		default:
			Convert(&vx,&_br);
			vr.bottom=vx.v;
			vr.right=vx.h;
			break;
	}
	
ivertices*	vxs;
	if(ivr2ivs(&vr,&vxs)){
_te_("ivr2ivs failed");
        prm->objErr++;
		return(false);
	}

bGenericGeoElement*	o=NULL;
	if(!tp->new_object(&o)){
_te_("new_object failed");
		ivs_free(vxs);
        prm->objErr++;
		return(false);
	}
	
	if(!o->setVertices(vxs)){
_te_("setVertices failed");
		ivs_free(vxs);
		tp->kill_object(o);
        prm->filErr++;
		return(false);
	}
	ivs_free(vxs);
	
	if(!o->setName(prm->name)){
_te_("setName failed");
		tp->kill_object(o);
        prm->filErr++;
		return(false);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss;	
ControlRef	c;
char		val[256];

	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportXID);
				GetTextControlValue(c,val,256);
				_tl.x=matof(val);
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportYID);
				GetTextControlValue(c,val,256);
				_tl.y=matof(val);
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportXpID);
				GetTextControlValue(c,val,256);
				_br.x=matof(val);
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportYpID);
				GetTextControlValue(c,val,256);
				_br.y=matof(val);
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportScaleID);
				GetTextControlValue(c,val,256);
				_scl=matof(val);
				break;
				
			case kHICommandCancel:
				break;
			
			case kXMapRasterImportPopKindID:
				c=get_control(kXMapRasterImportSignature,kXMapRasterImportKFrameID);
				_ik=GetControl32BitValue(c);
				switch(_ik){
					case kRasterImportKindBotRight:
						c=get_control(kXMapRasterImportSignature,kXMapRasterImportKind1ID);
						ShowControl(c);
						c=get_control(kXMapRasterImportSignature,kXMapRasterImportKind2ID);
						HideControl(c);
						break;
					case kRasterImportKindScale:
						c=get_control(kXMapRasterImportSignature,kXMapRasterImportKind1ID);
						HideControl(c);
						c=get_control(kXMapRasterImportSignature,kXMapRasterImportKind2ID);
						ShowControl(c);
						break;
				}
				break;
				
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapRasterImport::wd_init(WindowRef wd){
bGenericUnit*		u=_gapp->scaleMgr()->get();

	_ik=kRasterImportKindBotRight;
	_scl=u->coef();

	if(_gapp->cntMgr()->count()>0){
bGenericGeoElement*	o;
ivertices*			vxs;
		_gapp->cntMgr()->elements()->get(1,&o);
		o->getVertices(&vxs);
		Convert(&_tl,&vxs->vx.vx2[0]);
		_br=_tl;
	}
	else{
i2dvertex	vx;
		_gapp->mapIntf()->screenCenter(&vx);
		Convert(&_tl,&vx);
		_br=_tl;
	}
	
ControlRef	c;

	c=get_control(kXMapRasterImportSignature,kXMapRasterImportKind2ID);
	HideControl(c);
	
char		val[256];

	c=get_control(kXMapRasterImportSignature,kXMapRasterImportXID);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_tl.x);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportYID);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_tl.y);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportXpID);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_br.x);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportYpID);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_br.y);
	SetTextControlValue(c,val);

	c=get_control(kXMapRasterImportSignature,kXMapRasterImportScaleID);
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_scl);
	SetTextControlValue(c,val);
	
	u=_gapp->distMgr()->get();
	u->short_name(val);
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportXUnitID);
	SetTextControlValue(c,val);
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportYUnitID);
	SetTextControlValue(c,val);
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportXpUnitID);
	SetTextControlValue(c,val);
	c=get_control(kXMapRasterImportSignature,kXMapRasterImportYpUnitID);
	SetTextControlValue(c,val);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::parse_tfw(bStdFile& f){
char*	data;
int		sz;
	f.mount(&data,&sz);
	data=(char*)realloc(data,sz+1);
	data[sz]=0;
int		k=0;
double	d[6];
char	*addr;
char	dl[5]="\r\n";

	addr=strtok(data,dl);
	if(addr==NULL){
		sprintf(dl,"\n");
		addr=strtok(data,dl);
	}
	if(addr==NULL){
		sprintf(dl,"\r");
		addr=strtok(data,dl);
	}
	if(addr==NULL){
		free(data);
		return(false);
	}
	d[k]=matof(addr);
	while(addr){
		addr=strtok(NULL,dl);
		if(addr==NULL){
			break;
		}
		k++;
		d[k]=matof(addr);
		if(k==5){
			break;
		}
	}
	free(data);
	if(k!=5){
		return(false);
	}
	_tl.x=d[4];
	_tl.y=d[5];
	_xsz=d[0]-d[1];
	_ysz=d[2]-d[3];
	_ik=kRasterImportKindTFW;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRasterImport::parse_tab(bStdFile& f){
char*	data;
int		sz;
	f.mount(&data,&sz);
	data=(char*)realloc(data,sz+1);
	data[sz]=0;
char	*addr,*paddr,*co,*cc,*cv,*pco,*pcc,*pcv;
char	dl[5]="\r\n";
char	bf[256];
double	x,px;
int		k=0;
double	minx,miny,maxx,maxy;
double	pminx,pminy,pmaxx,pmaxy;

	addr=strtok(data,dl);
	if(addr==NULL){
		sprintf(dl,"\n");
		addr=strtok(data,dl);
	}
	if(addr==NULL){
		sprintf(dl,"\r");
		addr=strtok(data,dl);
	}
	if(addr==NULL){
        free(data);
		return(false);
	}
// on zappe le premier
	while(addr){
		addr=strtok(NULL,dl);
		if(addr==NULL){
			break;
		}
		co=strchr(addr,'(');
		if(!co){
			continue;
		}
		cc=strchr(addr,')');
		if(!cc){
			continue;
		}
		cv=strchr(addr,',');
		if(!cv){
			continue;
		}
		if(cv>cc){
			continue;
		}

		paddr=cc+1;
		pco=strchr(paddr,'(');
		if(!pco){
			continue;
		}
		pcc=strchr(paddr,')');
		if(!pcc){
			continue;
		}
		pcv=strchr(paddr,',');
		if(!pcv){
			continue;
		}
		if(pcv>pcc){
			continue;
		}
		
		memmove(bf,co+1,cv-co-1);
		bf[cv-co-1]=0;
		x=matof(bf);
		memmove(bf,pco+1,pcv-pco-1);
		bf[pcv-pco-1]=0;
		px=matof(bf);
		if(k==0){
			minx=maxx=x;
			pminx=pmaxx=px;
		}
		else{
			if(x<minx){
				minx=x;
				pminx=px;
			}
			else if(x>maxx){
				maxx=x;
				pmaxx=px;
			}
		}

		memmove(bf,cv+1,cc-cv-1);
		bf[cc-cv-1]=0;
		x=matof(bf);
		memmove(bf,pcv+1,pcc-pcv-1);
		bf[pcc-pcv-1]=0;
		px=matof(bf);
		if(k==0){
			miny=maxy=x;
			pminy=pmaxy=px;
		}
		else{
			if(x<miny){
				miny=x;
				pminy=px;
			}
			else if(x>maxy){
				maxy=x;
				pmaxy=px;
			}
		}
		k++;
	}
	if(k<2){
        free(data);
		return(false);
	}
	_tl.x=minx;
	_tl.y=miny;

	_br.x=maxx;
	_br.y=maxy;

	_xsz=(maxx-minx)/(pmaxx-pminx);
	_ysz=(maxy-miny)/(pmaxy-pminy);
	
	_ik=kRasterImportKindTAB;

    free(data);
    return(true);
}

