//----------------------------------------------------------------------------
// File : bToolRedraw.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (geometry redraw) tool class
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
// 29/05/2009 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolRedraw.h"
#include <std_ext/bXMapStdIntf.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <MacMapSuite/vx_association.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolRedraw	::bToolRedraw(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bToolGeomWithJoin(elt,gapp,bndl){
	setclassname("redrawtool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolRedraw::~bToolRedraw(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolRedraw::create(bGenericXMLBaseElement* elt){
	return(new bToolRedraw(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::open(int* flags){
	set_use_trace(true);
	bToolGeomWithJoin::open(flags);
	_ec=0;
	_cdrw=load_curs("DrawCursor.png");

	set_use_join(true);
	set_use_sel(true);

	set_use_locs(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::close(){
	bToolGeomWithJoin::close();
	if(_cdrw){
		ReleaseCocoaCursor(_cdrw);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::activate(){
	bToolGeomWithJoin::activate();
	if(_ec>0){
		set_curs(_cdrw);
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::clic(CGPoint pt, int count){
i2dvertex		br;
bGenericType*	tp;	
	
	if(_ec>1){
		if(count>1){
			if(get_locs()->count()>=1){
				action(0);
			}
			set_use_locs(false);
			set_obj(NULL);
			get_locs()->reset();
			_ec=0;
			set_curs();
		}
		
		if(get_join(&br)){
			bToolGeomWithJoin::clic(br,count);
		}
		else{
			bToolGeomWithJoin::clic(pt,count);
		}
		return;
	}
	else if((count>1)&&(_ec==1)){// =Annuler
		set_use_locs(false);
		set_obj(NULL);
		get_locs()->reset();
		_ec=0;
		set_curs();
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
		case kBaseKindPolygon:
		case kBaseKindRaster:
			if(_ec==0){
				set_use_locs(true);
				if(get_join(&br)){
					bToolGeomWithJoin::clic(br,count);
				}
				else{
					bToolGeomWithJoin::clic(pt,count);
				}
				set_use_locs(false);
				get_clic(&_vxa);
				_ec++;
			}
			else if(_ec==1){
				if(get_join(&br)){
					bToolGeomWithJoin::clic(br,count);
					get_clic(&_vxb);
					if(!eq_ivx2(&_vxa,&_vxb)){
						set_clic(&_vxa);
						set_use_locs(true);
						_ec++;
						set_curs(_cdrw);
					}
					else{
						NSBeep();
					}
				}
				else{
					NSBeep();
				}			
			}
			else{
				if(get_join(&br)){
					bToolGeomWithJoin::clic(br,count);
				}
				else{
					bToolGeomWithJoin::clic(pt,count);
				}
				_ec++;
			}
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
void bToolRedraw::update(bool global){
	if(get_active()){
		if(!global){
			clearTempPathContext(false);
		}
		if(get_use_locs()){
			if(_ec>1){
i2dvertex		bck;
				get_cur(&bck);
				if(bck.h!=LONG_MIN){
					get_locs()->add(&bck);
					get_locs()->add(&_vxb);
					draw_poly(true,false);
					get_locs()->rmv(get_locs()->count());
					get_locs()->rmv(get_locs()->count());
				}
			}
		}
		bToolGeomWithJoin::update(global);
		if(!global){
			validTempPathContext();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolRedraw::set_key(int k){
bool	b=false;
	
	if(_ec>0){
		switch(k){
			case kEnterCharCode:
			case kReturnCharCode:
				clearTempPathContext(true);
				validTempPathContext();
				if(get_locs()->count()>1){
					action(0);
				}
				(void)bToolGeomWithJoin::set_key(k);
				k=0;
				set_use_locs(false);
				set_obj(NULL);
				_ec=0;
				b=true;
				set_curs();
				break;
				
			case kEscapeCharCode:
				clearTempPathContext(true);
				validTempPathContext();				
				(void)bToolGeomWithJoin::set_key(k);
				k=0;
				set_use_locs(false);
				set_obj(NULL);
				b=true;
				_ec=0;
				set_curs();
				break;
			case kBackspaceCharCode:
				(void)bToolGeomWithJoin::set_key(k);
				clearTempPathContext(true);
				update(true);
				validTempPathContext();
				k=0;
				break;
		}
	}
	(void)bToolGeomWithJoin::set_key(k);
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::track_obj(){
	if(_ec==0){
bGenericGeoElement* o=get_obj();
		bToolGeomWithJoin::track_obj();
		if(o!=get_obj()){
			_gapp->selMgr()->flush();
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
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolRedraw::action(int count){
_bTrace_("bToolRedraw::action",true);
bGenericGeoElement*	o=get_obj();
	if(!o){
		return;
	}
bEventLog	log(_gapp,
				getbundle(),
				kRedrawMessageID,
				GetSignature(this));
				
				
bArray*				arr=get_locs();
	arr->add(&_vxb);
ivertices*			shape=ivs_new(_2D_VX,arr->count(),0);
ivertices*			vxs;
ivertices*			bf;
ivertices*			vsa;
ivertices*			vsb;
ivertices*			vsc;
i2dvertex			pa,pb;
int					i;
int					idxa,idxb;
double				ra,rb;
	
	o->getVertices(&bf);
	ivs2ivs(bf,&vxs);
	for(i=1;i<=arr->count();i++){
		arr->get(i,&shape->vx.vx2[i-1]);
	}	

	ivx_proj(&shape->vx.vx2[0],vxs,&pa,&idxa,&ra);
	if(ra>0&&ra<1){
_tm_("insert a");
		vxs=ivx2_insert(vxs,&pa,idxa);
		if(vxs==NULL){
_te_("ivx2_insert(vxs,&pa,idxa) returns NULL");
			return;
		}
	}
	
_tm_("ivx_proj b");
	ivx_proj(&shape->vx.vx2[shape->nv-1],vxs,&pb,&idxb,&rb);
	if(rb>0&&rb<1){
_tm_("insert b");
		vxs=ivx2_insert(vxs,&pb,idxb);
		if(vxs==NULL){
_te_("ivx2_insert(vxs,&pb,idxb) returns NULL");
			return;
		}
	}
	
	if(eq_ivx2(&pa,&pb)){
		ivs_free(vxs);
		ivs_free(shape);
_te_("pa==pb");
		return;
	}
	
_tm_("ivx_proj a 2");
	ivx_proj(&shape->vx.vx2[0],vxs,&pa,&idxa,&ra);
_tm_("ivx_proj b 2");
	ivx_proj(&shape->vx.vx2[shape->nv-1],vxs,&pb,&idxb,&rb);
	if(idxa==idxb){
		ivs_free(vxs);
		ivs_free(shape);
_te_("idxa==idxb :"+idxa+"->"+idxb);
		return;
	}

	if(idxa>idxb){
_tm_("reverse");
		ivs_reverse(shape);
		i=idxa;
		idxa=idxb;
		idxb=i;
	}
_tm_("idxa:"+idxa+"->idxb:"+idxb);


_tm_("ivs_split vxs");
	ivs_split(vxs,idxa,&vsa,&vsb);
_tm_("split "+(void*)vxs+"->"+(void*)vsa+":"+(void*)vsb);
	
	if((vsa!=NULL)&&(vsb!=NULL)){
_tm_("ivs_free vxs");
		ivs_free(vxs);
		vxs=vsb;
	}	
	
_tm_("ivs_split vxs (vsb)");
	ivs_split(vxs,idxb-idxa,&vsb,&vsc);
_tm_("split "+(void*)vxs+"->"+(void*)vsb+":"+(void*)vsc);

	if((vsb!=NULL)&&(vsc!=NULL)){
_tm_("ivs_free vxs (vsb)");
		ivs_free(vxs);
_tm_("ivs_free vsb");
		ivs_free(vsb);
	}	

_tm_("ivs_concat(vsa,shape)");
	if(vsa){
		vsa=ivs_concat(vsa,shape);
_tm_("ivs_free(shape)");
		ivs_free(shape);
	}
	else{
		vsa=shape;
	}
	
	if(vsa==NULL){
_te_("ivs_concat(vsa,shape) returns NULL");
		return;		
	}
	
_tm_("ivs_concat(vsa,vsc)");
	if(vsc){
		vsa=ivs_concat(vsa,vsc);
_tm_("ivs_free(vsc)");
		ivs_free(vsc);
	}
	
	if(vsa==NULL){
_te_("ivs_concat(vsa,vsc) returns NULL");
		return;		
	}
	
_tm_("setVertices(vsa)");
	o->setVertices(vsa);
	
_tm_("ivs_free(vsa)");
	ivs_free(vsa);
	log.close();
//	_gapp->mapIntf()->inval();// Pb de remise à jour
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolRedraw::load(){
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
bGenericXMLBaseElement* bToolRedraw::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	return(bToolGeomWithJoin::make(arr));
}
