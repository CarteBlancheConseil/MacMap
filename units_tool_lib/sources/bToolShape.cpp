//----------------------------------------------------------------------------
// File : bToolShape.cpp
// Project : MacMap
// Purpose : C++ source file : Rectangles and ovals objects creation tool class
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
// 18/06/2007 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolShape.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bTrace.h>
#include <std_ext/bXMapStdIntf.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolShape	::bToolShape(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdToolGeom(elt,gapp,bndl){
	setclassname("shapetool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolShape::~bToolShape(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolShape::create(bGenericXMLBaseElement* elt){
	return(new bToolShape(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::open(int* flags){
	set_use_trace(true);
    _ctr=true;
    _opn=true;
    _cm=0;
	bStdToolGeom::open(flags);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::clic(CGPoint pt, int count){
	bStdToolGeom::clic(pt,count);
	_tp=(bGenericType*)CurType(_gapp);
	if(_tp==NULL){
		return;
	}
	if(	(_tp->kind()==kBaseKindPoint)	||
		(_tp->kind()==kBaseKindText)	||
		(_tp->kind()==kBaseNoKind)		){
		_tp=NULL;
		return;
	}
	if(count>1){
i2dvertex		vx;
		if(get_mnu_cmd()==kShapeRect){
makerect_prm	prm;
			prm.justo=_ctr?1:2;
			get_clic(&vx);
			Convert(&prm.o,&vx);
			prm.h=_cm?10:100;
			prm.w=_cm?10:100;
			prm.tp=_tp;
			prm.sui=_cm;
			_gapp->xmapMgr()->edit(NULL,'xMkR',&prm);
		}
		else{
makepoly_prm	prm;
			prm.justo=_ctr?1:2;
			get_clic(&vx);
			Convert(&prm.o,&vx);
			prm.nb=180;
			prm.sz=_cm?10:100;
			prm.tp=_tp;
			prm.sui=_cm;
			_gapp->xmapMgr()->edit(NULL,'xMkP',&prm);
		}
		return;
	}
	set_use_drag(true);
//	set_use_track(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::end_clic(){
	bStdToolGeom::end_clic();
	if(!get_use_drag()){
		return;
	}
	if(!get_active()){
		return;
	}
	set_use_drag(false);
	set_use_track(false);

CGPoint	pa,pb;
	get_clic(&pa);
	get_cur(&pb);
CGFloat	d=CGPointsDist(&pa,&pb);
	if(d<=sqrt(2)){
		return;
	}
	
	clearTempPathContext(true);
	validTempPathContext();
	if(get_mnu_cmd()==kShapeRect){
		create_rect();
	}
	else{
		create_circle();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	if((get_active())&&(get_on_drag())){
		if(get_mnu_cmd()==kShapeRect){
			if(_ctr){
				draw_c_rect();
			}
			else{
				draw_rect();
			}
		}
		else{
			if(_ctr){
				draw_c_circle();
			}
			else{
				draw_circle();
			}
		}
	}
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::drag(CGPoint pt){
	bStdToolGeom::drag(pt);
	if(!get_active()){
		return;
	}
	report();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolShape::edit_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
ControlRef	c;
ControlID	cid={kShapeEditSign,kShapeCenterID};

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				GetControlByID(wd,&cid,&c);
				_ctr=GetControl32BitValue(c);
				cid.id=kShapeOpenFicheID;
				GetControlByID(wd,&cid,&c);
				_opn=GetControl32BitValue(c);
				cid.id=kShapeCMMeasID;
				GetControlByID(wd,&cid,&c);
				_cm=GetControl32BitValue(c);
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
// -----------
void bToolShape::edit_init(WindowRef wd){
ControlRef	c;
ControlID	cid={kShapeEditSign,kShapeCenterID};

	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_ctr);
	cid.id=kShapeOpenFicheID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_opn);
	cid.id=kShapeCMMeasID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_cm);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::create_rect(){
	if(_tp==NULL){
		return;
	}
ivertices*	vxs=ivs_new(_2D_VX,5,0);
	if(!vxs){
		return;
	}
i2dvertex	vxa,vxb;
	get_clic(&vxa);
	get_cur(&vxb);
	
int			b;
	if(vxa.h>vxb.h){
		b=vxa.h;
		vxa.h=vxb.h;
		vxb.h=b;
	}
	if(vxa.v>vxb.v){
		b=vxa.v;
		vxa.v=vxb.v;
		vxb.v=b;
	}
	if(_ctr){
i2dvertex	vxc=vxa;		
int			dh=vxb.h-vxa.h;
int			dv=vxb.v-vxa.v;
		vxa.h=vxc.h-dh;
		vxa.v=vxc.v-dv;
		vxb.h=vxc.h+dh;
		vxb.v=vxc.v+dv;
	}
	
	vxs->vx.vx2[0].h=vxa.h;
	vxs->vx.vx2[0].v=vxa.v;
	vxs->vx.vx2[1].h=vxb.h;
	vxs->vx.vx2[1].v=vxa.v;
	vxs->vx.vx2[2].h=vxb.h;
	vxs->vx.vx2[2].v=vxb.v;
	vxs->vx.vx2[3].h=vxa.h;
	vxs->vx.vx2[3].v=vxb.v;
	vxs->vx.vx2[4]=vxs->vx.vx2[0];
	
bEventLog	log(_gapp,
				getbundle(),
				kShapeMessageID,
				GetSignature(this));

bGenericGeoElement*	o=NULL;
	if(!_tp->new_object(&o)){
		return;
	}
	if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),o)){
		NSBeep();
	}	
	if(!o->setVertices(vxs)){
		ivs_free(vxs);
		if(_tp->kill_object(o)){
		}
		return;
	}
	ivs_free(vxs);

	log.close();
		
	_gapp->selMgr()->flush();
	o->setselect(true,true);
	if(_opn){
		SendCommandToApplication('fich');
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::create_circle(){
	if(_tp==NULL){
		return;
	}
i2dvertex	vxa,vxb;
	get_clic(&vxa);
	get_cur(&vxb);

	if(!_ctr){
i2dvertex	vxc;
int			b;
		if(vxa.h>vxb.h){
			b=vxa.h;
			vxa.h=vxb.h;
			vxb.h=b;
		}
		if(vxa.v>vxb.v){
			b=vxa.v;
			vxa.v=vxb.v;
			vxb.v=b;
		}
		vxc.h=round((vxb.h+vxa.h)/2.0);
		vxc.v=round((vxb.v+vxa.v)/2.0);
		vxa=vxc;
	}
	
double		d=ivx2_dist(&vxa,&vxb);
double		l=(2.0*M_PI)*d;
int			n=round(l/5.0);
	if(n<16){
		n=16;
	}
	else if(n>360){
		n=360;
	}
ivertices*	vxs=ivs_new(_2D_VX,n+1,0);
	if(!vxs){
		return;
	}

double		ac=(2.0*M_PI)/(double)n;
double		na=0;

	for(int i=0;i<n;i++){
		ivx_proj_at_dist(&vxa,&vxs->vx.vx2[i],d,na);
		na+=ac;
	}
	vxs->vx.vx2[vxs->nv-1]=vxs->vx.vx2[0];

bEventLog	log(_gapp,
				getbundle(),
				kShapeMessageID,
				GetSignature(this));

bGenericGeoElement*	o=NULL;
	if(!_tp->new_object(&o)){
		return;
	}
	if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),o)){
		NSBeep();
	}	
	if(!o->setVertices(vxs)){
		ivs_free(vxs);
		if(_tp->kill_object(o)){
		}
		return;
	}
	ivs_free(vxs);

	log.close();
	
	_gapp->selMgr()->flush();
	o->setselect(true,true);
	if(_opn){
		SendCommandToApplication('fich');
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolShape::report(){
double			dx,dy;
bGenericUnit*	u=_gapp->distMgr()->get();
char			str[256],lib[64];
int				dg=_gapp->document()->dist_pref_digits();
	
	if(_cm==0){
i2dvertex		vxa,vxb;
		get_clic(&vxb);
		get_cur(&vxa);
		if(get_mnu_cmd()==kShapeRect){
			dx=Measure_i2d(_gapp,fabs(vxb.h-vxa.h));
			dy=Measure_i2d(_gapp,fabs(vxb.v-vxa.v));
		}
		else{
			dx=dy=Measure_i2d(_gapp,ivx2_dist(&vxa,&vxb));
		}
		if(_ctr){
			dx*=2.0;
			dy*=2.0;
		}
		u->short_name(lib);
	}
	else{
CGPoint		pa,pb;
		get_clic(&pb);
		get_cur(&pa);
		if(get_mnu_cmd()==kShapeRect){
			dx=fabs(pb.x-pa.x);
			dy=fabs(pb.y-pa.y);
		}
		else{
			dx=dy=CGPointsDist(&pa,&pb);
		}
		if(_ctr){
			dx*=2.0;
			dy*=2.0;
		}
		if(_cm==1){
			dx/=(72.0/2.54);
			dy/=(72.0/2.54);
			strcpy(lib,"cm");
			dg=2;
		}
		else{
			strcpy(lib,"px");
			dg=0;
		}
	}
	
	sprintf(str,"dx=%.*f %s, dy=%.*f %s",dg,dx,lib,dg,dy,lib);
	WriteMessage(_gapp,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolShape::load(){
bGenericXMLBaseElement* groot=bStdToolGeom::load();
bGenericXMLBaseElement* elt=NULL;
	if(!groot){
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		return(NULL);
	}
char					val[_values_length_max_];
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_ctr=atoi(val);
		elt=_gapp->classMgr()->NthElement(root,2,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_opn=atoi(val);
		elt=_gapp->classMgr()->NthElement(root,1,"int");
		if(!elt){
			_cm=1;
			break;
		}
		elt->getvalue(val);
		_cm=atoi(val);
		break;
	}
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolShape::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	add_idesc(arr,2,"bool",_ctr);
	add_idesc(arr,2,"bool",_opn);
	add_idesc(arr,2,"int",_cm);
	return(bStdToolGeom::make(arr));
}
