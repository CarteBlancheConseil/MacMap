//----------------------------------------------------------------------------
// File : bXMapLineUp.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, order selected points into columns and lines
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
// 16/11/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapLineUp.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------

typedef int (*insidePtr)	(i2dvertex*,ivertices*,int);

// ---------------------------------------------------------------------------
// Constructeur
// ------------
static int dummy_ivx_in_ivs(i2dvertex* vx, ivertices* vxs, int limit){
	return(1);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapLineUp	::bXMapLineUp(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("lineup");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapLineUp::~bXMapLineUp(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapLineUp::create(bGenericXMLBaseElement* elt){
	return(new bXMapLineUp(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapLineUp::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_kind=kXMapLineUpKindLine;
	_nb=0;
	_hd=1;
	_vd=1;
	_use_surf=false;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLineUp::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			make();
			break;
		case kExtProcessCallWithParams:
			break;
		default:
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLineUp::test(void* prm){
	return(SelectionIsMonoGender(_gapp)==kBaseKindPoint);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLineUp::wd_event(EventRef evt, WindowRef wd){
bool			b=true;
HICommand		cmd;
UInt32			clss=GetEventClass(evt);
ControlRef		c;
char			val[256];

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectID);
				GetTextControlValue(c,val,sizeof(val)-1);
				_nb=atoi(val);
				
				c=get_control(kXMapLineUpSignature,kXMapLineUpPopKindID);
				_kind=GetControl32BitValue(c);
				
				c=get_control(kXMapLineUpSignature,kXMapLineUpHDistID);
				GetTextControlValue(c,val,sizeof(val)-1);
				_hd=matof(val);
				
				c=get_control(kXMapLineUpSignature,kXMapLineUpVDistID);
				GetTextControlValue(c,val,sizeof(val)-1);
				_vd=matof(val);
				
				c=get_control(kXMapLineUpSignature,kXMapLineUpUseSurfID);
				_use_surf=GetControl32BitValue(c);
				break;
			case kHICommandCancel:
				break;
			case kXMapLineUpUseSurfCmd:
				c=get_control(kXMapLineUpSignature,kXMapLineUpUseSurfID);
				if(GetControl32BitValue(c)>0){
					c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectID);
					DisableControl(c);
					c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectLegID);
					DisableControl(c);
				}
				else{
					c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectID);
					EnableControl(c);
					c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectLegID);
					EnableControl(c);				
				}
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
void bXMapLineUp::wd_init(WindowRef wd){
ControlRef	c;
char		val[256];

	if(_nb==0){
		_nb=round(sqrt(_gapp->selMgr()->count()));
	}
	
	c=get_control(kXMapLineUpSignature,kXMapLineUpPopKindID);
	SetControl32BitValue(c,_kind);

	c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectID);
	SetTextControlFilter(c,uint_filter);
	sprintf(val,"%d",_nb);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapLineUpSignature,kXMapLineUpHDistID);
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_hd);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapLineUpSignature,kXMapLineUpVDistID);
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_vd);
	SetTextControlValue(c,val);
	
bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);
	c=get_control(kXMapLineUpSignature,kXMapLineUpHDistLegID);	
	SetTextControlValue(c,val);
	c=get_control(kXMapLineUpSignature,kXMapLineUpVDistLegID);	
	SetTextControlValue(c,val);
	
	c=get_control(kXMapLineUpSignature,kXMapLineUpUseSurfID);	
	if(_gapp->cntMgr()->count()==1){
bGenericGeoElement*	o;
		_gapp->cntMgr()->elements()->get(1,&o);
bGenericType*		tp=_gapp->typesMgr()->get(o->getType());
		if(tp->kind()==kBaseKindPolygon){
			SetControl32BitValue(c,_use_surf);
			if(_use_surf){
				c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectID);
				DisableControl(c);
				c=get_control(kXMapLineUpSignature,kXMapLineUpNbObjectLegID);
				DisableControl(c);
			}
			return;
		}
	}
	_use_surf=false;
	SetControl32BitValue(c,_use_surf);
	DisableControl(c);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapLineUp::make(){
bGenericGeoElement* o;
ivx_rect			vr,vrb,bnd;
bArray				arr(*_gapp->selMgr()->elements());
int					i,j,ko,kv;
int					imax,jmax,n=0;
insidePtr			inside;
ivertices			*ref,*vxs;
i2dvertex			p,lp;
bool				end=false;
int					hd=round(Measure_d2i(_gapp,_hd));
int					vd=round(Measure_d2i(_gapp,_vd));

	if(_use_surf){
		inside=ivx_in_ivs;
		_gapp->cntMgr()->elements()->get(1,&o);
		o->getBounds(&bnd);
		o->getVertices(&ref);
		imax=1+(bnd.bottom-bnd.top)/vd;
		jmax=1+(bnd.right-bnd.left)/hd;
	}
	else{
		inside=dummy_ivx_in_ivs;
		arr.get(1,&o);
		o->getBounds(&bnd);
		vr=bnd;
		o->getVertices(&vxs);
		n+=vxs->nv;
		for(i=2;i<arr.count();i++){
			arr.get(i,&o);
			o->getBounds(&vrb);
			ivr_union(&vrb,&vr,&bnd);
			vr=bnd;
			o->getVertices(&vxs);
			n+=vxs->nv;
		}
		ref=NULL;
		if(_kind==kXMapLineUpKindLine){
			imax=ceil((double)n/_nb);
			jmax=_nb;
		}
		else{
			imax=_nb;
			jmax=ceil((double)n/_nb);	
		}
	}
		
	_gapp->layersMgr()->SetObjInvalidation(false);	
	
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
char			ttl[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapLineUpMessageID,ttl,getbundle(),0);
	message_string(kMsgProgress,msg,1);
bProgressWait	wt(ttl,msg,true,true,arr.count());	

bEventLog	log(_gapp,
				getbundle(),
				kXMapLineUpMessageID,
				GetSignature(this));
	
	ko=1;
	kv=0;
	arr.get(ko,&o);
	o->getVertices(&vxs);
	lp.h=bnd.left;
	lp.v=bnd.top;
	for(i=0;i<imax;i++){
		for(j=0;j<jmax;j++){
			p.h=bnd.left+j*hd;
			p.v=bnd.top+i*vd;
			if(!inside(&p,ref,1)){
				continue;
			}
			lp=p;
			vxs->vx.vx2[kv]=p;
			kv++;
			if(kv==vxs->nv){
				kv=0;
				ko++;
				o->setVertices(vxs);
				if(!wt.set_progress(ko)){
					end=true;
					break;
				}
				if(!arr.get(ko,&o)){
					end=true;
					break;
				}
				o->getVertices(&vxs);
			}
		}
		if(end){
			break;
		}
	}
	
	if(!end){
		if(kv<vxs->nv){
			for(i=kv;i<vxs->nv;i++){
				vxs->vx.vx2[kv]=lp;
			}
			o->setVertices(vxs);
			ko++;
		}
		for(i=ko;i<=arr.count();i++){
			if(!wt.set_progress(i)){
				break;
			}	
			arr.get(i,&o);
			o->getVertices(&vxs);
			for(j=0;j<vxs->nv;j++){
				vxs->vx.vx2[j]=lp;
			}
			o->setVertices(vxs);
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(true);	
	log.close();
}

