//----------------------------------------------------------------------------
// File : bXMapIntersection.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, polygon intersection
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
// 11/04/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapIntersection.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapIntersection::bXMapIntersection(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("intersection");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapIntersection::~bXMapIntersection(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapIntersection::create(bGenericXMLBaseElement* elt){
	return(new bXMapIntersection(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapIntersection::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			intersect();
			break;
		case kExtProcessCallWithParams:
			intersect((*((intersection_prm*)prm)));
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
				if(!tp){
					return(false);
				}
				intersect(tp);
				return(true);
			}	
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapIntersection::test(void* prm){
	if(_gapp->selMgr()->count()<2){
		return(false);
	}
bGenericType*	tp;
int k=-1;
	for(long i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->nb_sel()>0){
			if(	(tp->kind()==kBaseKindPoint)	||
				(tp->kind()==kBaseKindText)		||
				(tp->kind()==kBaseKindRaster)	||
				(tp->kind()==kBaseNoKind)		){
				return(false);
			}
			if(k==-1){
				k=tp->kind();
			}
			else if(tp->kind()!=k){
				return(false);
			}
		}
	}
	
	tp=(bGenericType*)CurType(_gapp);
	if(tp==NULL){
		return(false);
	}
	/*if((k==kBaseKindPolygon)&&(tp->kind()!=kBaseKindPolygon)){
		return(false);
	}
	if(	(k==kBaseKindPolyline)&&
		(	(tp->kind()==kBaseKindText)		||
			(tp->kind()==kBaseKindPolygon)	||
			(tp->kind()==kBaseKindRaster)	||
			(tp->kind()==kBaseNoKind)		)){
		return(false);
	}*/
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapIntersection::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIntersection::intersect(){
bGenericType*	tp=(bGenericType*)CurType(_gapp);
	if(!tp){
		return;
	}
bEventLog	log(_gapp,this);
	intersect(tp);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIntersection::intersect(bGenericType* tp){
_bTrace_("bXMapIntersection::intersect",false);
ivertices			*vsa,*vsb,*vsc;
bGenericGeoElement	*oa,*ob,*oc;
bArray				sel(*(_gapp->selMgr()->elements()));
ivx_rect			ra,rb;
int					i,j;

int					fa=tp->fields()->get_index("A_ID");
	if(fa==0){
		tp->fields()->add(_int,_kIntLen,0,"A_ID");
		fa=tp->fields()->get_index("A_ID");
		if(!fa){
			return;
		}
	}
int					fb=tp->fields()->get_index("B_ID");
	if(fb==0){
		tp->fields()->add(_int,_kIntLen,0,"B_ID");
		fb=tp->fields()->get_index("B_ID");
		if(!fb){
			return;
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(false);
	for(i=1;i<=sel.count();i++){
		sel.get(i,&oa);
		oa->getBounds(&ra);
		oa->getVertices(&vsa);
		for(j=i+1;j<=sel.count();j++){
			sel.get(j,&ob);
			ob->getBounds(&rb);
			if(ivr_sect(&ra,&rb,&rb)){
				ob->getVertices(&vsb);
				if(tp->kind()==kBaseKindPolygon){
					vsc=ivs_intersect_as_surf(vsa,vsb);
				}
				else if((tp->kind()==kBaseKindPolyline)	||
						(tp->kind()==kBaseKindPoint)	){
					vsc=ivs_intersect_as_line(vsa,vsb);
				}
				else{
					continue;
				}
				if(vsc){
					if((tp->kind()==kBaseKindPolygon)&&(ivs_surf(vsc)<=0)){
						NSBeep();
_te_("malformed polygon");
					}
					else if((tp->kind()==kBaseKindPolyline)&&(ivs_length(vsc)<=0)){
						NSBeep();
_te_("malformed polyline");
					}
					else if((tp->kind()==kBaseKindPoint)&&(vsc->nv<=0)){
						NSBeep();
_te_("malformed point");
					}
					else if(tp->new_object(&oc)){
                        if(!InitWithStyle(_gapp,_gapp->layersMgr()->get_current(),oc)){
							NSBeep();
						}
						oc->setVertices(vsc);
						oc->setValue(fa,oa->getID());
						oc->setValue(fb,ob->getID());
											}
					else{
						NSBeep();
_te_("new_object failed");
					}
					ivs_free(vsc);
				}
			}
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
}

