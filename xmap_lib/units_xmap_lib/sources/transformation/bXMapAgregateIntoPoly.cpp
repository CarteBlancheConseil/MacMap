//----------------------------------------------------------------------------
// File : bXMapAgregateIntoPoly.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, make new polygon from selected objects geometry
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
// 20/03/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapAgregateIntoPoly.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapAgregateIntoPoly	::bXMapAgregateIntoPoly(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						:bStdXMap(elt,gapp,bndl){
	setclassname("agregateintopoly");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapAgregateIntoPoly::~bXMapAgregateIntoPoly(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapAgregateIntoPoly::create(bGenericXMLBaseElement* elt){
	return(new bXMapAgregateIntoPoly(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAgregateIntoPoly::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			make();
			break;
		case kExtProcessCallWithParams:
			make((bGenericType*)prm);
			break;
		case kExtProcessCallWithXMLTree:{
bGenericType*			tp;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				if(tp){
					make(tp);
				}
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
bool bXMapAgregateIntoPoly::test(void* prm){
	if(_gapp->selMgr()->count()==0){
		return(false);
	}
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return(false);
	}
	if(tp->kind()!=kBaseKindPolygon){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAgregateIntoPoly::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAgregateIntoPoly::make(){
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return;
	}
	if(tp->kind()!=kBaseKindPolygon){
		return;
	}

bEventLog	log(_gapp,
				getbundle(),
				kXMapAgregateIntoPolyMessageID,
				GetSignature(this));
	make(tp);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAgregateIntoPoly::make(bGenericType* tp){
ivertices			*vxs,*vsb,*poly=ivs_new(_2D_VX,0,0);
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));
double				da,db;
bool				rfrst=false;
dst_end_kind		dk;

	if(sel.count()>1){
		sel.get(1,&o);
		o->getVertices(&vxs);
		sel.get(2,&o);
		o->getVertices(&vsb);
		/*da=*/(void)vxs_dist_end(vxs,vsb,&dk);
		switch(dk){
			case kDistEndKindAC:
			case kDistEndKindAD:
				rfrst=true;
				break;
			default:
				break;
		}
	}
	
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&vxs);
		if(ivs2ivs(vxs,&vsb)==noErr){
			if(rfrst){
				ivs_reverse(vsb);
				rfrst=false;
			}
			if(poly->nv>0){
				da=ivx2_dist(&(poly->vx.vx2[poly->nv-1]),&vsb->vx.vx2[0]);
				db=ivx2_dist(&(poly->vx.vx2[poly->nv-1]),&vsb->vx.vx2[vsb->nv-1]);
				if(db<da){
					ivs_reverse(vsb);
				}
			}
			poly=ivs_concat(poly,vsb);
			ivs_free(vsb);
		}
	}
	if(!eq_ivx2(&poly->vx.vx2[0],&poly->vx.vx2[poly->nv-1])){
i2dvertex	vx=poly->vx.vx2[0];
		poly=ivx2_add(poly,&vx);
	}
	if(poly->nv<4){
		ivs_free(poly);
		return;
	}
	if(!tp->new_object(&o)){
		ivs_free(poly);
		return;
	}
    if(!InitWithStyle(_gapp,_gapp->layersMgr()->get_current(),o)){
		NSBeep();
	}
	if(!o->setVertices(poly)){
		tp->kill_object(o);
		ivs_free(poly);
		return;
	}

	ivs_free(poly);
}
