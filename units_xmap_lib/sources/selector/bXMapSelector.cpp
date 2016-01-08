//----------------------------------------------------------------------------
// File : bXMapSelector.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, selector : selecting objects by database and/or geographic request
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
// 20/01/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapSelector.h"
#include "bXMapSelectorCriterion.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bSelectSolver.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/endian.h>

#include <std_ext/bStdUserExt.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct gan_prm{
	WindowRef		wd;
	bXMapSelector*	x;
	bool			ok;
}gan_prm;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSelector	::bXMapSelector(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_critarr(sizeof(bXMapSelectorCriterion*)){
	setclassname("selector");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSelector::~bXMapSelector(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSelector::create(bGenericXMLBaseElement* elt){
	return(new bXMapSelector(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::open(int* flags){
	bStdXMapWithModalIntf::open(flags);

	_tp=NULL;
	_dtsrc=kXMapSelectorDTSourceField;
	_selwhat=kSelReplace;
	_use_mask=true;
	
	_next=0;
	_add2cnt=false;
	_add2hdn=kAdd2HiddenNone;
	
	_geog=NULL;
	_cid=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::close(){
	bStdXMapWithModalIntf::close();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelector::process(int msg, void* prm){
_bTrace_("bXMapSelector::process",true);
bGenericXMLBaseElement* root=NULL;

	switch(msg){
		case kExtProcessCallFromIntf:
			root=make(NULL);
			break;
		case kExtProcessCallWithXMLTree:
			root=getelement(1);
			break;
		default:
_tw_("message "+msg+" not handled");
			break;
	}
	if(!root){
_te_("root == NULL");
		return(false);
	}
bSelectSolver	solver(_gapp,true);
	solver.exec(root);
	if(msg==kExtProcessCallFromIntf){
		_gapp->classMgr()->ReleaseXMLInstance(root);
	}
	flush_criterions();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelector::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelector::edit(void* prm){
_bTrace_("bXMapSelector::edit",true);
	_act=kXMapSelectorSelect;
	_root=NULL;
	_name[0]=0;
	_fsgn=0;
	_idx=0;
	_cid=1;
	if(prm){
		if((int)prm==kXMapSelectorCreateMacroCall){
			_act=kXMapSelectorCreateMacro;
		}
		else if(!parse((bGenericXMLBaseElement*)prm)){
			NSBeep();
			return(true);
		}
	}
	return(bStdXMapWithModalIntf::edit(prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelector::wd_event(EventRef evt, WindowRef wd){
_bTrace_("bXMapSelector::wd_event",false);
bool				b=true;
HICommand			cmd;
UInt32				clss=GetEventClass(evt);	
					
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				if(_geog){
					_geog->end_edit();
				}
				if(_act!=kXMapSelectorSelect){
					save_formula();
				}
				break;
			case kHICommandCancel:
				if(_geog){
					_geog->end_edit();
				}
				flush_criterions();
				break;
			
			case kHICommandSelectWindow:
				b=false;
				break;
				
			case kXMapSelectorTabsCmd:
				tab_action();
				break;
				
			case kXMapSelectorPopupTypeCmd:
				type_action();
				break;
			case kXMapSelectorPopupSourceCmd:
				source_action();
				break;
			case kXMapSelectorAddCriterionCmd:
				add_criterion();
				break;
			case kXMapSelectorRmvCriterionCmd:
				remove_criterion();
				break;
			case kXMapSelectorFieldListCmd:
				break;
			case kXMapSelectorPopupConstCmd:
				constraints_action();
				break;
				
			case kXMapSelectorPopupGeogCmd:
				break;
			case kXMapSelectorGeogResetCmd:
				reset_geog();
				b=false;
				break;
			case kXMapSelectorGeogViewCmd:
				break;
				
			case kXMapSelectorSelKindCmd:
				sel_kind_action();
				break;
			case kXMapSelectorUseMaskedCmd:
				use_masked_action();
				break;
				
			case kXMapSelectorThenContrastCmd:
				then_contrast_action();
				break;
			case kXMapSelectorThenMaskCmd:
				then_mask_action();
				break;
			case kXMapSelectorMaskTheseCmd:
				mask_these_action();
				break;
			case kXMapSelectorNextMacroCmd:
				next_macro_action();
				break;	
						
			case kXMapSelectorOpenQuestionCmd:
				break;
				
			default:
_tm_("kEventClassCommand "+&cmd.commandID);
				b=choose_geog(cmd.commandID,evt,wd);
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::wd_init(WindowRef wd){
ControlRef	c;

	if(_act==kXMapSelectorSelect){// Sélectionner
		_tp=NULL;
		_selwhat=kSelReplace;
		_use_mask=false;
		
		_next=0;
		_add2cnt=false;
		_add2hdn=kAdd2HiddenNone;

		_geog=NULL;
		c=get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameLegID);
		HideControl(c);
		c=get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameNameID);
		HideControl(c);
	}
	else if(_act==kXMapSelectorCreateMacro){// Nouvelle macro
		_tp=NULL;
		_selwhat=kSelReplace;
		_use_mask=false;

		_next=0;
		_add2cnt=false;
		_add2hdn=kAdd2HiddenNone;

		_geog=NULL;
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
		b_message_string(kXMapSelectorSaveMsgID,msg,getbundle(),0);
		c=get_control(kXMapSelectorStuffSignature,kXMapSelectorOKID);
		SetControlTitleWithCString(c,msg);
		c=get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameNameID);
const EventTypeSpec	kControlEvents[]={{kEventClassControl,kEventControlDraw}};
		InstallControlEventHandler(c,name_evt_hdlr,GetEventTypeCount(kControlEvents),kControlEvents,(void*)this,NULL);
	}
	else{// Editer une macro
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
		b_message_string(kXMapSelectorSaveMsgID,msg,getbundle(),0);
		c=get_control(kXMapSelectorStuffSignature,kXMapSelectorOKID);
		SetControlTitleWithCString(c,msg);
		c=get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameNameID);
		SetTextControlValue(c,_name);
const EventTypeSpec	kControlEvents[]={{kEventClassControl,kEventControlDraw}};
		InstallControlEventHandler(c,name_evt_hdlr,GetEventTypeCount(kControlEvents),kControlEvents,(void*)this,NULL);
	}
	
	_field=0;
	_dtsrc=kXMapSelectorDTSourceField;

	populate_types();
	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorFieldListID);
	DataBrowserInit(c,dt_fields,nt_fields,(long)this);
	
	populate_fields();
	populate_constraints();
	
	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorCriterionListID);
	DataBrowserInit(c,dt_conditions,NULL,(long)this);
	populate_conditions();

	populate_geogs();
	populate_macros();
	
	c=get_control(kXMapSelectorTabs,kXMapSelectorTabsID);
	SetControlValue(c,1);
	
	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorSelKindID);
	SetControl32BitValue(c,_selwhat);
	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorUseMaskedID);
	SetControl32BitValue(c,_use_mask);
	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorThenContrastID);
	SetControl32BitValue(c,_add2cnt);
	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorThenMaskID);
	SetControl32BitValue(c,(_add2hdn>0));
	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID);
	SetControl32BitValue(c,_add2hdn);
	SetControlAbility(c,(_add2hdn>0));

	set_alpha_visibility(true);
	set_geog_visibility(false);
	set_option_visibility(false);
}

// ---------------------------------------------------------------------------
// Handler
// -----------
pascal OSStatus bXMapSelector::dt_fields(	ControlRef browser, 
											DataBrowserItemID itemID, 
											DataBrowserPropertyID property, 
											DataBrowserItemDataRef itemData, 
											Boolean changeValue){
CFStringRef			name;
char				cname[256];
bXMapSelector*		editor=(bXMapSelector*)GetControlReference(browser);
bStdBaseAccessor*	fields=editor->_tp->fields();
bGenericExtMgr*		mgr=((bGenericMacMapApp*)editor->getapp())->calcMgr();

	switch(property){
		case kXMapSelectorDBProperty:
			if(!changeValue){
				if(editor->_dtsrc==kXMapSelectorDTSourceField){
					fields->get_name(itemID,cname);
				}
				else{
					mgr->ext_name(itemID,cname);
				}
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
// Handler
// -----------
pascal OSStatus bXMapSelector::dt_conditions(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue){
bXMapSelector*			editor=(bXMapSelector*)GetControlReference(browser);
bXMapSelectorCriterion*	crit;
CFStringRef				cfs;

	switch(property){
		case kXMapSelectorDBProperty:
			if(!changeValue){
				for(int i=1;i<=editor->_critarr.count();i++){
					editor->_critarr.get(i,&crit);
					if(crit->get_id()==itemID){
						cfs=crit->get_string(editor->getbundle());
						SetDataBrowserItemDataText(itemData,cfs);
						CFRelease(cfs);
						break;
					}
				}
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
void bXMapSelector::nt_fields(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
bXMapSelector*	editor=(bXMapSelector*)GetControlReference(browser);
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
void bXMapSelector::populate_types(){
ControlHandle		c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupTypeID);
	ClearPopupControl(c,1);
	if(!_tp){
		_tp=_gapp->typesMgr()->get(1);
	}
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,_tp->index());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::populate_constraints(){
ControlHandle	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupConstID);
	if(!c){
		return;
	}
	ClearPopupControl(c,1);
MenuRef	m=GetControlPopupMenuHandle(c);
	if(!m){
		SetControlAbility(c,false);
		return;
	}
	if(	(_field==0)											||
		(_dtsrc==kXMapSelectorDTSourceCalc)					||
		(_tp->fields()->count_constraints(_field)==0)		||
		(_tp->fields()->get_constraints_kind(_field)==_bit	)){
		SetControl32BitMaximum(c,0);
		SetControlAbility(c,false);
		return;
	}
	SetControlAbility(c,true);

char	buffer[1024];
char	str[256];
int		k=_tp->fields()->get_constraints_kind(_field);
int		d;
	_tp->fields()->get_decs(_field,&d);
	for(int i=1;i<=_tp->fields()->count_constraints(_field);i++){
		_tp->fields()->get_constraint(_field,i,buffer);
		xToChar(k,d,buffer,str);
		sprintf(buffer," %s",str);
		AddPopupControlMenuItemValue(c,buffer);
	}
	SetControl32BitMaximum(c,_tp->fields()->count_constraints(_field));
	SetControl32BitValue(c,1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::populate_fields(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapSelectorSelectSignature,kXMapSelectorFieldListID);
bStdBaseAccessor*	fields=_tp->fields();
	
	if(_dtsrc==kXMapSelectorDTSourceField){
		for(itemId=kOBJ_Name_;itemId<=fields->count();itemId++){
			AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
		}
	}
	else{
		for(itemId=1;itemId<=_gapp->calcMgr()->count();itemId++){
			AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::populate_conditions(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapSelectorSelectSignature,kXMapSelectorCriterionListID);
	
	for(itemId=1;itemId<=_critarr.count();itemId++){
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::reset_fields(){
DataBrowserItemID 	itemId=0;
ControlRef			c=get_control(kXMapSelectorSelectSignature,kXMapSelectorFieldListID);
bStdBaseAccessor*	fields=_tp->fields();

	if(_dtsrc==kXMapSelectorDTSourceField){
		for(itemId=fields->count();itemId>=kOBJ_Name_;itemId--){
			RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
		}
	}
	else{
		for(itemId=_gapp->calcMgr()->count();itemId>0;itemId--){
			RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
		}
	}
	SetDataBrowserScrollPosition(c,0,0);
	_field=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::reset_conditions(){
DataBrowserItemID 	itemId=0;
bStdBaseAccessor*	fields=_tp->fields();
ControlRef			c=get_control(kXMapSelectorSelectSignature,kXMapSelectorCriterionListID);
	for(itemId=fields->count();itemId>=kOBJ_Name_;itemId--){
		RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
	}
	
bXMapSelectorCriterion* crit;
	for(int i=1;i<=_critarr.count();i++){
		if(!_critarr.get(i,&crit)){
			break;
		}
		itemId=crit->get_id();
		RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
		delete crit;
	}
	_critarr.reset();
	SetDataBrowserScrollPosition(c,0,0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::tab_action(){
ControlRef	c=get_control(kXMapSelectorTabs,kXMapSelectorTabsID);
int			k=GetControl32BitValue(c);	
	set_alpha_visibility(k==1);
	set_geog_visibility(k==2);
	set_option_visibility(k==3);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::field_action(DataBrowserItemID item){
	_field=item;
	populate_constraints();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::type_action(){
ControlRef		c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupTypeID);
int				t=GetControl32BitValue(c);
bGenericType*	tp=_gapp->typesMgr()->get(t);
	if(tp==NULL){
		return;
	}
	if(tp!=_tp){
		reset_conditions();
		reset_fields();
		_tp=tp;
		populate_fields();
		populate_constraints();
		populate_geogs();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::constraints_action(){
	if((!_field)||(_dtsrc==kXMapSelectorDTSourceCalc)){
		return;
	}
ControlRef		c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupConstID);
int				idx=GetControl32BitValue(c);
	if(idx){
char	str[1024];
int		ck=_tp->fields()->get_constraints_kind(_field);
int		d,fk;
		_tp->fields()->get_decs(_field,&d);
		_tp->fields()->get_kind(_field,&fk);
		if(fk!=ck){
			sprintf(str,"%d",idx);
		}
		else{
char	buffer[1024];
			_tp->fields()->get_constraint(_field,idx,buffer);
			xToChar(ck,d,buffer,str);
		}
		c=get_control(kXMapSelectorSelectSignature,kXMapSelectorValueID);
		SetTextControlValue(c,str);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::source_action(){
ControlRef		c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupSourceID);
	reset_fields();
	_dtsrc=GetControl32BitValue(c);
	populate_fields();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::sel_kind_action(){
	_selwhat=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorSelKindID));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::use_masked_action(){
	_use_mask=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorUseMaskedID));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::then_contrast_action(){
	_add2cnt=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorThenContrastID));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::then_mask_action(){
int	ab=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorThenMaskID));
	if(ab){
		EnableControl(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID));
		_add2hdn=1;
		SetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID),_add2hdn);
	}
	else{
		DisableControl(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID));
		_add2hdn=0;
		SetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID),_add2hdn);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::mask_these_action(){
	_add2hdn=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorMaskTheseID));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::next_macro_action(){
_bTrace_("bXMapSelector::next_macro_action",false);
int	k=GetControl32BitValue(get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorNextMacroID))-2;
	if(k<0){
		_next=0;
	}
	else{
		_next=_gapp->macroMgr()->ext_signature(k);
_tm_(&_next);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::add_criterion(){	
	if(!make_criterion()){
		return;
	}
ControlRef			c=get_control(kXMapSelectorSelectSignature,kXMapSelectorCriterionListID);
DataBrowserItemID 	itemId=_cid;
	AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapSelectorDBProperty);
	_cid++;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::delete_criterion(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void *clientData){
deletePrm *prm=(deletePrm*)clientData;
	prm->id=item;
	(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kXMapSelectorDBProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::populate_geogs(){
//_bTrace_("bXMapSelector::populate_geogs",false);
// Activer/Désactiver les geogs en fct du type
	if(_geog&&(_act==kXMapSelectorSelect)){
		_geog->end_edit();
		_geog=NULL;
	}
ControlHandle	c=get_control(kXMapSelectorGeoOptsSignature,kXMapSelectorPopupGeogID);
	ClearPopupControl(c,3);
MenuRef			m=GetControlPopupMenuHandle(c);
	if(!m){
		return;
	}
	
int					sign,csign=0,cur=1;
char				cname[256];
bGenericExtMgr*		mgr=_gapp->geogMgr();

	if(_geog){
		csign=GetSignature(_geog);
	}
	for(int i=1;i<=mgr->count();i++){
		mgr->ext_name(i,cname);
		sign=mgr->ext_signature(i);
//_tm_(&sign+"->"+&csign);
		if(sign==csign){
			cur=i+2;
		}
		AddPopupControlMenuItemValue(c,cname);
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&sign,sizeof(int));
#endif
		SetMenuItemCommandID(m,i+2,sign);
	}
	SetControl32BitMaximum(c,mgr->count()+2);
	SetControl32BitValue(c,cur);
	
	if(_geog){
		c=get_control(kXMapSelectorGeoOptsSignature,kXMapSelectorGeogViewID);
Rect	r;
		GetControlBounds(c,&r);
		MMOffsetRect(&r,40,40);
		_geog->edit(_wd,&r);
		_geog->hide();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::populate_macros(){
//_bTrace_("bXMapSelector::populate_macros",false);
ControlHandle	c=get_control(kXMapSelectorSlctOptsSignature,kXMapSelectorNextMacroID);
	ClearPopupControl(c,3);
int					cur=1;
UInt32				sign;
char				cname[256];
bGenericExtMgr*		mgr=_gapp->macroMgr();

	for(int i=1;i<=mgr->count();i++){
		mgr->ext_name(i,cname);
		sign=mgr->ext_signature(i);
//_tm_(&sign+"->"+&_next);
		if(sign==_next){
			cur=i+2;
//_tm_("yep !!! cur="+cur);
		}
		AddPopupControlMenuItemValue(c,cname);
		if(sign==_fsgn){
			DisablePopupControlMenuItem(c,i+2);
		}
	}
	SetControl32BitMaximum(c,mgr->count()+2);
//_tm_(cur);
	SetControl32BitValue(c,cur);
//_tm_((int)GetControl32BitValue(c));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::reset_geog(){
	if(_geog){
		_geog->end_edit();
		_geog=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelector::choose_geog(int signature, EventRef evt, WindowRef wd){
bool			b=false;
bGenericGeog*	g=(bGenericGeog*)(void*)_gapp->geogMgr()->find(signature);
	if(!g){
		if(_geog){
			b=_geog->event(evt);
		}
		return(b);
	}
	if(_geog){
		_geog->end_edit();
	}	
	_geog=g;
ControlRef c=get_control(kXMapSelectorGeoOptsSignature,kXMapSelectorGeogViewID);
Rect	r;
	GetControlBounds(c,&r);
	MMOffsetRect(&r,40,40);
	_geog->edit(wd,&r);
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::remove_criterion(){
deletePrm		prm;
	prm.c=get_control(kXMapSelectorSelectSignature,kXMapSelectorCriterionListID);
	prm.id=0;
DataBrowserItemUPP	upp=NewDataBrowserItemUPP((DataBrowserItemProcPtr)delete_criterion);
	(void)ForEachDataBrowserItem(prm.c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&prm);
	DisposeDataBrowserItemUPP(upp);
	
	if(prm.id>0){
bXMapSelectorCriterion* crit;
		for(int i=1;i<=_critarr.count();i++){
			if(!_critarr.get(i,&crit)){
				break;
			}
			if(crit->get_id()==prm.id){
				_critarr.rmv(i);
				delete crit;
				break;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelector::make_criterion(){
_bTrace_("bXMapSelector::make_criterion",false);
	if(!_field){
_te_("no fld");
		NSBeep();
		return(false);
	}
	
char	fieldname[256];
char	intfname[256];
int		kind;
int		sign;
	if(_dtsrc==kXMapSelectorDTSourceField){
		_tp->fields()->get_name(_field,fieldname);
		_tp->fields()->get_kind(_field,&kind);
		sign=kDBFieldElement;
		sprintf(intfname,"%s",fieldname);
	}
	else{
bGenericCalc*	clc=(bGenericCalc*)(void*)_gapp->calcMgr()->get(_field);
char			sgn[10];
		clc->get_params(fieldname,sgn);
		if(strlen(sgn)>0){
			sign=*((int*)sgn);
		}
		else{
			sign=0;
		}
		kind=clc->kind();
		_gapp->calcMgr()->ext_name(_field,intfname);
	}
		
ControlRef	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorPopupOperatorsID);
int			opi=GetControl32BitValue(c);
char		cop[256];
MenuCommand	cmd; 

	GetPopupControlMenuItemValue(c,opi,cop,sizeof(cop)-1);
	GetPopupControlMenuItemCommandID(c,opi,&cmd);
	
	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorValueID);
char		val[1024];
	GetTextControlValue(c,val,sizeof(val));

	c=get_control(kXMapSelectorSelectSignature,kXMapSelectorOpenQuestionID);
	if(GetControl32BitValue(c)){
		kind='_??_';
	}
	
bool		status;	
bXMapSelectorCriterion* crit=new bXMapSelectorCriterion(_gapp,_cid,sign,fieldname,kind,cmd,val,&status);
	if(!status){
	}
	if(!_critarr.add(&crit)){
		NSBeep();
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::set_alpha_visibility(bool vis){
ControlRef	c=get_control(kXMapSelectorViews,kXMapSelectorViewSelectID);
	if(!vis){
		HideControl(c);
	}
	else{
		ShowControl(c);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::set_geog_visibility(bool vis){
ControlRef	c=get_control(kXMapSelectorViews,kXMapSelectorViewRelationID);
	if(!vis){
		HideControl(c);
		if(_geog){
			_geog->hide();
		}
	}
	else{
		ShowControl(c);
		if(_geog){
			_geog->show();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::set_option_visibility(bool vis){
ControlRef	c=get_control(kXMapSelectorViews,kXMapSelectorViewOptionID);
	if(!vis){
		HideControl(c);
	}
	else{
		ShowControl(c);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapSelector::flush_criterions(){
bXMapSelectorCriterion* crit;
	for(int i=1;i<_critarr.count();i++){
		if(!_critarr.get(i,&crit)){
			break;
		}
		delete crit; 
	}
	_critarr.reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapSelector::make(char* macro){
_bTrace_("bXMapSelector::make",false);
	if(_tp==NULL){
		return(NULL);
	}

bArray						l(sizeof(xmlelt));
bGenericClassesMgr*			mgr=_gapp->classMgr();
char						name[256];
int							i,k;
bXMapSelectorCriterion*		c;
bool						b=false,encap=(macro!=NULL);
bGenericXMLBaseElement*		root=NULL;
int							ii=(encap)?1:0;
char						csgn[10];

	for(;;){
		if(encap){
			if(_act==kXMapSelectorCreateMacro){
				_fsgn=UniqueSignature(_gapp);
			}
			sprintf(csgn,"%.4s",&_fsgn);
			add_cdesc(l,0,"umacro","");
			add_cdesc(l,1,"version","3.0.0");
			add_cdesc(l,1,"name",macro);
			add_cdesc(l,1,"sign",csgn);
			add_idesc(l,1,"bool",0);
		}
		add_cdesc(l,0+ii,"formula","");
		_tp->name(name);
		add_cdesc(l,1+ii,"dbtype",name);
		add_idesc(l,1+ii,"dbselkind",_selwhat);
		add_idesc(l,1+ii,"dbusemasked",_use_mask);
		if(_add2cnt){
			add_idesc(l,1+ii,"dbaddtocontrast",_add2cnt);
		}
		if(_add2hdn){
			add_idesc(l,1+ii,"dbaddtomasked",_add2hdn);			
		}
		if(_next){
			sprintf(csgn,"%.4s",&_next);
			add_cdesc(l,1+ii,"dbsetnext",csgn);
		}
		if(_geog){
			_geog->get_params(&l,1+ii);
		}
		for(i=1;i<_critarr.count();i++){
			add_cdesc(l,i+ii,"and","");
		}
		if(b){
			break;
		}
		k=i;
		for(i=1;i<=_critarr.count();i++){
			if(!_critarr.get(i,&c)){
				b=true;
				break;
			}
			if(!c->make(&l,k+ii,_bndl)){
_te_("criterion make error");
				b=true;
				break;
			}
			if(i>1){
				k--;
			}
		}
		
		if(b){
			break;
		}
		root=mgr->ParseXMLDescriptors(&l);
		if(!root){
_te_("ParseXMLDescriptors failed");
		}
		
		break;
	}
	
	free_descs(l);
	
	return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelector::parse(bGenericXMLBaseElement* root){
_bTrace_("bXMapSelector::parse",false);
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
	_idx=RecoverMacro(_gapp,sgn);

	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(elt){
		elt->getvalue(sgn);
		if(atoi(sgn)==1){
_te_("protected macro");	
			return(false);
		}
	}

parse_prm	prm;
bArray		arr(sizeof(bXMapSelectorCriterion*));
	prm.stack=&arr;
	prm.tp=NULL;
	prm.selwhat=kSelReplace;
	prm.use_mask=false;
	prm.next=0;
	prm.add2cnt=false;
	prm.add2hdn=kAdd2HiddenNone;
	prm.geog=NULL;
	prm.cid=_cid;
	prm.targetsign=0;
	prm.targetvalue[0]=0;
	prm.targetkind=0;
	prm.opsgn=0;
	prm.val[0]=0;
	
	elt=_gapp->classMgr()->NthElement(root,1,"formula");
	if(elt){
		if(!elt->dotoall(&prm,0,bXMapSelectorCriterion::parse)){
			return(false);
		}
	}
	
bool	b;

	if(	(prm.targetkind!=0)		&&
		(prm.targetvalue[0]!=0)	&&
		(prm.opsgn!=0)			){
bXMapSelectorCriterion*	crit=new bXMapSelectorCriterion(_gapp,
														prm.cid,
														prm.targetsign,
														prm.targetvalue,
														prm.targetkind,
														prm.opsgn,
														prm.val,
														&b);
		arr.add(&crit);
		prm.cid++;
	}

	_cid=prm.cid;
	
	_critarr.reset();
	_critarr+arr;
	
	_tp=prm.tp;
	_selwhat=prm.selwhat;
	_use_mask=prm.use_mask;
	_next=prm.next;
	_add2cnt=prm.add2cnt;
	_add2hdn=prm.add2hdn;

	_geog=prm.geog;
	if(_geog){
_tm_("with geog");
	}
	_act=kXMapSelectorEditMacro;
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapSelector::save_formula(){
	if(!get_a_name()){
		_tp=NULL;
		flush_criterions();
		return;
	}
	if(_geog){
		_geog->end_edit();
ControlRef c=get_control(kXMapSelectorGeoOptsSignature,kXMapSelectorGeogViewID);
Rect	r;
		GetControlBounds(c,&r);
		MMOffsetRect(&r,40,40);
		_geog->edit(_wd,&r);
	}
bGenericXMLBaseElement* root=make(_name);
	if(!root){
		flush_criterions();
		return;
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapSelectorCreateMacroMsgID,
				GetSignature(this));
	if(_act==kXMapSelectorCreateMacro){
		if(!_gapp->macroMgr()->add(root)){
			flush_criterions();
			return;
		}
	}
	else{
		if(!_gapp->macroMgr()->modify(_idx,root)){
			flush_criterions();
			return;
		}
	}
	_tp=NULL;
	flush_criterions();
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelector::get_a_name(){
ControlRef	c=get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameNameID);
	GetTextControlValue(c,_name,FILENAME_MAX);
	return(strlen(_name)>0);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapSelector::name_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
bXMapSelector*	editor=(bXMapSelector*)up;
char			str[FILENAME_MAX];
ControlRef		c=editor->get_control(kXMapSelectorGetANameSignature,kXMapSelectorGetANameNameID);
	
	GetTextControlValue(c,str,sizeof(str)-1);
	c=editor->get_control(kXMapSelectorStuffSignature,kXMapSelectorOKID);
	if(strlen(str)==0){
		DisableControl(c);
		return(eventNotHandledErr);
	}
	if(editor->_act==kXMapSelectorCreateMacro){
		if(_gapp->macroMgr()->find(str)!=NULL){
			DisableControl(c);
		}
		else{
			EnableControl(c);
		}
	}
	else if(editor->_act==kXMapSelectorEditMacro){
bStdUserExt*	ext=(bStdUserExt*)(void*)_gapp->macroMgr()->find(str);
		if(!ext){
			EnableControl(c);
		}
		else{
user_ext_data	prm={NULL,0,NULL};
			ext->process(kExtProcessCallGetData,&prm);
			if(prm.sign==editor->_fsgn){
				EnableControl(c);
			}
			else{
				DisableControl(c);
			}
		}
	}
	else{
		EnableControl(c);
	}
	return(eventNotHandledErr);
}
