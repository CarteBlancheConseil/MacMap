//----------------------------------------------------------------------------
// File : bXMapDataFill.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classe, fills objects data
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
// 16/03/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapDataFill.h"
#include "bXMapDataFillRun.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct del_p{
	ControlRef 			c;
	bArray*				runs;
}del_p;


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFill	::bXMapDataFill(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_runs(sizeof(bXMapDataFillRun*)){
	setclassname("datafill");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFill::~bXMapDataFill(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDataFill::create(bGenericXMLBaseElement* elt){
	return(new bXMapDataFill(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDataFill::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tp=NULL;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataFill::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			fill_data();
			break;
		case kExtProcessCallWithParams:
			break;
		case kExtProcessCallWithXMLTree:
			if(parse_tree()){
				fill_data(true);
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
bool bXMapDataFill::test(void* prm){
	if(_gapp->typesMgr()->count()==0){
		return(false);
	}
	if(!SelectionIsMonoType(_gapp)){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataFill::wd_event(EventRef evt, WindowRef wd){
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
				DataBrowserClose(get_control(kXMapDataFillSignature,kXMapDataFillFieldListID));
				DataBrowserClose(get_control(kXMapDataFillSignature,kXMapDataFillRunListID));
				break;
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapDataFillSignature,kXMapDataFillFieldListID));
				DataBrowserClose(get_control(kXMapDataFillSignature,kXMapDataFillRunListID));
				reset();
				break;
			
			case kXMapDataFillPopFillCmd:
				kind_action();
				break;

			case kXMapDataFillPopConstCmd:
				constraints_action();
				break;
			
			case kXMapDataFillRmvRunCmd:
				remove_run();
				break;
			case kXMapDataFillAddRunCmd:
				add_run();
				break;
			case kXMapDataFillModRunCmd:
				modify_run();
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
void bXMapDataFill::wd_init(WindowRef wd){
ControlRef			c;
bGenericGeoElement	*o;
	_gapp->selMgr()->elements()->get(1,&o);
	_tp=_gapp->typesMgr()->get(o->getType());
	_fld=0;
	_run=0;
	
	c=get_control(kXMapDataFillSignature,kXMapDataFillFieldListID);
	DataBrowserInit(c,field_handler,field_notifier,(long)this);
	
	c=get_control(kXMapDataFillSignature,kXMapDataFillRunListID);
	DataBrowserInit(c,run_handler,run_notifier,(long)this);

	populate_fields();
	field_action(0);
	run_action(0);

	kind_action();
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapDataFill::field_handler(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){

CFStringRef			name;
char				cname[256];
bXMapDataFill*		editor=(bXMapDataFill*)GetControlReference(browser);
bStdBaseAccessor*	fields=editor->_tp->fields();
	
	switch(property){
		case kXMapDataFillDBProperty:
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
void bXMapDataFill::field_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapDataFill*	editor=(bXMapDataFill*)GetControlReference(browser);
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
// Handler
// -----------
OSStatus bXMapDataFill::run_handler(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){

CFStringRef			name;
bXMapDataFill*		editor=(bXMapDataFill*)GetControlReference(browser);
bXMapDataFillRun*	run=NULL;
	
	switch(property){
		case kXMapDataFillDBProperty:
			if(!changeValue){
				for(int i=1;i<=editor->_runs.count();i++){
					editor->_runs.get(i,&run);
					if(itemID==run->get_id()){
						break;
					}
				}
                name=   run?
                        CFStringCreateWithCString(kCFAllocatorDefault,run->get_text(),CFStringGetSystemEncoding()):
                        CFStringCreateWithCString(kCFAllocatorDefault,"????",CFStringGetSystemEncoding());
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
void bXMapDataFill::run_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapDataFill*	editor=(bXMapDataFill*)GetControlReference(browser);
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
			editor->run_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->run_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::populate_constraints(){
ControlHandle	c=get_control(kXMapDataFillSignature,kXMapDataFillPopConstID);
	ClearPopupControl(c,1);
	if(	(_fld==0)										||
		(_tp->fields()->count_constraints(_fld)==0)		||
		(_tp->fields()->get_constraints_kind(_fld)==_bit)){
		SetControl32BitMaximum(c,0);
		SetControlAbility(c,false);
		return;
	}
	SetControlAbility(c,true);
char	buffer[1024];
char	str[256];
int		k=_tp->fields()->get_constraints_kind(_fld);
int		d;
	_tp->fields()->get_decs(_fld,&d);
	for(int i=1;i<=_tp->fields()->count_constraints(_fld);i++){
		_tp->fields()->get_constraint(_fld,i,buffer);
		xToChar(k,d,buffer,str);
		sprintf(buffer," %s",str);
		AddPopupControlMenuItemValue(c,buffer);
	}
	SetControl32BitMaximum(c,_tp->fields()->count_constraints(_fld));
	SetControl32BitValue(c,1);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::populate_fields(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapDataFillSignature,kXMapDataFillFieldListID);
bStdBaseAccessor*	fields=_tp->fields();
	
	for(int i=kOBJ_Name_;i<=fields->count();i++){
		itemId=fields->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapDataFillDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::populate_popfields(){
ControlRef	c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
	ClearPopupControl(c,1);
char	str[256];
	for(int i=kOBJ_Name_;i<=_tp->fields()->count();i++){
		_tp->fields()->get_name(i,str);
		AddPopupControlMenuItemValue(c,str);
	}
	SetControl32BitMaximum(c,(_tp->fields()->count()-kOBJ_Name_)+1);
	SetControl32BitValue(c,1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::populate_popcalcs(){
ControlRef	c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
	ClearPopupControl(c,1);
char	str[256];
	for(int i=1;i<=_gapp->calcMgr()->count();i++){
		_gapp->calcMgr()->ext_name(i,str);
		AddPopupControlMenuItemValue(c,str);
	}
	SetControl32BitMaximum(c,_gapp->calcMgr()->count());
	SetControl32BitValue(c,1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::constraints_action(){
	if(!_fld){
		return;
	}
ControlRef		c=get_control(kXMapDataFillSignature,kXMapDataFillPopConstID);
int				idx=GetControl32BitValue(c);
	if(idx){
char	str[1024];
int		ck=_tp->fields()->get_constraints_kind(_fld);
int		d,fk;
		_tp->fields()->get_decs(_fld,&d);
		_tp->fields()->get_kind(_fld,&fk);
		if(fk!=ck){
			sprintf(str,"%d",idx);
		}
		else{
char	buffer[1024];
			_tp->fields()->get_constraint(_fld,idx,buffer);
			xToChar(ck,d,buffer,str);
		}
		c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
		SetTextControlValue(c,str);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::field_action(DataBrowserItemID item){
	_fld=_tp->fields()->get_index(item);
	populate_constraints();
	constraints_action();
	check_add();
	check_modify();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::run_action(DataBrowserItemID item){
	_run=item;
	check_modify();
	check_del();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::kind_action(){
ControlRef	c=get_control(kXMapDataFillSignature,kXMapDataFillPopFillID);
int			k=GetControl32BitValue(c);

	switch(k){
		case 1:
			c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
			ShowControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopConstID);
			ShowControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			HideControl(c);
			populate_constraints();
			break;
			
		case 2:
			c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
			HideControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopConstID);
			HideControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			ShowControl(c);
			populate_popcalcs();
			break;
			
		case 3:
			c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
			HideControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopConstID);
			HideControl(c);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			ShowControl(c);
			populate_popfields();
			break;

	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::add_run(){
ControlRef			c=get_control(kXMapDataFillSignature,kXMapDataFillPopFillID);
int					mode=GetControl32BitValue(c);
bXMapDataFillRun*	run;
int					k,sz,d;
char				runstr[256];

	_tp->fields()->get_kind(_fld,&k);
	_tp->fields()->get_size(_fld,&sz);
	_tp->fields()->get_decs(_fld,&d);
	make_text(runstr);
	switch(mode){
		case 1:{
char	str[256];
			c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
			GetTextControlValue(c,str,sizeof(str));			
			run=new bXMapDataFillRunValue(	runstr,
											_fld,sz,k,d,
											str);
			}break;
		case 2:{			
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
bGenericCalc* clc=(bGenericCalc*)(void*)_gapp->calcMgr()->get(GetControl32BitValue(c));
			if(!clc){
				return;
			}
			run=new bXMapDataFillRunCalc(	runstr,
											_fld,sz,k,d,
											clc);
			}break;
		case 3:{
int		f,fk,fsz,fd;
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			f=GetControl32BitValue(c)+(kOBJ_Name_-1);
			_tp->fields()->get_kind(f,&fk);
			_tp->fields()->get_size(f,&fsz);
			_tp->fields()->get_decs(f,&fd);
			run=new bXMapDataFillRunField(	runstr,
											_fld,sz,k,d,
											f,fsz,fk,fd);
			}break;
		default:
			return;
			break;
	}
	if(!_runs.add(&run)){
		delete run;
		return;
	}
DataBrowserItemID 	itemId=run->get_id();
	c=get_control(kXMapDataFillSignature,kXMapDataFillRunListID);
	AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapDataFillDBProperty);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::add_run(char* field, int mode, char* value){
_bTrace_("bXMapDataFill::add_run",false);
bXMapDataFillRun*	run;
int					k,sz,d;
char				runstr[256]="";

	_fld=_tp->fields()->get_index(field);
	_tp->fields()->get_kind(_fld,&k);
	_tp->fields()->get_size(_fld,&sz);
	_tp->fields()->get_decs(_fld,&d);
	switch(mode){
		case 1:{
			run=new bXMapDataFillRunValue(	runstr,
											_fld,sz,k,d,
											value);
			}break;
		case 2:{			
bGenericCalc* clc=(bGenericCalc*)(void*)_gapp->calcMgr()->find(value);
			if(!clc){
_te_("calc "+value+" not found");
				return;
			}
			run=new bXMapDataFillRunCalc(	runstr,
											_fld,sz,k,d,
											clc);
			}break;
		case 3:{
int		f,fk,fsz,fd;
			f=_tp->fields()->get_index(value);
			if(!f){
				return;
			}
			_tp->fields()->get_kind(f,&fk);
			_tp->fields()->get_size(f,&fsz);
			_tp->fields()->get_decs(f,&fd);
			run=new bXMapDataFillRunField(	runstr,
											_fld,sz,k,d,
											f,fsz,fk,fd);
			}break;
		case 4:		
			run=new bXMapDataFillRunAskValue(value,
											 _fld,sz,k,d,
											 _tp);
			break;
			
		default:
			return;
			break;
	}
	if(!_runs.add(&run)){
		delete run;
		return;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::modify_run(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::remove_run(){
del_p			prm;
	prm.c=get_control(kXMapDataFillSignature,kXMapDataFillRunListID);
	prm.runs=&_runs;
DataBrowserItemUPP	upp=delete_run;
	(void)ForEachDataBrowserItem(prm.c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&prm);
	check_del();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::check_del(){
UInt32	count=DataBrowserCountSelected(get_control(	kXMapDataFillSignature,
													kXMapDataFillRunListID));
	SetControlAbility(	get_control(kXMapDataFillSignature,kXMapDataFillRmvRunID),
						(count>0));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::check_add(){
	SetControlAbility(get_control(kXMapDataFillSignature,kXMapDataFillAddRunID),(_fld>0));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::check_modify(){
UInt32	count=DataBrowserCountSelected(get_control(	kXMapDataFillSignature,
													kXMapDataFillRunListID));
	SetControlAbility(	get_control(kXMapDataFillSignature,kXMapDataFillModRunID),
						((_fld>0)&&(count>0)));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::delete_run(	DataBrowserItemID item, 
								DataBrowserItemState state, 
								void *clientData){
del_p*				prm=(del_p*)clientData;
bXMapDataFillRun*	run;
	for(int i=1;i<=prm->runs->count();i++){
		prm->runs->get(i,&run);
		if(item==run->get_id()){
			(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kXMapDataFillDBProperty);
			delete run;
			prm->runs->rmv(i);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::make_text(char* text){
char	s1[256],s2[256],s3[256],s4[256];

	b_message_string(kXMapDataFillMsgFillField,s1,getbundle(),0);
	_tp->fields()->get_name(_fld,s2);
ControlRef	c=get_control(kXMapDataFillSignature,kXMapDataFillPopFillID);
	switch(GetControl32BitValue(c)){
		case 1:
			b_message_string(kXMapDataFillMsgWithValue,s3,getbundle(),0);
			c=get_control(kXMapDataFillSignature,kXMapDataFillValueID);
			GetTextControlValue(c,s4,sizeof(s4));
			break;
		case 2:
			b_message_string(kXMapDataFillMsgWithCalc,s3,getbundle(),0);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			GetPopupControlMenuItemValue(c,GetControl32BitValue(c),s4,sizeof(s4));
			break;
		case 3:
			b_message_string(kXMapDataFillMsgWithField,s3,getbundle(),0);
			c=get_control(kXMapDataFillSignature,kXMapDataFillPopValueID);
			GetPopupControlMenuItemValue(c,GetControl32BitValue(c),s4,sizeof(s4));
			break;
	}
	sprintf(text,"%s %s %s %s",s1,s2,s3,s4);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::fill_data(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapDataFillMessageID,
				GetSignature(this));	
	_gapp->layersMgr()->SetObjInvalidation(false);
	fill_data(false);
	_gapp->layersMgr()->SetObjInvalidation(true);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::fill_data(bool silent){
_bTrace_("bXMapDataFill::fill_data",true);
bArray				sel(*(_gapp->selMgr()->elements()));
//bArray				cnt(*(_gapp->cntMgr()->elements()));
int					j,err=0;
bGenericGeoElement*	o;
bXMapDataFillRun*	run;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	GetName(this,ttl);
	
	_gapp->selMgr()->flush();
//	_gapp->cntMgr()->flush();
	
	message_string(kMsgProgress,msg,1);
bProgressWait wt(ttl,msg,true,true,sel.count());
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
_tm_("user break");
			break;
		}
		sel.get(i,&o);
		if(o->getType()!=_tp->index()){
			continue;
		}
//_m_(o->getID());
		o->setselect(true,false);
		for(j=1;j<=_runs.count();j++){
			_runs.get(j,&run);
			if(!run->apply(o)){
				err=-1;
_te_("run->apply failed at ("+i+":"+j+")");
				//break;
			}
		}
		o->setselect(false,false);
		
		//if(err){
			//break;
		//}
	}
	
	_gapp->selMgr()->flush();
//	_gapp->cntMgr()->flush();
	
	_gapp->selMgr()->add(&sel);
//	_gapp->cntMgr()->add(&cnt);

	reset();
	if(err){
		b_error_string(kXMapDataFillErrorCode,msg,getbundle());
bAlertStop	alrt(msg,"",silent);
	}
}

/*
// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::fill_data(bool silent){
	_bTrace_("bXMapDataFill::fill_data",true);
	bArray				sel(*(_gapp->selMgr()->elements()));
	bArray				cnt(*(_gapp->cntMgr()->elements()));
	int					j,err=0;
	bGenericGeoElement*	o;
	bXMapDataFillRun*	run;
	char				msg[__MESSAGE_STRING_LENGTH_MAX__];
	char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	GetName(this,ttl);
	
	//	_gapp->selMgr()->flush();
	//	_gapp->cntMgr()->flush();
	
	message_string(kMsgProgress,msg,1);
	bProgressWait wt(ttl,msg,true,true,sel.count());
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			_m_("user break");
			break;
		}
		sel.get(i,&o);
		//_m_(o->getID());
		//		o->setselect(true,false);
		for(j=1;j<=_runs.count();j++){
			_gapp->selMgr()->flush();
			_gapp->cntMgr()->flush();
			o->setcontrast(true,false);
			_runs.get(j,&run);
			if(!run->apply(o)){
				err=-1;
				_e_("run->apply failed at ("+i+":"+j+")");
				//break;
			}
			o->setcontrast(false,false);			
		}
		//		o->setselect(false,false);
		
		
		//if(err){
		//break;
		//}
	}
	
	_gapp->selMgr()->flush();
	_gapp->cntMgr()->flush();
	
	_gapp->selMgr()->add(&sel);
	_gapp->cntMgr()->add(&cnt);
	
	reset();
	if(err){
		b_error_string(kXMapDataFillErrorCode,msg,getbundle());
		bAlertStop	alrt(msg,"",silent);
	}
}
*/
// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataFill::reset(){
bXMapDataFillRun*	run;
	for(int i=1;i<=_runs.count();i++){
		_runs.get(i,&run);
		delete run;
	}
	_runs.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFill::parse_tree(){
//	if(!test(NULL)){
//		return(false);
//	}

	
	_tp=_gapp->typesMgr()->get(SelectionIsMonoType(_gapp));

bGenericXMLBaseElement*	elt;
	
char	name[_values_length_max_],value[_values_length_max_];
int		mode;
int		n=countelements();
int		r=n%3;
	if(r==1){
		elt=getelement(n);
		elt->getvalue(value);
		_tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(value));
		n--;
	}
	else{
		if(!test(NULL)){
			return(false);
		}
		_tp=_gapp->typesMgr()->get(SelectionIsMonoType(_gapp));
	}
	
	n/=3;
	for(int i=1;i<=n;i++){
		elt=getelement((i-1)*3+1);
		if(!elt){
			return(true);
		}
		elt->getvalue(name);
		
		elt=getelement((i-1)*3+2);
		if(!elt){
			return(true);
		}
		elt->getvalue(value);
		mode=atoi(value);
		
		elt=getelement((i-1)*3+3);
		if(!elt){
			return(true);
		}
		elt->getvalue(value);
		
		add_run(name,mode,value);
	}
	
	return(true);
}



