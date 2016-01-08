//----------------------------------------------------------------------------
// File : bXMapShake.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, apply random translation on points of selected geometries
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
// 08/11/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapShake.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapShake	::bXMapShake(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("shake");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapShake::~bXMapShake(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapShake::create(bGenericXMLBaseElement* elt){
	return(new bXMapShake(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShake::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShake::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(shake());
			break;
		case kExtProcessCallWithParams:{
shake_prm* p=(shake_prm*)prm;
			return(shake(*p));
			}break;
		case kExtProcessCallWithXMLTree:{
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				return(shake(atof(val)));
			}break;
		default:
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShake::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShake::wd_event(EventRef evt, WindowRef wd){
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
void bXMapShake::wd_init(WindowRef wd){
ControlRef			c;
char				val[256];

	c=get_control(kXMapShakeSignature,kXMapShakeDistID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

bGenericUnit*		u=_gapp->distMgr()->get();
	c=get_control(kXMapShakeSignature,kXMapShakeDistUnitID);
	u->short_name(val);
	SetTextControlValue(c,val);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShake::shake(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapShakeMessageID,
				GetSignature(this));
	(void)shake(_prm);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShake::shake(double d){
d2dvertex			dvx={d,d};
i2dvertex			ivx;
i2dvertex*			p;
ivertices*			vxs;
bGenericGeoElement*	o;
int					nv;
//bool				ring;
bArray				sel(*(_gapp->selMgr()->elements()));

	_gapp->locConverter()->convert(&ivx,&dvx);
	d=ivx.h;
	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->inval(kOBJ_Vertices_);
		o->getVertices(&vxs);
		for(int j=0;j<ivs_n_parts(vxs);j++){
			p=ivs2_part(vxs,j,&nv);
			/*ring=*/(void)ivx2_closed(p,nv);
			for(int k=0;k<nv;k++){
				p[k].h+=round((rand()-((double)LONG_MAX/2.0))/(double)LONG_MAX*d);
				p[k].v+=round((rand()-((double)LONG_MAX/2.0))/(double)LONG_MAX*d);
			}
		}
		o->setVertices(vxs);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShake::puts(){
ControlRef		c;
char			val[256];
		
	c=get_control(kXMapShakeSignature,kXMapShakeDistID);
	GetTextControlValue(c,val,sizeof(val));
	_prm=matof(val);
}
