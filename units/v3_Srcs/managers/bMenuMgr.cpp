//----------------------------------------------------------------------------
// File : bMenuMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Menu management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 29/11/2005 creation.
//----------------------------------------------------------------------------

#include "bMenuMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/endian.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMenuMgr::bMenuMgr(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMenuMgr::~bMenuMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMenuMgr::track(EventRef evt){
//_bTrace_("bMenuMgr::track",true);
	if(evt==NULL){
MenuRef			mnu=GetMenuRef(2);
bGenericExt*	ext;
UInt32			cmd;
		
		cmd='undo';
//		cmd='und3';
//		EnableMenuCommand(mnu,cmd);
//_tm_("nb und3 :"+(int)CountMenuItemsWithCommandID(mnu,cmd));
		ext=find_ext(cmd);
		if(ext){
			if(ext->test(NULL)){
				EnableMenuCommand(mnu,cmd);
//_tm_("activation undo3");
			}
			else{
				DisableMenuCommand(mnu,cmd);
//_tm_("deactivation undo3");
			}
		}
		else{
//_tm_("ext not found undo:"+&cmd);
		}

		cmd='redo';
//		cmd='red3';
//		EnableMenuCommand(mnu,cmd);
//_tm_("nb red3 :"+(int)CountMenuItemsWithCommandID(mnu,cmd));
		ext=find_ext(cmd);
		if(ext){
			if(ext->test(NULL)){
				EnableMenuCommand(mnu,cmd);
//_tm_("activation redo3");
			}
			else{
				DisableMenuCommand(mnu,cmd);
//_tm_("deactivation redo3");
			}
		}
		else{
//_tm_("ext not found redo:"+&cmd);
		}
		
		
		
	}
	else{
UInt32	flg;
				
		GetEventParameter(evt,kEventParamMenuContext,typeUInt32,NULL,sizeof(UInt32),NULL,&flg);
		if(!(flg&kMenuContextMenuBar)){
			return;
		}

		switch(GetEventKind(evt)){
			case kEventMenuOpening:
//				_tm_("kEventMenuOpening");
				item_check(evt);
				break;
			case kEventMenuEnableItems:
//				_tm_("kEventMenuEnableItems");
				item_check(evt);
				break;
			case kEventMenuClosed:
//				_tm_("kEventMenuClosed");
				item_restore(evt);
				break;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMenuMgr::load(){
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMenuMgr::unload(){
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMenuMgr::add_item(int menu, const char* name, UInt32 cmd){
_bTrace_("bMenuMgr::add_item",false);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&cmd,sizeof(UInt32));
#endif
MenuRef	mnu=GetMenuRef(menu);
	if(!mnu){
_te_("menu "+menu+" not found");
		return(0);
	}
UInt32	pcmd;
	for(int i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemCommandID(mnu,i,&pcmd);
		if(pcmd==cmd){
_te_("command "+(UInt32*)&cmd+" exist in menu "+menu);
			return(0);
		}
	}	
CFStringRef		cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
MenuItemIndex	item;
OSStatus		status=AppendMenuItemTextWithCFString(mnu,cfs,kMenuItemAttrDisabled,cmd,&item);
	if(status){
_te_("AppendMenuItemTextWithCFString returns "+status);
		CFRelease(cfs);
		return(0);
	}
	CFRelease(cfs);
	SetMenuItemModifiers(mnu,item,kMenuNoModifiers);
	return(item);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMenuMgr::rmv_item(int menu, UInt32 cmd){
_bTrace_("bMenuMgr::rmv_item",false);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&cmd,sizeof(UInt32));
#endif
MenuRef	mnu=GetMenuRef(menu);
	if(!mnu){
_te_("menu "+menu+" not found");
		return(false);
	}
UInt32		pcmd;
OSStatus	status;
	
	for(int i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemCommandID(mnu,i,&pcmd);
		if(pcmd==cmd){
			status=DeleteMenuItems(mnu,i,1);
			if(status){
				return(false);
_te_("DeleteMenuItems returns "+status);
			}
			return(true);
		}
	}
_te_("command "+(UInt32*)&cmd+" exist in menu "+menu);
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMenuMgr::ren_item(int menu, const char* name, UInt32 cmd){
_bTrace_("bMenuMgr::ren_item",false);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&cmd,sizeof(UInt32));
#endif
MenuRef	mnu=GetMenuRef(menu);
	if(!mnu){
_te_("menu "+menu+" not found");
		return(false);
	}
UInt32	pcmd;
	for(MenuItemIndex i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemCommandID(mnu,i,&pcmd);
		if(pcmd==cmd){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
			SetMenuItemTextWithCFString(mnu,i,cfs);
			CFRelease(cfs);
			return(true);
		}
	}	
_te_("command "+(UInt32*)&cmd+" not found in menu "+menu);
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericExt* bMenuMgr::find_ext(UInt32 cmd){
bGenericExt* ext=xmapmgr->find(cmd);
	if(!ext){
		ext=calcmgr->find(cmd);
	}
	if(!ext){
		ext=macromgr->find(cmd);
	}
	if(!ext){
		ext=scriptmgr->find(cmd);
	}
	if(!ext){
		ext=xboxmgr->find(cmd);
	}
	return(ext);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMenuMgr::item_check(EventRef evt){
_bTrace_("bMenuMgr::item_check",false);
Boolean	first;
	GetEventParameter(evt,kEventParamMenuFirstOpen,typeBoolean,NULL,sizeof(Boolean),NULL,&first);
	if(!first){
//		return;
	}
//_tm_("testing");
UInt32			cmd;
MenuRef			mnu;
bGenericExt*	ext;
	GetEventParameter(evt,kEventParamDirectObject,typeMenuRef,NULL,sizeof(MenuRef),NULL,&mnu);
//_tm_("GetMenuID==%d",GetMenuID(mnu));
	if(GetMenuID(mnu)==0){
		return;
	}
	for(int i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemCommandID(mnu,i,&cmd);
//_tm_("%.4s/%d",&cmd,cmd);
		if(cmd==0){
//_tm_("tada");
			continue;
		}
		if((cmd==kHICommandUndo)||(cmd==kHICommandRedo)){
			continue;
		}
		
		//GetMenuType
		ext=find_ext(cmd);
		if(!ext){
_tm_("ext not found :"+&cmd);
			continue;
		}
		if(ext->test(NULL)){
//			EnableMenuItem(mnu,i);
			EnableMenuCommand(mnu,cmd);
		}
		else{
//			DisableMenuItem(mnu,i);
			DisableMenuCommand(mnu,cmd);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMenuMgr::item_restore(EventRef evt){
_bTrace_("bMenuMgr::item_restore",false);

/*Boolean	first;
	GetEventParameter(evt,kEventParamMenuFirstOpen,typeBoolean,NULL,sizeof(Boolean),NULL,&first);
	if(!first){
//		return;
	}*/
	
//_tm_("testing");
UInt32			cmd;
MenuRef			mnu;
bGenericExt*	ext;
	GetEventParameter(evt,kEventParamDirectObject,typeMenuRef,NULL,sizeof(MenuRef),NULL,&mnu);
//_tm_("GetMenuID==%d",GetMenuID(mnu));
	if(GetMenuID(mnu)==0){
		return;
	}
	for(int i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemCommandID(mnu,i,&cmd);
//_tm_("%.4s/%d",&cmd,cmd);
		if(cmd==0){
//_tm_("tada");
			continue;
		}
		if((cmd==kHICommandUndo)||(cmd==kHICommandRedo)){
			continue;
		}

		
		//GetMenuType
		ext=find_ext(cmd);
		if(!ext){
			continue;
		}
		if(!IsMenuItemEnabled(mnu,i)){
			EnableMenuItem(mnu,i);
		}
	}
}
