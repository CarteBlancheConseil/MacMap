//----------------------------------------------------------------------------
// File : bXMapViewMgr.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, views management
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
// 29/05/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapViewMgr.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct deletePrm{
	ControlRef 			c;
	DataBrowserItemID	id;
}deletePrm;

// ---------------------------------------------------------------------------
// 
// -----------
static void _viewDelete(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void *clientData){
deletePrm*	prm=(deletePrm*)clientData;
	prm->id=item;
	(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kXMapViewMgrNameProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
static void _findMaxId(	DataBrowserItemID item, 
						DataBrowserItemState state, 
						void *clientData){
DataBrowserItemID*	max=(DataBrowserItemID*)clientData;
	if(item>(*max)){
		(*max)=item;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
static DataBrowserItemID findMaxId(ControlRef c){
DataBrowserItemID	item=0;
DataBrowserItemUPP	upp=NewDataBrowserItemUPP((DataBrowserItemProcPtr)_findMaxId);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,upp,&item);
	DisposeDataBrowserItemUPP(upp);
	return(item);
}

// ---------------------------------------------------------------------------
// 
// -----------
static void _getSelectedId(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void *clientData){
DataBrowserItemID*	id=(DataBrowserItemID*)clientData;
	(*id)=item;
}

// ---------------------------------------------------------------------------
// 
// -----------
static DataBrowserItemID getSelectedId(ControlRef c){
DataBrowserItemID	item=0;
DataBrowserItemUPP	upp=NewDataBrowserItemUPP((DataBrowserItemProcPtr)_getSelectedId);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&item);
	DisposeDataBrowserItemUPP(upp);
	return(item);
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
static int id_comp(const void* a, const void* b){
int*	ia=(int*)a;
int*	ib=(int*)b;
	if((*ib)>(*ia)){
		return(1);
	}
	if((*ib)<(*ia)){
		return(-1);
	}
	return(0);
	//return((*ib)-(*ia));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapViewMgr::bXMapViewMgr(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_arr(sizeof(int)){
	setclassname("viewmgr");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapViewMgr::~bXMapViewMgr(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapViewMgr::create(bGenericXMLBaseElement* elt){
	return(new bXMapViewMgr(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapViewMgr::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapViewMgr::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapViewMgr::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
	
	
	SetPortWindowPort(wd);
	
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapViewMgrSignature,kXMapViewMgrViewListCmd));
				_arr.reset();
				break;

			case kXMapViewMgrNewCmd:
				add_view();
				break;
		
			case kXMapViewMgrRenameCmd:
				rename_view();
				break;

			case kXMapViewMgrDuplicateCmd:
				duplicate_view();
				break;
				
			case kXMapViewMgrDeleteCmd:
				delete_view();
				break;

			case kXMapViewMgrViewListCmd:
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
void bXMapViewMgr::wd_init(WindowRef wd){
ControlRef	c;
	
	_arr.reset();

	c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
	DataBrowserInit(c,view_handler,view_notifier,(long)this);
	SetDataBrowserPropertyFlags(c,kXMapViewMgrNameProperty,kDataBrowserPropertyIsEditable);
	SetDataBrowserSortOrder(c,kDataBrowserOrderIncreasing);

	populate_views();	
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapViewMgr::view_handler(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){

bXMapViewMgr*		editor=(bXMapViewMgr*)GetControlReference(browser);
CFStringRef			cfname;
char				name[256];
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)editor->getapp();

	switch(property){
		case kXMapViewMgrNameProperty:
			if(changeValue){
				GetDataBrowserItemDataText(itemData,&cfname);
				CFStringGetCString(cfname,name,256,kCFStringEncodingMacRoman);
				if(!gapp->viewMgr()->set_name(editor->id_to_idx(itemID),name)){
				}
				CFRelease(cfname);
			}
			else{
				gapp->viewMgr()->get_name(editor->id_to_idx(itemID),name);
				cfname=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
				SetDataBrowserItemDataText(itemData,cfname);
				CFRelease(cfname);
			}
			break;
		case kDataBrowserItemIsEditableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
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
void bXMapViewMgr::view_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapViewMgr*	editor=(bXMapViewMgr*)GetControlReference(browser);
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
			editor->view_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->view_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::populate_views(){
int					n;
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);

	n=_gapp->viewMgr()->count();
	for(itemId=1;itemId<=n;itemId++){		
		if(AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapViewMgrNameProperty)){
		}
		id_add(itemId);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::reset_views(){
DataBrowserPropertyID	pid=kXMapViewMgrNameProperty;
ControlRef				c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
	
	DataBrowserReset(c,1,&pid);
	_arr.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::view_action(DataBrowserItemID item){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::add_view(){
ControlRef			c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
DataBrowserItemID	item=0;
	
	item=findMaxId(c)+1;
	if(_gapp->viewMgr()->append()){
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapViewMgrNameProperty);
		id_add(item);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::rename_view(){
ControlRef		c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
	SetDataBrowserEditItem(c,getSelectedId(c),kXMapViewMgrNameProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::duplicate_view(){
ControlRef			c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
DataBrowserItemID	item=getSelectedId(c);
	if(_gapp->viewMgr()->duplicate(id_to_idx(item))){	
		item=findMaxId(c)+1;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapViewMgrNameProperty);
		id_add(item);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::delete_view(){
deletePrm			prm;
	prm.c=get_control(kXMapViewMgrSignature,kXMapViewMgrViewListID);
	prm.id=0;
	prm.id=getSelectedId(prm.c);
	if(_gapp->viewMgr()->remove(id_to_idx(prm.id))){
		DataBrowserItemUPP	upp=NewDataBrowserItemUPP((DataBrowserItemProcPtr)_viewDelete);
		(void)ForEachDataBrowserItem(prm.c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&prm);
		DisposeDataBrowserItemUPP(upp);
		id_rmv(prm.id);
	}
	else{
		NSBeep();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bXMapViewMgr::id_to_idx(DataBrowserItemID itemID){
int	k=_arr.search(&itemID,id_comp);
	if(k){
		_arr.get(k,&k);
	}
	return(k);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::id_add(DataBrowserItemID itemID){
	_arr.add(&itemID);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapViewMgr::id_rmv(DataBrowserItemID itemID){
int	k=_arr.search(&itemID,id_comp);
	if(k){
		_arr.rmv(k);
	}
}

