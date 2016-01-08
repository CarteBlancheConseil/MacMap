//----------------------------------------------------------------------------
// File : bXMapShortCuts.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, shortcuts management
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
// 15/03/2010 creation.
//----------------------------------------------------------------------------

#include "bXMapShortcuts.h"

#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/xmldesc_utils.h>

#include <std_ext/bStdUserExt.h>
#include <std_ext/bXMapStdIntf.h>

#include <MacMapSuite/valconv.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>

/*
⇧
⌘
⌃
⌥
*/

#define kAllModifiers "⌃⌥⇧⌘"

// ---------------------------------------------------------------------------
// 
// -----------
static int comp_shortcut(const void* a, const void* b){
shortcut_prm* ga=(shortcut_prm*)a;
shortcut_prm* gb=(shortcut_prm*)b;
	
    if(ga->modi>gb->modi){
		return(1);
	}
	if(ga->modi<gb->modi){
		return(-1);
	}

	if(ga->key>gb->key){
		return(1);
	}
	if(ga->key<gb->key){
		return(-1);
	}

	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
static int comp_shortcutxtrn(const void* a, const void* b){
shortcut_prm* ga=(shortcut_prm*)a;
shortcut_prm* gb=(shortcut_prm*)b;
	
	if(ga->xtrn>gb->xtrn){
		return(1);
	}
	if(ga->xtrn<gb->xtrn){
		return(-1);
	}
	
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
static int comp_shortcutname(const void* a, const void* b){
shortcut_prm* ga=(shortcut_prm*)a;
shortcut_prm* gb=(shortcut_prm*)b;
	return(strcmp(ga->name,gb->name));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapShortcuts::bXMapShortcuts(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_xmap(sizeof(shortcut_prm))
				,_xbox(sizeof(shortcut_prm))
				,_macr(sizeof(shortcut_prm))
				,_scrp(sizeof(shortcut_prm)){
	set_flags(kXMapNeedIdle+kXMapNeedEvents);
	setclassname("shortcut_prms");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapShortcuts::~bXMapShortcuts(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapShortcuts::create(bGenericXMLBaseElement* elt){
	return(new bXMapShortcuts(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_call=0;
	_first=false;
// lecture des params
	read_p();
// lecture des raccoucis menus	
	scan_read_menu(AcquireRootMenu());
// réaffectation des raccoucis aux menu	
	scan_write_menu(AcquireRootMenu());
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::close(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapShortcuts::event(EventRef evt){
bool	res=false;
	if(!_first){
		return(false);
	}

UInt32			clss=GetEventClass(evt);
UInt32			kind=GetEventKind(evt);
UInt32			modi;
UInt32			key;
UInt32			xtrn;
bool			inmenu;
bArray*			arr;
bGenericEvent*	mevt;
bGenericExt*	ext;
bArray*			exts;
bGenericExtMgr*	mgr;
user_ext_data	edt;

	if(clss==kEventClassMacMap){
		switch(kind){
			case kEventMacMapDataBase:
				arr=_gapp->eventMgr()->events();
				for(int i=1;i<=arr->count();i++){
					arr->get(i,&mevt);
					if(mevt->kind()!=kEventKindUserExt){
						continue;
					}
					
					for(int j=1;j<=mevt->elements()->count();j++){
						mevt->elements()->get(j,&ext);
						if(ext->getsubclass()==kXMLSubClassExtMacro){
							exts=&_macr;
							mgr=_gapp->macroMgr();
							ext->process(kExtProcessCallGetData,&edt);
						}
						else if(ext->getsubclass()==kXMLSubClassExtScript){							
							exts=&_scrp;
							mgr=_gapp->scriptMgr();
							ext->process(kExtProcessCallGetData,&edt);
						}
						else{							
							continue;
						}
						
						switch(mevt->action()){
							case kEventActionCreate:
								add_ext(*exts,mgr,edt.sign);
								break;
							case kEventActionModify:
								modify_ext(*exts,mgr,edt.sign);
								break;
							case kEventActionDestroy:
								remove_ext(*exts,mgr,edt.sign);
								break;
						}
					}
					
				}
				break;
			default:
				break;
		}
	}
	else if(clss==kEventClassKeyboard){
		switch(kind){
			case kEventRawKeyDown:
				GetEventParameter(evt,kEventParamKeyCode,typeUInt32,NULL,sizeof(UInt32),NULL,&key);					
				GetEventParameter(evt,kEventParamKeyModifiers,typeUInt32,NULL,sizeof(UInt32),NULL,&modi);
	
                xtrn=scan_array(_xmap,key,modi,&inmenu);
				if((xtrn)&&(!inmenu)){
					_call=xtrn;
					break;
				}
				xtrn=scan_array(_xbox,key,modi,&inmenu);
				if((xtrn)&&(!inmenu)){
					_call=xtrn;
					break;
				}
				xtrn=scan_array(_scrp,key,modi,&inmenu);
				if((xtrn)&&(!inmenu)){
					_call=xtrn;
					break;
				}
				xtrn=scan_array(_macr,key,modi,&inmenu);
				if((xtrn)&&(!inmenu)){
					_call=xtrn;
					break;
				}
                break;
			default:
				break;
		}
	}
	return(res);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::idle(void* prm){
	if(_first){
		if(_call!=0){
			SendCommandToApplication(_call);
			_call=0;
		}
		return;
	}

bArray	xmap(_xmap);
bArray	xbox(_xbox);
bArray	scrp(_scrp);
bArray	macr(_macr);

	_xmap.reset();
	_xbox.reset();
	_scrp.reset();
	_macr.reset();
	
	xmap.sort(comp_shortcutxtrn);
	xbox.sort(comp_shortcutxtrn);
	scrp.sort(comp_shortcutxtrn);
	macr.sort(comp_shortcutxtrn);
	
	_first=true;
int			k;
	for(int i=1;i<=_gapp->xmapMgr()->count();i++){
shortcut_prm	p={0,0,0,0,false};
		p.xtrn=_gapp->xmapMgr()->ext_signature(i);
		k=xmap.search(&p,comp_shortcutxtrn);
		if(k){
			xmap.get(k,&p);
		}
		_gapp->xmapMgr()->ext_name(i,p.name);
		_xmap.add(&p);
	}
	for(int i=1;i<=_gapp->xboxMgr()->count();i++){
shortcut_prm	p={0,0,0,0,false};
		p.xtrn=_gapp->xboxMgr()->ext_signature(i);
		k=xbox.search(&p,comp_shortcutxtrn);
		if(k){
			xbox.get(k,&p);
		}
		_gapp->xboxMgr()->ext_name(i,p.name);
		_xbox.add(&p);
	}
	for(int i=1;i<=_gapp->scriptMgr()->count();i++){
shortcut_prm	p={0,0,0,0,false};
		p.xtrn=_gapp->scriptMgr()->ext_signature(i);
		k=scrp.search(&p,comp_shortcutxtrn);
		if(k){
			scrp.get(k,&p);
		}
		_gapp->scriptMgr()->ext_name(i,p.name);
		_scrp.add(&p);
	}
	for(int i=1;i<=_gapp->macroMgr()->count();i++){
shortcut_prm	p={0,0,0,0,false};
		p.xtrn=_gapp->macroMgr()->ext_signature(i);
		k=macr.search(&p,comp_shortcutxtrn);
		if(k){
			macr.get(k,&p);
		}
		_gapp->macroMgr()->ext_name(i,p.name);
		_macr.add(&p);
	}

	_xmap.sort(comp_shortcut);
	_xbox.sort(comp_shortcut);
	_scrp.sort(comp_shortcut);
	_macr.sort(comp_shortcut);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShortcuts::process(int msg, void* prm){
shortcut_prm*	p;
int				k;

	switch(msg){
		case kExtProcessCallFromIntf:
// Traitement en sortie de Edit
			break;
		case kExtProcessCallWithParams:			
// Pas nécessaire
			break;
		case kExtProcessCallWithXMLTree:
// Pas nécessaire
			break;
		case kExtProcessCallGetData:
			p=(shortcut_prm*)prm;
			_xmap.setsorted(false);
			k=_xmap.search(p,comp_shortcutxtrn);
			if(k){
				_xmap.get(k,p);
				_xmap.setsorted(true);
				break;
			}
			_xbox.setsorted(false);
			k=_xbox.search(p,comp_shortcutxtrn);
			if(k){
				_xbox.get(k,p);
				_xbox.setsorted(true);
				break;
			}
			_scrp.setsorted(false);
			k=_scrp.search(p,comp_shortcutxtrn);
			if(k){
				_scrp.get(k,p);
				_scrp.setsorted(true);
				break;
			}
			_macr.setsorted(false);
			k=_macr.search(p,comp_shortcutxtrn);
			if(k){
				_macr.get(k,p);
				_macr.setsorted(true);
				break;
			}
			break;
		case kExtProcessCallSetData:			
// Pas autorisé
			break;
		default:
			break;
	}	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShortcuts::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
	
	check_modify();
		
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				write_p();
				scan_write_menu(AcquireRootMenu());
				
			case kHICommandCancel:
				_xmap.sort(comp_shortcut);
				_xbox.sort(comp_shortcut);
				_scrp.sort(comp_shortcut);
				_macr.sort(comp_shortcut);
				DataBrowserClose(get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID));
				break;
				
			case kXMapShortcutsPopupFamilyCmd:
				family_action();
				break;
			
			case kXMapShortcutsApplyModiCmd:
				modify_ext();
				break;
			case kXMapShortcutsModiShiftCmd:
			case kXMapShortcutsModiCtrlCmd:
			case kXMapShortcutsModiAltCmd:
			case kXMapShortcutsModiCmdCmd:
				check_modify();
				break;
				
			default:
				check_modify();
				b=false;
				break;
		}
	}
	else{
		check_modify();
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::wd_init(WindowRef wd){
ControlRef	c;

	_fml=1;
	
	_xmap.sort(comp_shortcutname);
	_xbox.sort(comp_shortcutname);
	_scrp.sort(comp_shortcutname);
	_macr.sort(comp_shortcutname);
	
	_xmap.setsorted(false);
	_xbox.setsorted(false);
	_scrp.setsorted(false);
	_macr.setsorted(false);
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsPopupFamilyID);
	SetControl32BitValue(c,_fml);
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID);
	DataBrowserInit(c,shrt_handler,shrt_notifier,(long)this);
	populate_ext();
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID);
	SetControlAbility(c,false);
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID);
	SetControlAbility(c,false);
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID);
	SetControlAbility(c,false);
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID);
	SetControlAbility(c,false);

const EventTypeSpec	kWindowEvents[]={{kEventClassControl,kEventControlDraw}};
EventHandlerUPP	hdlr=ctrl_evt_hdlr;
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID);
	SetTextControlFilter(c,name_fltr);
	SetControlReference(c,(long)this);
	InstallControlEventHandler(c,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)this,NULL);
	SetTextControlValue(c,"");

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarNumID);
	SetTextControlValue(c,"");
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapShortcuts::shrt_handler(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){
CFStringRef		name;
bXMapShortcuts*	editor=(bXMapShortcuts*)GetControlReference(browser);
shortcut_prm	p;
	
	switch(editor->_fml){
		case 1:
			editor->_xmap.get(itemID,&p);
			break;
		case 2:
			editor->_xbox.get(itemID,&p);
			break;
		case 3:
			editor->_scrp.get(itemID,&p);
			break;
		case 4:
			editor->_macr.get(itemID,&p);
			break;
	}
	
	switch(property){
		case kXMapShortcutsDBProperty:
			if(!changeValue){
				name=CFStringCreateWithCString(kCFAllocatorDefault,p.name,CFStringGetSystemEncoding());
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
void bXMapShortcuts::shrt_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapShortcuts*	editor=(bXMapShortcuts*)GetControlReference(browser);
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
			editor->ext_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->ext_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::scan_read_menu(MenuRef mnu){
MenuRef			chld;
shortcut_prm	p={0,0,0,0,true};
UInt8			ui8;
UInt16			ui16;
int				k;

	for(MenuItemIndex i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemHierarchicalMenu(mnu,i,&chld);
		if(chld){
			scan_read_menu(chld);
		}
		else{
			GetMenuItemCommandID(mnu,i,&p.xtrn);
			if(p.xtrn==0){
				continue;
			}
			k=_xmap.search(&p,comp_shortcutxtrn);
			if(!k){
				k=_xbox.search(&p,comp_shortcutxtrn);
			}
			if(!k){
				k=_macr.search(&p,comp_shortcutxtrn);
			}
			if(!k){
				k=_scrp.search(&p,comp_shortcutxtrn);
			}
			
			if(!k){
				GetMenuItemCommandKey(mnu,i,true,&ui16);
				p.key=ui16;
				GetMenuItemCommandKey(mnu,i,false,&ui16);
				p.code=ui16;
				GetMenuItemModifiers(mnu,i,&ui8);
				p.modi=0;
				if(ui8&kMenuShiftModifier){
					p.modi|=shiftKey;
				}
				if(ui8&kMenuControlModifier){
					p.modi|=controlKey;
				}
				if(ui8&kMenuOptionModifier){
					p.modi|=optionKey;
				}
				if(!(ui8&kMenuNoCommandModifier)){
					p.modi|=cmdKey;
				}

				if(	((p.modi)&&(p.key))		||
					((p.modi)&&(p.code))	){
					if(_gapp->xmapMgr()->ext_index(p.xtrn)){
#ifdef __LITTLE_ENDIAN__
						LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
						_xmap.add(&p);
					}
					else if(_gapp->xboxMgr()->ext_index(p.xtrn)){
#ifdef __LITTLE_ENDIAN__
						LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
						_xbox.add(&p);
					}
					else if(_gapp->scriptMgr()->ext_index(p.xtrn)){
#ifdef __LITTLE_ENDIAN__
						LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
						_scrp.add(&p);
					}
					else if(_gapp->macroMgr()->ext_index(p.xtrn)){
#ifdef __LITTLE_ENDIAN__
						LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
						_macr.add(&p);
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::scan_write_menu(MenuRef mnu){
MenuRef		chld;
UInt8		ui8;
UInt16		ui16;
int			k;
bArray*		arr=NULL;
	
	for(MenuItemIndex i=1;i<=CountMenuItems(mnu);i++){
		GetMenuItemHierarchicalMenu(mnu,i,&chld);
		if(chld){
			scan_write_menu(chld);
		}
		else{
shortcut_prm	p={0,0,0,0,false};
			GetMenuItemCommandID(mnu,i,&p.xtrn);
			if(p.xtrn==0){
				continue;
			}
			
#ifdef __LITTLE_ENDIAN__
			LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
			k=_xmap.search(&p,comp_shortcutxtrn);
			if(!k){
				k=_xbox.search(&p,comp_shortcutxtrn);
				if(!k){
					k=_macr.search(&p,comp_shortcutxtrn);
					if(!k){
						k=_scrp.search(&p,comp_shortcutxtrn);
						if(k){
							arr=&_scrp;
						}
					}
					else{
						arr=&_macr;
					}
				}
				else{
					arr=&_xbox;
				}
			}
			else{
				arr=&_xmap;
			}
			
			if(!arr){
				continue;
			}
			
			arr->get(k,&p);
			
			if(p.inmenu){
				continue;
			}
			
			if(	((p.modi)&&(p.key))		||
				((p.modi)&&(p.code))	){
				//ui16=p.key;
				//SetMenuItemCommandKey(mnu,i,true,ui16);
				ui16=p.code;
				SetMenuItemCommandKey(mnu,i,false,ui16);
				ui8=0;
							
				if((p.modi&shiftKey)!=0){
					ui8|=kMenuShiftModifier;
				}
				if((p.modi&controlKey)!=0){
					ui8|=kMenuControlModifier;
				}
				if((p.modi&optionKey)!=0){
					ui8|=optionKey;
				}
				if(!((p.modi&cmdKey)!=0)){
					ui8|=kMenuNoCommandModifier;
				}
				
				SetMenuItemModifiers(mnu,i,ui8);
			}
			
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 bXMapShortcuts::scan_array(bArray& arr, UInt32 key, UInt32 modi, bool* inmenu){
//_bTrace_("bXMapShortcuts::scan_array",true);
//_tm_((int)key+":"+(int)modi);
//    
shortcut_prm	p;
	p.key=key;
	p.modi=modi;
	p.xtrn=-1;
	(*inmenu)=false;
	
int			k=arr.search(&p,comp_shortcut);
	if(k){
		arr.get(k,&p);
		(*inmenu)=p.inmenu;
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&p.xtrn,sizeof(UInt32));
#endif
	}
    else{
//_tw_("not found");
        p.xtrn=0;
    }
//_tm_("inmenu="+(*inmenu));
   
	return(p.xtrn);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::populate_ext(){
int					i;
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID);
bArray*				arr=NULL;

	switch(_fml){
		case 1:
			arr=&_xmap;
			break;
		case 2:
			arr=&_xbox;
			break;
		case 3:
			arr=&_scrp;
			break;
		case 4:
			arr=&_macr;
			break;
        default:
            arr=&_xmap;
            break;
	}
	for(i=1;i<=arr->count();i++){
		itemId=i;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapShortcutsDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::family_action(){
ControlRef				c=get_control(kXMapShortcutsSignature,kXMapShortcutsPopupFamilyID);
int						x=GetControl32BitValue(c);
DataBrowserPropertyID	pid=kXMapShortcutsDBProperty;

	if(x!=_fml){
		c=get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID);
		DataBrowserReset(c,1,&pid);
		_fml=x;
		populate_ext();
		ext_action(0);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::ext_action(DataBrowserItemID item){
	if(!item){
		SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID),false);
		SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID),false);
		SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID),false);
		SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID),false);
		SetTextControlValue(get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID),"");
		SetTextControlValue(get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID),"");
		set_modifiers(0);
		return;
	}

	SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID),true);
	SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID),true);
	SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID),true);
	SetControlAbility(get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID),true);

shortcut_prm	p;
char            str[32];
	switch(_fml){
		case 1:
			_xmap.get(item,&p);
			break;
		case 2:
			_xbox.get(item,&p);
			break;
		case 3:
			_scrp.get(item,&p);
			break;
		case 4:
			_macr.get(item,&p);
			break;
        default:
            _xmap.get(item,&p);
            break;
	}
	
	str[1]=0;
	str[0]=p.code;
	SetTextControlValue(get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID),str);
