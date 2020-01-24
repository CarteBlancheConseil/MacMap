//----------------------------------------------------------------------------
// File : bExtLibMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Plugins manager/loader class
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
// 17/09/2015 creation.
//----------------------------------------------------------------------------

#include "bExtLibMgr.h"
#include "bStdExtLib.h"
#include "bStdCompLib.h"
#include "bSplashWd.h"
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bExtLibMgr	::bExtLibMgr(bGenericMacMapApp* gapp)
			:_elts(sizeof(bStdExtLib*))
			,_acls(sizeof(bStdCompLib*))
			,_rez(sizeof(void*)){
    _gapp=gapp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bExtLibMgr::~bExtLibMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bExtLibMgr::load(){
_bTrace_("bExtLibMgr::load()",true);
OSErr			pErr1=noErr,pErr2=noErr;
CFArrayRef		bundles;
CFBundleRef		bndl=CFBundleGetMainBundle();
CFURLRef		url;

	url=CFBundleCopyBuiltInPlugInsURL(bndl);
	if(url!=NULL){
		bundles=CFBundleCreateBundlesFromDirectory(kCFAllocatorSystemDefault,url,CFSTR("bundle"));
		CFRelease(url);
		if(bundles!=NULL){
_tm_((int)CFArrayGetCount(bundles)+" plugins");
			pErr2=load(bundles);
			CFRelease(bundles);
		}
		else{
			pErr2=coreFoundationUnknownErr;
		}
	}
	else{
_tm_("Plug-Ins directory not found");
		pErr2=coreFoundationUnknownErr;
	}

	if(pErr1&&pErr2){
_te_("Plug-Ins load ("+pErr1+"/"+pErr2+")");
		return(-1000);
	}

	return(noErr);
}

// ---------------------------------------------------------------------------
//
// -----------
int bExtLibMgr::load(CFArrayRef bundles){
_bTrace_("bExtLibMgr::load(CFArrayRef)",false);
CFBundleRef		bundle;
int				i,n;
UInt32			cr,tp;
bStdExtLib*		ext;
bStdCompLib*	cmp;
bArray			warn(sizeof(bStdExtLib*));
char			c[256];
CFBundleRef		frmkbndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.loader.framework"));

    (void)bSplashWd::instance(false,_gapp);
    
OSStatus  status;
/*IBNibRef  nib=NULL;
    
    status=CreateNibReferenceWithCFBundle(frmkbndl,CFSTR("menubar"),&nib);
    if(status){
_te_("CreateNibReferenceWithCFBundle="+(int)status);
        return(status);
    }
    if((status=SetMenuBarFromNib(nib,CFSTR("MenuBar")))){
_te_("SetMenuBarFromNib"+status);
        return(status);
    }
    DisposeNibReference(nib);*/

	n=(bundles!=NULL)?CFArrayGetCount(bundles):0;
	for(i=0;i<n;i++){
		bundle=(CFBundleRef)CFArrayGetValueAtIndex(bundles,i);
		CFBundleGetPackageInfo(bundle,&tp,&cr);
// App component
		if(cr=='kCom'){
			cmp=new bStdCompLib(bundle);
			if(cmp->status()!=noErr){
_te_("kernel component");
				exit(0);
			}
char	name[256],vers[256];
			cmp->name(name);
			cmp->vers(vers);
_tm_("app component "+name+" ("+vers+")");
			if(!_acls.add(&cmp)){
				return(-1);
			}
			continue;
		}

		if(cr!='MaMo'){
// Unknown
			continue;
		}
		
// External library
		switch(tp){
			case kXMLSubClassExt:
			case kXMLSubClassExtTool:
			case kXMLSubClassExtXMap:
			case kXMLSubClassExtvDef:
			case kXMLSubClassExtGeog:
			case kXMLSubClassExtCalc:
			case kXMLSubClassExtXBox:
			case kXMLSubClassExtUserCalc:
			case kXMLSubClassExtMacro:
			case kXMLSubClassExtVar:
			case kXMLSubClassExtScript:
				ext=new bStdExtLib(bundle,_gapp);
				break;
			default:
				ext=NULL;
				break;
		}
		if(ext==NULL){
_te_("unknown ext type : "+(UInt32*)&tp);		
			continue;
		}
		if(ext->status()!=noErr){
_te_("bad ext status : "+ext->status());		
			warn.add(&ext);
			continue;
		}
		if(!_elts.add(&ext)){
			return(-1);
		}
	}
	
_tm_("----- BEGIN REMOVING -----");	
	for(i=warn.count();i>0;i--){
		warn.get(i,&ext);
		ext->name(c);
_tm_("cant activate "+c);
		delete ext;
	}
_tm_("----- END REMOVING -----");
    
    (void)bSplashWd::release_instance();

	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bExtLibMgr::unload(){
bStdExtLib*		ext;
	for(int i=1;i<_elts.count();i++){
		if(_elts.get(i,&ext)){
			delete ext;
		}
	}
	_elts.reset();

bStdCompLib*	cmp;
	for(int i=1;i<_acls.count();i++){
		if(_acls.get(i,&cmp)){
			delete cmp;
		}
	}
	_acls.reset();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bExtLibMgr::getlibs(bArray& arr, UInt32 family){
bStdExtLib*	ext;
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&ext)){
			if(family!=_kNULLSign_){			
				if(ext->family()!=family){
					continue;
				}
			}
			if(!arr.add(&ext)){
				return(-2);
			}
		}
		else{
			return(-1);
		}
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericCompLib* bExtLibMgr::get_component(const char* name){
bStdCompLib     dummy(name);
bStdCompLib*	pdummy=&dummy;
int				idx=_acls.search(&pdummy,comp);
	if(idx==0){
		return(NULL);
	}
	_acls.get(idx,&pdummy);
	return(pdummy);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bExtLibMgr::comp(const void *a, const void *b){
bGenericCompLib*	ea=(*((bGenericCompLib**)a));
bGenericCompLib*	eb=(*((bGenericCompLib**)b));
char                aname[256];
char                bname[256];
	
	ea->name(aname);
	eb->name(bname);
	return(strcmp(aname,bname));
}


