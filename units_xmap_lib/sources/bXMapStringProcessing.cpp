//----------------------------------------------------------------------------
// File : bXMapStringProcessing.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, string formating utility
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
// 28/05/2009 creation.
//----------------------------------------------------------------------------

#include "bXMapStringProcessing.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>
#include "mm_config.h"


// ---------------------------------------------------------------------------
// 
// ------------
static int formatinfo(char* fmt, int* pos){
char	*p=strchr(fmt,'#');
	if(p==NULL){
		return(0);
	}
int		l=0;
	*pos=p-fmt;
	while(p[0]=='#'){
		p++;
		l++;
	}
	return(l);
}

// ---------------------------------------------------------------------------
// 
// ------------
static void strformat(char* fmt, char* str, int pos, int len){
int		i,n=len-strlen(str);
char	buff1[__MESSAGE_STRING_LENGTH_MAX__*3];
char	buff2[__MESSAGE_STRING_LENGTH_MAX__*3];

	strcpy(buff1,fmt);
	strcpy(buff2,str);
	
	for(i=1;i<=n;i++){
		strinsert(buff2,"0",0);
	}
	strrep(buff1,"#","");
	strinsert(buff1,buff2,pos);
	strcpy(str,buff1);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapStringProcessing	::bXMapStringProcessing(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						:bStdXMapWithModalIntf(elt,gapp,bndl)
						,_arr(sizeof(int)){
	setclassname("stringprocessing");
	_prm.arr=&_arr;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapStringProcessing::~bXMapStringProcessing(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapStringProcessing::create(bGenericXMLBaseElement* elt){
	return(new bXMapStringProcessing(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStringProcessing::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_arr.reset();
	_prm.tp=0;
	_prm.sep[0]=0;
	_prm.cas=kStrProcessCaseDontChange;
	_prm.srch[0]=0;
	_prm.repl[0]=0;
	_prm.fmt[0]=0;	
	_prm.target=kOBJ_Name_;	
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStringProcessing::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			process_str();
			break;
		case kExtProcessCallWithParams:
			process_str((strprocess_prm*)prm,true);
			break;
		case kExtProcessCallWithXMLTree:
			process_str(true);
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStringProcessing::test(void* prm){
	return(SelectionIsMonoType(_gapp));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStringProcessing::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss;	
			
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				report();
				DataBrowserClose(get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID));
				DataBrowserClose(get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID));
				GetTextControlValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingSepID),_prm.sep,255);
				_prm.cas=GetControl32BitValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingCaseID));
				GetTextControlValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingSearchID),_prm.srch,255);
				GetTextControlValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingReplaceID),_prm.repl,255);
				GetTextControlValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingFormatID),_prm.fmt,255);
				_prm.target=GetControl32BitValue(get_control(kXMapStringProcessingSignature,kXMapStringProcessingTargetID))+kOBJ_Name_-1;
				break;
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID));
				DataBrowserClose(get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID));
				break;
			case kXMapStringProcessingAddCmd:
				add_action();
				break;
			case kXMapStringProcessingInsertCmd:
				insert_action();
				break;
			case kXMapStringProcessingRemoveCmd:
				rmv_action();
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
void bXMapStringProcessing::wd_init(WindowRef wd){
	_prm.arr->reset();
	_prm.tp=SelectionIsMonoType(_gapp);
	if(_prm.tp==0){
		return;
	}
	
ControlRef		c;

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID);
	DataBrowserInit(c,left_handler,left_notifier,(long)this);
	populate_left();
	
	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
	DataBrowserInit(c,right_handler,right_notifier,(long)this);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingSepID);
	SetTextControlValue(c,_prm.sep);
	
	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingCaseID);
	SetControl32BitValue(c,_prm.cas);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingSearchID);
	SetTextControlValue(c,_prm.srch);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingReplaceID);
	SetTextControlValue(c,_prm.repl);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingFormatID);
	SetTextControlValue(c,_prm.fmt);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingCaseID);
	SetControl32BitValue(c,_prm.cas);

	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingTargetID);
	ClearPopupControl(c,1);
