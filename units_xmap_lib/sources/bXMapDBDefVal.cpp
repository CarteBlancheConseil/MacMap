//----------------------------------------------------------------------------
// File : bXMapDBDefVal.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, sets standard fields default values
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
// 16/03/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapDBDefVal.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDBDefVal::bXMapDBDefVal(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("dbdefval");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDBDefVal::~bXMapDBDefVal(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDBDefVal::create(bGenericXMLBaseElement* elt){
	return(new bXMapDBDefVal(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDBDefVal::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tp=NULL;
	_fidx=kOBJ_Name_;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBDefVal::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBDefVal::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBDefVal::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss;	
		
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				_gapp->eventMgr()->close_log();
				b=true;
				break;
				
			case kXMapDBDefValPopupTypeCmd:
				type_action();
				break;
			
			case kXMapDBDefValPopupFieldCmd:
				populate_field();
				break;
	
			case kXMapDBDefValModifyCmd:
				modify();
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
void bXMapDBDefVal::wd_init(WindowRef wd){
	_tp=NULL;
	_fidx=kOBJ_Name_;
	
	populate_types();
	populate_field();	
	
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapDBDefValMessageID,msg,getbundle(),1);
	_gapp->eventMgr()->init_log(kXMapDBDefValSignature,msg);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBDefVal::populate_types(){
ControlRef	c=get_control(kXMapDBDefValSignature,kXMapDBDefValPopupTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,1);	
	_tp=_gapp->typesMgr()->get(1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBDefVal::populate_field(){
ControlRef	c=get_control(kXMapDBDefValSignature,kXMapDBDefValPopupFieldID);
	_fidx=kOBJ_Name_+(GetControl32BitValue(c)-1);
char		def[512]="";
int			n=_tp->fields()->count_constraints(_fidx);
	if(n>0){
int			idx=0;

		_tp->fields()->get_default_value(_fidx,def);
		idx=_tp->fields()->get_constraint_index(_fidx,def);

		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValuePopupID);
		ClearPopupControl(c,1);
		PopulatePopupControlWithConstraints(_tp,_fidx,c,idx);
		ShowControl(c);
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValueTextID);
		HideControl(c);
	}
	else{
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValueTextID);
		_tp->fields()->get_default_value(_fidx,def);
		SetTextControlValue(c,def);
		ShowControl(c);
		
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValuePopupID);
		HideControl(c);	
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBDefVal::type_action(){
ControlRef		c=get_control(kXMapDBDefValSignature,kXMapDBDefValPopupTypeID);
int				t=GetControl32BitValue(c);
bGenericType*	tp=_gapp->typesMgr()->get(t);

	if(tp==NULL){
		return;
	}
	if(tp!=_tp){
		_tp=tp;
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValPopupFieldID);
		SetControl32BitValue(c,1);
		populate_field();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBDefVal::modify(){
_bTrace_("bXMapDBDefVal::modify",true);
ControlRef	c;
int			n=_tp->fields()->count_constraints(_fidx);
char		def[512];
	if(n>0){
int		idx=0;
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValuePopupID);
		idx=GetControl32BitValue(c);
		_tp->fields()->get_constraint(_fidx,idx,def);
		_tp->fields()->set_default_value(_fidx,def);
	}
	else{
		c=get_control(kXMapDBDefValSignature,kXMapDBDefValValueTextID);
		GetTextControlValue(c,def,sizeof(def)-1);
		_tp->fields()->set_default_value(_fidx,def);
//int		ck,d;
//char	buf[512];
//		_tp->fields()->get_decs(_fidx,&d);
//		ck=_tp->fields()->get_constraints_kind(_fidx);
//		if(!charToX(ck,d,def,buf)){
//			_tp->fields()->set_default_value(_fidx,buf);
//		}
	}
}
