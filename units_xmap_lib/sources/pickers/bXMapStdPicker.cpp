//----------------------------------------------------------------------------
// File : bXMapStdPicker.cpp
// Project : MacMap
// Purpose : C++ source file : XMap base class for pickers
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
// 30/05/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapStdPicker.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Type_Utils.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bStdFile.h>

#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapStdPicker	::bXMapStdPicker(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	strcpy(_sub,"icons");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapStdPicker::~bXMapStdPicker(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapStdPicker::create(bGenericXMLBaseElement* elt){
	return(new bXMapStdPicker(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapStdPicker::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapStdPicker::edit(void* prm){
	if(prm){
		_prm=*((picker_prm*)prm);
	}
	else{
		_prm.tp=(bGenericType*)CurType(_gapp);
		strcpy(_prm.name,"");
	}
bool	b=bStdXMapWithModalIntf::edit(NULL);
	if(prm){
		*((picker_prm*)prm)=_prm;
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStdPicker::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStdPicker::test(void* prm){
	return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapStdPicker::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);
	
	if(clss==kEventClassCommand){
		GetEventParameter(	evt,
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				DataBrowserClose(get_control(	kXMapStdPickerSignature,
												kXMapStdPickerListID));
				break;
			case kHICommandCancel:
				DataBrowserClose(get_control(	kXMapStdPickerSignature,
												kXMapStdPickerListID));
				strcpy(_prm.name,"");
				break;
			case kXMapStdPickerImportCmd:
				import(NULL,0);
				browser_fill();
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
void bXMapStdPicker::wd_init(WindowRef wd){
	DataBrowserInit(get_control(kXMapStdPickerSignature,
								kXMapStdPickerListID),
					list_handler,
					list_notifier,
					(long)this);
	browser_fill();
HIViewID	vid={kXMapStdPickerSignature,kXMapStdPickerViewID};
HIViewRef	vw;
	HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
EventTypeSpec	spec[]={{kEventClassControl,kEventControlDraw}};
	InstallEventHandler(GetControlEventTarget(vw),
						draw_proc,
						GetEventTypeCount(spec),
						spec,
						(void*)this,
						NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStdPicker::browser_fill(){
ControlRef	c=get_control(	kXMapStdPickerSignature,
							kXMapStdPickerListID);
DataBrowserPropertyID	pid=kXMapStdPickerListProperty;
int                     n;
	
	if(_prm.tp){
		n=_prm.tp->fields()->count_param(_sub);
	}
	else{
		n=_gapp->document()->countParams(_sub);
	}
	
	DataBrowserReset(c,1,&pid);
	AddDataBrowserItems(c,
						kDataBrowserNoItem,
						n,
						NULL,
						kXMapStdPickerListProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapStdPicker::list_handler(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){
CFStringRef			name;
bXMapStdPicker*		editor=(bXMapStdPicker*)GetControlReference(browser);
char				val[256];

	switch(property){
		case kXMapStdPickerListProperty:
			if(!changeValue){
				if(editor->_prm.tp){
					if(!(editor->_prm.tp->fields()->get_param_name(editor->_sub,itemID,val))){
						sprintf(val,"????");
					}
				}
				else{
					//_gapp->document()->getParamName(editor->_sub,itemID,val);
				}
				name=CFStringCreateWithCString(kCFAllocatorDefault,val,CFStringGetSystemEncoding());
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
void bXMapStdPicker::list_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapStdPicker*	editor=(bXMapStdPicker*)GetControlReference(browser);
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
			editor->choose_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->choose_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStdPicker::choose_action(DataBrowserItemID item){
	if(item!=0){
		if(item>0){
			if(_prm.tp){
				(void)_prm.tp->fields()->get_param_name(_sub,item,_prm.name);
			}
			else{
				//(void)_gapp->document()->getParamName(_sub,item,_prm.name);
			}
		}
	}
	else{
		if(DataBrowserCountSelected(get_control(kXMapStdPickerSignature,
												kXMapStdPickerListID))==0){
			strcpy(_prm.name,"");
		}
	}	
	Draw1Control(get_control(kXMapStdPickerSignature,kXMapStdPickerViewID));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapStdPicker::draw(CGContextRef ctx){
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapStdPicker::import(const char** pnsft, int nnsft){
_bTrace_("bXMapStdPicker::import",true);
bArray          paths(sizeof(char[PATH_MAX]));
    
    if(NSGetFilePaths(pnsft,
                      nnsft,
                      cNSMacOSRomanStringEncoding,
                      false,
                      true,
                      NULL,
                      paths)==cNSCancelButton){
_te_("user canceled");
        return;
    }
    
    
void*	data;
int		sz;
char    fpath[PATH_MAX];
char	path[PATH_MAX];
char	name[FILENAME_MAX];
    for(long i=1;i<=paths.count();i++){
        paths.get(i,&fpath);
        splitPath(fpath,path,name);
//        _prm.name[strlen(_prm.name)-4]=0;
_tm_(path);
_tm_(name);

bStdFile		f(fpath,"r");
		if(f.status()){
_te_("File bad status "+f.status());
			return;
		}

		f.mount((char**)&data,&sz);
		if(f.status()){
_te_("File bad status at mount "+f.status());
			return;
		}

		if(_prm.tp){
			(void)_prm.tp->fields()->set_param(_sub,name,data,sz);
		}
		else{
			//(void)_gapp->document()->set_param(_sub,name,data,sz);
		}
		free(data);

    }
}


// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapStdPicker::draw_proc	(	EventHandlerCallRef hdlr,
										EventRef evt,
										void* udt){
bXMapStdPicker* picker=(bXMapStdPicker*)udt;
CGContextRef	ctx;
OSStatus		status=eventNotHandledErr;

	for(;;){
		status=GetEventParameter(	evt,
									kEventParamCGContextRef,
									typeCGContextRef,
									NULL,
									sizeof(CGContextRef),
									NULL,
									&ctx);
		if(status){
			break;
		}
		picker->draw(ctx);
		break;
	}
	return(status);
}
