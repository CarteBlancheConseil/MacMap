//----------------------------------------------------------------------------
// File : bXMapConvexHull.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, make new convex hull polygon from selected objects geometry
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
// 15/03/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapConvexHull.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapConvexHull	::bXMapConvexHull(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("convexhull");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapConvexHull::~bXMapConvexHull(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapConvexHull::create(bGenericXMLBaseElement* elt){
	return(new bXMapConvexHull(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapConvexHull::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			convexify();
			break;
		case kExtProcessCallWithParams:
			convexify((bGenericType*)prm);
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
					convexify(tp);
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
bool bXMapConvexHull::test(void* prm){
	if(_gapp->selMgr()->count()==0){
		return(false);
	}
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return(false);
	}
	if(	(tp->kind()!=kBaseKindPolyline)	&&
		(tp->kind()!=kBaseKindPolygon)	){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapConvexHull::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapConvexHull::convexify(){
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return;
	}
	if(	(tp->kind()!=kBaseKindPolyline)	&&
		(tp->kind()!=kBaseKindPolygon)	){
		return;
	}

bEventLog	log(_gapp,
				getbundle(),
				kXMapConvexHullMessageID,
				GetSignature(this));
	convexify(tp);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapConvexHull::convexify(bGenericType* tp){
ivertices			*vxs,*poly=ivs_new(_2D_VX,0,0);
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));
	
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&vxs);
		poly=ivs_concat(poly,vxs);
	}
	vxs=ivs_convexify(poly->vx.vx2,poly->nv);
	ivs_free(poly);
	if(!vxs){
		return;
	}
	if(vxs->nv<4){
		ivs_free(vxs);
		return;
	}
	if(!tp->new_object(&o)){
		ivs_free(vxs);
		return;
	}
	if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),o)){
		NSBeep();
	}
	o->setVertices(vxs);
	ivs_free(vxs);
}
