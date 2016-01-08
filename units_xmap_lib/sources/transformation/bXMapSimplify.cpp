//----------------------------------------------------------------------------
// File : bXMapSimplify.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, simplify selected geometries
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
// 29/11/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapSimplify.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSimplify	::bXMapSimplify(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("simplify");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSimplify::~bXMapSimplify(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSimplify::create(bGenericXMLBaseElement* elt){
	return(new bXMapSimplify(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSimplify::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_a=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplify::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(simplify());
			break;
		case kExtProcessCallWithParams:{
double*		a=(double*)prm;
			return(simplify(*a));
			}break;
		case kExtProcessCallWithXMLTree:{
double					a;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				a=atof(val);
				return(simplify(a));
			}	
			break;
		default:
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplify::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplify::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				puts();
				break;
			case kHICommandCancel:
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
void bXMapSimplify::wd_init(WindowRef wd){
ControlRef			c;
char				val[256];

	c=get_control(kXMapSimplifySignature,kXMapSimplifyID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.2f",_a);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplify::simplify(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapSimplifyMessageID,
				GetSignature(this));
	(void)simplify(_a);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplify::simplify(double d){
ivertices			*vxo,*vxs;
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));

	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&vxs);
		ivs2ivs(vxs,&vxo);
		ivs_simplify_with_angle(&vxo,deg2rad(_a));
		if(vxo->nv!=vxs->nv){
			o->setVertices(vxo);
		}
		ivs_free(vxo);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSimplify::puts(){
ControlRef		c;
char			val[256];
		
	c=get_control(kXMapSimplifySignature,kXMapSimplifyID);
	GetTextControlValue(c,val,sizeof(val));
	_a=matof(val);
}






// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSimplifyToDeath::bXMapSimplifyToDeath(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("simplifytodeath");
	_d=5;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSimplifyToDeath::~bXMapSimplifyToDeath(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSimplifyToDeath::create(bGenericXMLBaseElement* elt){
	return(new bXMapSimplifyToDeath(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplifyToDeath::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplifyToDeath::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(simplify());
			break;
		case kExtProcessCallWithParams:
			return(simplify(_d));
			break;
		default:
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplifyToDeath::test(void* prm){
	return(SelectionIsMonoGender(_gapp)==kBaseKindPolyline);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplifyToDeath::simplify(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapSimplifyToDeathMessageID,
				GetSignature(this));
	(void)simplify(_d);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
static ivertices* rmv_at_dist(ivertices* vxs, double d){
//	_bTrace_("rmv_at_dist",true);
ivertices* vxo=NULL;
	ivs2ivs(vxs,&vxo);
	if(vxo==NULL){
		return(NULL);
	}
//double dp,dn;
	for(int i=vxo->nv-2;i>0;i-=2){
//		_tm_("i->i+1="+ivx2_dist(&vxo->vx.vx2[i],&vxo->vx.vx2[i+1]));
//		_tm_("i->i-1="+ivx2_dist(&vxo->vx.vx2[i],&vxo->vx.vx2[i-1]));
		if(	(ivx2_dist(&vxo->vx.vx2[i],&vxo->vx.vx2[i+1])<d)	&&
			(ivx2_dist(&vxo->vx.vx2[i],&vxo->vx.vx2[i-1])<d)	){
//			_tm_("ivx_remove(vxo,"+i+")");
			vxo=ivx_remove(vxo,i);
		}
	}
	return(vxo);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSimplifyToDeath::simplify(double d){
ivertices			*vxo,*vxs;
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));

	//vxo=ivs_new(_2D_VX,2,0);
	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&vxs);
		vxo=rmv_at_dist(vxs,d);
		if(vxo==NULL){
			continue;
		}
		//vxo->vx.vx2[0]=vxs->vx.vx2[0];
		//vxo->vx.vx2[1]=vxs->vx.vx2[vxs->nv-1];
		o->setVertices(vxo);
		ivs_free(vxo);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	//ivs_free(vxo);
	return(true);
}
