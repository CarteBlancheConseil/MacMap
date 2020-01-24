//----------------------------------------------------------------------------
// File : bStdNSXMap.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Cocoa floating window based XMap
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
// 14/04/2014 creation.
//----------------------------------------------------------------------------

#include "bStdNSXMap.h"
#include "bStdNSXMapCocoaStuff.h"

#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------

#define _DEB_BNDS_	100,100,0,0

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdNSXMap	::bStdNSXMap(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	_controller=NULL;
	_sign=GetSignature(this);
    _wd_open=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdNSXMap::~bStdNSXMap(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdNSXMap::create(bGenericXMLBaseElement* elt){
	return(new bStdNSXMap(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXMap::open(int* flags){
_bTrace_("bStdNSXMap::open",true);
_tm_(_cfname);
	bStdXMap::open(flags);
	_wd_open=false;
	_bnds=CGRectMake(_DEB_BNDS_);
	_controller=NULL;	
	
	read_p();

// IL NE FAUT PAS METTRE UNE TAILLE MINI CAR 
// CA REND LES XMAP A TAILLE FIXE INUTILISABLES
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
_tm_((unsigned int*)&_sign+" => frame:"+_bnds.origin.x+";"+_bnds.origin.y+";"+_bnds.size.width+";"+_bnds.size.height);
_tm_(_wd_open);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXMap::close(){
_bTrace_("bStdNSXMap::close",true);
//_tm_(_cfname);
	if(_wd_open){
		CCWDXMap_Close(_controller);
		CCWDXMap_Dispose(_controller);
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
bool bStdNSXMap::nsevent(void* nsevt){
//_bTrace_("bStdNSXMap::nsevent",true);
//_tm_(_cfname);
	if(!_controller){
		return(false);
	}

NSEvent* evt=(NSEvent*)nsevt;
    if( ([evt type]==NSEventTypeApplicationDefined)     &&
       ([evt subtype]==NSEventSubtypeMacMapDataBase)	){
        CCWDXMap_CheckMMEvt(_controller);
    }
	
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdNSXMap::idle(void* prm){
//_bTrace_("bStdNSXMap::idle",false);
//_tm_(_cfname);
	if((_wd_open==true)&&(_controller==NULL)){
        edit(NULL);
	}
    else if((_wd_open==false)&&(_controller!=NULL)){
        CCWDXMap_Dispose(_controller);
        _controller=NULL;
    }
	else if(_controller!=NULL){
		CCWDXMap_Idle(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSXMap::process(int msg, void* prm){
//_bTrace_("bStdNSXMap::process",true);
//_tm_(_cfname);
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSXMap::test(void* prm){
//_bTrace_("bStdNSXMap::test",true);
//_tm_(_cfname+": "+!_wd_open);
    return((!_wd_open)&&(_gapp->document()!=NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSXMap::edit(void* prm){
//_bTrace_("bStdNSXMap::edit",true);
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
void bStdNSXMap::get_bounds(CGRect* bnds){
//_bTrace_("bStdNSXMap::get_bounds",true);
//_tm_(_cfname);
	(*bnds)=_bnds;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXMap::set_bounds(CGRect* bnds){
//_bTrace_("bStdNSXMap::set_bounds",true);
//_tm_(_cfname);
	_bnds=(*bnds);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXMap::wd_close(){
//_bTrace_("bStdNSXMap::wd_close",true);
//_tm_(_cfname);
	write_p();
	_wd_open=false;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSXMap::read_p(){
//_bTrace_("bStdNSXMap::read_p",true);
//_tm_(_cfname);
char					name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,kXMapSignature,name);
char					val[_values_length_max_];
	
	if((!b)||(!root)){
		write_p();
		/*b=*/(void)_gapp->document()->readTree(&root,kXMapSignature,name);
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
void bStdNSXMap::write_p(){
_bTrace_("bStdNSXMap::write_p",false);
//_tm_(_cfname);
    if(!_gapp->document()){
_te_(_cfname +" : document already released");
        return;
    }
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
	if(!_gapp->document()->writeTree(root,kXMapSignature,name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStdNSXMap::make_p(){
//_bTrace_("bStdNSXMap::make_p",true);
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
void* bStdNSXMap::ccinit(){
//_bTrace_("bStdNSXMap::ccinit",true);
//_tm_(_cfname);
	return(CCWDXMap_Init(getapp(),this));
}
