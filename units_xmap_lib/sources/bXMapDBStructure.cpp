//----------------------------------------------------------------------------
// File : bXMapDBStructure.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, database structure modification (adding, deleting, modifying fields, etc...)
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
// 28/04/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapDBStructure.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdAlert.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDBStructure::bXMapDBStructure(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("dbstructure");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDBStructure::~bXMapDBStructure(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDBStructure::create(bGenericXMLBaseElement* elt){
	return(new bXMapDBStructure(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDBStructure::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tp=NULL;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBStructure::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBStructure::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDBStructure::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
	
	check_add();
	check_modify();
		
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID));
				_gapp->eventMgr()->close_log();
				break;
				
			case kXMapDBStructurePopupTypeCmd:
				type_action();
				break;
			case kXMapDBStructureFieldKindCmd:
				kind_action();
				break;
			
			case kXMapDBStructureRmvFieldCmd:
				remove_field();
				break;
			case kXMapDBStructureAddFieldCmd:
				add_field();
				break;
			case kXMapDBStructureModFieldCmd:
				modify_field();
				break;
			case kXMapDBStructureFieldMaskCmd:
			case kXMapDBStructureFieldDelLockCmd:
			case kXMapDBStructureFieldWriteLockCmd:
				check_modify();
				break;
			default:
				check_modify();
				check_add();
				b=false;
				break;
		}
	}
	else{
		check_modify();
		check_add();
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDBStructure::wd_init(WindowRef wd){
ControlRef	c;

	_tp=NULL;
	_fld=0;
	
	populate_types();
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
	DataBrowserInit(c,field_handler,field_notifier,(long)this);
	populate_fields();
	
const EventTypeSpec	kWindowEvents[]={{kEventClassControl,kEventControlDraw}};
EventHandlerUPP	hdlr=ctrl_evt_hdlr;
	
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID);
	SetTextControlFilter(c,name_fltr);
	SetControlReference(c,(long)this);
	InstallControlEventHandler(c,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);

	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID);
	SetTextControlFilter(c,uint_filter);
	SetControlReference(c,(long)this);
	InstallControlEventHandler(c,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);

	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID);
	SetControlReference(c,(long)this);
	InstallControlEventHandler(c,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);
	
	SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureRmvFieldID),false);
	SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureAddFieldID),false);
	SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureModFieldID),false);

