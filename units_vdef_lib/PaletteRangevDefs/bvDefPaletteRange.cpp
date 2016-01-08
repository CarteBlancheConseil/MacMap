//----------------------------------------------------------------------------
// File : bvDefPaletteRange.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for range vDefs
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

#include "bvDefPaletteRange.h"
#include "bvDefPaletteRangeStyle.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/valconv.h>
#include <vdef_lib/vdef_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteRange	::bvDefPaletteRange(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdNSPalettevDef(elt,gapp,bndl),
					_limits(sizeof(double)){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPaletteRange::~bvDefPaletteRange(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPaletteRange::stl_alloc(int idx){
	_bTrace_("bvDefPaletteRange::stl_alloc(int)",true);
bvDefPaletteRangeStyle*	stl;
	
	if(!idx){
		for(int i=1;i<_limits.count();i++){
			stl=new bvDefPaletteRangeStyle(_gapp,_tp,i);
			_styles.add(&stl);
		}
	}
	else{
bvDefPaletteRangeStyle*	prv;
		if(_styles.count()>1){
			_styles.get(_styles.count()-1,&prv);
			stl=new bvDefPaletteRangeStyle(prv,_limits.count()-1);
		}
		else{
			stl=new bvDefPaletteRangeStyle(_gapp,_tp,_limits.count()-1);
		}
		_styles.add(&stl);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bvDefPaletteRange::make(){
_bTrace_("bvDefPaletteRange::make",true);
	_fld=kOBJ_SubType_;
bArray	arr(sizeof(xmlelt));
char	val[_values_length_max_],lg[_values_length_max_];
int		i,n=_tp->fields()->count_constraints(_fld),nbclass=5,d;
double	amp=(double)(n-1)/nbclass,e=1,e2;
UInt32	sign=_sign;

	_tp->fields()->get_decs(_fld,&d);
	
	_limits.reset();
	for(i=1;i<=nbclass+1;i++){
		_limits.add(&e);
		e+=amp;
	}
	
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
	add_ddesc(arr,2,"scaleref",0,0);
	add_ddesc(arr,2,"unitcoef",1,0);
	add_idesc(arr,2,"sort",2);
	add_idesc(arr,2,"passcount",1);
	add_cdesc(arr,2,"classboundsarray","");
	for(i=1;i<_limits.count();i++){
		_limits.get(i,&e);
		_limits.get(i+1,&e2);
		add_ddesc(arr,3,"classbound",e,15);
		sprintf(lg,"[%.*f-%.*f[",d,e,d,e2);
		add_cdesc(arr,3,"classname",lg);
	}
	_limits.get(_limits.count(),&e);
	e+=kvDefMaxPrecs;
	add_ddesc(arr,3,"classbound",e,15);
	add_cdesc(arr,3,"classname","");
	
// Condition	
	add_cdesc(arr,1,"condition","");
// Nothing else, condition are optional.
	
// StyleRuns
bvDefPaletteRangeStyle* stl;
	for(int i=1;i<=_styles.count();i++){
		_styles.get(i,&stl);
		stl->_clss=i;
		stl->dump(arr,0);
	}
	
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPaletteRange::read(){
bool	made=false,b=false;
	if(!_root){
		make();
		made=true;
	}
	
bGenericXMLBaseElement* chld;
bGenericXMLBaseElement* elt;
bvDefPaletteRangeStyle*		stl;
char					val[_values_length_max_];
int						i;
double					e;
	
	stl_free();
	
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
		
		_limits.reset();
		i=0;
		do{	
			i++;
			elt=_gapp->classMgr()->NthElement(chld,i,"classbound");
			if(elt){
				elt->getvalue(val);
				e=matof(val);
				_limits.add(&e);
			}
		}while(elt);
		_limits.get(_limits.count(),&e);
		e-=kvDefMaxPrecs;
		_limits.put(_limits.count(),&e);
		
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
		elt=_gapp->classMgr()->NthElement(chld,1,"sort");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_sort=atoi(val);
		
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
		stl_alloc(0);
		for(int i=1;i<=_styles.count();i++){
			_styles.get(i,&stl);
			stl->load(_root);
			_cmyk=(stl->_cc==5);
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
bool bvDefPaletteRange::write(){
_bTrace_("bvDefPaletteRange::write",true);
bArray	arr(sizeof(xmlelt));
char	val[_values_length_max_];
char	lg[_values_length_max_];
int		i,k,d;
double	e,e2;
UInt32	sign=_sign;

	_tp->fields()->get_decs(_fld,&d);
	
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
	add_idesc(arr,2,"sort",_sort);
	add_idesc(arr,2,"passcount",1);
	add_cdesc(arr,2,"classboundsarray","");
	
	for(i=1;i<_limits.count();i++){
		_limits.get(i,&e);
		_limits.get(i+1,&e2);
		add_ddesc(arr,3,"classbound",e,15);
		sprintf(lg,"[%.*f-%.*f[",d,e,d,e2);
		add_cdesc(arr,3,"classname",lg);
	}
	_limits.get(_limits.count(),&e);
	e+=kvDefMaxPrecs;
	add_ddesc(arr,3,"classbound",e,15);
	add_cdesc(arr,3,"classname","");
	
// Conditions	
	add_cdesc(arr,1,"condition","");
	if(_cfld){
		add_cdesc(arr,2,vdef_op_list[_cop],"");
		_tp->fields()->get_name(_cfld,val);
		add_cdesc(arr,3,"dbfield",val);
		_tp->fields()->get_kind(_cfld,&k);
		switch(k){
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
bvDefPaletteRangeStyle* stl;
	for(int i=1;i<=_styles.count();i++){
		_styles.get(i,&stl);
		stl->_clss=i;
		stl->dump(arr,0);
	}
	
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	
	free_descs(arr);

	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPaletteRange::rebuild_limits(){
	stl_free();
	stl_alloc(0);
}

