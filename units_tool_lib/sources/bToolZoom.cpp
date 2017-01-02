//----------------------------------------------------------------------------
// File : bToolZoom.cpp
// Project : MacMap
// Purpose : C++ source file : Zoom tool class
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
// 05/01/2005 creation.
//----------------------------------------------------------------------------

#include "bToolZoom.h"
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/NSContextualMenuWrappers.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// -----------
typedef struct rmv_prm{
	ControlRef 			c;
	DataBrowserItemID	id;
}rmv_prm;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolZoom	::bToolZoom(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolNav(elt,gapp,bndl)
			,_scales(sizeof(scale)){
	setclassname("zoomtool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolZoom::~bToolZoom(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolZoom::create(bGenericXMLBaseElement* elt){
	return(new bToolZoom(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::open(int* flags){	
	set_use_trace(true);
	bStdToolNav::open(flags);
	_cminus=load_curs("OutCursor.png");
	set_mnu_cmd(1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::close(){
	bStdToolNav::close();
	if(_cminus){
		ReleaseCocoaCursor(_cminus);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::pop(cocoaMenuPrm prm){
int				cmd,x;
bGenericUnit*	u;
long			nb=_gapp->scaleMgr()->count();
menuitem_desc*	desc=new menuitem_desc[nb];
	
	x=_gapp->scaleMgr()->get_current();
	for(int i=1;i<=nb;i++){
		u=_gapp->scaleMgr()->get(i);
		u->long_name(desc[i-1].name);
		if(strlen(desc[i-1].name)<=0){
			sprintf(desc[i-1].name,"1/%.0f",u->coef());
		}
		desc[i-1].checked=(i==x);
        desc[i-1].disabled=false;
        desc[i-1].index=0;
	}
	if(prm.wdIsCocoa){
		cmd=popUpContextMenuWithCGPointAndNSWindow(prm.location,
												   prm.inWindow,
												   desc,
												   nb);		
	}
	else{
		cmd=popUpContextMenuWithCGPointAndWindowRef(prm.location,
													(WindowRef)prm.inWindow,
													desc,
													nb);		
	}
	delete[] desc;
	if((cmd>0)&&(cmd!=x)){
i2dvertex	vx;
		Convert(&vx,&prm.location);
		if(_gapp->scaleMgr()->set_current(cmd)){
			_gapp->mapIntf()->setScreenCenter(vx);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::clic(CGPoint pt, int count){
//_bTrace_("bToolZoom::clic(CGPoint, int)",true);
	bStdToolNav::clic(pt,count);
i2dvertex	vx;
	get_clic(&vx);
	if(is_modifiers(shiftKey)||is_modifiers(optionKey)){
		if(_gapp->scaleMgr()->set_current(_gapp->scaleMgr()->get_current()+1)){
		}
	}
	else{
		if(_gapp->scaleMgr()->set_current(_gapp->scaleMgr()->get_current()-1)){
		}
	}
	_gapp->mapIntf()->setScreenCenter(vx);
	set_cur(&pt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::update(bool global){
	if(get_active()&&get_use_track()){
		if(!global){
			clearTempPathContext(false);
		}
CGPoint	a;
		get_cur(&a);
		if((a.x!=SHRT_MIN)&&(_gapp->scaleMgr()->get_current()>1)){
bGenericUnit*	scl=_gapp->scaleMgr()->get();
bGenericUnit*	scl2=_gapp->scaleMgr()->get(_gapp->scaleMgr()->get_current()-1);		
double			d=scl2->coef()/scl->coef();
CGRect			cgr=getTempPathContextRect();
			cgr.size.width*=d;
			cgr.size.height*=d;	
			hilite_rect(getTempPathContext(),CGRectOffset(cgr,a.x-CGRectGetMidX(cgr),a.y-CGRectGetMidY(cgr)),true,true);
		}
		if(!global){
			validTempPathContext();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::set_modifiers(int k){
	bStdToolNav::set_modifiers(k);
	if(!get_active()){
		return;
	}
	if(is_modifiers(shiftKey)||is_modifiers(optionKey)){
		set_curs(_cminus);
		set_use_track(false);
		clearTempPathContext(true);
	}
	else{
		set_curs();
		set_use_track(true);
		clearTempPathContext(true);
		update(false);
	}
	validTempPathContext();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolZoom::edit_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
ControlID	id={kZoomEditSign,kZoomScalesID};
ControlRef	c;

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				dump();
				_scales.reset();
				GetControlByID(wd,&id,&c);
				DataBrowserClose(c);
				break;
			case kHICommandCancel:
				_scales.reset();
				GetControlByID(wd,&id,&c);
				DataBrowserClose(c);
				break;
			case kZoomScalesCmd:
				break;
			case kZoomRmvCmd:
				remove(wd);
				break;
			case kZoomAddCmd:
				add(wd);
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
// -----------
void bToolZoom::edit_init(WindowRef wd){
ControlRef		c;
ControlID		cid={kZoomEditSign,0};
bGenericUnit*	u;
scale			scl;
	
	for(int i=1;i<=_gapp->scaleMgr()->count();i++){
		u=_gapp->scaleMgr()->get(i);
		scl.idx=i;
		scl.modified=false;
		scl.created=false;
		scl.deleted=false;
		scl.value=u->coef();
		u->long_name(scl.name);
		scl.u=u;
		_scales.add(&scl);
	}
		
	cid.id=kZoomScalesID;
	GetControlByID(wd,&cid,&c);
	DataBrowserInit(c,dtb_proc,NULL,(long)this);
	for(DataBrowserItemID itemId=1;itemId<=_gapp->scaleMgr()->count();itemId++){
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kZoomScalesValueProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kZoomScalesNameProperty);
	}
	SetDataBrowserPropertyFlags(c,kZoomScalesValueProperty,kDataBrowserPropertyIsEditable);
	SetDataBrowserPropertyFlags(c,kZoomScalesNameProperty,kDataBrowserPropertyIsEditable);
}

// ---------------------------------------------------------------------------
// Contextual menu for tool choice
// -----------
void bToolZoom::pop_tools(CGPoint location,
						  WindowRef inWindow){
	if(is_modifiers(controlKey)){
cocoaMenuPrm prm;
		prm.location=location;
		prm.inWindow=inWindow;
		prm.wdIsCocoa=false;
		pop(prm);
	}
	else{
		bStdToolNav::pop_tools(location,inWindow);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bToolZoom::dtb_proc(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){
bToolZoom*			t=(bToolZoom*)GetControlReference(browser);
char				buff[256];
CFStringRef			cfs;
scale				scl;
	
	switch(property){
		case kZoomScalesValueProperty:
			if(!changeValue){
				if(t->get_scale(itemID,&scl)){
					sprintf(buff,"%.0f",scl.value);
					cfs=CFStringCreateWithCString(kCFAllocatorDefault,buff,CFStringGetSystemEncoding());
					SetDataBrowserItemDataText(itemData,cfs);
					CFRelease(cfs);
				}
			}
			else{
				if(t->get_scale(itemID,&scl)){
					GetDataBrowserItemDataText(itemData,&cfs);
					CFStringGetCString(cfs,buff,256,kCFStringEncodingMacRoman);
					CFRelease(cfs);
					scl.value=matof(buff);
					if(scl.value>0){
						if((!scl.created)&&(!scl.deleted)){
							scl.modified=true;
							
						}
						t->put_scale(itemID,&scl);
					}
				}
			}
			break;
			
		case kZoomScalesNameProperty:
			if(!changeValue){
				if(t->get_scale(itemID,&scl)){
					sprintf(buff,"%s",scl.name);
					if(strlen(buff)==0){
						sprintf(buff,"_");
					}
					cfs=CFStringCreateWithCString(kCFAllocatorDefault,buff,CFStringGetSystemEncoding());
					SetDataBrowserItemDataText(itemData,cfs);
					CFRelease(cfs);
				}
			}
			else{
				if(t->get_scale(itemID,&scl)){
					GetDataBrowserItemDataText(itemData,&cfs);
					CFStringGetCString(cfs,buff,256,kCFStringEncodingMacRoman);
					CFRelease(cfs);
					if((strlen(buff)==1)&&(buff[0]=='_')){
						buff[0]=0;
					}
					sprintf(scl.name,"%s",buff);
					if((!scl.created)&&(!scl.deleted)){
						scl.modified=true;
					}
					t->put_scale(itemID,&scl);
				}
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
			
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolZoom::get_scale(int idx, scale* scl){	
	for(int i=1;i<=_scales.count();i++){
		_scales.get(i,scl);
		if(scl->idx==idx){
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolZoom::put_scale(int idx, scale* scl){
scale	lscl;
	
	for(int i=1;i<=_scales.count();i++){
		_scales.get(i,&lscl);
		if(lscl.idx==idx){
			_scales.put(i,scl);
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolZoom::rmv_scale(int idx){
scale	scl;
	
	for(int i=1;i<=_scales.count();i++){
		_scales.get(i,&scl);
		if(scl.idx==idx){
			scl.modified=false;
			scl.created=false;
			scl.deleted=true;
			_scales.put(i,&scl);
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolZoom::add_scale(scale* scl){
int		max=0;
scale	lscl;
	
	for(int i=1;i<=_scales.count();i++){
		_scales.get(i,&lscl);
		if(lscl.idx>max){
			max=lscl.idx;
		}
	}
	scl->idx=max+1;
	scl->created=true;
	scl->modified=false;
	scl->deleted=false;
	return(_scales.add(scl));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::add(WindowRef wd){
ControlRef			c;
ControlID			id={kZoomEditSign,kZoomScalesID};
DataBrowserItemID	item=0;
scale				scl;
	
	GetControlByID(wd,&id,&c);
	scl.name[0]=0;
	scl.value=100000;
	if(add_scale(&scl)){
		item=scl.idx;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kZoomScalesValueProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kZoomScalesNameProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::remove(WindowRef wd){
ControlID	id={kZoomEditSign,kZoomScalesID};
rmv_prm		prm;
	
	prm.id=0;
	GetControlByID(wd,&id,&prm.c);
	DataBrowserItemUPP	upp=NewDataBrowserItemUPP((DataBrowserItemProcPtr)_remove);
	(void)ForEachDataBrowserItem(prm.c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&prm);
	DisposeDataBrowserItemUPP(upp);
	rmv_scale(prm.id);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::_remove(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void *clientData){
rmv_prm *prm=(rmv_prm*)clientData;
	prm->id=item;
	(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kZoomScalesValueProperty);
	(void)RemoveDataBrowserItems(prm->c,kDataBrowserNoItem,1,&item,kZoomScalesNameProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolZoom::dump(){
int		i,k;
scale	scl;
	
// Modified	
	for(i=_scales.count();i>0;i--){
		_scales.get(i,&scl);
		if(scl.modified){
			k=RecoverScale(_gapp,scl.u);
			_gapp->scaleMgr()->set(k,scl.name,scl.value);
		}
	}
	
// Created	
	for(i=_scales.count();i>0;i--){
		_scales.get(i,&scl);
		if(scl.created){
			_gapp->scaleMgr()->add(scl.name,scl.value);
		}
	}

// Deleted	
	for(i=_scales.count();i>0;i--){
		_scales.get(i,&scl);
		if(scl.deleted){
			k=RecoverScale(_gapp,scl.u);
			_gapp->scaleMgr()->rmv(k);
		}
	}
}

