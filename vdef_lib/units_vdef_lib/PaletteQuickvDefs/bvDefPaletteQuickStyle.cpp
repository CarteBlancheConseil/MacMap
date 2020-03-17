//----------------------------------------------------------------------------
// File : bvDefPaletteQuickStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for quick (standard) vDef styles
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
// 02/06/2006 creation.
//----------------------------------------------------------------------------

#include "bvDefPaletteQuickStyle.h"
#include <limits.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteQuickStyle::bvDefPaletteQuickStyle(bGenericMacMapApp* gapp,
											   bGenericType* gtp,
											   int clss){
	_gapp=gapp;
	_gtp=gtp;
	_gstl=NULL;

	_clss=clss;
	
	_smin=1;
	_smax=INT_MAX;
	_pass=1;

	_offx=0;
	_offy=0;
	_cc=4;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPaletteQuickStyle::~bvDefPaletteQuickStyle(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::load(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::dump(bArray& arr, int indent){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::std_load(bGenericXMLBaseElement* root){
	_gstl=NULL;
	find_stylerun(root);
	if(!_gstl){
		return(false);
	}
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];

// _clss & _pass predefined
	elt=_gapp->classMgr()->NthElement(_gstl,1,"scalemin");
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	_smin=matof(val);
	elt=_gapp->classMgr()->NthElement(_gstl,1,"scalemax");
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	_smax=matof(val);
	
	off_load(root);
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::std_dump(bArray& arr, int indent){
	add_cdesc(arr,indent+1,"stylerun","");
	add_cdesc(arr,indent+2,"validity","");
	add_idesc(arr,indent+3,"classmin",_clss);
	add_idesc(arr,indent+3,"classmax",_clss+1);
	add_ddesc(arr,indent+3,"scalemin",_smin,6);
	add_ddesc(arr,indent+3,"scalemax",(_smax==INT_MAX)?_smax:_smax+1,6);
	add_idesc(arr,indent+3,"pass",_pass);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::off_load(bGenericXMLBaseElement* root){
bGenericXMLBaseElement*		chld;
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];

	chld=_gapp->classMgr()->NthElement(_gstl,1,"offsetcoord");
	if(!chld){
		_offx=0;
		_offy=0;
		return(true);
	}
	elt=_gapp->classMgr()->NthElement(chld,1,"field");
	if(!elt){
		_offx=0;
		_offy=0;
		return(true);
	}
	elt->getvalue(val);
	_offx=_gtp->fields()->get_index(val);
	elt=_gapp->classMgr()->NthElement(chld,2,"field");
	if(!elt){
		_offx=0;
		_offy=0;
		return(true);
	}
	elt->getvalue(val);
	_offy=_gtp->fields()->get_index(val);
	if(!_offx||!_offy){
		_offx=0;
		_offy=0;
		return(true);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteQuickStyle::off_dump(bArray& arr, int indent){
	if((_offx==0)||(_offy==0)){
		return(true);
	}
char	xname[256],yname[256];
	if(!_gtp->fields()->get_name(_offx,xname)){
		_offx=0;
	}
	if(!_gtp->fields()->get_name(_offy,yname)){
		_offy=0;
	}
	if((_offx>0)&&(_offy>0)){
		add_cdesc(arr,indent+3,"offsetcoord","");
		add_cdesc(arr,indent+4,"field",xname);
		add_cdesc(arr,indent+4,"field",yname);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPaletteQuickStyle::find_stylerun(bGenericXMLBaseElement* root){
	(void)root->dotoall(this,0,test_stylerun);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefPaletteQuickStyle::test_stylerun(bGenericXMLBaseElement* root, void* prm, int indent){
bvDefPaletteQuickStyle*			stl=(bvDefPaletteQuickStyle*)prm;	
char						val[_values_length_max_];
bGenericXMLBaseElement*		elt;

	root->getclassname(val);
	if(!strcmp("stylerun",val)){
		elt=stl->_gapp->classMgr()->NthElement(root,1,"classmin");
		if(!elt){
			return(true);
		}
		elt->getvalue(val);
		if(stl->_clss!=atoi(val)){
			return(true);
		}
				
		elt=stl->_gapp->classMgr()->NthElement(root,1,"pass");
		if(!elt){
			return(true);
		}
		elt->getvalue(val);
		if(stl->_pass!=atoi(val)){
			return(true);
		}
		stl->_gstl=root;
		return(false);
	}
	return(true);
}

