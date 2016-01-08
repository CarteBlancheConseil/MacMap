//----------------------------------------------------------------------------
// File : bXMapSplitToPoints.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, make points from selected objects
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

#include "bXMapSplitToPoints.h"
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
bXMapSplitToPoints	::bXMapSplitToPoints(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("splittopoints");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSplitToPoints::~bXMapSplitToPoints(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSplitToPoints::create(bGenericXMLBaseElement* elt){
	return(new bXMapSplitToPoints(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSplitToPoints::process(int msg, void* prm){
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
bool bXMapSplitToPoints::test(void* prm){
	if(_gapp->selMgr()->count()==0){
		return(false);
	}
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return(false);
	}
	if(	(tp->kind()!=kBaseKindPoint)	&&
		(tp->kind()!=kBaseKindText)		){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSplitToPoints::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSplitToPoints::make(){
bGenericType* tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return;
	}
	if(tp->kind()!=kBaseKindPoint){
		return;
	}

bEventLog	log(_gapp,
				getbundle(),
				kXMapSplitToPointsMessageID,
				GetSignature(this));
	make(tp);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSplitToPoints::make(bGenericType* tp){
_bTrace_("bXMapSplitToPoints::make",true);
ivertices			*vxs,*pt;
bGenericGeoElement	*o,*d;
bArray				sel(*(_gapp->selMgr()->elements()));
int					i,j;

	for(i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&vxs);
		for(j=0;j<vxs->nv;j++){
			pt=ivs_new(_2D_VX,1,0);
			if(!pt){
_te_("ivs_new");
				return;
			}
			pt->vx.vx2[0]=vxs->vx.vx2[j];
			if(!tp->new_object(&d)){
_te_("new_object");
				ivs_free(pt);
				return;
			}
			if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),d)){
				NSBeep();
			}
			if(!d->setVertices(pt)){
_te_("setVertices");
				ivs_free(pt);
				tp->kill_object(d);
				return;
			}
			ivs_free(pt);
		}
	}
}
