//----------------------------------------------------------------------------
// File : bXMapMakePoly.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, create new polygon
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

#include "bXMapMakePoly.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_Utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapMakePoly	::bXMapMakePoly(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("makepoly");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMakePoly::~bXMapMakePoly(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapMakePoly::create(bGenericXMLBaseElement* elt){
	return(new bXMapMakePoly(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapMakePoly::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm.justo=0;
	_prm.o.x=0;
	_prm.o.y=0;
	_prm.nb=5;
	_prm.sz=100;
	_prm.tp=NULL;
	_prm.sui=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMakePoly::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(make());
			break;
		case kExtProcessCallWithParams:{
makepoly_prm* p=(makepoly_prm*)prm;
				return(make(p->tp,&p->o,p->sz,p->nb,p->sui,p->justo));
			}
			break;
		case kExtProcessCallWithXMLTree:{
makepoly_prm			p;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				p.justo=0;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				if(!p.tp){
					return(false);
				}
				elt=getelement(2);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.o.x=atof(val);
				
				elt=getelement(3);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.o.y=atof(val);
				
				elt=getelement(4);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.sz=atof(val);
				
				elt=getelement(5);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.nb=atoi(val);

				elt=getelement(6);
				if(elt){
					elt->getvalue(val);
					p.sui=atoi(val);
				}
				else{
					p.sui=0;
				}
			
				return(make(p.tp,&p.o,p.sz,p.nb,p.sui,p.justo));
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
bool bXMapMakePoly::test(void* prm){
	_prm.tp=(bGenericType*)CurType(_gapp);
	if(!_prm.tp){
		return(false);
	}
	if(	(_prm.tp->kind()!=kBaseKindPolyline)	&&
		(_prm.tp->kind()!=kBaseKindPolygon)		){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMakePoly::edit(void* prm){
makepoly_prm p=_prm;
	if(prm){
		_prm=(*(makepoly_prm*)prm);			
	}
	bStdXMapWithModalIntf::edit(NULL);
	if(prm){
		_prm=p;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMakePoly::wd_event(EventRef evt, WindowRef wd){
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
void bXMapMakePoly::wd_init(WindowRef wd){
ControlRef	c;
char		val[256];

	if(_prm.justo){
		
	}
	else if(_gapp->cntMgr()->count()>0){
bGenericGeoElement*	o;
ivertices*			vxs;
		_gapp->cntMgr()->elements()->get(1,&o);
		o->getVertices(&vxs);
		Convert(&_prm.o,&vxs->vx.vx2[0]);
	}
	else{
i2dvertex	vx;
		//_gapp->mapwd()->screen_center(&vx);
		_gapp->mapIntf()->screenCenter(&vx);
		Convert(&_prm.o,&vx);
	}
	
	c=get_control(kXMapMakePolySignature,kXMapMakePolyXID);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.o.x);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

	c=get_control(kXMapMakePolySignature,kXMapMakePolyYID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.o.y);
	SetTextControlValue(c,val);

	c=get_control(kXMapMakePolySignature,kXMapMakePolyRayID);
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.sz);
	SetTextControlValue(c,val);

	c=get_control(kXMapMakePolySignature,kXMapMakePolyNbID);	
	SetTextControlFilter(c,uint_filter);
	sprintf(val,"%d",_prm.nb);
	SetTextControlValue(c,val);

bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);
	for(int i=kXMapMakePolyXUnitID;i<=kXMapMakePolyYUnitID;i++){
		c=get_control(kXMapMakePolySignature,i);	
		SetTextControlValue(c,val);
	}
	
	c=get_control(kXMapMakePolySignature,kXMapMakePolyRayUnitID);	
	if(_prm.sui==1){
		sprintf(val,"cm*");
	}
	SetTextControlValue(c,val);	
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMakePoly::make(){
	if(!_prm.tp){
		return(true);
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapMakePolyMessageID,
				GetSignature(this));
	(void)make(_prm.tp,&_prm.o,_prm.sz,_prm.nb,_prm.sui,_prm.justo);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMakePoly::make(bGenericType* tp, d2dvertex* o, double r, int n, int sui, int justo){
_bTrace_("bXMapMakePoly::make",false);
double		a,a0;
	
	if(n&0x00000001){
		a0=M_PI/2.0;
	}
	else{
		a0=M_PI/2.0+M_PI/n;
	}

	if(sui==1){
bGenericUnit*	scl=_gapp->scaleMgr()->get();
double			c=0.01/GetRawPrecisionCoef(_gapp)*scl->coef();
		c*=GetPrecisionCoef(_gapp);
		r*=c;
	}
	
	if(justo==2){
		o->x+=r;
		o->y-=r;
	}
	
ivertices*	vxs=ivs_new(_2D_VX,0,0);
i2dvertex	ivx;
d2dvertex	dvx;

	for(int i=0;i<n;i++){
		a=a0-i*2.0*M_PI/(double)n;
		dvx.x=o->x+r*cos(a);
		dvx.y=o->y+r*sin(a);
		_gapp->locConverter()->convert(&ivx,&dvx);
		if((vxs->nv>0)&&(eq_ivx2(&vxs->vx.vx2[vxs->nv-1],&ivx))){
			continue;
		}
		vxs=ivx2_add(vxs,&ivx);
		if(!vxs){
			return(false);
		}
	}
	if(!eq_ivx2(&vxs->vx.vx2[0],&vxs->vx.vx2[vxs->nv-1])){
		ivx=vxs->vx.vx2[0];
		vxs=ivx2_add(vxs,&ivx);
	}
	if(!vxs){
		return(false);
	}
	if(vxs->nv<4){
		ivs_free(vxs);
		return(false);
	}
bGenericGeoElement*	go;
	if(!tp->new_object(&go)){
_te_("new_object");
		ivs_free(vxs);
		return(false);
	}
	if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),go)){
		NSBeep();
	}
	if(!go->setVertices(vxs)){
_te_("setVertices");
		tp->kill_object(go);
		ivs_free(vxs);
		return(false);
	}
	ivs_free(vxs);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapMakePoly::puts(){
ControlRef	c;
char		val[256];
	
	c=get_control(kXMapMakePolySignature,kXMapMakePolyXID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.o.x=matof(val);
	c=get_control(kXMapMakePolySignature,kXMapMakePolyYID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.o.y=matof(val);
	c=get_control(kXMapMakePolySignature,kXMapMakePolyRayID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.sz=matof(val);
	c=get_control(kXMapMakePolySignature,kXMapMakePolyNbID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.nb=atoi(val);
}