char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapDBStructureMessageID,msg,getbundle(),1);
	_gapp->eventMgr()->init_log(kXMapDBStructureSignature,msg);	
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapDBStructure::field_handler(	ControlRef browser, 
											DataBrowserItemID itemID, 
											DataBrowserPropertyID property, 
											DataBrowserItemDataRef itemData, 
											Boolean changeValue){

CFStringRef			name;
char				cname[256];
bXMapDBStructure*	editor=(bXMapDBStructure*)GetControlReference(browser);
bStdBaseAccessor*	fields=editor->_tp->fields();
	
	switch(property){
		case kXMapDBStructureDBProperty:
			if(!changeValue){
				fields->get_name(fields->get_index(itemID),cname);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		case kDataBrowserItemIsContainerProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// Notifier
// -----------
void bXMapDBStructure::field_notifier(	ControlRef browser, 
										DataBrowserItemID item, 
										DataBrowserItemNotification msg){
bXMapDBStructure*	editor=(bXMapDBStructure*)GetControlReference(browser);
    switch(msg){  
        case kDataBrowserItemAdded:
			break;
        case kDataBrowserItemRemoved:
            break;
        case kDataBrowserContainerOpened:
            break;
        case kDataBrowserContainerClosing:
            break;
        case kDataBrowserContainerClosed:
			break;
        case kDataBrowserItemSelected:
			editor->field_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->field_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::populate_types(){
ControlHandle		c=get_control(kXMapDBStructureSignature,kXMapDBStructurePopupTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,1);
	_tp=_gapp->typesMgr()->get(1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::populate_fields(){
int					i;
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
bStdBaseAccessor*	fields=_tp->fields();
	
	for(i=kOBJ_Dir_+1;i<=fields->count();i++){
		itemId=fields->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapDBStructureDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::type_action(){
ControlRef				c=get_control(kXMapDBStructureSignature,kXMapDBStructurePopupTypeID);
int						t=GetControl32BitValue(c);
bGenericType*			tp=_gapp->typesMgr()->get(t);
DataBrowserPropertyID	pid=kXMapDBStructureDBProperty;

	if(tp==NULL){
		return;
	}
	if(tp!=_tp){
		c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
		DataBrowserReset(c,1,&pid);
		_tp=tp;
		populate_fields();
		field_action(0);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::check_field(){
	field_action(DataBrowserSelectedItem(get_control(	kXMapDBStructureSignature,
														kXMapDBStructureFieldListID)));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::field_action(DataBrowserItemID item){
_bTrace_("bXMapDBStructure::field_action",false);
	_fld=_tp->fields()->get_index(item);
	if(!_fld){
_te_("no field");
		for(int i=kXMapDBStructureFieldKindID;i<=kXMapDBStructureFieldWriteLockID;i++){
			SetControlAbility(get_control(kXMapDBStructureSignature,i),true);
		}
		set_kind(_char);
		SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),"");
		set_title(kMsgLength);
		set_len(-1);
		SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),"");
		set_prot(false,false,false);
		check_del();
		check_add();
		check_modify();
		SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldInfoID),"");
		return;
	}
	
char	name[256];
	_tp->fields()->get_name(_fld,name);
	SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),name);

int	kind,d,l,ck;
	_tp->fields()->get_kind(_fld,&kind);
	_tp->fields()->get_len(_fld,&l);
	_tp->fields()->get_decs(_fld,&d);
	ck=_tp->fields()->get_constraints_kind(_fld);
	if(ck==0){
		ck=kind;
	}
	set_kind(kind);

	switch(kind){
		case _char:
			set_len(l);
			break;
		case _bool:
		case _int:
			set_len(-1);
			break;
		case _double:
			set_len(d);
			break;
		case _date:
		case _time:
			set_len(-1);
			break;
		default:
			set_len(-1);
			break;
	}
	
char	def[256];
//	if(_tp->fields()->get_default_value(_fld,&def)){
	if(_tp->fields()->get_default_value(_fld,def)){
		if(kind==ck){// Pour les champs naturels, on prend le type naturel
			if(!xToChar(kind,d,def,name)){
_te_("xToChar for (kind==ck)");
			}
		}
		else if(ck==_bit){// Pour les champs bit, on prend le type naturel
			if(!xToChar(kind,d,def,name)){
_te_("xToChar for (kind==ck)");
			}
		}
		else{// Pour les champs classe, on prend le nom de la contrainte
			sprintf(name,"%s",def);
		}
	}
	else{
		name[0]=0;
	}
	SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),name);

	set_kind(kind);
	kind_action();
	
	set_prot(	_tp->fields()->is_hidden(_fld),
				_tp->fields()->is_delprotected(_fld),
				_tp->fields()->is_writeprotected(_fld));

	sprintf(name,"id=%lu, idx=%d",item,_fld);
	SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldInfoID),name);
	
	check_del();
	check_add();
	check_modify();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::kind_action(){
ControlRef	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID);
int			i,k=GetControl32BitValue(c);

	switch(k){
		case 1:
			for(i=kXMapDBStructureFieldKindID;i<=kXMapDBStructureFieldWriteLockID;i++){
				SetControlAbility(get_control(kXMapDBStructureSignature,i),true);
			}
			set_title(kMsgLength);
			break;
			
		case 2:
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenNameID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValNameID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID),true);
			set_title(kMsgLength);
			break;
			
		case 3:
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenNameID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID),true);
			set_title(kMsgLength);
			break;
			
		case 4:
			for(i=kXMapDBStructureFieldKindID;i<=kXMapDBStructureFieldWriteLockID;i++){
				SetControlAbility(get_control(kXMapDBStructureSignature,i),true);
			}
			set_title(kMsgDecimals);
			break;
			
		case 5:
		case 6:
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenNameID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValNameID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),false);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID),true);
			SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID),true);
			set_title(kMsgLength);
			break;
			
		default:
			for(i=kXMapDBStructureFieldKindID;i<=kXMapDBStructureFieldWriteLockID;i++){
				SetControlAbility(get_control(kXMapDBStructureSignature,i),false);
			}
			set_title(kMsgLength);
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::add_field(){
ControlRef	c;
char		name[256];
int			len,decs,kind,k;
char		msg[__MESSAGE_STRING_LENGTH_MAX__];
char		exp[__MESSAGE_STRING_LENGTH_MAX__];
	
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID);
	k=GetControl32BitValue(c);
	switch(k){
		case 1:
			kind=_char;
			c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID);
			GetTextControlValue(c,name,sizeof(name)-1);
			len=atoi(name);
			decs=0;
			break;
		case 2:
			kind=_bool;
			len=1;
			decs=0;
			break;
		case 3:
			kind=_int;
			len=16;
			decs=0;
			break;
		case 4:
			kind=_double;
			len=32;
			c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID);
			GetTextControlValue(c,name,sizeof(name)-1);
			decs=atoi(name);
			break;
		case 5:
			kind=_date;
			len=24;
			decs=0;
			break;
		case 6:
			kind=_time;
			len=24;
			decs=0;
			break;
	}
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID);
	GetTextControlValue(c,name,sizeof(name)-1);

