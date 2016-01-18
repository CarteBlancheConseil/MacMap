//----------------------------------------------------------------------------
// File : bStdXBox.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for window based XMapBoxes (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSXBox instead)
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
// 02/12/2005 creation.
//----------------------------------------------------------------------------

#include "bStdXBox.h"
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define kXBoxSignature				'xoBX'
#else
#define kXBoxSignature				'XBox'
#endif

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXBox::bStdXBox(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		:bStdXMapWithIntf(elt,gapp,bndl){
	setclassname("xbox");
	_sign=GetSignature(this);
    if(elt==NULL){
char	name[FILENAME_MAX];
        GetName(this,name);
        (void)_gapp->menuMgr()->add_item(kMenuMgrMenuPalettesID,name,_sign);
    }
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXBox::~bStdXBox(){
//_bTrace_("bStdXBox::~bStdXBox",true);
    if(_creator==NULL){
//_tm_("yep");
        (void)_gapp->menuMgr()->rmv_item(kMenuMgrMenuPalettesID,_sign);
    }
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdXBox::create(bGenericXMLBaseElement* elt){
	return(new bStdXBox(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::open(int* flags){
	bStdXMapWithIntf::open(flags);
	_on=false;
	_wd_open=false;
	_h=100;
	_v=100;
	read_p();
	if(_h<=16){
		_h=100;
	}
	if(_v<=50){
		_v=100;
	}
CGDirectDisplayID	cgdid=CGMainDisplayID();
CGRect				cgr=CGDisplayBounds(cgdid);
	if(_h>cgr.size.width){
		_h=100;
	}
	if(_v>cgr.size.height){
		_v=100;
	}
	if(_wd_open){
		_on=true;
		edit(NULL);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::close(){
	if(_wd_open){
		wd_close();
		_wd_open=false;
	}
	else{
		write_p();
	}
	bStdXMapWithIntf::close();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXBox::event(EventRef evt){
	if(!_wd_open){
		return(false);
	}
UInt32		clss,kind;	

	clss=GetEventClass(evt);
	if(clss==kEventClassWindow){
		kind=GetEventKind(evt);
		switch(kind){
			case kEventWindowActivated:
				_on=true;
				break;
			case kEventWindowDeactivated:
				_on=false;
				break;
			default:
				break;
		}
	}
	else if(clss==kEventClassMouse){
		kind=GetEventKind(evt);
		switch(kind){
			case kEventMouseEntered:
//				if(!IsWindowActive((WindowRef)_gapp->mapwd()->ref())){
//					break;
//				}
				if(!_gapp->mapIntf()->active()){
					break;
				}				
				_on=true;
				break;
			case kEventMouseExited:
//				if(!IsWindowActive((WindowRef)_gapp->mapwd()->ref())){
//					break;
//				}
				if(!_gapp->mapIntf()->active()){
					break;
				}
				if(_on){
					_on=false;
				}
				break;
			default:
				break;
		}
	}
    return(false);
//    return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXBox::nsevent(void* nsevt){
	if(!_wd_open){
		return(false);
	}
    return(false);
    //    return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXBox::idle(void* prm){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdXBox::process(int msg, void* prm){
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdXBox::test(void* prm){
    return((!_wd_open)&&(_gapp->document()!=NULL));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdXBox::wd_event(EventRef evt, WindowRef wd){
HICommand	cmd;
UInt32		clss;	
bool		b=false;
		
	clss=GetEventClass(evt);

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandClose:
				_wd_open=false;
				wd_close();
				break;
			default:
				break;
		}
	}
	else if(clss==kEventClassWindow){
		switch(GetEventKind(evt)){
			case kEventWindowClose:
				_wd_open=false;
				wd_close();
				break;
			case kEventWindowDrawContent:
				break;
			case kEventWindowHandleContentClick:
				break;
			case kEventWindowActivated:
			case kEventWindowDeactivated:
				break;
			default:
				break;
		}
	}	
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::wd_init(WindowRef wd){
	_wd_open=true;
	MoveWindow(wd,_h,_v,true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::wd_close(){
HIRect		hir;
	HIWindowGetBounds(_wd,
					  kWindowGlobalPortRgn,
					  kHICoordSpaceScreenPixel,
					  &hir);
	_h=hir.origin.x;
	_v=hir.origin.y;
	_on=false;
	write_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::read_p(){
_bTrace_("bStdXBox::read_p",false);
char					name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,kXBoxSignature,name);
char					val[_values_length_max_];
	
	if((!b)||(!root)){
_tw_("param not found "+name);
		write_p();
		/*b=*/(void)_gapp->document()->readTree(&root,kXBoxSignature,name);
		return;
	}
	if((!b)||(!root)){
_te_("((!b)||(!root)) for "+name);
		return;
	}
	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_wd_open=atoi(val);
	
	elt=_gapp->classMgr()->NthElement(root,1,"int");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_h=atoi(val);
	
	elt=_gapp->classMgr()->NthElement(root,2,"int");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_v=atoi(val);
		
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdXBox::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
	if(!_gapp->document()->writeTree(root,kXBoxSignature,name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStdXBox::make_p(){
bArray	arr(sizeof(xmlelt));

	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	add_idesc(arr,1,"bool",_wd_open);	
	add_idesc(arr,1,"int",_h);
	add_idesc(arr,1,"int",_v);

bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}
