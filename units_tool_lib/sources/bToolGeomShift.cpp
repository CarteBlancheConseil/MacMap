//----------------------------------------------------------------------------
// File : bToolGeomShift.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (geometry shift) tool class
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
// 19/06/2007 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolGeomShift.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolGeomShift	::bToolGeomShift(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdToolGeom(elt,gapp,bndl){
	setclassname("geomshifttool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolGeomShift::~bToolGeomShift(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolGeomShift::create(bGenericXMLBaseElement* elt){
	return(new bToolGeomShift(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::open(int* flags){
	set_use_trace(true);
	bStdToolGeom::open(flags);
	
	_vxs=NULL;
	_d=0;

	set_use_sel(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::clic(CGPoint pt, int count){
	bStdToolGeom::clic(pt,count);
bGenericGeoElement* o=get_obj();
	if(!o){
		return;
	}
	_d=0;
	o->getVertices(&_vxs);
	if(ivs2ivs(_vxs,&_vxs)){
		return;
	}
	set_use_sel(false);
	set_use_drag(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::end_clic(){
	bStdToolGeom::end_clic();
	set_use_drag(false);
	set_use_sel(true);

bGenericGeoElement*	o=get_obj();
	if(!o){
		return;
	}
	if(!_vxs){
		return;
	}

	if(_d==0){
		ivs_free(_vxs);
		return;
	}
ivertices*  out=get_output();
    if(!out){
        ivs_free(_vxs);
        _vxs=NULL;
        _d=0;
        return;
    }
bEventLog	log(_gapp,
				getbundle(),
				kGeomShiftMessageID,
				GetSignature(this));
//    ivs_shift(&out,_d);
//    if(is_modifiers(optionKey)){
//        if(ivs_n_parts(out)==ivs_n_parts(_vxs)){
//i2dvertex   *pout,*pvxs;
//int         nout,nvxs;
//            for(long i=0;i<ivs_n_parts(out);i++){
//                pout=ivs2_part(out,i,&nout);
//                pvxs=ivs2_part(_vxs,i,&nvxs);
//                pout[0]=pvxs[0];
//                pout[nout-1]=pvxs[nvxs-1];
//            }
//        }
//        else{
//            out->vx.vx2[0]=_vxs->vx.vx2[0];
//            out->vx.vx2[out->nv-1]=_vxs->vx.vx2[_vxs->nv-1];
//        }
//    }
	o->setVertices(out);
	log.close();
	
	set_obj(NULL);
    ivs_free(out);
    ivs_free(_vxs);
	_vxs=NULL;
	_d=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}	
	if(get_on_drag()){
ivertices*	vxs=get_output();
        if(vxs){
            draw_poly(vxs,false);
            ivs_free(vxs);
        }
//ivertices*	vxs=NULL;
//		for(;;){
//			if(!_vxs){
//				break;
//			}
//			ivs2ivs(_vxs,&vxs);
//			if(!_vxs){
//				break;
//			}	
//			ivs_shift(&vxs,_d);
//			if(!vxs){
//				break;
//			}
//			break;
//		}
//		draw_poly(vxs,false);
//		ivs_free(vxs);
	}
	if(!global){
		validTempPathContext();
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::drag(CGPoint pt){
	bStdToolGeom::drag(pt);
	if(!get_obj()||!_vxs){
		return;
	}
	get_dist_to_path();
	report();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::track_obj(){
bArray*				ga;
bGenericStyle*		style;
bGenericGeoElement	*o=NULL,*prev;
bGenericType*		tp;
long				i,n=CountLayers(_gapp);
CGPoint				pt;
	
	get_cur(&pt);
	prev=get_obj();
	for(i=n;i>0;i--){
		style=(bGenericStyle*)GetIndLayer(_gapp,i);
		if(!style){
			continue;
		}
		tp=style->gettype();
		if(tp==NULL){
			continue;
		}
		if(	(tp->kind()!=kBaseKindPolyline)	&&
			(tp->kind()!=kBaseKindPolygon)	&&
			(tp->kind()!=kBaseKindRaster)	){
			continue;
		}
		ga=style->objsatpoint(&pt,false);
		if(!ga){
			continue;
		}
		if(ga->count()>0){
			ga->get(1,&o);
			delete ga;
			if(o!=prev){
				FlushSelection(_gapp);
				set_obj(o);
				get_obj()->setselect(true,true);
			}
			break;
		}
		
		delete ga;
	}
	if(o==NULL){
		FlushSelection(_gapp);
		set_obj(NULL);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::get_dist_to_path(){
i2dvertex	vx,vr;
	get_cur(&vx);
int		idx;
double	r,d=ivx_proj(&vx,_vxs,&vr,&idx,&r);
	if(idx==0){
		idx++;
	}
	_d=d*XYSIAM(_vxs->vx.vx2[idx-1].h,-_vxs->vx.vx2[idx-1].v,_vxs->vx.vx2[idx].h,-_vxs->vx.vx2[idx].v,vx.h,-vx.v);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolGeomShift::report(){
double			d;
bGenericUnit*	u=_gapp->distMgr()->get();
char			str[256],lib[64];
int				dg=_gapp->document()->dist_pref_digits();
i2dvertex		vxa,vxb;
	
	get_clic(&vxb);
	get_cur(&vxa);
	d=Measure_i2d(_gapp,_d);
	u->short_name(lib);
	
	sprintf(str,"d=%.*f %s",dg,d,lib);
	WriteMessage(_gapp,str);
}

// ---------------------------------------------------------------------------
//
// -----------
ivertices* bToolGeomShift::get_output(){
    if(!_vxs){
        return NULL;
    }
ivertices*  out;
    if(ivs2ivs(_vxs,&out)){
        return NULL;
    }
    ivs_shift(&out,_d);
    if(is_modifiers(optionKey)){
        if(ivs_n_parts(out)==ivs_n_parts(_vxs)){
            i2dvertex   *pout,*pvxs;
            int         nout,nvxs;
            for(long i=0;i<ivs_n_parts(out);i++){
                pout=ivs2_part(out,i,&nout);
                pvxs=ivs2_part(_vxs,i,&nvxs);
                pout[0]=pvxs[0];
                pout[nout-1]=pvxs[nvxs-1];
            }
        }
        else{
            out->vx.vx2[0]=_vxs->vx.vx2[0];
            out->vx.vx2[out->nv-1]=_vxs->vx.vx2[_vxs->nv-1];
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolGeomShift::load(){
bGenericXMLBaseElement* groot=bStdToolGeom::load();
	if(!groot){
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		return(NULL);
	}
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolGeomShift::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	return(bStdToolGeom::make(arr));
}