int status=_tp->fields()->add(kind,len,decs,name);
	if(status){
		error_string(error_num(_bDataBaseErr_,_bDataBaseAddFieldErr_),msg);
		sprintf(exp,"\"%s\"",name);
bAlertStop	alrt(msg,exp);
		return;
	}
	
	k=_tp->fields()->get_index(name);
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID);
	_tp->fields()->set_hidden(k,GetControl32BitValue(c));
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID);
	_tp->fields()->set_delprotected(k,GetControl32BitValue(c));
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID);
	_tp->fields()->set_writeprotected(k,GetControl32BitValue(c));

DataBrowserItemID 	itemId=_tp->fields()->get_id(_tp->fields()->count());
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
	AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapDBStructureDBProperty);
	
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID);
	GetTextControlValue(c,name,sizeof(name)-1);
char	buff[256];
	if(!charToX(kind,decs,name,buff)){
		error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
		b_message_string("unsupported def val err exp",exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
		return;
	}

	if((strlen(name)>0)&&(_tp->nb_live()>0)){
		b_message_string("init existing",msg,getbundle(),1);
		b_message_string("init existing exp",exp,getbundle(),1);
bAlertWarningYes	alrt(msg,exp);
			if(alrt.result()){	
				for(int i=1;i<=_tp->fields()->count_records();i++){
					if(_tp->fields()->write(i,k,buff)){
						error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
bAlertStop	alrt(msg,"");
						return;
					}
				}
			}
	}
	if(!_tp->fields()->set_default_value(k,buff)){
		error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
bAlertStop	alrt(msg,"");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::modify_field(){
_bTrace_("bXMapDBStructure::modify_field",false);
ControlRef		c;
char			name[256];
int				len,decs,kind,k;
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
char			exp[__MESSAGE_STRING_LENGTH_MAX__];

	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID);
	k=GetControl32BitValue(c);
	switch(k){
		case 1:
			kind=_char;
			c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID);
			GetTextControlValue(c,name,sizeof(name)-1);
			len=atoi(name);
			decs=0;
			break;
		case 2:
			kind=_bool;
			len=1;
			decs=0;
			break;
		case 3:
			kind=_int;
			len=16;
			decs=0;
			break;
		case 4:
			kind=_double;
			len=32;
			c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID);
			GetTextControlValue(c,name,sizeof(name)-1);
			decs=atoi(name);
			break;
		case 5:
			kind=_date;
			len=24;
			decs=0;
			break;
		case 6:
			kind=_time;
			len=24;
			decs=0;
			break;
	}
	
