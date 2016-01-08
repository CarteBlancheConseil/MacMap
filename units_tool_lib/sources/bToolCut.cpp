//----------------------------------------------------------------------------
// File : bToolCut.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (geometry cut, scissors) tool class
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
// 24/10/2006 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolCut.h"
#include <std_ext/bXMapStdIntf.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/bEventLog.h>
#include <MacMapSuite/vx_association.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolCut	::bToolCut(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bToolGeomWithJoin(elt,gapp,bndl){
	setclassname("cuttool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolCut::~bToolCut(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolCut::create(bGenericXMLBaseElement* elt){
	return(new bToolCut(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::open(int* flags){
	set_use_trace(true);
	bToolGeomWithJoin::open(flags);
	
	set_use_join(true);
	set_use_sel(true);
	
	set_use_locs(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::clic(CGPoint pt, int count){
i2dvertex		br;
bGenericType*	tp;

	if(get_use_locs()){// Cas des polygones
		if(count>1){
			if(get_locs()->count()>1){
				action(0);
			}
			set_use_locs(false);
			set_use_sel(true);
			set_obj(NULL);
			get_locs()->reset();
		}		
		if(get_join(&br)){
			bToolGeomWithJoin::clic(br,count);
		}
		else{
			bToolGeomWithJoin::clic(pt,count);
		}
		
		if(get_obj()){
ivertices*	vxs;
			get_clic(&br);
			get_obj()->getVertices(&vxs);
			if(	(	(ivx2ivs_dist(&br,vxs)>__FUZZY_DIST__)		&&
					(!ivx_in_ivs(&br,vxs,false))			)	||
				(	(get_locs()->count()>1)						&&							
					(ivx2ivs_dist(&br,vxs)<=__FUZZY_DIST__)	)	){
				if(get_locs()->count()>1){
					action(0);
				}
				set_use_locs(false);
				set_use_sel(true);
				set_obj(NULL);
				get_locs()->reset();
			}
		}
		return;
	}

	if(!get_obj()){
		if(get_join(&br)){
			bToolGeomWithJoin::clic(br,count);
		}
		else{
			bToolGeomWithJoin::clic(pt,count);
		}
		return;
	}
	
	tp=_gapp->typesMgr()->get(get_obj()->getType());

	switch(tp->kind()){
		case kBaseKindPolyline:
			if(get_join(&br)){
				bToolGeomWithJoin::clic(br,count);
			}
			else{
				bToolGeomWithJoin::clic(pt,count);
			}
			action(0);
			break;
			
		case kBaseKindPolygon:
		case kBaseKindRaster:
			set_use_locs(true);
			if(get_join(&br)){
				bToolGeomWithJoin::clic(br,count);
			}
			else{
				bToolGeomWithJoin::clic(pt,count);
			}
			set_use_sel(false);
			break;
			
		default:
			if(get_join(&br)){
				bToolGeomWithJoin::clic(br,count);
			}
			else{
				bToolGeomWithJoin::clic(pt,count);
			}
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	if(get_use_locs()){
		draw_poly(true,true);
	}
	bToolGeomWithJoin::update(global);
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolCut::set_key(int k){
bool	b=false;
	if(get_use_locs()){
		switch(k){
			case kEnterCharCode:
			case kReturnCharCode:
				clearTempPathContext(true);
				validTempPathContext();
				if(get_locs()->count()>1){
					action(0);
				}
				(void)bToolGeomWithJoin::set_key(k);
				set_use_locs(false);
				set_use_sel(true);
				set_obj(NULL);
				k=0;
				b=true;
				break;
			case kEscapeCharCode:
				clearTempPathContext(true);
				validTempPathContext();				
				(void)bToolGeomWithJoin::set_key(k);
				set_use_locs(false);
				set_use_sel(true);
				set_obj(NULL);
				k=0;
				b=true;
				break;
			case kBackspaceCharCode:
				(void)bToolGeomWithJoin::set_key(k);
				clearTempPathContext(true);
				update(true);
				validTempPathContext();
				k=0;
				b=true;
				break;
		}
	}
	(void)bToolGeomWithJoin::set_key(k);
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::track_obj(){
bGenericGeoElement* o=get_obj();
	bToolGeomWithJoin::track_obj();
	if(o!=get_obj()){
		FlushSelection(_gapp);
	}
	if((get_obj())&&(o!=get_obj())){
bGenericType*	tp=_gapp->typesMgr()->get(get_obj()->getType());
		switch(tp->kind()){
			case kBaseKindPolyline:
			case kBaseKindPolygon:
			case kBaseKindRaster:
				get_obj()->setselect(true,true);
				break;
			default:
				set_obj(NULL);
				break;
		}
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::action(int count){
bGenericGeoElement*	o=get_obj();
	if(!o){
		return;
	}
	
bGenericType*	tp=_gapp->typesMgr()->get(o->getType());
bEventLog		log(_gapp,
					getbundle(),
					kCutMessageID,
					GetSignature(this));
	switch(tp->kind()){
		case kBaseKindPolyline:
			cut_line(o);
			break;
		case kBaseKindPolygon:
		case kBaseKindRaster:
			cut_surf(o);
			break;
		default:
			break;
	}
	log.close();
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolCut::load(){
bGenericXMLBaseElement* groot=bToolGeomWithJoin::load();
	if(!groot){
		return(NULL);
	}
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolCut::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	return(bToolGeomWithJoin::make(arr));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::cut_line(bGenericGeoElement*	o){
_bTrace_("bToolCut::cut_line",true);
bGenericGeoElement	*no;
ivertices			*vxs,*nvxs,*vsa,*vsb;
i2dvertex			vx,vr;
double				d,r;
int					idx;

	get_clic(&vx);
	o->getVertices(&vxs);
	d=ivx_proj(&vx,vxs,&vr,&idx,&r);
	if(	(idx==0)						&&
		(eq_ivx2(&vr,&vxs->vx.vx2[0]))	){
_te_("bad index "+idx);
		return;
	}
	if(	(idx==vxs->nv-1)						&&
		(eq_ivx2(&vr,&vxs->vx.vx2[vxs->nv-1]))	){
		_te_("bad index "+idx);
		return;
	}
	ivs2ivs(vxs,&nvxs);
	if((r>0)&&(r<1)){
		nvxs=ivx2_insert(nvxs,&vr,idx);
	}	
	ivs_split(nvxs,idx,&vsa,&vsb);
	if((!vsa)||(!vsb)){
		if(vsa){
_te_("vsb==NULL, idx="+idx);
			ivs_free(vsa);
		}
		if(vsb){
_te_("vsa==NULL, idx="+idx);
			ivs_free(vsb);
		}
		ivs_free(nvxs);
		return;
	}
	ivs_free(nvxs);

bGenericType* tp=_gapp->typesMgr()->get(o->getType());
	if(!tp->clone_object(o,&no)){
		ivs_free(vsa);
		ivs_free(vsb);
_te_("clone_object failed");
		return;
	}
	
	if(!no->setVertices(vsb)){
_te_("setVertices failed (b)");
		tp->kill_object(no);
		ivs_free(vsa);
		ivs_free(vsb);
		return;
	}

	if(!o->setVertices(vsa)){
_te_("setVertices failed (a)");
		tp->kill_object(no);		
	}
	
	ivs_free(vsa);
	ivs_free(vsb);
	
#pragma unused(d)
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolCut::cut_surf(bGenericGeoElement*	o){
_bTrace_("bToolCut::cut_surf",true);
bArray*				arr=get_locs();
ivertices*			shape=ivs_new(_2D_VX,arr->count(),0);
ivertices*			vxs;
ivertices*			bf;
i2dvertex*			p;
int					i,nv;
bGenericGeoElement	*oa,*ob;
bGenericType*		tp=_gapp->typesMgr()->get(o->getType());
bool				cut=false;

	o->getVertices(&bf);
	ivs2ivs(bf,&vxs);
	for(i=0;i<arr->count();i++){
		arr->get(i+1,&shape->vx.vx2[i]);
	}	

// cut or include
	for(i=0;i<shape->nv;i++){
_tm_(ivx2ivs_dist(&shape->vx.vx2[i],vxs));
		if(	(ivx2ivs_dist(&shape->vx.vx2[i],vxs)<=__FUZZY_DIST__)	||
			(!ivx_in_ivs(&shape->vx.vx2[i],vxs,0))					){
			cut=true;
			break;
		}
	}
	if(!cut){
		for(i=0;i<vxs->nv;i++){
_tm_(ivx2ivs_dist(&vxs->vx.vx2[i],shape));
			if(ivx2ivs_dist(&vxs->vx.vx2[i],shape)<=__FUZZY_DIST__){
				cut=true;
				break;
			}
		}
	}
	
// inclusion 
	if(!cut){
_tm_("inclusion");
		if(shape->nv<3){
_te_("nb points < 3");
			ivs_free(shape);
			ivs_free(vxs);
			return;
		}
		if(!eq_ivx2(&shape->vx.vx2[0],&shape->vx.vx2[shape->nv-1])){
			shape=ivx2_add(shape,&shape->vx.vx2[0]);
		}
		p=ivs2_part(vxs,0,&nv);
		if(ivx2_clockwise(p,nv)==-1){
			ivs_reverse(vxs);
		}
		if(ivx2_clockwise(shape->vx.vx2,shape->nv)==1){
			ivs_reverse(shape);
		}
		bf=ivs_group(vxs,shape);
		if(!bf){
_te_("ivs_group failed");
			ivs_free(shape);
			ivs_free(vxs);
			return;
		}
		vxs=bf;			
		ivs_reverse(shape);
		if(!tp->clone_object(o,&oa)){
_te_("clone_object a failed");
			ivs_free(shape);
			ivs_free(vxs);
			return;
		}
		if(!tp->clone_object(o,&ob)){
_te_("clone_object b failed");
			ivs_free(shape);
			ivs_free(vxs);
			tp->kill_object(oa);
			return;
		}
		if(!oa->setVertices(vxs)){
_te_("setVertices failed (a)");
			tp->kill_object(oa);
			tp->kill_object(ob);
			ivs_free(shape);
			ivs_free(vxs);
			return;
		}
		if(!ob->setVertices(shape)){
_te_("setVertices failed (b)");
			tp->kill_object(oa);
			tp->kill_object(ob);
			ivs_free(shape);
			ivs_free(vxs);
			return;
		}
		tp->kill_object(o);
		ivs_free(vxs);
		ivs_free(shape);
		return;
	}
	
// Découpe
_tm_("cut");
ivertices	*ra,*rb;

	ivs_split_surf(vxs,shape,&ra,&rb);
	if((!ra)||(!rb)){
_te_("rb||ra==NULL : "+ra+"->"+rb);
		if(ra){
_te_("rb==NULL");
			ivs_free(ra);
		}
		else if(rb){
_te_("ra==NULL");
			ivs_free(rb);
		}
		else{
_te_("ra&&rb==NULL");
		}
		ivs_free(shape);
		ivs_free(vxs);
		return;
	}
	if(!tp->clone_object(o,&oa)){
_te_("clone_object failed");
		ivs_free(ra);
		ivs_free(rb);
		ivs_free(shape);
		ivs_free(vxs);
		return;
	}
	if(!tp->clone_object(o,&ob)){
_te_("clone_object failed");
		tp->kill_object(oa);
		ivs_free(ra);
		ivs_free(rb);
		ivs_free(shape);
		ivs_free(vxs);
		return;
	}
	if(!oa->setVertices(ra)){
_te_("setVertices failed (a)");
		tp->kill_object(oa);
		tp->kill_object(ob);
		ivs_free(ra);
		ivs_free(rb);
		ivs_free(shape);
		ivs_free(vxs);
		return;
	}
	if(!ob->setVertices(rb)){
_te_("setVertices failed (b)");
		tp->kill_object(oa);
		tp->kill_object(ob);
		ivs_free(ra);
		ivs_free(rb);
		ivs_free(shape);
		ivs_free(vxs);
		return;
	}
	tp->kill_object(o);
	ivs_free(ra);
	ivs_free(rb);
	ivs_free(shape);
	ivs_free(vxs);
_tm_("end cut");
}
