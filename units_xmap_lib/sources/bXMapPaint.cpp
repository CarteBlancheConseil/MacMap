//----------------------------------------------------------------------------
// File : bXMapPaint.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, paint objects (i.e. fill color field) 
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

#include "bXMapPaint.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bStdWait.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPaint	::bXMapPaint(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("paint");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPaint::~bXMapPaint(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPaint::create(bGenericXMLBaseElement* elt){
	return(new bXMapPaint(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapPaint::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm=1;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaint::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(paint());
			break;
		case kExtProcessCallWithParams:{
paint_prm* p=(paint_prm*)prm;
			return(paint(*p));
			}break;
		case kExtProcessCallWithXMLTree:{
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				paint(atoi(val));
			}break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaint::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaint::wd_event(EventRef evt, WindowRef wd){
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
void bXMapPaint::wd_init(WindowRef wd){
ControlRef c=get_control(kXMapPaintSignature,kXMapPaintPopColorID);	
	SetControl32BitValue(c,_prm);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaint::paint(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapPaintMessageID,
				GetSignature(this));
	_gapp->layersMgr()->SetObjInvalidation(false);
	(void)paint(_prm);
	_gapp->layersMgr()->SetObjInvalidation(true);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaint::paint(int c){
//_bTrace_("bXMapPaint::paint",true);
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
bArray	sel(*(_gapp->selMgr()->elements()));
//_tm_("color=%d",c);
	message_string(kMsgProgress,msg,1);
bProgressWait wt("",msg,true,true,sel.count());
bGenericGeoElement*	o;
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
//_te_("user break");
			break;
		}
		sel.get(i,&o);
		o->setColor(c);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapPaint::puts(){
ControlRef	c=get_control(kXMapPaintSignature,kXMapPaintPopColorID);
	_prm=GetControl32BitValue(c);
}