int	ok,ol,od;
	_tp->fields()->get_kind(_fld,&ok);
	_tp->fields()->get_len(_fld,&ol);
	_tp->fields()->get_decs(_fld,&od);
	if(	(kind!=ok)	||
		(decs!=od)	||
		(len!=ol)	){
		if(!_tp->fields()->is_dyn(_fld)){
_tm_(kind+":"+ok+" ; "+decs+":"+od+" ; "+len+":"+ol);
			b_message_string("lossy conversion",msg,getbundle(),1);
			b_message_string("lossy conversion exp",exp,getbundle(),1);
bAlertWarningNo	alrt(msg,exp);
			if(alrt.result()){
				return;
			}
		}
	}
	
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID);
	GetTextControlValue(c,name,sizeof(name)-1);
	
	if(!_tp->fields()->is_dyn(_fld)&&!_tp->fields()->is_dyn_ref(_fld)){
int status=_tp->fields()->modify(_fld,kind,len,decs,name);
		if(status){
			error_string(error_num(_bDataBaseErr_,_bDataBaseModifyFieldErr_),msg);
			sprintf(exp,"\"%s\"",name);
bAlertStop	alrt(msg,exp);
			return;
		}
	}
	else{
		_tp->fields()->get_name(_fld,name);
		SetTextControlValue(c,name);

		set_kind(ok);
		if(ok==_double){
			set_len(od);
		}
		else{
			set_len(ol);
		}
	}
	
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID);
	_tp->fields()->set_hidden(_fld,GetControl32BitValue(c));
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID);
	_tp->fields()->set_delprotected(_fld,GetControl32BitValue(c));

	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID);
	if(!_tp->fields()->is_dyn(_fld)){
		_tp->fields()->set_writeprotected(_fld,GetControl32BitValue(c));
	}
	else{
		SetControl32BitValue(c,1);
	}
	
DataBrowserItemID 	itemId=_tp->fields()->get_id(_fld);
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
	(void)UpdateDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapDBStructureDBProperty,kXMapDBStructureDBProperty);

	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID);
	GetTextControlValue(c,name,sizeof(name)-1);
	if(strlen(name)>0){
//_tm_("setting default val");
char	buff[256];
int		ck=_tp->fields()->get_constraints_kind(_fld);
		if(ck==0){
			ck=kind;
		}
		if(kind==ck){// Pour les champs naturels, on prend le type naturel
			if(!charToX(kind,decs,name,buff)){
				error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
				b_message_string("unsupported def val err exp",exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
				return;
			}
		}
		else if(ck==_bit){// Pour les champs bit, on prend le type naturel
			if(!charToX(kind,decs,name,buff)){
				error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
				b_message_string("unsupported def val err exp",exp,getbundle(),1);
bAlertStop	alrt(msg,exp);
				return;
			}
		}
		else{// Pour les champs classe, on prend le nom de la contrainte
			sprintf(buff,"%s",name);
		}

		if(!_tp->fields()->set_default_value(_fld,buff)){
			error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
bAlertStop	alrt(msg,"");
		}
	}
	else{
		if(!_tp->fields()->set_default_value(_fld,NULL)){
			error_string(error_num(_bDataBaseErr_,_bDataBaseSetDefaultValueErr_),msg);
bAlertStop	alrt(msg,"");
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::remove_field(){
del_p			prm;

	prm.status=noErr;
	prm.c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID);
	prm.flds=_tp->fields();
	prm.gapp=_gapp;
DataBrowserItemUPP	upp=delete_field;
	(void)ForEachDataBrowserItem(prm.c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&prm);
	check_del();
	if(prm.status){
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
		error_string(error_num(_bDataBaseErr_,_bDataBaseRemoveFieldErr_),msg);
bAlertStop	alrt(msg,"");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::set_kind(int kind){
ControlRef	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID);
int			k;
	
	switch(kind){
		case _char:
			k=1;
			break;
		case _bool:
			k=2;
			break;
		case _int:
			k=3;
			break;
		case _double:
			k=4;
			break;
		case _date:
			k=5;
			break;
		case _time:
			k=6;
			break;
		default:
			k=1;
			break;
	}
	SetControl32BitValue(c,k);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::set_title(char* msg_id){
char		msg[__MESSAGE_STRING_LENGTH_MAX__];	
	message_string(msg_id,msg,1);
	strcat(msg," :");
	SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenNameID),msg);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::set_len(int len){
char		msg[256]="";	
	if(len>-1){
		sprintf(msg,"%d",len);
	}
	SetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),msg);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::set_prot(bool msk, bool del, bool wrt){
ControlRef	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID);
	SetControl32BitValue(c,msk);
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID);
	SetControl32BitValue(c,del);
	c=get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID);
	SetControl32BitValue(c,wrt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::check_del(){
ControlRef	c=get_control(	kXMapDBStructureSignature,
							kXMapDBStructureFieldListID);
UInt32		count=DataBrowserCountSelected(c);
	_flg=true;
	if(count<=0){
		_flg=false;
	}
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,sel_test,this);
	c=get_control(	kXMapDBStructureSignature,
					kXMapDBStructureRmvFieldID);
	SetControlAbility(c,_flg);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::check_add(){
bool	b=false;
	
	_flg=true;
	for(;;){
char	val[256];
		
		GetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),val,sizeof(val)-1);
		if(strlen(val)==0){
			break;
		}
		if(_tp->fields()->get_index(val)>0){
			break;
		}
		GetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),val,sizeof(val)-1);
