//----------------------------------------------------------------------------
// File : bXMapVarEditor.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, Var/Const editor
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
// 16/05/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapVarEditor.h"
#include <mox_intf/CGUtils.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bSelectSolver.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapVarEditor	::bXMapVarEditor(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("vareditor");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapVarEditor::~bXMapVarEditor(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapVarEditor::create(bGenericXMLBaseElement* elt){
	return(new bXMapVarEditor(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapVarEditor::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapVarEditor::close(){
	bStdXMapWithModalIntf::close();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapVarEditor::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapVarEditor::test(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapVarEditor::edit(void* prm){
	_new=true;
	_name[0]=0;
	_value[0]=0;
	_fknd=0;
	_fsgn=0;
	if(prm){
		if(!parse((bGenericXMLBaseElement*)prm)){
			NSBeep();
			return(true);
		}
	}
	return(bStdXMapWithModalIntf::edit(prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapVarEditor::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss=GetEventClass(evt);	
					
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				if(!save_action()){
					return(false);
				}
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
void bXMapVarEditor::wd_init(WindowRef wd){
ControlRef	c;
	
	if(!_new){
		c=get_control(kXMapVarEditorSignature,kXMapVarEditorPopupKindID);
		switch(_fknd){
			case _char:
				SetControl32BitValue(c,1);
				break;
			case _bool:
				SetControl32BitValue(c,2);
				break;
			case _int:
				SetControl32BitValue(c,3);
				break;
			case _double:
				SetControl32BitValue(c,4);
				break;
			case _date:
				SetControl32BitValue(c,5);
				break;
			case _time:
				SetControl32BitValue(c,6);
				break;
		}
		c=get_control(kXMapVarEditorSignature,kXMapVarEditorNameID);
		SetTextControlValue(c,_name);
		c=get_control(kXMapVarEditorSignature,kXMapVarEditorValueID);
		SetTextControlValue(c,_value);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapVarEditor::save_action(){
ControlRef	c=get_control(kXMapVarEditorSignature,kXMapVarEditorNameID);
	GetTextControlValue(c,_name,FILENAME_MAX);
	if(strlen(_name)==0){
		NSBeep();
		return(false);
	}
	
	c=get_control(kXMapVarEditorSignature,kXMapVarEditorValueID);
	GetTextControlValue(c,_value,_values_length_max_);
	if(strlen(_value)==0){
		NSBeep();
		return(false);
	}

	if(_gapp->varMgr()->find(_name)){
		if(_new){
			NSBeep();
			return(false);
		}
	}
	
	if(_new){
		_fsgn=UniqueSignature(_gapp);
	}

	c=get_control(kXMapVarEditorSignature,kXMapVarEditorPopupKindID);
	switch(GetControl32BitValue(c)){
		case 1:
			_fknd=_char;
			break;
		case 2:
			_fknd=_bool;
			break;
		case 3:
			_fknd=_int;
			break;
		case 4:
			_fknd=_double;
			break;
		case 5:
			_fknd=_date;
			break;
		case 6:
			_fknd=_time;
			break;
	}
	
	
bArray	arr(sizeof(xmlelt));
char	csgn[10];
	add_cdesc(arr,0,"uvar","");
	add_cdesc(arr,1,"version","3.0.0");
	add_cdesc(arr,1,"name",_name);
	sprintf(csgn,"%.4s",&_fsgn);
	add_cdesc(arr,1,"sign",csgn);
	sprintf(csgn,"%.4s",&_fknd);
	add_cdesc(arr,1,"sign",csgn);
	add_idesc(arr,1,"bool",0);
	add_cdesc(arr,1,"formula","");
	switch(_fknd){
		case _char:
			add_cdesc(arr,2,"dbchar",_value);
			break;
		case _bool:
			add_cdesc(arr,2,"dbbool",_value);
			break;
		case _int:
			add_cdesc(arr,2,"dbint",_value);
			break;
		case _double:
			add_cdesc(arr,2,"dbfloat",_value);
			break;
		case _date:
			add_cdesc(arr,2,"dbdate",_value);
			break;
		case _time:
			add_cdesc(arr,2,"dbtime",_value);
			break;
	}
	
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
		NSBeep();
		return(true);
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapVarEditorCreateCalcMsgID,
				GetSignature(this));
	if(_new){
		if(!_gapp->varMgr()->add(root)){
			NSBeep();
			return(true);
		}
	}
	else{
		if(!_gapp->varMgr()->modify(_idx,root)){
			NSBeep();
			return(true);
		}
	}
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapVarEditor::parse(bGenericXMLBaseElement* root){
_bTrace_("bXMapVarEditor::parse",true);
bGenericXMLBaseElement*	elt;
char					sgn[10];


	elt=_gapp->classMgr()->NthElement(root,1,"name");
	if(elt){
		elt->getvalue(_name);
	}
	else{
_te_("no name");
		return(false);
	}

	elt=_gapp->classMgr()->NthElement(root,1,"sign");
	if(elt){
		elt->getvalue(sgn);
		_fsgn=*((UInt32*)sgn);
	}
	else{
_te_("no sign 1");
		return(false);
	}
	_idx=RecoverVar(_gapp,sgn);

	elt=_gapp->classMgr()->NthElement(root,2,"sign");
	if(elt){
		elt->getvalue(sgn);
		_fknd=*((UInt32*)sgn);
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&_fknd,sizeof(UInt32));
#endif
_tm_("sgn="+sgn+" to "+&_fknd);
	}
	else{
_te_("no sign 2");
		return(false);
	}
	
	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(elt){
		elt->getvalue(sgn);
		if(atoi(sgn)==1){
_te_("protected var");	
			return(false);
		}
	}

	elt=_gapp->classMgr()->NthElement(root,1,"formula");
	if(!elt){
_te_("no formula");
		return(false);
	}
	elt=elt->getelement(1);
	if(!elt){
_te_("no element 1");
		return(false);
	}
	elt->getvalue(_value);

	
	_new=false;
	return(true);
}
