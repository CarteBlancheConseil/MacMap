//----------------------------------------------------------------------------
// File : bvDefVirtualStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for virtual styles
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
// 07/07/2009 creation.
//----------------------------------------------------------------------------

#include "bvDefVirtualStyle.h"
#include <limits.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefVirtualStyle::bvDefVirtualStyle(bGenericMacMapApp* gapp){
	_gapp=gapp;
	_gstl=NULL;	
	_smin=1;
	_smax=INT_MAX;
	_pass=1;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefVirtualStyle::~bvDefVirtualStyle(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefVirtualStyle::load(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefVirtualStyle::dump(bArray& arr, int indent){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefVirtualStyle::std_load(bGenericXMLBaseElement* root){
	_gstl=NULL;
	find_stylerun(root);
	if(!_gstl){
		return(false);
	}
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];

// _pass predefined
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
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefVirtualStyle::std_dump(bArray& arr, int indent){
	add_cdesc(arr,indent+1,"stylerun","");
	add_cdesc(arr,indent+2,"validity","");
	add_ddesc(arr,indent+3,"scalemin",_smin,6);
	add_ddesc(arr,indent+3,"scalemax",(_smax==INT_MAX)?_smax:_smax+1,6);
	add_idesc(arr,indent+3,"pass",_pass);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefVirtualStyle::find_stylerun(bGenericXMLBaseElement* root){
	(void)root->dotoall(this,0,test_stylerun);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefVirtualStyle::test_stylerun(bGenericXMLBaseElement* root, void* prm, int indent){
bvDefVirtualStyle*			stl=(bvDefVirtualStyle*)prm;	
char						val[_values_length_max_];
bGenericXMLBaseElement*		elt;

	root->getclassname(val);
	if(!strcmp("stylerun",val)){
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

