//----------------------------------------------------------------------------
// File : bXMapToolMenuConfig.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, contextual tool menu configuration
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
// 08/06/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapToolMenuConfig.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/endian.h>
#include <std_ext/bStdTool.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>
#include "mm_config.h"
											
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapToolMenuConfig	::bXMapToolMenuConfig(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMapWithModalIntf(elt,gapp,bndl){
		setclassname("toolmenuconfig");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapToolMenuConfig::~bXMapToolMenuConfig(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapToolMenuConfig::create(bGenericXMLBaseElement* elt){
	return(new bXMapToolMenuConfig(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapToolMenuConfig::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapToolMenuConfig::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			break;
		case kExtProcessCallWithParams:
			break;
		case kExtProcessCallGetData:
			break;
		case kExtProcessCallSetData:
			break;
		case kExtProcessCallWithXMLTree:
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapToolMenuConfig::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss;	
			
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				write_p();
				DataBrowserClose(get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID));
				DataBrowserClose(get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID));
				break;
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID));
				DataBrowserClose(get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID));
				break;
			
			case kXMapToolMenuConfigAddCmd:
				add_action();
				break;
			case kXMapToolMenuConfigInsertCmd:
				insert_action();
				break;
			case kXMapToolMenuConfigAllCmd:
				addall_action();
				break;
			case kXMapToolMenuConfigRemoveCmd:
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
void bXMapToolMenuConfig::wd_init(WindowRef wd){
ControlRef	c;

	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID);
	DataBrowserInit(c,tool_handler,NULL,(long)this);
	populate_left();
	
	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	DataBrowserInit(c,tool_handler,NULL,(long)this);
	populate_right();

}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapToolMenuConfig::tool_handler(	ControlRef browser, 
											DataBrowserItemID itemID, 
											DataBrowserPropertyID property, 
											DataBrowserItemDataRef itemData, 
											Boolean changeValue){
CFStringRef	name;
char		cname[256];
	
	switch(property){
		case kXMapToolMenuConfigDBProperty:
			if(!changeValue){
				_gapp->toolMgr()->ext_name(itemID,cname);
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
// 
// -----------
void bXMapToolMenuConfig::populate_left(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID);
	
	for(int i=1;i<=_gapp->toolMgr()->count();i++){
		itemId=i;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapToolMenuConfigDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::populate_right(){
DataBrowserItemID		itemId;
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
char					val[_values_length_max_];
UInt32					sgn;
ControlRef				c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);	
	
	_gapp->document()->readTree(&root,kStdToolConfigSign,kStdToolMenuConfig);
	if(root){
		for(int i=1;i<=_gapp->classMgr()->CountElements(root,"sign");i++){
			elt=_gapp->classMgr()->NthElement(root,i,"sign");
			if(!elt){
				break;
			}
			elt->getvalue(val);
			sgn=*((UInt32*)val);
#ifdef __LITTLE_ENDIAN__
			LBSwapWord(&sgn,sizeof(UInt32));
#endif
			itemId=_gapp->toolMgr()->ext_index(sgn);
			if(itemId!=0){
				AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapToolMenuConfigDBProperty);
			}
		}
		_gapp->classMgr()->ReleaseXMLInstance(root);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::db_resets(){
ControlRef	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapToolMenuConfigDBProperty);
	SetDataBrowserScrollPosition(c,0,0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::add_action(){
ControlRef			c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID);
bArray				arr(sizeof(DataBrowserItemID));
DataBrowserItemID	item;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_tool,&arr);
	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	for(int i=arr.count();i>0;i--){
		arr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapToolMenuConfigDBProperty);
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::insert_action(){
ControlRef			c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigLeftListID);
bArray				arrl(sizeof(DataBrowserItemID)),arrr(sizeof(DataBrowserItemID)),rev(sizeof(DataBrowserItemID));
DataBrowserItemID	item;
int					i,j,idx;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_tool,&arrl);
	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_tool,&arrr);
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

	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapToolMenuConfigDBProperty);
	for(int i=1;i<=arrr.count();i++){
		arrr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapToolMenuConfigDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::addall_action(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	
	for(int i=1;i<=_gapp->toolMgr()->count();i++){
		itemId=i;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapToolMenuConfigDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::rmv_action(){
ControlRef	c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_rmv_tool,this);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::_add_tool(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void* clientData){
bArray*	arr=(bArray*)clientData;
	arr->add(&item);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapToolMenuConfig::_rmv_tool(	DataBrowserItemID item, 
										DataBrowserItemState state, 
										void* clientData){
bXMapToolMenuConfig*	editor=(bXMapToolMenuConfig*)clientData;
ControlRef				c=editor->get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapToolMenuConfigDBProperty);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapToolMenuConfig::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	getclassname(name);

	if(!_gapp->document()->writeTree(root,kStdToolConfigSign,kStdToolMenuConfig)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapToolMenuConfig::make_p(){
ControlRef			c=get_control(kXMapToolMenuConfigSignature,kXMapToolMenuConfigRightListID);
bArray				tools(sizeof(DataBrowserItemID));
DataBrowserItemID	item;
char				val[_values_length_max_];
UInt32				sgn;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_tool,&tools);
	
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

	for(int i=tools.count();i>0;i--){
		tools.get(i,&item);
		sgn=_gapp->toolMgr()->ext_signature(item);
		sprintf(val,"%.4s",&sgn);
		add_cdesc(arr,1,"sign",val);
	}
	
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

