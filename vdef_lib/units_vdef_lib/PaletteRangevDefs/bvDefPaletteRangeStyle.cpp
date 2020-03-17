//----------------------------------------------------------------------------
// File : bvDefPaletteRangeStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for range vDef styles
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
// 20/08/2011 creation.
//----------------------------------------------------------------------------

#include "bvDefPaletteRangeStyle.h"
#include <limits.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteRangeStyle::bvDefPaletteRangeStyle(bGenericMacMapApp* gapp,
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
	
	strcpy(_fld,"");
	_vmin=0;
	_vmax=0;
	_fcolor[0]=0;
	_fcolor[1]=0;
	_fcolor[2]=0;
	_fcolor[3]=1;
	_fcolor[4]=1;
	_tcolor[0]=0;
	_tcolor[1]=0;
	_tcolor[2]=0;
	_tcolor[3]=1;
	_tcolor[4]=1;
	
	_visible=true;
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteRangeStyle::bvDefPaletteRangeStyle(bvDefPaletteRangeStyle* stl,
											   int clss){
	_gapp=stl->_gapp;
	_gtp=stl->_gtp;
	_gstl=NULL;
	
	_clss=clss;
	
	_smin=stl->_smin;
	_smax=stl->_smax;
	_pass=stl->_pass;
	
	_offx=stl->_offx;
	_offy=stl->_offy;
	
	_cc=stl->_cc;
	
	strcpy(_fld,stl->_fld);
	_vmin=stl->_vmin;
	_vmax=stl->_vmax;
	_fcolor[0]=stl->_fcolor[0];
	_fcolor[1]=stl->_fcolor[1];
	_fcolor[2]=stl->_fcolor[2];
	_fcolor[3]=stl->_fcolor[3];
	_fcolor[4]=stl->_fcolor[4];
	_tcolor[0]=stl->_tcolor[0];
	_tcolor[1]=stl->_tcolor[1];
	_tcolor[2]=stl->_tcolor[2];
	_tcolor[3]=stl->_tcolor[3];
	_tcolor[4]=stl->_tcolor[4];
	
	_visible=stl->_visible;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPaletteRangeStyle::~bvDefPaletteRangeStyle(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::load(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::dump(bArray& arr, int indent){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::std_load(bGenericXMLBaseElement* root){
	_bTrace_("bvDefPaletteRangeStyle::std_load",true);
	_gstl=NULL;
	find_stylerun(root);
	if(!_gstl){
		_tm_("_gstl==NULL");
		_visible=false;
		return(false);
	}
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	elt=_gapp->classMgr()->NthElement(_gstl,1,"classmin");
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	_clss=atoi(val);
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
bool bvDefPaletteRangeStyle::std_dump(bArray& arr, int indent){
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
bool bvDefPaletteRangeStyle::off_load(bGenericXMLBaseElement* root){
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
bool bvDefPaletteRangeStyle::off_dump(bArray& arr, int indent){
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
bool bvDefPaletteRangeStyle::color_load(bGenericXMLBaseElement* root, double* clr){
//_bTrace_("bvDefPaletteRangeStyle::color_load",tru);
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	if((elt=_gapp->classMgr()->NthElement(root,1,"red"))){
		//_tm_("red");
		elt->getvalue(val);
		clr[0]=matof(val);
		_cc=4;
	}
	if((elt=_gapp->classMgr()->NthElement(root,1,"green"))){
		//_tm_("green");
		elt->getvalue(val);
		clr[1]=matof(val);
		_cc=4;
	}
	if((elt=_gapp->classMgr()->NthElement(root,1,"blue"))){
		//_tm_("blue");
		elt->getvalue(val);
		clr[2]=matof(val);
		_cc=4;
	}
	
	if((elt=_gapp->classMgr()->NthElement(root,1,"cyan"))){
		//_tm_("cyan");
		elt->getvalue(val);
		clr[0]=matof(val);
		_cc=5;
	}
	if((elt=_gapp->classMgr()->NthElement(root,1,"magenta"))){
		//_tm_("magenta");
		elt->getvalue(val);
		clr[1]=matof(val);
		_cc=5;
	}
	if((elt=_gapp->classMgr()->NthElement(root,1,"yellow"))){
		//_tm_("yellow");
		elt->getvalue(val);
		clr[2]=matof(val);
		_cc=5;
	}
	if((elt=_gapp->classMgr()->NthElement(root,1,"black"))){
		//_tm_("black");
		elt->getvalue(val);
		clr[3]=matof(val);
		_cc=5;
	}
	
	if((elt=_gapp->classMgr()->NthElement(root,1,"alpha"))){
		//_tm_("alpha");
		elt->getvalue(val);
		clr[_cc-1]=matof(val);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::color_dump(bArray& arr, int indent, double* clr){
	if(_cc==4){
		add_ddesc(arr,indent,"red",clr[0],2);
		add_ddesc(arr,indent,"green",clr[1],2);
		add_ddesc(arr,indent,"blue",clr[2],2);
		add_ddesc(arr,indent,"alpha",clr[3],2);		
	}
	else{
		add_ddesc(arr,indent,"cyan",clr[0],2);
		add_ddesc(arr,indent,"magenta",clr[1],2);
		add_ddesc(arr,indent,"yellow",clr[2],2);
		add_ddesc(arr,indent,"black",clr[3],2);
		add_ddesc(arr,indent,"alpha",clr[4],2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::prop_load(bGenericXMLBaseElement* root){
bGenericXMLBaseElement*		chld;
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	chld=_gapp->classMgr()->NthElement(root,1,"propcolor");
	if(!chld){
		return(false);
	}
	elt=_gapp->classMgr()->NthElement(chld,1,"field");
	if(!elt){
		return(true);
	}
	elt->getvalue(_fld);
	elt=_gapp->classMgr()->NthElement(chld,1,"float");
	if(!elt){
		return(true);
	}
	elt->getvalue(val);
	_vmin=matof(val);
	elt=_gapp->classMgr()->NthElement(chld,2,"float");
	if(!elt){
		return(true);
	}
	elt->getvalue(val);
	_vmax=matof(val);
	
	_cc=(chld->countelements()-3)/2;
	for(int i=0;i<_cc;i++){
		elt=_gapp->classMgr()->NthElement(chld,i+3,"float");
		if(!elt){
			return(true);
		}
		elt->getvalue(val);
		_fcolor[i]=matof(val);
	}
	for(int i=0;i<_cc;i++){
		elt=_gapp->classMgr()->NthElement(chld,i+3+_cc,"float");
		if(!elt){
			return(true);
		}
		elt->getvalue(val);
		_tcolor[i]=matof(val);
	}
	if((_cc!=4)&&(_cc!=5)){
		_cc=4;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRangeStyle::prop_dump(bArray& arr, int indent){
	if(strlen(_fld)==0){
		return(false);
	}
	add_cdesc(arr,indent,"propcolor","");
	add_cdesc(arr,indent+1,"field",_fld);
	add_ddesc(arr,indent+1,"float",_vmin,15);
	add_ddesc(arr,indent+1,"float",_vmax,15);
	for(int i=0;i<_cc;i++){
		add_ddesc(arr,indent+1,"float",_fcolor[i],2);
	}
	for(int i=0;i<_cc;i++){
		add_ddesc(arr,indent+1,"float",_tcolor[i],2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPaletteRangeStyle::find_stylerun(bGenericXMLBaseElement* root){
	(void)root->dotoall(this,0,test_stylerun);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefPaletteRangeStyle::test_stylerun(bGenericXMLBaseElement* root, void* prm, int indent){
bvDefPaletteRangeStyle*		stl=(bvDefPaletteRangeStyle*)prm;	
char						val[_values_length_max_];
bGenericXMLBaseElement*		elt;
	_bTrace_("bvDefPaletteRangeStyle::test_stylerun",false);
	
	root->getclassname(val);
	if(!strcmp("stylerun",val)){
		elt=stl->_gapp->classMgr()->NthElement(root,1,"classmin");
		if(!elt){
			_te_("no classmin");
			return(true);
		}
		elt->getvalue(val);
		if(stl->_clss!=atoi(val)){
			_tm_("bad class "+stl->_clss+":"+atoi(val));
			return(true);
		}
		elt=stl->_gapp->classMgr()->NthElement(root,1,"pass");
		if(!elt){
			_te_("no pass");
			return(true);
		}
		elt->getvalue(val);
		if(stl->_pass!=atoi(val)){
			_tm_("bad pass "+stl->_pass+":"+atoi(val));
			return(true);
		}
		stl->_gstl=root;
		return(false);
	}
	return(true);
}
