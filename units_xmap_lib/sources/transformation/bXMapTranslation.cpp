//----------------------------------------------------------------------------
// File : bXMapTranslation.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, translate selected geometries in x and y
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
// 19/10/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapTranslation.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapTranslation	::bXMapTranslation(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("translation");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapTranslation::~bXMapTranslation(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapTranslation::create(bGenericXMLBaseElement* elt){
	return(new bXMapTranslation(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTranslation::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm.x=0;
	_prm.y=0;
	_prm.kind=kXMapTranslationWithOffset;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			switch(_prm.kind){
				case kXMapTranslationWithOffset:
					return(translation_with_offset());
					break;
				case kXMapTranslationToCoord:
					return(translation_to_coord());
					break;
			}
			break;
		case kExtProcessCallWithParams:{
translation_prm* p=(translation_prm*)prm;
				switch(p->kind){
					case kXMapTranslationWithOffset:
						return(translation_with_offset(p->x,p->y));
						break;
					case kXMapTranslationToCoord:
						return(translation_to_coord(p->x,p->y));
						break;
				}
			}
			break;
		case kExtProcessCallWithXMLTree:{
translation_prm			p;
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
				p.kind=atoi(val);
					
				switch(p.kind){
					case kXMapTranslationWithOffset:
						return(translation_with_offset(p.x,p.y));
						break;
					case kXMapTranslationToCoord:
						return(translation_to_coord(p.x,p.y));
						break;
					}			
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
bool bXMapTranslation::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::wd_event(EventRef evt, WindowRef wd){
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
			case kXMapTranslationPopKindCmd:
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
void bXMapTranslation::wd_init(WindowRef wd){
ControlRef			c;
char				val[256];

	c=get_control(kXMapTranslationSignature,kXMapTranslationPopKindID);	
	SetControl32BitValue(c,_prm.kind);
	if((_prm.kind==kXMapTranslationToCoord)&&(_gapp->cntMgr()->count()>0)){
bGenericGeoElement*	o;
ivertices*			vxs;
d2dvertex			dvx;
		_gapp->cntMgr()->elements()->get(1,&o);
		o->getVertices(&vxs);
		_gapp->locConverter()->convert(&dvx,&vxs->vx.vx2[0]);
		_prm.x=dvx.x;
		_prm.y=dvx.y;
	}
	c=get_control(kXMapTranslationSignature,kXMapTranslationXID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.x);
	SetTextControlValue(c,val);	
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

	c=get_control(kXMapTranslationSignature,kXMapTranslationYID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.y);
	SetTextControlValue(c,val);	

bGenericUnit*		u=_gapp->distMgr()->get();
	c=get_control(kXMapTranslationSignature,kXMapTranslationUnitID);
	u->short_name(val);
	SetTextControlValue(c,val);	
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::translation_with_offset(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapTranslationMessageID,
				GetSignature(this));
	translation_with_offset(_prm.x,_prm.y);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::translation_with_offset(double dx, double dy){
d2dvertex			dvx={dx,dy};
i2dvertex			ivx;
ivertices*			vxs;
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));

	_gapp->locConverter()->convert(&ivx,&dvx);
	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->inval(kOBJ_Vertices_);
		o->getVertices(&vxs);
		ivs_move(vxs,ivx.h,ivx.v);
		o->setVertices(vxs);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::translation_to_coord(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapTranslationMessageID,
				GetSignature(this));
	(void)translation_to_coord(_prm.x,_prm.y);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTranslation::translation_to_coord(double dx, double dy){
d2dvertex			dvx={dx,dy};
i2dvertex			ivx;
ivertices*			vxs;
bGenericGeoElement*	o;
int					dh,dv;

	_gapp->locConverter()->convert(&ivx,&dvx);
	for(int i=1;i<=_gapp->selMgr()->count();i++){
		_gapp->selMgr()->elements()->get(i,&o);
		o->inval(kOBJ_Vertices_);
		o->getVertices(&vxs);
		dh=ivx.h-vxs->vx.vx2[0].h;
		dv=ivx.v-vxs->vx.vx2[0].v;
		ivs_move(vxs,dh,dv);
		o->setVertices(vxs);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTranslation::puts(){
ControlRef		c;
char			val[256];
Size			sz;
		
	c=get_control(kXMapTranslationSignature,kXMapTranslationXID);
	GetControlData(c,kControlEditTextPart,kControlEditTextTextTag,sizeof(val),val,&sz);
	val[sz]=0;
	_prm.x=matof(val);
	c=get_control(kXMapTranslationSignature,kXMapTranslationYID);
	GetControlData(c,kControlEditTextPart,kControlEditTextTextTag,sizeof(val),val,&sz);
	val[sz]=0;
	_prm.y=matof(val);
	c=get_control(kXMapTranslationSignature,kXMapTranslationPopKindID);
	_prm.kind=GetControl32BitValue(c);
}
