//----------------------------------------------------------------------------
// File : bXMapRotation.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, rotate selected geometries with angle
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
// 07/11/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapRotation.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapRotation	::bXMapRotation(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("rotation");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapRotation::~bXMapRotation(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapRotation::create(bGenericXMLBaseElement* elt){
	return(new bXMapRotation(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapRotation::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm.x=0;
	_prm.y=0;
	_dir=kXMapRotationRetro;
	_kind=kXMapRotationOnSelf;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRotation::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(rotate());
			break;
		case kExtProcessCallWithParams:{
rotation_prm* p=(rotation_prm*)prm;
			return(rotate(p->x,p->y,p->rad));
			}break;
		case kExtProcessCallWithXMLTree:{
rotation_prm			p;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.x=atof(val);

				elt=getelement(2);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.y=atof(val);

				elt=getelement(3);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.rad=deg2rad(atof(val));
					
				_dir=kXMapRotationDir;
				elt=getelement(4);
				if(elt){
					elt->getvalue(val);
					_dir=atoi(val);
				}

				_kind=kXMapRotationOnCoord;
				elt=getelement(5);
				if(elt){
					elt->getvalue(val);
					_kind=atoi(val);
				}

				return(rotate(p.x,p.y,p.rad));
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
bool bXMapRotation::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRotation::wd_event(EventRef evt, WindowRef wd){
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
			case kXMapRotationPopKindCmd:
				showhide();
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
void bXMapRotation::wd_init(WindowRef wd){
ControlRef	c;
char		val[256];

	c=get_control(kXMapRotationSignature,kXMapRotationDegID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.2f",rad2deg(_prm.rad));
	SetTextControlValue(c,val);

	c=get_control(kXMapRotationSignature,kXMapRotationMinID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"0");
	SetTextControlValue(c,val);

	c=get_control(kXMapRotationSignature,kXMapRotationPopDirID);	
	SetControl32BitValue(c,_dir);

	c=get_control(kXMapRotationSignature,kXMapRotationPopKindID);	
	_kind=(_gapp->cntMgr()->count()>0)?kXMapRotationOnCoord:kXMapRotationOnSelf;
	SetControl32BitValue(c,_kind);
	if(_kind==kXMapRotationOnCoord){
bGenericGeoElement*	o;
ivertices*			vxs;
d2dvertex			dvx;
		_gapp->cntMgr()->elements()->get(1,&o);
		o->getVertices(&vxs);
		_gapp->locConverter()->convert(&dvx,&vxs->vx.vx2[0]);
		_prm.x=dvx.x;
		_prm.y=dvx.y;
	}
/*	else{
i2dvertex			ivx;
d2dvertex			dvx;
		center(&ivx);
		_gapp->locConverter()->convert(&dvx,&ivx);
		_prm.x=dvx.x;
		_prm.y=dvx.y;
	}*/
	
	c=get_control(kXMapRotationSignature,kXMapRotationXID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.x);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

	c=get_control(kXMapRotationSignature,kXMapRotationYID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.y);
	SetTextControlValue(c,val);


bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);
	c=get_control(kXMapRotationSignature,kXMapRotationUnitXID);
	SetTextControlValue(c,val);
	c=get_control(kXMapRotationSignature,kXMapRotationUnitYID);
	SetTextControlValue(c,val);
	
	showhide();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRotation::rotate(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapRotationMessageID,
				GetSignature(this));
	(void)rotate(_prm.x,_prm.y,_prm.rad);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRotation::rotate(double x, double y, double rad){
d2dvertex			dvx={x,y};
i2dvertex			ivx;
ivertices*			vxs;
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));

	_gapp->layersMgr()->SetObjInvalidation(false);	
	_gapp->locConverter()->convert(&ivx,&dvx);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->inval(kOBJ_Vertices_);
		o->getVertices(&vxs);
		ivs_rotate_anchor(vxs,rad,&ivx);
		o->setVertices(vxs);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapRotation::puts(){
ControlRef		c;
char			val[256];
int				dir;

	c=get_control(kXMapRotationSignature,kXMapRotationPopDirID);	
	dir=GetControl32BitValue(c);

	c=get_control(kXMapRotationSignature,kXMapRotationPopKindID);
	_kind=GetControl32BitValue(c);
	if(_kind==kXMapRotationOnCoord){
		c=get_control(kXMapRotationSignature,kXMapRotationXID);
		GetTextControlValue(c,val,sizeof(val));
		_prm.x=matof(val);
		c=get_control(kXMapRotationSignature,kXMapRotationYID);
		GetTextControlValue(c,val,sizeof(val));
		_prm.y=matof(val);
	}
	else{
i2dvertex			ivx;
d2dvertex			dvx;
		center(&ivx);
		_gapp->locConverter()->convert(&dvx,&ivx);
		_prm.x=dvx.x;
		_prm.y=dvx.y;
	}
	
	c=get_control(kXMapRotationSignature,kXMapRotationDegID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.rad=deg2rad(matof(val));
	
	c=get_control(kXMapRotationSignature,kXMapRotationMinID);
	GetTextControlValue(c,val,sizeof(val));
double	min=deg2rad(matof(val))/60.0;
	
	if(_prm.rad<0){
		_prm.rad=-(-_prm.rad+min);
	}
	else{
		_prm.rad=-(-_prm.rad+min);
	}
	if(dir==kXMapRotationDir){
		_prm.rad=-_prm.rad;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapRotation::center(i2dvertex* c){
bGenericGeoElement*	o;
ivx_rect			vr,bounds;
	_gapp->selMgr()->elements()->get(1,&o);
	o->getBounds(&bounds);
	for(int i=2;i<=_gapp->selMgr()->count();i++){
		_gapp->selMgr()->elements()->get(i,&o);
		o->getBounds(&vr);
		ivr_union(&vr,&bounds,&bounds);
	}
	ivr_mid(&bounds,c);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapRotation::showhide(){
ControlRef	c=get_control(kXMapRotationSignature,kXMapRotationPopKindID);
int			k=GetControl32BitValue(c);
	
	for(int i=kXMapRotationXXID;i<=kXMapRotationUnitYID;i++){
		c=get_control(kXMapRotationSignature,i);
		if(k==kXMapRotationOnSelf){
			HideControl(c);
		}
		else{
			ShowControl(c);
		}
	}
}

