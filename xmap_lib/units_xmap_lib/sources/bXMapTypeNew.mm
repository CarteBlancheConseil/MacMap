//----------------------------------------------------------------------------
// File : bXMapTypeNew.mm
// Project : MacMap
// Purpose : Objective C++ source file : XMap class, type creation
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
// 22/03/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapTypeNew.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <Cocoa/Cocoa.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

#include <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------
@interface TypeNewViewController : NSViewController{
    IBOutlet NSPopUpButton* _tpk_pop;
    IBOutlet NSPopUpButton* _bsk_pop;
    IBOutlet NSButton*      _atv_chk;
}
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
-(long)typeKind;
-(long)baseKind;
-(BOOL)addToView;
//----------------------------------------------------------------------------
@end


// ---------------------------------------------------------------------------
//
// -----------
@implementation TypeNewViewController
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass{
_bTrace_("[TypeNewViewController allocWithNibNamed owner class]",true);
    return [[loadClass alloc] initWithNibName:nibNamed
                                       bundle:[NSBundle bundleWithIdentifier:@"com.cbconseil.xmap-lib"]];
}
// ---------------------------------------------------------------------------
//
// ------------
-(long)typeKind{
//    return 1;
    return [_tpk_pop indexOfSelectedItem]+1;
}
// ---------------------------------------------------------------------------
//
// ------------
-(long)baseKind{
//    return 1;
    return [_bsk_pop indexOfSelectedItem]+1;
}
// ---------------------------------------------------------------------------
//
// ------------
-(BOOL)addToView{
//    return 1;
    return [_atv_chk intValue];
}
// ---------------------------------------------------------------------------
//
// -----------
@end


// ---------------------------------------------------------------------------
//
// ------------
static void* TypeNew_getAccessoryViewController(){
_bTrace_("TypeNew_getAccessoryViewController",true);
TypeNewViewController   *ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    ctrlr=[TypeNewViewController allocWithNibNamed:@"TypeNewAccessoryView"
                                            class:[TypeNewViewController class]];
    [localPool release];
    return((void*)ctrlr);
}