bGenericType*	tp=_gapp->typesMgr()->get(_prm.tp);
	PopulatePopupControlWithFields(tp,c,kOBJ_Name_,_prm.target-kOBJ_Name_+1);
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapStringProcessing::left_handler(	ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){
CFStringRef				name;
char					cname[256];
bXMapStringProcessing*	editor=(bXMapStringProcessing*)GetControlReference(browser);
bGenericType*			tp=_gapp->typesMgr()->get(editor->_prm.tp);
bStdBaseAccessor*		fields=tp->fields();
	
	switch(property){
		case kXMapStringProcessingDBProperty:
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
void bXMapStringProcessing::left_notifier(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
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
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapStringProcessing::right_handler(	ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){

CFStringRef				name;
char					cname[256];
bXMapStringProcessing*	editor=(bXMapStringProcessing*)GetControlReference(browser);
bGenericType*			tp=_gapp->typesMgr()->get(editor->_prm.tp);
bStdBaseAccessor*		fields=tp->fields();
	
	switch(property){
		case kXMapStringProcessingDBProperty:
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
void bXMapStringProcessing::right_notifier(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
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
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::populate_left(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID);
bStdBaseAccessor*	fields=(_gapp->typesMgr()->get(_prm.tp))->fields();
	
	for(int i=kOBJ_Name_;i<=fields->count();i++){
		itemId=fields->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapStringProcessingDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::add_action(){
ControlRef			c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID);
bArray				arr(sizeof(DataBrowserItemID));
DataBrowserItemID	item;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_field,&arr);
	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
	for(int i=arr.count();i>0;i--){
		arr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapStringProcessingDBProperty);
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::insert_action(){
ControlRef			c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingLeftListID);
bArray				arrl(sizeof(DataBrowserItemID)),arrr(sizeof(DataBrowserItemID)),rev(sizeof(DataBrowserItemID));
DataBrowserItemID	item;
int					i,j,idx;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_field,&arrl);
	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_field,&arrr);
	for(i=arrr.count();i>0;i--){
		arrr.get(i,&item);
		rev.add(&item);
	}
	arrr.reset();
	arrr+rev;
	
	for(i=1;i<=arrr.count();i++){
		arrr.get(i,&item);
		if(IsDataBrowserItemSelected(c,item)){
			break;
		}
	}
	idx=i;

	for(j=1;j<=arrl.count();j++){
		arrl.get(j,&item);
		arrr.insert(idx,&item);
	}

	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapStringProcessingDBProperty);
	for(int i=1;i<=arrr.count();i++){
		arrr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapStringProcessingDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::rmv_action(){
ControlRef	c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_rmv_field,this);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::_add_field(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void* clientData){
bArray*	arr=(bArray*)clientData;
	arr->add(&item);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::_rmv_field(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void* clientData){
bXMapStringProcessing*	editor=(bXMapStringProcessing*)clientData;
ControlRef				c=editor->get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapStringProcessingDBProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStringProcessing::report(){
ControlRef		c=get_control(kXMapStringProcessingSignature,kXMapStringProcessingRightListID);
bArray			rev(sizeof(int));
int				fid,fidx;
bGenericType*	tp=_gapp->typesMgr()->get(_prm.tp);

	_prm.arr->reset();
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_field,_prm.arr);	
	for(int i=_prm.arr->count();i>0;i--){
		_prm.arr->get(i,&fid);
		fidx=tp->fields()->get_index(fid);
		rev.add(&fidx);
	}
	_prm.arr->reset();
	(*(_prm.arr))+rev;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapStringProcessing::process_str(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapStringProcessingMessageID,
				GetSignature(this));	
	_gapp->layersMgr()->SetObjInvalidation(false);
	process_str(&_prm,false);
	_gapp->layersMgr()->SetObjInvalidation(true);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapStringProcessing::process_str(bool dummy){
_bTrace_("bXMapStringProcessing::process_str",true);
strprocess_prm			p;
bGenericXMLBaseElement*	elt;
bGenericXMLBaseElement*	cnt;
bGenericType*			tp;
int						idx;
char					value[_values_length_max_];
bArray					arr(sizeof(int));

	p.tp=SelectionIsMonoType(_gapp);
	if(p.tp==0){
_te_("bad selection");
        return(false);
	}

	tp=_gapp->typesMgr()->get(p.tp);
	p.arr=&arr;
	
// Champs
	cnt=getelement(1);
	if(!cnt){
_te_("no field container");
		return(false);
	}
	for(int i=1;i<=cnt->countelements();i++){
		elt=cnt->getelement(i);
		elt->getvalue(value);
		idx=tp->fields()->get_index(value);
		if(idx==0){
_te_("bad field "+value);
			return(false);
		}
		arr.add(&idx);
	}
	if(arr.count()==0){
_te_("no field");
		return(false);
	}
// Séparateur
	elt=getelement(2);
	if(!elt){
_te_("no separator");
		return(false);
	}
	elt->getvalue(p.sep);
// Casse
	elt=getelement(3);
	if(!elt){
_te_("no case");
		return(false);
	}
	elt->getvalue(value);
	p.cas=atoi(value);
	if((p.cas<kStrProcessCaseDontChange)||(p.cas>kStrProcessCaseFWUpper)){
_te_("bad case "+p.cas);
		return(false);
	}
// Rechercher
	elt=getelement(4);
	if(!elt){
_te_("no search");
		return(false);
	}
	elt->getvalue(p.srch);
char pat[4]={'<','>','&',0};
    
//    if(strcmp(pat,p.srch)==0){
//        strcpy(p.srch," ");
//    }
    _tm_("tst :"+pat[0]+"="+(pat[0]/16)+(pat[0]%16));
    _tm_("tst :"+pat[1]+"="+(pat[1]/16)+(pat[1]%16));
    _tm_("tst :"+pat[2]+"="+(pat[2]/16)+(pat[2]%16));
    
_tm_("search string :"+p.srch);
// Remplacer
	elt=getelement(5);
	if(!elt){
_te_("no replacement");
		return(false);
	}
	elt->getvalue(p.repl);
//    if(strcmp(pat,p.repl)==0){
//        strcpy(p.repl," ");
//    }
_tm_("replacement string :"+p.repl);
// Format
	elt=getelement(6);
	if(!elt){
_te_("no format");
		return(false);
	}
	elt->getvalue(p.fmt);
// Champ à remplir
	elt=getelement(7);
	if(!elt){
_te_("no target field");
		return(false);
	}
	elt->getvalue(value);
	p.target=tp->fields()->get_index(value);
	if(p.target==0){
_te_("bad target field "+value);
		return(false);
	}

	return(process_str(&p,true));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapStringProcessing::process_str(strprocess_prm* prm, bool silent){
_bTrace_("bXMapStringProcessing::process_str(strprocess_prm*,bool)",true);
int					j,err=0,fidx;
char				val[__MESSAGE_STRING_LENGTH_MAX__];
char				buffer[__MESSAGE_STRING_LENGTH_MAX__*3+1];
bArray				sel(*(_gapp->selMgr()->elements()));
bGenericGeoElement*	o;

	b_message_string(kXMapStringProcessingProcessMessageID,val,getbundle(),1);
bProgressWait wt("",val,true,true,sel.count());

int	fpos=0;
int	flen=formatinfo(prm->fmt,&fpos);
	
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		sel.get(i,&o);
		buffer[0]=0;
		
// Concaténer
		for(j=1;j<=prm->arr->count()-1;j++){
			prm->arr->get(j,&fidx);
			val[0]=0;
			if(o->getValue(fidx,val)==false){
_te_("o->getValue for field "+fidx);
				err++;
				break;
			}
			if(strncat(buffer,val,__MESSAGE_STRING_LENGTH_MAX__*3)==NULL){
_te_("strncat "+buffer+"+"+val);
				err++;
				break;
			}
			if(strncat(buffer,prm->sep,__MESSAGE_STRING_LENGTH_MAX__*3)==NULL){
_te_("strncat "+buffer+"+"+prm->sep);
				err++;
				break;
			}
//_tm_(buffer);
		}
		prm->arr->get(prm->arr->count(),&fidx);
		val[0]=0;
		if(o->getValue(fidx,val)==false){
_te_("o->getValue for field "+fidx);
			err++;
		}
		else if(strncat(buffer,val,__MESSAGE_STRING_LENGTH_MAX__*3)==NULL){
_te_("strncat "+buffer+"+"+val);
			err++;
		}
// Changement de casse 
		switch(prm->cas){
			case kStrProcessCaseLower:
				strlower(buffer);
				break;
			case kStrProcessCaseUpper:
				strupper(buffer);
				break;
			case kStrProcessCaseFUpper:
				strlower(buffer);
				strupperfcs(buffer);
				break;
			case kStrProcessCaseFWUpper:
				strlower(buffer);
				strupperfcw(buffer);
				break;
		}
// Rechercher-Remplacer
		if(strlen(prm->srch)>0){
			strrep(buffer,prm->srch,prm->repl);
		}
// Formater
		if(flen>0){
			strformat(prm->fmt,buffer,fpos,flen);
		}
// Affecter
		if(o->setValue(prm->target,buffer)==false){
_te_("o->setValue "+buffer+" for field "+fidx);
			err++;
		}
	}

	if(err){
		b_message_string(kXMapStringProcessingErrorsOccursID,val,getbundle(),0);
		b_message_string(kXMapStringProcessingReadLogID,buffer,getbundle(),0);
bAlertStop	alrt(val,buffer,silent);
	}

	return(true);
}