int		n=atoi(val);		
		switch(GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID))){
			case 1:
				b=((n>0)&&(n<256));
				break;
			case 2:
			case 3:
				b=true;
				break;
			case 4:
                //b=((n<30)&&(strlen(val)>=0));
                b=(n<30);
				break;
			case 5:
			case 6:
				b=true;
				break;
		}
		break;
	}
	SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureAddFieldID),b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::check_modify(){
//_bTrace_("bXMapDBStructure::check_modify",false);
bool	b=false,bad=false;
int		idx,ibf,ck,curc;
char	val[256],buffer[1024],buffer2[1024];

	_flg=true;
	for(;;){
		if(DataBrowserCountSelected(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID))>1){
			break;
		}
		(void)ForEachDataBrowserItem(	get_control(kXMapDBStructureSignature,kXMapDBStructureFieldListID),
										kDataBrowserNoItem,
										false,
										kDataBrowserItemIsSelected,
										sel_test,
										this);
		if(!_flg){
//_tm_("(!_flg)");
			break;
		}
		
		GetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldNameID),val,sizeof(val)-1);
		if(strlen(val)==0){
			break;
		}
//_tm_("name "+b);
		idx=_tp->fields()->get_index(val);
		if(idx==0){
//_tm_("(idx==0)");
			b=true;
			//break;
		}
		GetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldLenID),val,sizeof(val)-1);
//_tm_("len "+b);
int		n=atoi(val);		
		switch(GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID))){
			case 1:
				_tp->fields()->get_len(idx,&ibf);
				if((n<=0)||(n>255)){
					bad=true;
				}
				else if(ibf!=n){
					b=true;
				}
				break;
			case 2:
			case 3:
				b=false;
				break;
			case 4:
				_tp->fields()->get_decs(idx,&ibf);
				if((n<=0)||(n>29)){
					bad=true;
				}
				else if(ibf!=n){
					b=true;
				}
				break;
			case 5:
			case 6:
				b=false;
				break;
		}
		if(bad){
			break;
		}
