//----------------------------------------------------------------------------
// File : bXMapNetCleanOptions.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, network cleaning options
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
// 08/11/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapNetCleanOptions.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapNetCleanOptions	::bXMapNetCleanOptions(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("netcleanoptions");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapNetCleanOptions::~bXMapNetCleanOptions(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapNetCleanOptions::create(bGenericXMLBaseElement* elt){
	return(new bXMapNetCleanOptions(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapNetCleanOptions::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);

	if(clss==kEventClassCommand){
		GetEventParameter(	evt,	
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				param_put();
				break;
			case kHICommandCancel:
				delete _prm.nodes;
				delete _prm.edges;
				break;
			case kXMapNetCleanOptionsNodesCmd:
			case kXMapNetCleanOptionsEdgesCmd:
			case kXMapNetCleanOptionsNodesCrTypeCmd:
			case kXMapNetCleanOptionsNodesCrSubtypeCmd:
			case kXMapNetCleanOptionsBridgesTypeCmd:
			case kXMapNetCleanOptionsBridgesSubtypeCmd:
				check_intf(cmd.commandID);
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
void bXMapNetCleanOptions::wd_init(WindowRef wd){
ControlRef			c;
	
	_prm.nodes=new bArray(sizeof(bGenericType*));
	_prm.edges=new bArray(sizeof(bGenericType*));
	
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,LONG_MAX,0);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsEdgesID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,LONG_MAX,0);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,LONG_MAX,0);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrSubtypeID);
	ClearPopupControl(c,1);

