//----------------------------------------------------------------------------
// File : bStdNSPalettevDef.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Cocoa floating window based vDef
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

#include "bStdNSPalettevDef.h"
#include "bStdNSPalettevDefCocoaStuff.h"
#include "bvDefPaletteQuickStyle.h"

#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------

#define _DEB_BNDS_	100,100,0,0

char * const nsvdef_op_list[]={	(char*)"",
                                (char*)"equal",
                                (char*)"notequal",
                                (char*)"lower",
                                (char*)"lowerorequal",
                                (char*)"higher",
                                (char*)"higherorequal",
                                (char*)"contain",
                                (char*)"beginwith",
                                (char*)"endwith"};

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdNSPalettevDef	::bStdNSPalettevDef(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdExt(elt,gapp,bndl)
                    //:bStdvDef(elt,gapp,bndl)
					,_styles(sizeof(bvDefPaletteQuickStyle*)){
//_bTrace_("bStdNSPalettevDef::bStdNSPalettevDef",true);
	_openit=false;
	_controller=NULL;
	_sign=GetSignature(this);
	strcpy(_vers,"3.0.0");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdNSPalettevDef::~bStdNSPalettevDef(){
//_bTrace_("bStdNSPalettevDef::~bStdNSPalettevDef",true);
//_tm_(_cfname);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdNSPalettevDef::create(bGenericXMLBaseElement* elt){
//_bTrace_("bStdNSPalettevDef::create",true);
//_tm_(_cfname);
	return(new bStdNSPalettevDef(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::open(int* flags){
//_bTrace_("bStdNSPalettevDef::open",true);
//_tm_(_cfname);
//    bStdvDef::open(flags);
    bStdExt::open(flags);
	_openit=false;
	_wd_open=false;
	_bnds=CGRectMake(_DEB_BNDS_);
	_controller=NULL;
	_style_idx=0;
	
	read_p();
	
/*	if(_bnds.size.width<=16){
		_bnds.size.width=100;
	}
	if(_bnds.size.height<=16){
		_bnds.size.height=100;
	}*/
	
CGDirectDisplayID	cgdid=CGMainDisplayID();
CGRect				cgr=CGDisplayBounds(cgdid);
	
	cgr.size.height-=60;
	if(!CGRectContainsRect(cgr,_bnds)){
		_bnds.origin.x=100;
		_bnds.origin.y=100;
	}
	if(_wd_open){
		_openit=true;
	//_tm_("need to open it");
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::close(){
//_bTrace_("bStdNSPalettevDef::close",true);
//_tm_(_cfname);
	_openit=false;
	if(_wd_open){
		CCWDvDef_Close(_controller);
		CCWDvDef_Dispose(_controller);
		_controller=NULL;
		_wd_open=false;
	}
	else{
		write_p();
	}
//    bStdvDef::close();
    bStdExt::close();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSPalettevDef::event(EventRef evt){	
//_bTrace_("bStdNSPalettevDef::event",true);
//_tm_(_cfname);
	if(!_controller){
		return(false);
	}
	
UInt32	clss=GetEventClass(evt);
UInt32	kind=GetEventKind(evt);
	
	if(clss==kEventClassMacMap){
		switch(kind){
			case kEventMacMapDataBase:
				CCWDvDef_CheckMMEvt(_controller);
				break;
			default:
				break;
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdNSPalettevDef::idle(void* prm){
//_bTrace_("bStdNSPalettevDef::idle",true);
//_tm_(_cfname);
	if(_openit){
		_openit=false;
		edit(NULL);
	}
	else if((_wd_open==false)&&(_controller!=NULL)){
		CCWDvDef_Dispose(_controller);
		_controller=NULL;
	}
	else if(_controller!=NULL){
		CCWDvDef_Idle(_controller);
		if(_style_idx==-1){
			if(_tp!=NULL){
				_style_idx=_tp->styles()->index(_root);
				if(_style_idx!=0){
					_tp->styles()->set_on_edit(_style_idx,true);
				}
			}
			else{
				_style_idx=_gapp->document()->styles()->index(_root);
				if(_style_idx!=0){
					_gapp->document()->styles()->set_on_edit(_style_idx,true);
				}				
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSPalettevDef::process(int msg, void* prm){
//_bTrace_("bStdNSPalettevDef::process",true);
//_tm_(_cfname);
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSPalettevDef::test(void* prm){
//_bTrace_("bStdNSPalettevDef::test(void*)",true);
	return(test((bGenericType*)prm));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSPalettevDef::edit(void* prm){
_bTrace_("bStdNSPalettevDef::edit(void*)",true);
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
bool bStdNSPalettevDef::test(bGenericType* tp){
//_bTrace_("bStdNSPalettevDef::test(bGenericType*)",true);
	//_tm_(_cfname);
    return((!_wd_open)&&(_gapp->document()!=NULL));
//	if(_wd_open){
//		return(false);
//	}
//	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdNSPalettevDef::edit(bGenericXMLBaseElement** root, 
							 bGenericType* tp){
_bTrace_("bStdNSPalettevDef::edit(bGenericXMLBaseElement**,bGenericType* tp)",true);
	if(_wd_open){
_te_("Fenetre déjà ouverte");
		return(false);
	}
	
	_root=*root;
	_tp=tp;
	
_tm_("_root="+(void*)_root);
_tm_("_tp="+(void*)_tp);

	_name[0]=0;
	_fld=kOBJ_SubType_;
	_cmyk=false;
	_sref=0;
	_coef=1;
	_cfld=0;
	_cop=0;
	_cval[0]=0;
	
	_pssmax=1;
	_sort=1;
		
	stl_alloc();
	read();
	if(*root==NULL){
		write();
		*root=_root;
	}
	
	_controller=ccinit();
	if(_controller){
		_wd_open=true;
	}
	
	if(_tp!=NULL){ 
		_style_idx=_tp->styles()->index(_root);
		if(_style_idx!=0){
			_tp->styles()->set_on_edit(_style_idx,true);
		}
		else{
			_style_idx=-1;
		}
	}
	else{
		_style_idx=_gapp->document()->styles()->index(_root);
		if(_style_idx!=0){
			_gapp->document()->styles()->set_on_edit(_style_idx,true);
		}
		else{
			_style_idx=-1;
		}
	}
// sinon rien a faire, l'édition se fait a postériori, lors de l'apply
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::get_bounds(CGRect* bnds){
//_bTrace_("bStdNSPalettevDef::get_bounds",true);
//_tm_(_cfname);
	(*bnds)=_bnds;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::set_bounds(CGRect* bnds){
//_bTrace_("bStdNSPalettevDef::set_bounds",true);
//_tm_(_cfname);
	_bnds=(*bnds);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::wd_close(){
//_bTrace_("bStdNSPalettevDef::wd_close",true);
//_tm_(_cfname);
	
int	stli;
	if(_tp!=NULL){
		stli=_tp->styles()->index(_root);
		if(stli!=0){
			_tp->styles()->set_on_edit(stli,false);
		}
	}
	else{
		stli=_gapp->document()->styles()->index(_root);
		if(stli!=0){
			_gapp->document()->styles()->set_on_edit(stli,false);
		}
	}
	write_p();	
	_wd_open=false;
	stl_free();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStdNSPalettevDef::update(){
_bTrace_("bStdNSPalettevDef::update",true);
bGenericXMLBaseElement* elt=_gapp->classMgr()->NthElement(_root,1,"styleidentification");
	if(elt){
		elt=_gapp->classMgr()->NthElement(elt,1,"name");
		if(elt){
			elt->getvalue(_name);
		}
	}
	
	_gapp->layersMgr()->StopDraw();
	
int	stli;
	if(_tp!=NULL){
		stli=_tp->styles()->index(_root);
		if(stli==0){
			return(false);
		}
	}
	else{
		stli=_gapp->document()->styles()->index(_root);
		if(stli==0){
			return(false);
		}
	}
	
	write();
	
	if(_tp!=NULL){
		_tp->styles()->set_root(stli,_root);
	}
	else{
		_gapp->document()->styles()->set_root(stli,_root);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdNSPalettevDef::read_p(){
//_bTrace_("bStdNSPalettevDef::read_p",true);
//_tm_(_cfname);
char					name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,kvDefSignature,name);
char					val[_values_length_max_];
	
	_wd_open=false;

	if((!b)||(!root)){
		write_p();
		/*b=*/(void)_gapp->document()->readTree(&root,kvDefSignature,name);
		return;
	}
	if((!b)||(!root)){
		return;
	}
	
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
void bStdNSPalettevDef::write_p(){
//_bTrace_("bStdNSPalettevDef::write_p",true);
//_tm_(_cfname);
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	sprintf(name,"%.4s",(char*)&_sign);
	if(!_gapp->document()->writeTree(root,kvDefSignature,name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStdNSPalettevDef::make_p(){
//_bTrace_("bStdNSPalettevDef::make_p",true);
//_tm_(_cfname);
bArray	arr(sizeof(xmlelt));
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
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
void* bStdNSPalettevDef::ccinit(){
//_bTrace_("bStdNSPalettevDef::ccinit",true);
//_tm_(_cfname);
	return(CCWDvDef_Init(getapp(),this));
}
