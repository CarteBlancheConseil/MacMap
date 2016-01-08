//----------------------------------------------------------------------------
// File : bXMapBuffer.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, buffer creation
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

#include "bXMapBuffer.h"
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
bXMapBuffer::bXMapBuffer(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("buffer");
	_prm.d=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapBuffer::~bXMapBuffer(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapBuffer::create(bGenericXMLBaseElement* elt){
	return(new bXMapBuffer(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapBuffer::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			bufferize();
			break;
		case kExtProcessCallWithParams:{
buffer_prm		p=(*((buffer_prm*)prm));
				bufferize(p);
			}
			break;
		case kExtProcessCallWithXMLTree:{
buffer_prm				p;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
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
				p.d=atof(val);
				
				bufferize(p);
				return(true);
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
bool bXMapBuffer::test(void* prm){
	if(_gapp->selMgr()->count()==0){
		return(false);
	}
bGenericType*	tp=(bGenericType*)CurType(_gapp);
	if((tp!=NULL)&&(tp->kind()!=kBaseKindPolygon)){
		return(false);
	}
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->nb_sel()>0){
			if(	(tp->kind()==kBaseKindText)		||
				(tp->kind()==kBaseKindRaster)	||
				(tp->kind()==kBaseNoKind)		){
				return(false);
			}
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapBuffer::wd_event(EventRef evt, WindowRef wd){
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
void bXMapBuffer::wd_init(WindowRef wd){
ControlRef			c;
char				val[256];

	c=get_control(kXMapBufferSignature,kXMapBufferDistID);	
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.d);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

bGenericUnit*		u=_gapp->distMgr()->get();
	c=get_control(kXMapBufferSignature,kXMapBufferDistUnitID);
	u->short_name(val);
	SetTextControlValue(c,val);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapBuffer::bufferize(){
	_prm.tp=(bGenericType*)CurType(_gapp);
	if(!_prm.tp){
		return;
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapBufferMessageID,
				GetSignature(this));
	bufferize(_prm);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapBuffer::bufferize(buffer_prm prm){
ivertices			*vsa,*vsc;
bGenericGeoElement	*oa,*oc;
bArray				sel(*(_gapp->selMgr()->elements()));
int					i;
bGenericType*		otp;
double				d=Measure_d2i(_gapp,prm.d);

/*int					fa=tp->fields()->get_index("A_ID");
	if(fa==0){
		tp->fields()->add(_int,_kIntLen,0,"A_ID");
		fa=tp->fields()->get_index("A_ID");
		if(!fa){
			return;
		}
	}*/

	_gapp->layersMgr()->SetObjInvalidation(false);
	for(i=1;i<=sel.count();i++){
		sel.get(i,&oa);
		oa->getVertices(&vsa);
		otp=_gapp->typesMgr()->get(oa->getType());
		switch(otp->kind()){
			case kBaseKindPoint:
				vsc=ivs_bufferize_point(vsa,d);
				break;
			case kBaseKindPolyline:
				vsc=ivs_bufferize_line(vsa,d);
				break;
			case kBaseKindPolygon:
				vsc=ivs_bufferize_surf(vsa,d);
				break;
			default:
				vsc=NULL;
		}
		if(vsc){
			if(prm.tp->new_object(&oc)){
				if(!InitWithStyle(_gapp,_gapp->layersAccessCtx()->get_current(),oc)){
					NSBeep();
				}
				oc->setVertices(vsc);
				//oc->setValue(fa,oa->getID());
			}
			ivs_free(vsc);
		}
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapBuffer::puts(){
ControlRef		c;
char			val[256];
		
	c=get_control(kXMapBufferSignature,kXMapBufferDistID);
	GetTextControlValue(c,val,sizeof(val));
	_prm.d=matof(val);
}