char				val[256];
bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeID);
	SetTextControlFilter(c,ufloat_filter);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeUnitID);
	SetTextControlValue(c,val);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistBridgeID);
	SetTextControlFilter(c,ufloat_filter);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistBridgeUnitID);
	SetTextControlValue(c,val);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,LONG_MAX,0);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesSubtypeID);
	ClearPopupControl(c,1);
	
	param_get();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetCleanOptions::param_get(){
bGenericExt*	ext;
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
bGenericType*	tp;
int				idx;
ControlRef		c;

	ext=_gapp->xmapMgr()->find('NtCl');
	if(!ext){
		b_message_string("",msg,getbundle(),1);
bAlertStop	alrt(msg,"");
		return;
	}
	ext->process(kExtProcessCallGetData,&_prm);

	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->kind()==kBaseKindPoint){
			EnablePopupControlMenuItem(	get_control(kXMapNetCleanOptionsSignature,
													kXMapNetCleanOptionsNodesID),
										i);
		}
		else if(tp->kind()==kBaseKindPolyline){
			EnablePopupControlMenuItem(	get_control(kXMapNetCleanOptionsSignature,
													kXMapNetCleanOptionsEdgesID),
										i);			
		}
	}


	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesID);
	for(int i=1;i<=_prm.nodes->count();i++){
		_prm.nodes->get(i,&tp);
		if(tp){
			idx=tp->index();
			EnablePopupControlMenuItem(c,idx);
			CheckPopupControlMenuItem(c,idx,true);
		}
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsEdgesID);
	for(int i=1;i<=_prm.edges->count();i++){
		_prm.edges->get(i,&tp);
		if(tp){
			idx=tp->index();
			EnablePopupControlMenuItem(c,idx);
			CheckPopupControlMenuItem(c,idx,true);
		}
	}
	
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrTypeID);
	for(int i=1;i<=_prm.nodes->count();i++){
		_prm.nodes->get(i,&tp);
		if(tp){
			idx=tp->index();
			EnablePopupControlMenuItem(c,idx);
			if(tp==_prm.tnod){
				SetControl32BitValue(c,idx);
			}
		}
	}
	if(_prm.tnod){
		c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrSubtypeID);
		ClearPopupControl(c,1);
		PopulatePopupControlWithConstraints(_prm.tnod,kOBJ_SubType_,c,_prm.stnod);
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrNameID);
	SetTextControlValue(c,_prm.name);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesTypeID);
	for(int i=1;i<=_prm.nodes->count();i++){
		_prm.nodes->get(i,&tp);
		if(tp){
			idx=tp->index();
			EnablePopupControlMenuItem(c,idx);
			if(tp==_prm.tbdg){
				SetControl32BitValue(c,idx);
			}
		}
	}
	if(_prm.tbdg){
		c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesSubtypeID);
		ClearPopupControl(c,1);
		PopulatePopupControlWithConstraints(_prm.tbdg,kOBJ_SubType_,c,_prm.stbdg);
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeID);
	SetTextControlValue(c,_prm.name);
	
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeID);
	sprintf(msg,"%.*f",_gapp->document()->dist_pref_digits(),_prm.dnod);
	SetTextControlValue(c,msg);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistBridgeID);
	sprintf(msg,"%.*f",_gapp->document()->dist_pref_digits(),_prm.dbdg);
	SetTextControlValue(c,msg);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsAutoID);
	SetControl32BitValue(c,_prm.autoclean);
    DisableControl(c);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetCleanOptions::param_put(){
bGenericExt*	ext;
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
bGenericType*	tp;
int				idx;
ControlRef		c;

	ext=_gapp->xmapMgr()->find('NtCl');
	if(!ext){
		b_message_string("",msg,getbundle(),1);
bAlertStop	alrt(msg,"");
		return;
	}

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesID);
	for(int i=1;i<=CountPopupControlMenuItems(c);i++){
		if(IsPopupControlMenuItemChecked(c,i)){
			tp=_gapp->typesMgr()->get(i);
		}
		else{
			tp=NULL;
		}
		_prm.nodes->put(i,&tp);
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsEdgesID);
	for(int i=1;i<=CountPopupControlMenuItems(c);i++){
		if(IsPopupControlMenuItemChecked(c,i)){
			tp=_gapp->typesMgr()->get(i);
		}
		else{
			tp=NULL;
		}
		_prm.edges->put(i,&tp);
	}
	
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrTypeID);
	idx=GetControl32BitValue(c);
	if(idx){
		_prm.tnod=_gapp->typesMgr()->get(idx);
	}
	else{
		_prm.tnod=NULL;
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrSubtypeID);
	_prm.stnod=GetControl32BitValue(c);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrNameID);
	GetTextControlValue(c,_prm.name,sizeof(_prm.name)-1);
	

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesTypeID);
	idx=GetControl32BitValue(c);
	if(idx){
		_prm.tbdg=_gapp->typesMgr()->get(idx);
	}
	else{
		_prm.tbdg=NULL;
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesSubtypeID);
	_prm.stbdg=GetControl32BitValue(c);

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeID);
	GetTextControlValue(c,msg,sizeof(msg)-1);
	_prm.dnod=matof(msg);
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistBridgeID);
	GetTextControlValue(c,msg,sizeof(msg)-1);
	_prm.dbdg=matof(msg);
	
	ext->process(kExtProcessCallSetData,&_prm);

	delete _prm.nodes;
	delete _prm.edges;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetCleanOptions::check_intf(UInt32 sign){
ControlRef		c,c1,c2;
int				v1,v2,vb1,vb2;
bGenericType*	tp;
char			val[256];
bool			flg=false;

	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesID);
	c1=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrTypeID);
	c2=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesTypeID);
	vb1=v1=GetControl32BitValue(c1);
	vb2=v2=GetControl32BitValue(c2);
	for(int i=1;i<=CountPopupControlMenuItems(c);i++){
		if(IsPopupControlMenuItemChecked(c,i)){
			EnablePopupControlMenuItem(c1,i);
			EnablePopupControlMenuItem(c2,i);
		}
		else{
			DisablePopupControlMenuItem(c1,i);
			DisablePopupControlMenuItem(c2,i);
			if(v1==i){
				flg=true;
				v1=0;
			}
			if(v2==i){
				flg=true;
				v2=0;
			}
		}
	}
	if(!v1){
		for(int i=1;i<=CountPopupControlMenuItems(c);i++){
			if(IsPopupControlMenuItemChecked(c,i)){
				v1=i;
				flg=true;
				SetControl32BitValue(c1,i);
				break;
			}
		}
	}
	if(!v2){
		for(int i=CountPopupControlMenuItems(c);i>0;i--){
			if(IsPopupControlMenuItemChecked(c,i)){
				v2=i;
				flg=true;
				SetControl32BitValue(c2,i);
				break;
			}
		}
	}
	if(!flg){
		flg=((v1!=vb1)||(v2!=vb2));
	}

	if((flg)||(sign==kXMapNetCleanOptionsNodesCrTypeCmd)){
		c1=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsNodesCrSubtypeID);
		ClearPopupControl(c1,1);
		if(v1){
			tp=_gapp->typesMgr()->get(v1);
			PopulatePopupControlWithConstraints(tp,kOBJ_SubType_,c1,1);
		}
	}
	if((flg)||(sign==kXMapNetCleanOptionsBridgesTypeCmd)){
		c2=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsBridgesSubtypeID);
		ClearPopupControl(c2,1);
		if(v2){
			tp=_gapp->typesMgr()->get(v2);
			PopulatePopupControlWithConstraints(tp,kOBJ_SubType_,c2,(v1==v2)?2:1);
		}
	}
	
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistNodeID);
	GetTextControlValue(c,val,sizeof(val)-1);
	if(!strfloat(val,0)){
		SetTextControlValue(c,"0");
	}
	c=get_control(kXMapNetCleanOptionsSignature,kXMapNetCleanOptionsDistBridgeID);
	GetTextControlValue(c,val,sizeof(val)-1);
	if(!strfloat(val,0)){
		SetTextControlValue(c,"0");
	}
}