/**/		if(b){
			break;
/**/		}
// modif de la valeur par défaut
//_tm_("defval"+b);
		GetTextControlValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDefValID),val,sizeof(val)-1);
		if(strlen(val)>0){
			if(_tp->fields()->has_default_value(idx)){
				if(_tp->fields()->count_constraints(idx)>0){
					ck=_tp->fields()->get_constraints_kind(idx);
					if(ck==_bit){
						ck=_char;
					}
					charToX(ck,0,val,(void*)buffer);
					curc=_tp->fields()->get_constraint_index(idx,(void*)buffer);
					if(curc>0){
						_tp->fields()->get_default_value(idx,buffer);
						b=(curc!=_tp->fields()->get_constraint_index(idx,(void*)buffer));
					}
					else{
						bad=true;
						b=false;
					}
				}
				else{
					_tp->fields()->get_default_value(idx,buffer2);
					_tp->fields()->get_kind(idx,&ck);
					_tp->fields()->get_size(idx,&ibf);
					charToX(ck,0,val,(void*)buffer);
					if(ck==_char){
						b=(strcmp(buffer,buffer2)!=0);
					}
					else{
						b=(memcmp(buffer,buffer2,ibf)!=0);
					}
				}
			}
			else{
				b=true;
			}
		}
		else{
			if(	(_tp->fields()->count_constraints(idx)>0)		&&
				(_tp->fields()->get_constraints_kind(idx)!=_bit)){
				b=false;
				bad=true;
//_tm_("_tp->fields()->count_constraints(idx)>0");
			}
			else{
				b=_tp->fields()->has_default_value(idx);
			}
		}
		if(bad){
			break;
		}
		if(b){
			break;
		}
// modif du genre		
//_tm_("gender"+b);
		_tp->fields()->get_kind(idx,&ibf);
		switch(GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldKindID))){
			case 1:
				b=(ibf!=_char);
				if(b){
					_tp->fields()->get_len(idx,&ibf);
					b=((n>0)&&(n<256));
				}
				break;
			case 2:
				b=(ibf!=_bool);
				break;
			case 3:
				b=(ibf!=_int);
				break;
			case 4:
				b=(ibf!=_double);
				if(b){
					_tp->fields()->get_decs(idx,&ibf);
					b=((n<30)&&(strlen(val)>0));
				}
				break;
			case 5:
				b=(ibf!=_date);
				break;
			case 6:
				b=(ibf!=_time);
				break;
		}
// modif des flags		
//_tm_("flags"+b);
//_tm_((int)GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID))+"->"+(int)_tp->fields()->is_hidden(idx));
		if(	GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldMaskID))		!=
			_tp->fields()->is_hidden(idx)																	){
			b=true;
		}
		if(	GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldDelLockID))		!=
			_tp->fields()->is_delprotected(idx)																){
			b=true;
		}
		if(	GetControl32BitValue(get_control(kXMapDBStructureSignature,kXMapDBStructureFieldWriteLockID))	!=
			_tp->fields()->is_writeprotected(idx)															){
			b=true;
		}
		break;
	}
	
	
	b=true;
	
	SetControlAbility(get_control(kXMapDBStructureSignature,kXMapDBStructureModFieldID),b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::delete_field(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void *clientData){
del_p* prm=(del_p*)clientData;
	IdleApp();
int idx=prm->flds->get_index(item);
int status=prm->flds->remove(idx);
	if(status){
		prm->status=status;
		return;
	}
	(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kXMapDBStructureDBProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDBStructure::sel_test(	DataBrowserItemID item, 
									DataBrowserItemState state, 
									void *clientData){
bXMapDBStructure*	x=(bXMapDBStructure*)clientData;
int					idx=x->_tp->fields()->get_index(item);
	if(x->_tp->fields()->is_dyn_ref(idx)){
		x->_flg=false;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult bXMapDBStructure::name_fltr(	ControlRef c, 
													SInt16* key, 
													SInt16* car, 
													EventModifiers* m){
	return(kControlKeyFilterPassKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapDBStructure::ctrl_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
bXMapDBStructure*	x=(bXMapDBStructure*)up;
	x->check_add();
	x->check_modify();
	return(eventNotHandledErr);
}

