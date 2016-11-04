//----------------------------------------------------------------------------
// File : bStdNSXBox.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Cocoa based XMapBox
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------

#include "bStdNSXBox.h"
#include "bStdNSXBoxCocoaStuff.h"

#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------

#define _DEB_BNDS_	100,100,0,0

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdNSXBox	::bStdNSXBox(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	_openit=false;
	_controller=NULL;
	_sign=GetSignature(this);
    _wd_open=false;
    if(elt==NULL){
        AddToPalettesMenu(gapp,this);
    }
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdNSXBox::~bStdNSXBox(){
//_bTrace_("bStdNSXBox::~bStdNSXBox",false);
    if(_creator==NULL){
//_tm_("yep");
        (void)_gapp->menuMgr()->rmv_item(kMenuMgrMenuPalettesID,_sign);
    }
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdNSXBox::create(bGenericXMLBaseElement* elt){
	return(new bStdNSXBox(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::open(int* flags){
_bTrace_("bStdNSXBox::open",true);
_tm_(_cfname);
	bStdXMap::open(flags);
	_openit=false;
	_wd_open=false;
	_bnds=CGRectMake(_DEB_BNDS_);
	_controller=NULL;	
	
	read_p();

// IL NE FAUT PAS METTRE UNE TAILLE MINI CAR 
// CA REND LES XMAPBOX A TAILLE FIXE INUTILISABLES
//	if(_bnds.size.width<=16){
//		_bnds.size.width=100;
//	}
//	if(_bnds.size.height<=16){
//		_bnds.size.height=100;
//	}
	
CGDirectDisplayID	cgdid=CGMainDisplayID();
CGRect				cgr=CGDisplayBounds(cgdid);
	
	cgr.size.height-=60;
	if(!CGRectContainsRect(cgr,_bnds)){
		_bnds.origin.x=100;
		_bnds.origin.y=100;
	}
	if(_wd_open){
		_openit=true;
	}
_tm_((unsigned int*)&_sign+" => frame:"+_bnds.origin.x+";"+_bnds.origin.y+";"+_bnds.size.width+";"+_bnds.size.height);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::close(){
//_bTrace_("bStdNSXBox::close",true);
//_tm_(_cfname);
	_openit=false;
	if(_wd_open){
		CCWDXBOX_Close(_controller);
		CCWDXBOX_Dispose(_controller);
		_controller=NULL;
		_wd_open=false;
	}
	else{
		write_p();
	}
	bStdXMap::close();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSXBox::event(EventRef evt){	
//_bTrace_("bStdNSXBox::event",true);
//_tm_(_cfname);
	if(!_controller){
		return(false);
	}
	
UInt32	clss=GetEventClass(evt);
UInt32	kind=GetEventKind(evt);
	
	if(clss==kEventClassMacMap){
		switch(kind){
			case kEventMacMapDataBase:
				CCWDXBOX_CheckMMEvt(_controller);
				break;
			default:
				break;
		}
	}
	
// MODIF 12/02/2014
	return(false);
//	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdNSXBox::idle(void* prm){
//_bTrace_("bStdNSXBox::idle",true);
//_tm_(_cfname);
	if(_openit){
		_openit=false;
		edit(NULL);
	}
	else if((_wd_open==false)&&(_controller!=NULL)){
		CCWDXBOX_Dispose(_controller);
		_controller=NULL;
	}
	else if(_controller!=NULL){
		CCWDXBOX_Idle(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSXBox::process(int msg, void* prm){
//_bTrace_("bStdNSXBox::process",true);
//_tm_(_cfname);
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSXBox::test(void* prm){
//_bTrace_("bStdNSXBox::test",true);
//_tm_(_cfname);
	return((!_wd_open)&&(_gapp->document()!=NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSXBox::edit(void* prm){
//_bTrace_("bStdNSXBox::edit",true);
//_tm_(_cfname);
	_controller=ccinit();
	if(_controller){
		_wd_open=true;
	}	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::get_bounds(CGRect* bnds){
//_bTrace_("bStdNSXBox::get_bounds",true);
//_tm_(_cfname);
	(*bnds)=_bnds;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::set_bounds(CGRect* bnds){
//_bTrace_("bStdNSXBox::set_bounds",true);
//_tm_(_cfname);
	_bnds=(*bnds);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::wd_close(){
//_bTrace_("bStdNSXBox::wd_close",true);
//_tm_(_cfname);
	write_p();
	_wd_open=false;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::read_p(){
//_bTrace_("bStdNSXBox::read_p",true);
//_tm_(_cfname);
char					name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,kXBoxSignature,name);
char					val[_values_length_max_];
	
	if((!b)||(!root)){
		write_p();
		(void)_gapp->document()->readTree(&root,kXBoxSignature,name);
		return;
	}
	if((!b)||(!root)){
		return;
	}
	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_wd_open=atoi(val);
	
	elt=_gapp->classMgr()->NthElement(root,1,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_bnds.origin.x=atof(val);
	
	elt=_gapp->classMgr()->NthElement(root,2,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_bnds.origin.y=atof(val);
	
	elt=_gapp->classMgr()->NthElement(root,3,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_bnds.size.width=atof(val);
	
	elt=_gapp->classMgr()->NthElement(root,4,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_bnds.size.height=atof(val);
	
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXBox::write_p(){
//_bTrace_("bStdNSXBox::write_p",true);
//_tm_(_cfname);
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
bGenericXMLBaseElement* bStdNSXBox::make_p(){
//_bTrace_("bStdNSXBox::make_p",true);
//_tm_(_cfname);
bArray	arr(sizeof(xmlelt));
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	add_idesc(arr,1,"bool",_wd_open);	
	add_ddesc(arr,1,"float",_bnds.origin.x,0);
	add_ddesc(arr,1,"float",_bnds.origin.y,0);
	add_ddesc(arr,1,"float",_bnds.size.width,0);
	add_ddesc(arr,1,"float",_bnds.size.height,0);
	
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void* bStdNSXBox::ccinit(){
//_bTrace_("bStdNSXBox::ccinit",true);
//_tm_(_cfname);
	return(CCWDXBOX_Init(getapp(),this));
}