int	c=str[0];
	if(c==0){
		c=p.key;
	}
	sprintf(str,"%d",c);
	SetTextControlValue(get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarNumID),str);
	set_modifiers(p.modi);
	check_modify();
		
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::modify_ext(){
shortcut_prm		p;
ControlRef			c=get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID);
DataBrowserItemID	item=DataBrowserSelectedItem(c);
char				val[256];

	switch(_fml){
		case 1:
			_xmap.get(item,&p);
			break;
		case 2:
			_xbox.get(item,&p);
			break;
		case 3:
			_scrp.get(item,&p);
			break;
		case 4:
			_macr.get(item,&p);
			break;
	}
	
	p.modi=0;
	p.key=_curk;
	p.inmenu=false;
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID);
	GetTextControlValue(c,val,sizeof(val)-1);
	if(val[0]!=0){	
		c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID);
		if(GetControl32BitValue(c)){
			p.modi|=shiftKey;
		}
		c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID);
		if(GetControl32BitValue(c)){
			p.modi|=controlKey;
		}
		c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID);
		if(GetControl32BitValue(c)){
			p.modi|=optionKey;
		}
		c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID);
		if(GetControl32BitValue(c)){
			p.modi|=cmdKey;
		}
	}
	p.code=val[0];
	switch(_fml){
		case 1:
			_xmap.put(item,&p);
			break;
		case 2:
			_xbox.put(item,&p);
			break;
		case 3:
			_scrp.put(item,&p);
			break;
		case 4:
			_macr.put(item,&p);
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::set_modifiers(UInt32 modi){
ControlRef	c;

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID);
	SetControl32BitValue(c,(modi&shiftKey)!=0);

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID);
	SetControl32BitValue(c,(modi&controlKey)!=0);

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID);
	SetControl32BitValue(c,(modi&optionKey)!=0);

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID);
	SetControl32BitValue(c,(modi&cmdKey)!=0);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapShortcuts::check_modify(){
UInt32		xtrn;
UInt32		modi=0;
ControlRef	c;
bool		b,good=false;
char		val[256];

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiShiftID);
	if(GetControl32BitValue(c)){
		modi|=shiftKey;
	}
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCtrlID);
	if(GetControl32BitValue(c)){
		modi|=controlKey;
	}
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiAltID);
	if(GetControl32BitValue(c)){
		modi|=optionKey;
	}
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsModiCmdID);
	if(GetControl32BitValue(c)){
		modi|=cmdKey;
	}

	c=get_control(kXMapShortcutsSignature,kXMapShortcutsShortCarID);
	GetTextControlValue(c,val,sizeof(val)-1);
	if(strlen(val)!=0){
		for(;;){
			xtrn=scan_array(_xmap,_curk,modi,&b);
			if(xtrn){
				break;
			}
			xtrn=scan_array(_xbox,_curk,modi,&b);
			if(xtrn){
				break;
			}
			xtrn=scan_array(_scrp,_curk,modi,&b);
			if(xtrn){
				break;
			}
			xtrn=scan_array(_macr,_curk,modi,&b);
			if(xtrn){
				break;
			}
			good=true;
			break;
		}
	}
	else{
		good=true;
	}	
	
	c=get_control(kXMapShortcutsSignature,kXMapShortcutsApplyModiID);
	SetControlAbility(c,good);
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult bXMapShortcuts::name_fltr(	ControlRef c, 
													SInt16* key, 
													SInt16* car, 
													EventModifiers* m){
char	val[256];

	if((*car)<32){
		return(kControlKeyFilterPassKey);
	}
	GetTextControlValue(c,val,sizeof(val)-1);
	if(strlen(val)>0){
		return(kControlKeyFilterBlockKey);
	}

bXMapShortcuts*		editor=(bXMapShortcuts*)GetControlReference(c);
ControlRef			brow=editor->get_control(kXMapShortcutsSignature,kXMapShortcutsExternListID);
DataBrowserItemID	item=DataBrowserSelectedItem(brow);
	if(item==0){
		return(kControlKeyFilterBlockKey);
	}
	val[0]=(*car);
	val[1]=0;
	strupper(val);
	(*car)=val[0];
	SetTextControlValue(c,val);	
	editor->_curk=*key;
	return(kControlKeyFilterBlockKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapShortcuts::ctrl_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
bXMapShortcuts*	x=(bXMapShortcuts*)up;
	x->check_modify();
	return(eventNotHandledErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::read_p(){
	read_p("xmaps",_xmap);
	read_p("xmapboxes",_xbox);
	read_p("scripts",_scrp);
	read_p("macros",_macr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::write_p(){
	write_p("xmaps",_xmap);
	write_p("xmapboxes",_xbox);
	write_p("scripts",_scrp);
	write_p("macros",_macr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::read_p(const char* name, bArray& arr){
_bTrace_("bXMapShortcuts::read_p",false);
UInt32					sign=GetSignature(this);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bGenericXMLBaseElement* chld=NULL;
bool					b=_gapp->document()->readTree(&root,sign,(char*)name);
char					val[_values_length_max_];
shortcut_prm			shrt;

	if((!b)||(!root)){
		write_p(name,arr);
		return;
	}
	if((!b)||(!root)){
_tw_("((!b)||(!root)) for "+name);
		return;
	}

	for(int i=2;i<=root->countelements();i++){
		elt=root->getelement(i);
		
		chld=elt->getelement(1);
		chld->getvalue(val);
		shrt.xtrn=(*(UInt32*)val);
		
		chld=elt->getelement(2);
		chld->getvalue(val);
		shrt.modi=atoi(val);
		
		chld=elt->getelement(3);
		chld->getvalue(val);
		shrt.key=atoi(val);
		
		chld=elt->getelement(4);
		chld->getvalue(val);
		shrt.code=val[0];
		
        shrt.inmenu=false;

		arr.add(&shrt);
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::write_p(const char* name, bArray& arr){
bGenericXMLBaseElement* root=make_p(arr);
	if(!root){
		return;
	}
UInt32	sign=GetSignature(this);
	if(!_gapp->document()->writeTree(root,sign,(char*)name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapShortcuts::make_p(bArray& arr){
bArray			xml(sizeof(xmlelt));
char			val[_values_length_max_];
shortcut_prm	shrt;

	add_cdesc(xml,0,"param","");
	add_cdesc(xml,1,"version","3.0.0");
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&shrt);
		if((!shrt.inmenu)&&(shrt.key)){
			add_cdesc(xml,1,"param","");
			sprintf(val,"%.4s",(char*)&shrt.xtrn);
			add_cdesc(xml,2,"sign",val);
			add_idesc(xml,2,"int",shrt.modi);
			add_idesc(xml,2,"int",shrt.key);
			val[1]=0;
			val[0]=shrt.code;
			add_cdesc(xml,2,"string",val);
		}
	}
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&xml);
	free_descs(xml);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::add_ext(	bArray& arr,
								bGenericExtMgr* mgr,
								UInt32 sgn){
shortcut_prm	p={sgn,0,0,0,false,""};
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sgn,sizeof(UInt32));
#endif
int			k=mgr->ext_index(sgn);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&sgn,sizeof(UInt32));
#endif
	if(k){
		mgr->ext_name(k,p.name);
		arr.add(&p);
		arr.sort(comp_shortcutxtrn);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::modify_ext(bArray& arr,
								bGenericExtMgr* mgr,
								UInt32 sgn){
shortcut_prm	p={sgn,0,0,0,false};
int			k=arr.search(&p,comp_shortcutxtrn);
	if(k){
		arr.get(k,&p);
		mgr->ext_name(k,p.name);
		arr.put(k,&p);
		arr.setsorted(true);
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapShortcuts::remove_ext(bArray& arr,
								bGenericExtMgr* mgr,
								UInt32 sgn){
shortcut_prm	p={sgn,0,0,0,false};
int			k=arr.search(&p,comp_shortcutxtrn);
	if(k){
		arr.rmv(k);
		arr.setsorted(true);
	}	
}
