//----------------------------------------------------------------------------
// File : bXMapTypeAdd.mm
// Project : MacMap
// Purpose : Objective C++ source file : XMap class, adding type
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

#include "bXMapTypeAdd.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

#include <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------
@interface TypeAddViewController : NSViewController{
    IBOutlet NSButton* _atv_chk;
}
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
-(BOOL)addToView;
//----------------------------------------------------------------------------
@end


// ---------------------------------------------------------------------------
//
// -----------
@implementation TypeAddViewController
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass{
_bTrace_("[TypeAddViewController allocWithNibNamed owner class]",true);
    return [[loadClass alloc] initWithNibName:nibNamed
                                       bundle:[NSBundle bundleWithIdentifier:@"com.cbconseil.xmap_lib.framework"]];
}
// ---------------------------------------------------------------------------
//
// ------------
-(BOOL)addToView{
    return [_atv_chk intValue];
}
// ---------------------------------------------------------------------------
//
// -----------
@end


// ---------------------------------------------------------------------------
//
// ------------
static void* TypeAdd_getAccessoryViewController(bool hideCreateIfNeeded){
_bTrace_("TypeAdd_getAccessoryViewController",true);
TypeAddViewController   *ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    ctrlr=[TypeAddViewController allocWithNibNamed:@"TypeAddAccessoryView"
                                            class:[TypeAddViewController class]];
    [localPool release];
    return((void*)ctrlr);
}

// ---------------------------------------------------------------------------
//
// ------------
static void* TypeAdd_getAccessoryViewFromController(void* ctrlr){
_bTrace_("TypeAdd_getAccessoryViewFromController",true);
TypeAddViewController   *controller=(TypeAddViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
NSView*   view=[controller view];
    [localPool release];
    return((void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
static bool TypeAdd_getAddToViewFromController(void* ctrlr){
_bTrace_("TypeAdd_getCreateIfNeededFromController",true);
TypeAddViewController   *controller=(TypeAddViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
bool   value=[controller addToView];
    [localPool release];
    return(value);
}

// ---------------------------------------------------------------------------
//
// ------------
static void TypeAdd_releaseController(void* ctrlr){
_bTrace_("TypeAdd_releaseController",true);
TypeAddViewController   *controller=(TypeAddViewController*)ctrlr;
NSAutoreleasePool       *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    [localPool release];
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapTypeAdd::bXMapTypeAdd(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("typeadd");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapTypeAdd::~bXMapTypeAdd(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapTypeAdd::create(bGenericXMLBaseElement* elt){
	return(new bXMapTypeAdd(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTypeAdd::process(int msg, void* prm){
_bTrace_("bXMapTypeAdd::process",false);
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}

char			name[FILENAME_MAX];
char			path[PATH_MAX];
char			fpath[PATH_MAX];
char			docp[PATH_MAX];
char			rel[PATH_MAX];
const char*		nsft[1]={"TYPE"};
const char**	pnsft=nsft;
bArray          paths(sizeof(char[PATH_MAX]));
void*           ctrlr=TypeAdd_getAccessoryViewController(false);
    
    if(NSGetFilePaths(pnsft,
                      1,
                      cNSMacOSRomanStringEncoding,
                      false,
                      true,
                      TypeAdd_getAccessoryViewFromController(ctrlr),
                      paths)==cNSCancelButton){
        TypeAdd_releaseController(ctrlr);
_te_("user canceled");
        return(true);
    }
bool    atv=TypeAdd_getAddToViewFromController(ctrlr);
    TypeAdd_releaseController(ctrlr);
_tm_("addToView: "+atv);
    
bEventLog	log(_gapp,this);
    for(long i=1;i<=paths.count();i++){
        paths.get(i,&fpath);
        splitPath(fpath,path,name);
//RELATIF
		GetBasePath(_gapp,docp);
		strcat(docp,"/");

		relpath(docp,fpath,rel);
		strcat(rel,"/");
		for(int j=strlen(name)-1;j>=0;j--){
			if(name[j]=='.'){
				name[j]=0;
			}
		}
//_tm_(path);
_tm_(name);
_tm_(rel);
//RELATIF
		if(_gapp->typesMgr()->add(name,rel,2)!=noErr){
_te_("Adding type failed for "+name);
		}
		else if(atv){
            _gapp->layersMgr()->add(_gapp->typesMgr()->count(),1);
		}
    }
    log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTypeAdd::edit(void* prm){
bEventLog	log(_gapp,this);
	process(kExtProcessCallFromIntf,prm);
	log.close();
	return(true);
}

