//----------------------------------------------------------------------------
// File : bToolAlter.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (geometry) tool class
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
// 30/03/2006 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolAlter.h"
#include <std_ext/bXMapStdIntf.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>	
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolAlter	::bToolAlter(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bToolGeomWithJoin(elt,gapp,bndl){
	setclassname("altertool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolAlter::~bToolAlter(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolAlter::create(bGenericXMLBaseElement* elt){
	return(new bToolAlter(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::open(int* flags){
	set_use_trace(true);
	bToolGeomWithJoin::open(flags);
	_cadd=load_curs("AddCursor.png");
	_crmv=load_curs("EraseCursor.png");
	_vxs=NULL;
	_mode=kToolAlterMovePoint;
	_kind=kBaseNoKind;
	_idx=0;
	
	set_use_sel(true);
	set_use_join(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::close(){
	bToolGeomWithJoin::close();
	if(_cadd){
		ReleaseCocoaCursor(_cadd);
	}
	if(_crmv){
		ReleaseCocoaCursor(_crmv);
	}
	if(_vxs){
		ivs_free(_vxs);
		_vxs=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::clic(CGPoint pt, int count){
	bToolGeomWithJoin::clic(pt,count);	
// Edit	current vxs
	if(_vxs){
		_idx=find_vx();
		switch(_mode){
			case kToolAlterMovePoint:
				if(_idx){
					set_use_drag(true);
					set_use_join(true);
				}
				break;
			case kToolAlterAddPoint:
				if(!_idx){
i2dvertex			vx;
					_idx=insert_vx(&vx);
					if(_idx){
						_vxs=ivx2_insert(_vxs,&vx,_idx-1);
					}
					clearTempPathContext(true);
					update(true);
					validTempPathContext();
				}
				break;
			case kToolAlterRemovePoint:
				remove(_idx,(count>2));
				break;
		}
		return;
	}

// Begin obj edition
bGenericGeoElement* o=get_obj();
	if(!o){
		return;
	}
	FlushSelection(_gapp);
	o->getVertices(&_vxs);
	if(ivs2ivs(_vxs,&_vxs)){
		return;
	}
	set_use_sel(false);
	clearTempPathContext(true);
	update(true);
	validTempPathContext();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::end_clic(){
	bToolGeomWithJoin::end_clic();	
	if(_vxs){
		set_use_join(false);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
bGenericGeoElement*	o=get_obj();
	if(o&&_vxs){
		draw_poly(_vxs,true);
	}
	bToolGeomWithJoin::update(global);
	if(!global){
		validTempPathContext();
	}
	
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolAlter::set_key(int k){
bool	b=false;
	(void)bToolGeomWithJoin::set_key(k);
	if(!_vxs){
		return(b);
	}
	switch(k){
		case kEnterCharCode:
		case kReturnCharCode:
// Poutine
			put_vxs();
		case kEscapeCharCode:
// Free & erase
			update(true);
			ivs_free(_vxs);
			_vxs=NULL;
			set_obj(NULL);
			set_use_sel(true);
			_kind=kBaseNoKind;
			_idx=0;
			clearTempPathContext(true);
			validTempPathContext();				
			b=true;
			break;
		case kBackspaceCharCode:
			if(is_modifiers(shiftKey)){
				remove(1,false);
			}
			else{
				remove(_vxs->nv,false);
			}
			clearTempPathContext(true);
			update(true);
			validTempPathContext();				
			b=true;
			break;
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::set_modifiers(int k){	
	bToolGeomWithJoin::set_modifiers(k);
	if(!get_active()){
		return;
	}
	if(!_vxs){
		_mode=kToolAlterMovePoint;
		set_curs();
		return;
	}
	if(is_modifiers(cmdKey)){
		_mode=kToolAlterAddPoint;
		set_curs(_cadd);
	}
	else if(is_modifiers(optionKey)){
		_mode=kToolAlterRemovePoint;
		set_curs(_crmv);
	}
	else{
		_mode=kToolAlterMovePoint;
		set_curs();
	}
}

// ---------------------------------------------------------------------------
// Ici on doit désactiver le drag normal et placer le update après
// -----------
void bToolAlter::drag(CGPoint pt){
	if((_vxs)&&(_idx)){
		clearTempPathContext(true);
		set_use_drag(false);
		bToolGeomWithJoin::drag(pt);
		bToolGeomWithJoin::track_join();
		set_use_drag(true);		
		get_cur(&_vxs->vx.vx2[_idx-1]);
		if(	(_kind==kBaseKindPolygon)	||
			(_kind==kBaseKindRaster)	){
			if(_idx==1){
				_vxs->vx.vx2[_vxs->nv-1]=_vxs->vx.vx2[0];
			}
		}
		update(true);
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::track_obj(){
bArray*				ga;
bGenericStyle*		style;
bGenericGeoElement	*o=NULL,*prev;
bGenericType*		tp;
int					i,n=CountLayers(_gapp);
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
				set_obj(o);
				_kind=tp->kind();
				if(o!=prev){
					FlushSelection(_gapp);
				}
				if(o!=NULL){
					get_obj()->setselect(true,true);
				}					
			}
			break;
		}
		delete ga;
	}
	if(o==NULL){
		set_obj(NULL);
		_kind=kBaseNoKind;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolAlter::load(){
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
bGenericXMLBaseElement* bToolAlter::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	return(bToolGeomWithJoin::make(arr));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bToolAlter::find_vx(){
	if(!_vxs){
		return(0);
	}
CGPoint	pa,pb;
	get_clic(&pa);
	for(int	i=0;i<_vxs->nv;i++){
		Convert(&pb,&_vxs->vx.vx2[i]);
		if(	(labs(pa.x-pb.x)<3)	&&
			(labs(pa.y-pb.y)<3)	){
			return(i+1);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bToolAlter::insert_vx(i2dvertex* vx){
	if(!_vxs){
		return(0);
	}
i2dvertex	vr;
	get_clic(vx);
double		r;
int			idx;
double		d=ivx_proj(vx,_vxs,&vr,&idx,&r);
CGPoint		pa,pb;
	get_clic(&pa);
	Convert(&pb,&vr);
	if(	(fabs(pa.x-pb.x)<3)	&&
		(fabs(pa.y-pb.y)<3)	){
		*vx=vr;
		return(idx+1);
	}

#pragma unused(d)
#pragma unused(r)

	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::put_vxs(){
_bTrace_("bToolAlter::put_vxs",false);
	if(!_vxs){
_te_("_vxs==NULL");
		return;
	}
bGenericGeoElement*	o=get_obj();
	if(!o){
_te_("o==NULL");
		return;
	}
bEventLog	log(_gapp,
				getbundle(),
				kAlterMessageID,
				GetSignature(this));
	if(!o->setVertices(_vxs)){
_te_("setVertices(_vxs) failed");
		return;
	}
	log.close();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolAlter::remove(int idx, bool all){
	if(	(_kind==kBaseKindPolyline)	&&
		(all==true)					){
int		n=_vxs->nv-2;
		for(int i=0;i<n;i++){
			_vxs=ivx_remove(_vxs,1);
		}
		clearTempPathContext(true);
		update(true);
		validTempPathContext();
	}
	else if(idx){
		if(	(_kind==kBaseKindPolygon)	||
			(_kind==kBaseKindRaster)	){
			if(_vxs->nv<=4){
				NSBeep();
				return;
			}
		}
		else if(_kind==kBaseKindPolyline){
			if(_vxs->nv<=2){
				NSBeep();
				return;
			}
		}
		_vxs=ivx_remove(_vxs,idx-1);
		if(	(_kind==kBaseKindPolygon)	||
			(_kind==kBaseKindRaster)	){
			if(idx==1){
				_vxs->vx.vx2[_vxs->nv-1]=_vxs->vx.vx2[0];
			}
			else if(idx==_vxs->nv+1){
				_vxs->vx.vx2[0]=_vxs->vx.vx2[_vxs->nv-1];
			}
		}
		clearTempPathContext(true);
		update(true);
		validTempPathContext();
	}
}
