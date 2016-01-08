//----------------------------------------------------------------------------
// File : bXMapDataJoin.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, data join from DBF, text, csv, ... file
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
// 29/06/2006 creation.
// 11/02/2015 NAV to NS transformation.
//----------------------------------------------------------------------------

#include "bXMapDataJoin.h"

#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/MacMapSuiteCPP.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct val_rec{
	int		fk;
	void*	fdat;
	int		tk;
	void*	tdat;
}val_rec;

typedef struct it_prm{
	int		n;
	int		msk;
	bArray*	ft;
	int		fx;
	int		sz;
}it_prm;

typedef struct g_rec{
	bGenericGeoElement*	o;
	void*				val;
}g_rec;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataJoin	::bXMapDataJoin(bGenericXMLBaseElement* elt, 
								bGenericMacMapApp* gapp, 
								CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_cols(sizeof(datajoin_act)){
	setclassname("datajoin");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataJoin::~bXMapDataJoin(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDataJoin::create(bGenericXMLBaseElement* elt){
	return(new bXMapDataJoin(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDataJoin::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
	_tpi=1;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataJoin::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			join(false);
			break;
		case kExtProcessCallWithParams:
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataJoin::edit(void* prm){
_bTrace_("bXMapDataJoin::edit",false);
int             status;
char            name[FILENAME_MAX];
char            ext[FILENAME_MAX];
char            path[SHRT_MAX];
char            msg[__MESSAGE_STRING_LENGTH_MAX__];
const char*		nsft[4]={"TXT","CSV","DBF","MMT"};
const char**	pnsft=nsft;
    
    if(NSGetFilePath(pnsft,
                     4,
                     cNSUTF8StringEncoding,
                     false,
                     NULL,
                     path)==cNSCancelButton){
_te_("user canceled");
        return(true);
    }
    splitPath(path,path,name);
char	*bf=strrchr(name,'.');
    if(bf){
        bf++;
        strcpy(ext,bf);
    }
int	sign=ext2sign(ext);
_tm_(path);
_tm_(name);
_tm_(ext);
_tm_(sign);

	if(sign==0){
_te_("unsupported format");
b_error_string(error_num(0,kXMapDataJoinUnsupportedFormatErr),msg,getbundle());
bAlertStop(msg,"");
		return(true);
	}
	status=0;
	_tbl=wtbl_alloc(sign,
					path,
					name,
					false, 
					_gapp->document()->dist_resolution(),
					_gapp->document()->srid(),
					&status);
	if(!_tbl){
_te_("table not allocated");
b_error_string(error_num(0,kXMapDataJoinOpenErr),msg,getbundle());
bAlertStop(msg,"");
		return(true);
	}
	if(status<0){
_te_("wtbl_alloc failed with "+status);
b_error_string(error_num(0,kXMapDataJoinOpenErr),msg,getbundle());
bAlertStop(msg,"");
		wtbl_free(_tbl);
		_tbl=NULL;
		return(true);
	}
/**/	
	strcpy(_tpth,path);
	strcpy(_tnam,name);
/**/	
	return(bStdXMapWithModalIntf::edit(prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataJoin::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
	
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID));
				break;
				
			case kXMapDataJoinTypesCmd:
				type_action();
				break;
	
			case kXMapDataJoinActionCmd:
				act_action();
				break;
				
			case kXMapDataJoinFieldsCmd:
				field_action();
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
void bXMapDataJoin::wd_init(WindowRef wd){
	_cur=0;

	populate_act();
	
ControlRef	c;

	c=get_control(kXMapDataJoinSignature,kXMapDataJoinTypesID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,0,_tpi);

	c=get_control(kXMapDataJoinSignature,kXMapDataJoinListID);
	DataBrowserInit(c,col_handler,col_notifier,(long)this);
	AddDataBrowserItems(c,
						kDataBrowserNoItem,
						_tbl->CountFields(),
						NULL,
						kXMapDataJoinColumnsProperty);
						
	c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
	ClearPopupControl(c,1);
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
	PopulatePopupControlWithFields(tp,c,kOBJ_Dir_+1,1);
	for(int i=1;i<=tp->fields()->count();i++){
		if(tp->fields()->is_dyn(kOBJ_Dir_+i)){
			DisablePopupControlMenuItem(c,i);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapDataJoin::col_handler(ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){

CFStringRef			name;
char				cname[256];
bXMapDataJoin*		x=(bXMapDataJoin*)GetControlReference(browser);
	
	switch(property){
		case kXMapDataJoinColumnsProperty:
			if(!changeValue){
				x->_tbl->FieldName(itemID,cname);
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
			if(SetDataBrowserItemDataBooleanValue(itemData,false)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::col_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapDataJoin*	x=(bXMapDataJoin*)GetControlReference(browser);
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
			x->col_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				x->col_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::col_action(DataBrowserItemID item){
ControlRef	c=get_control(kXMapDataJoinSignature,kXMapDataJoinActionID);
	_cur=item;
	if(!test_search(_cur)){
		DisablePopupControlMenuItem(c,1);
	}
	else{
		EnablePopupControlMenuItem(c,1);
	}
	if(!test_create(_cur)){
		DisablePopupControlMenuItem(c,2);
	}
	else{
		EnablePopupControlMenuItem(c,2);
	}
	if(_cur){
datajoin_act	action;
		_cols.get(_cur,&action);
		if(action.k==kDataJoinActionSearch){
			SetControl32BitValue(c,1);
		}
		else{
			SetControl32BitValue(c,2);
		}
		SetControlAbility(c,true);
		c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
		if(action.fidx>0){
			SetControl32BitValue(c,(action.fidx-kOBJ_Dir_));
		}
		else{
			SetControl32BitValue(c,1);
		}
		if(action.k==kDataJoinActionCreate){
			SetControlAbility(c,false);
		}
		else{
			SetControlAbility(c,true);
		}
	}
	else{
		SetControlAbility(c,false);
		c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
		SetControlAbility(c,false);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::type_action(){
ControlRef	c;

	c=get_control(kXMapDataJoinSignature,kXMapDataJoinTypesID);
	_tpi=GetControl32BitValue(c);
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
	c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithFields(tp,c,kOBJ_Dir_+1,1);
	for(int i=1;i<=tp->fields()->count();i++){
		if(tp->fields()->is_dyn(kOBJ_Dir_+i)){
			DisablePopupControlMenuItem(c,i);
		}
	}
	populate_act();
	
//	(void)SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsAssign);
//	_cur=0;

	col_action(_cur);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::act_action(){
_bTrace_("bXMapDataJoin::act_action()",true);
	if(!_cur){
_tm_("no _cur");
		return;
	}
ControlRef		c;
	c=get_control(kXMapDataJoinSignature,kXMapDataJoinActionID);
datajoin_act	action;
	_cols.get(_cur,&action);
	if(GetControl32BitValue(c)==1){
		action.k=kDataJoinActionSearch;
	}
	else{
		action.k=kDataJoinActionCreate;
	}
	_cols.put(_cur,&action);
	if(action.k==kDataJoinActionSearch){
		field_action();
	}
	
	c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
	if(action.k==kDataJoinActionCreate){
		SetControlAbility(c,false);
	}
	else{
		SetControlAbility(c,true);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::field_action(){
_bTrace_("bXMapDataJoin::field_action()",true);
	if(!_cur){
_tm_("no _cur");
		return;
	}
ControlRef		c;
	c=get_control(kXMapDataJoinSignature,kXMapDataJoinFieldsID);
datajoin_act	action;
	_cols.get(_cur,&action);
	
	action.fidx=GetControl32BitValue(c)+kOBJ_Dir_;
	_cols.put(_cur,&action);
_tm_("field "+action.fidx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::populate_act(){
	_cols.reset();

bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
wbase			bse=tp->fields();
char			name[256];
datajoin_act	action;
bool			srch=false;

	for(int i=1;i<=_tbl->CountFields();i++){
		_tbl->FieldName(i,name);
		action.fidx=bse->get_index(name);
		if(action.fidx){
			if(srch){
				action.k=kDataJoinActionCreate;
			}
			else{
				action.k=kDataJoinActionSearch;
				srch=true;
			}
		}
		else{
			action.k=kDataJoinActionCreate;
		}
		_cols.add(&action);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataJoin::test_search(int f){
	if(!f){
		return(false);
	}
datajoin_act	action;
bool	srch=false;
int		i;
	for(i=1;i<=_cols.count();i++){
		_cols.get(i,&action);
		if(action.k==kDataJoinActionSearch){
			srch=true;
			break;
		}
	}
	return((srch&&(f==i))||(!srch));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataJoin::test_create(int f){
	if(!f){
		return(false);
	}
datajoin_act	action;
bool	srch=false;
int		i;
	for(i=1;i<=_cols.count();i++){
		_cols.get(i,&action);
		if(action.k==kDataJoinActionSearch){
			srch=true;
			break;
		}
	}
	return((srch&&(f==i))||(!srch));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataJoin::join(bool mode){
_bTrace_("bXMapDataJoin::join",false);

bEventLog	log(_gapp,
				getbundle(),
				kXMapDataJoinMessageID,
				GetSignature(this));

char	msg[__ERROR_STRING_LENGTH_MAX__];

datajoin_act	action;
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
wbase			bse=tp->fields();
int				i,x=0;

// Allocation buffers et recherche de la clef 
	for(i=1;i<=_cols.count();i++){
		_cols.get(i,&action);

		if(action.k==kDataJoinActionSearch){
			x=i;
			break;
		}
	}
_tm_(x);

	if(x>0){
_tm_("field "+action.fidx);
		if(bse->add_dyn(_tpth,_tnam,x,action.fidx)){
			b_error_string(error_num(0,kXMapDataJoinKeyNotDefinedErr),msg,getbundle());
bAlertStop	alrt(msg,"",mode);
		}
	}
	else{
		b_error_string(error_num(0,kXMapDataJoinKeyNotDefinedErr),msg,getbundle());
bAlertStop	alrt(msg,"",mode);
	}
	
	log.close();
	_cols.reset();
	wtbl_free(_tbl);
	_tbl=NULL;	
}