// ---------------------------------------------------------------------------
//
// ------------
static void* TypeNew_getAccessoryViewFromController(void* ctrlr){
_bTrace_("TypeNew_getAccessoryViewFromController",true);
TypeNewViewController   *controller=(TypeNewViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
NSView*   view=[controller view];
    [localPool release];
    return((void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
static void TypeNew_getParamsFromController(void* ctrlr, int* tpk, int* bsk, bool* atv){
_bTrace_("TypeNew_getCreateIfNeededFromController",true);
TypeNewViewController   *controller=(TypeNewViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    *tpk=[controller typeKind];
    *bsk=[controller baseKind];
    *atv=[controller addToView];
    [localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
static void TypeNew_releaseController(void* ctrlr){
_bTrace_("TypeNew_releaseController",true);
TypeNewViewController   *controller=(TypeNewViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    [localPool release];
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapTypeNew::bXMapTypeNew(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("typenew");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapTypeNew::~bXMapTypeNew(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapTypeNew::create(bGenericXMLBaseElement* elt){
	return(new bXMapTypeNew(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTypeNew::process(int msg, void* prm){
_bTrace_("bXMapTypeNew::process",true);
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}

int		kind,tbl;
char	name[FILENAME_MAX];
char	path[PATH_MAX];
char	data[1024];
char*	dt=NULL;
bool	atv=true;

	data[0]=0;
	if(choose_location(path,name,&kind,&tbl,&atv)!=noErr){
		return(false);
	}
	switch(tbl){
		case kBasePostGISID:
			if(!get_conn_prm(data)){
				return(false);
			}
			dt=data;
			break;
	}
	
	if(_gapp->typesMgr()->add(name,path,dt,tbl,kind)!=noErr){
_te_("_gapp->typesMgr()->add failed");
		return(false);
	}
	else if(atv){
        _gapp->layersMgr()->add(_gapp->typesMgr()->count(),1);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTypeNew::edit(void* prm){
bEventLog	log(_gapp,
				getbundle(),
				kXMapTypeNewMessageID,
				GetSignature(this));
	process(kExtProcessCallFromIntf,prm);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bXMapTypeNew::choose_location(char* path, char* name, int* kind, int* tbl, bool* atv){
_bTrace_("bXMapTypeNew::choose_location",true);
char    fpath[PATH_MAX];
void*   ctrlr=TypeNew_getAccessoryViewController();
    
    if(NSPutFilePath("type",
                     cNSMacOSRomanStringEncoding,
                     TypeNew_getAccessoryViewFromController(ctrlr),
                     fpath)==cNSCancelButton){
        TypeNew_releaseController(ctrlr);
        return -1;
    }
    
    TypeNew_getParamsFromController(ctrlr,kind,tbl,atv);
    TypeNew_releaseController(ctrlr);
   
    switch(*tbl){
        case 1:
            (*tbl)=kBaseLocalDBID;
_tm_("kBaseLocalDBID");
            break;
        case 2:
            (*tbl)=kBasePostGISID;
_tm_("kBasePostGISID");
           break;
    }

    splitPath(fpath,path,name);
    
char		docp[PATH_MAX];
char		rel[PATH_MAX];
int			i;

_tm_(fpath);
_tm_(path);
_tm_(name);
    GetBasePath(_gapp,docp);
    strcat(docp,"/");
				
    relpath(docp,fpath,rel);
    rel[strlen(rel)-1]=0;
    
    for(i=strlen(rel)-1;i>=0;i--){
        if(rel[i]=='/'){
            rel[i]=0;
            break;
        }
    }
    if(i<=0){
        strcpy(path,"");
    }
    else{
        strcpy(path,rel);
    }
_tm_("path: "+path);
    strrep(name,".type","");
    return noErr;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTypeNew::get_conn_prm(char* data){
OSStatus			status;
IBNibRef			nib=NULL;
const EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
char				str[256];
bool				ret=false;
	_wd=NULL;
	
#warning COMMENTE
	/*for(;;){
		status=CreateNibReferenceWithCFBundle(getbundle(),CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("Window"),&_wd);
		if(status){
			break;
		}
		InstallWindowEventHandler(	_wd,
									conn_evt,
									GetEventTypeCount(kWindowEvents),
									kWindowEvents,
									(void*)this,
									NULL);
		ShowWindow(_wd);
		status=RunAppModalLoopForWindow(_wd);
		if(status){
			break;
		}
		get_string(kXMapTypeNewConnPrmDataBaseID,str);
		sprintf(data,"%s\n",str);
		get_string(kXMapTypeNewConnPrmHostID,str);
		strcat(data,str);
		strcat(data,"\n");
		get_string(kXMapTypeNewConnPrmPortID,str);
		strcat(data,str);
		strcat(data,"\n");
		if(get_string(kXMapTypeNewConnPrmUserID,str)){
			//strcat(data,str);
			//strcat(data,"\n");
			//get_string(kXMapTypeNewConnPrmPasswordID,str);
			//strcat(data,str);
			//strcat(data,"unused");
			//strcat(data,"\n");
		}
		ret=true;
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(_wd){
		DisposeWindow(_wd);
		_wd=NULL;
	}*/
	return(ret);
}

// ---------------------------------------------------------------------------
// 
// -----------
#warning COMMENTE
/*OSStatus bXMapTypeNew::conn_evt(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus		result=eventNotHandledErr;
HICommand		cmd;
bXMapTypeNew*	x=(bXMapTypeNew*)up;
char			str[256];

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(	evt,
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				result=noErr;
				if(!x->get_string(kXMapTypeNewConnPrmDataBaseID,str)){
					x->set_focus(kXMapTypeNewConnPrmDataBaseID);
					break;
				}
				if(!x->get_string(kXMapTypeNewConnPrmHostID,str)){
					x->set_focus(kXMapTypeNewConnPrmHostID);
					break;
				}
				if(!x->get_string(kXMapTypeNewConnPrmPortID,str)){
					x->set_focus(kXMapTypeNewConnPrmPortID);
					break;
				}
				if(	(x->get_string(kXMapTypeNewConnPrmUserID,str))		&&
					(!x->get_string(kXMapTypeNewConnPrmPasswordID,str))	){
					x->set_focus(kXMapTypeNewConnPrmPasswordID);
					break;
				}
				if(	(!x->get_string(kXMapTypeNewConnPrmUserID,str))		&&
					(x->get_string(kXMapTypeNewConnPrmPasswordID,str))	){
					x->set_focus(kXMapTypeNewConnPrmUserID);
					break;
				}
				QuitAppModalLoopForWindow(x->_wd);
				break;
			case kHICommandCancel:
				result=noErr;
				QuitAppModalLoopForWindow(x->_wd);
				break;
		}
	}
	return(result);
}*/


// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapTypeNew::get_string(int cid, char* str){
#warning COMMENTE
/*ControlRef	c;
ControlID	xid={kXMapTypeNewConnPrmSignature,cid};

	GetControlByID(_wd,&xid,&c);
	GetTextControlValue(c,str,256);
    return(strlen(str)>0);*/
    
    str[0]=0;
    return false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapTypeNew::set_focus(int cid){
#warning COMMENTE
/*ControlRef	c,c2;
ControlID	xid={kXMapTypeNewConnPrmSignature,cid};

	GetControlByID(_wd,&xid,&c);
	GetKeyboardFocus(_wd,&c2);
	if(c2!=c){
		SetKeyboardFocus(_wd,c,kControlFocusNextPart);
	}*/
	NSBeep();
}
