//----------------------------------------------------------------------------
// File : bvDefPaletteProp.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for proportionnal vDefs
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
// 02/01/2013 creation.
//----------------------------------------------------------------------------

#include "bvDefPaletteProp.h"
#include "bvDefPalettePropStyle.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/valconv.h>
#include <vdef_lib/vdef_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteProp	::bvDefPaletteProp(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdNSPalettevDef(elt,gapp,bndl){
	_oor=kvDefPropOutOfRangeEmpty;
	_subfld=kOBJ_SubType_;
	_cmin=0;
	_cmax=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPaletteProp::~bvDefPaletteProp(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPaletteProp::stl_alloc(){
	_bTrace_("bvDefPaletteProp::stl_alloc()",true);
bvDefPalettePropStyle*	stl;
	stl=new bvDefPalettePropStyle(_gapp,_tp,true);
	_styles.add(&stl);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bvDefPaletteProp::make(){
_bTrace_("bvDefPaletteProp::make",true);
bArray	arr(sizeof(xmlelt));
char	val[256];
UInt32	sign=_sign;
	
	add_cdesc(arr,0,_stylename,"");
	
	// Identification		
	add_cdesc(arr,1,"styleidentification","");
	add_cdesc(arr,2,"name",_name);
	sprintf(val,"%.4s",&sign);
	_tm_(val);
	add_cdesc(arr,2,"sign",val);
	add_cdesc(arr,2,"version",_vers);
	
	// Globals
	add_cdesc(arr,1,"globals","");
	_tp->fields()->get_name(_fld,val);
	add_cdesc(arr,2,"classfield",val);
	bGenericUnit*	u=_gapp->scaleMgr()->get();
	add_ddesc(arr,2,"scaleref",u->coef(),6);
	add_ddesc(arr,2,"unitcoef",1,0);
	add_idesc(arr,2,"sort",-1);
	add_idesc(arr,2,"passcount",1);
	add_cdesc(arr,2,"classboundsarray","");
	add_ddesc(arr,3,"classbound",1,0);
	add_cdesc(arr,3,"classname","min");
	add_ddesc(arr,3,"classbound",LONG_MAX,0);
	add_cdesc(arr,3,"classname","max");
	
// Condition	
	add_cdesc(arr,1,"condition","");
// Nothing else, condition are optional.
	
// StyleRuns
bvDefPalettePropStyle* stl;
	for(int i=1;i<=_styles.count();i++){
		_styles.get(i,&stl);
		stl->dump(arr,0);
	}
	
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteProp::read(){
bool	made=false,b=false;
	if(!_root){
		make();
		made=true;
	}
	
bGenericXMLBaseElement* chld;
bGenericXMLBaseElement* elt;
bvDefPalettePropStyle*	stl;
char					val[_values_length_max_];
int						i;
double					bmax=LONG_MAX;
	
	for(;;){
// Identification		
		chld=_gapp->classMgr()->NthElement(_root,1,"styleidentification");
		if(!chld){
			break;
		}
		elt=_gapp->classMgr()->NthElement(chld,1,"name");
		if(!elt){
			break;
		}
		elt->getvalue(_name);
		elt=_gapp->classMgr()->NthElement(chld,1,"version");
		if(!elt){
			break;
		}
		elt->getvalue(_vers);
		if(strlen(_vers)==0){
			strcpy(_vers,"3.0.0");
		}
// Globals
		chld=_gapp->classMgr()->NthElement(_root,1,"globals");
		if(!chld){
			break;
		}
		
		elt=_gapp->classMgr()->NthElement(chld,1,"classfield");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_fld=_tp->fields()->get_index(val);
		if(_fld==0){
			_fld=kOBJ_SubType_;
		}
		if(_fld!=kOBJ_SubType_){
			stl_free();
			stl_alloc();
		}
		
		i=0;
		do{	
			i++;
			elt=_gapp->classMgr()->NthElement(chld,i,"classbound");
			if(elt){
				elt->getvalue(val);
				bmax=matof(val);
			}
		}while(elt);
		_oor=(bmax==LONG_MAX)?kvDefPropOutOfRangeEmpty:kvDefPropOutOfRangeInvisible;
		
		elt=_gapp->classMgr()->NthElement(chld,1,"scaleref");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_sref=matof(val);
		elt=_gapp->classMgr()->NthElement(chld,1,"unitcoef");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_coef=matof(val);
		
// Condition		
		chld=_gapp->classMgr()->NthElement(_root,1,"condition");
		if(!chld){
			break;
		}
		if(chld->countelements()>0){
			elt=chld->getelement(1);
			elt->getclassname(val);
			for(i=1;i<=vdef_op_max;i++){
				if(!strcmp(val,vdef_op_list[i])){
					chld=elt;
					if(chld->countelements()==2){
						elt=chld->getelement(1);
						elt->getvalue(val);
						_cfld=_tp->fields()->get_index(val);
						if(_cfld){
							elt=chld->getelement(2);
							elt->getvalue(_cval);
						}
					}
					else{
						_cfld=0;
					}
					break;
				}
			}
			if(i==vdef_op_max+1){
				_cfld=0;
			}
			else{
				_cop=i;
			}
		}
		else{
			_cfld=0;
		}
		
// StyleRuns
color_range64	rng;
		
		for(long i=1;i<=_styles.count();i++){
			_styles.get(i,&stl);
			stl->load(_root);
			_cmyk=(stl->_cc==5);
			if(stl->_arr.count()>1){
				stl->_arr.get(1,&rng);
				_cmin=rng.bound;
				stl->_arr.get(stl->_arr.count(),&rng);
				_cmax=rng.bound;
				_subfld=stl->_subfld;
			}
		}
		
		b=true;
		break;
	}
	
	if(made){
		_gapp->classMgr()->ReleaseXMLInstance(_root);
		_root=NULL;
	}
	
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------


bool bvDefPaletteProp::write(){
_bTrace_("bvDefPaletteProp::write",true);
UInt32					sign=_sign;
bArray					arr(sizeof(xmlelt));
char					val[_values_length_max_];
int						n;
bvDefPalettePropStyle*	stl;
	
	_styles.get(1,&stl);
	
	add_cdesc(arr,0,_stylename,"");
	
// Identification			
	add_cdesc(arr,1,"styleidentification","");
	add_cdesc(arr,2,"name",_name);
	sprintf(val,"%.4s",&sign);
	add_cdesc(arr,2,"sign",val);
	add_cdesc(arr,2,"version",_vers);
	
// Globals		
	add_cdesc(arr,1,"globals","");
	_tp->fields()->get_name(_fld,val);
	add_cdesc(arr,2,"classfield",val);
	add_ddesc(arr,2,"scaleref",_sref,0);
	add_ddesc(arr,2,"unitcoef",_coef,6);
	add_idesc(arr,2,"sort",-1);
	add_idesc(arr,2,"passcount",1);
	add_cdesc(arr,2,"classboundsarray","");
	add_ddesc(arr,3,"classbound",stl->_qref,15);
	add_cdesc(arr,3,"classname","min");
	if(_oor==kvDefPropOutOfRangeEmpty){
		add_ddesc(arr,3,"classbound",LONG_MAX,0);
	}
	else{
		add_ddesc(arr,3,"classbound",stl->_qmax+0.000001,6);
	}
	add_cdesc(arr,3,"classname","max");
	
// Conditions	
	add_cdesc(arr,1,"condition","");
	if(_cfld){
		add_cdesc(arr,2,vdef_op_list[_cop],"");
		_tp->fields()->get_name(_cfld,val);
		add_cdesc(arr,3,"dbfield",val);
		_tp->fields()->get_kind(_cfld,&n);
		switch(n){
			case _bool:
				add_cdesc(arr,3,"dbbool",_cval);
				break;
			case _int:
				add_cdesc(arr,3,"dbint",_cval);
				break;
			case _double:
				add_cdesc(arr,3,"dbfloat",_cval);
				break;
			case _date:
				add_cdesc(arr,3,"dbdate",_cval);
				break;
			case _time:
				add_cdesc(arr,3,"dbtime",_cval);
				break;
			default:
				add_cdesc(arr,3,"dbchar",_cval);
				break;
		}
	}
	
// StyleRuns	
	for(int i=1;i<=_styles.count();i++){
		_styles.get(i,&stl);
		stl->dump(arr,0);
	}
	
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	
	free_descs(arr);

	return(true);
}
