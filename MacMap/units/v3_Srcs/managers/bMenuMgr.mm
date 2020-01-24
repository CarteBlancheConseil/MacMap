//----------------------------------------------------------------------------
// File : bMenuMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Menu management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2019 Carte Blanche Conseil.
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
// 07/11/2018 portage Cocoa.
//----------------------------------------------------------------------------

#include "bMenuMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

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
    
NSMenuItem  *subMenuItem=[[[NSApplication sharedApplication] mainMenu] itemWithTag:menu];
    if(subMenuItem==nil){
        return 0;
    }
NSMenu      *subMenu=[subMenuItem submenu];
    if(subMenu==nil){
        return 0;
    }
NSMenuItem  *item = [[NSMenuItem alloc] initWithTitle:[NSString stringWithCString:name encoding:NSMacOSRomanStringEncoding]
                                               action:@selector(menuAction:) keyEquivalent:@""];
    [item autorelease];
    [item setTarget:[[NSApplication sharedApplication] delegate]];
    [item setTag:cmd];

    [subMenu addItem:item];
    
    return 0;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMenuMgr::rmv_item(int menu, UInt32 cmd){
_bTrace_("bMenuMgr::rmv_item",false);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&cmd,sizeof(UInt32));
#endif

NSMenuItem  *subMenuItem=[[[NSApplication sharedApplication] mainMenu] itemWithTag:menu];
    if(subMenuItem==nil){
        return 0;
    }
NSMenu      *subMenu=[subMenuItem submenu];
    if(subMenu==nil){
        return 0;
    }
    
    [subMenu removeItem:[subMenu itemWithTag:(NSInteger)cmd]];
    
_te_("command "+(UInt32*)&cmd+" exist in menu "+menu);
    return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMenuMgr::ren_item(int menu, const char* name, UInt32 cmd){
_bTrace_("bMenuMgr::ren_item",false);
/*#ifdef __LITTLE_ENDIAN__
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
_te_("command "+(UInt32*)&cmd+" not found in menu "+menu);*/
	return(false);
}

// ---------------------------------------------------------------------------
//
// -----------
bool bMenuMgr::is_enabled(UInt32 cmd){
bGenericExt* ext=find_ext(cmd);
    ext=find_ext(cmd);
    if((ext)&&(map_doc)){
        return ext->test(NULL);
    }
    return false;
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
void bMenuMgr::check_menus(){
NSMenuItem  *subMenuItem=[[[NSApplication sharedApplication] mainMenu] itemWithTag:kMenuMgrMenuFileID];
    if(subMenuItem==nil){
        return;
    }
NSMenu      *subMenu=[subMenuItem submenu];
    if(subMenu==nil){
        return;
    }
    checkMenu(subMenu);
}

// ---------------------------------------------------------------------------
//
// -----------
void bMenuMgr::checkMenu(NSMenu *subMenu){
//_bTrace_("bMenuMgr::checkMenu",true);
    if(subMenu==NULL){
        //_te_("subMenu==NULL");
        return;
    }
bMacMapApp* gapp=(bMacMapApp*)bMacMapApp::instance();
NSInteger   i,n=[subMenu numberOfItems];
NSMenuItem  *item;
    
    for(i=0;i<n;i++){
        item=[subMenu itemAtIndex:i];
//NSLog(@"%@", [item title]);

        if([item tag]!=0){
UInt32      cmd=(UInt32)[item tag];
            [item setEnabled:(is_enabled(cmd))];
//_tm_("tag "+cmd);
        }
        else{
SEL         act=[item action];
            if((act==@selector(newDocument:))           ||
               (act==@selector(openDocument:))          ||
               (act==@selector(_openRecentDocument:))   ){
                [item setEnabled:(gapp->document()==NULL)];
                //_tm_("new/open");
            }
            else if(act==@selector(clearRecentDocuments:)){
                //_tm_("clearRecentDocuments");
                [item setEnabled:YES];
            }
            else if((act==@selector(closeAll:))||
                    (act==@selector(performClose:))){
                [item setEnabled:(gapp->document()!=NULL)];
                //_tm_("close");
            }
            else if(act==@selector(submenuAction:)){
                //_tm_("sous menu");
NSMenu          *menu=[item submenu];
                checkMenu(menu);
            }
            else if(act!=NULL){
//                _tm_("autre");
                NSLog(@"%@", NSStringFromSelector(act));
            }
        }
    }
}

/*
// ---------------------------------------------------------------------------
// 
// -----------
virtual void item_check			(	EventRef evt);//
virtual void item_restore		(	EventRef evt);//

// ---------------------------------------------------------------------------
//
// -----------
void bMenuMgr::item_check(EventRef evt){
_bTrace_("bMenuMgr::item_check",true);
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
_bTrace_("bMenuMgr::item_restore",true);
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
			continue;
		}
		if(!IsMenuItemEnabled(mnu,i)){
			EnableMenuItem(mnu,i);
		}
	}
}*/
