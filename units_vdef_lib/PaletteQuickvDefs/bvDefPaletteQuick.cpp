//----------------------------------------------------------------------------
// File : bvDefPaletteQuick.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for quick (standard) vDefs
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

#include "bvDefPaletteQuick.h"
#include "bvDefPaletteQuickStyle.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/valconv.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPaletteQuick	::bvDefPaletteQuick(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdNSPalettevDef(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPaletteQuick::~bvDefPaletteQuick(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bvDefPaletteQuick::make(){
_bTrace_("bvDefPaletteQuick::make",true);
bArray	arr(sizeof(xmlelt));
char	val[256];
int		i,n=_tp->fields()->count_constraints(_fld);
UInt32	sign=_sign;
	
/*#ifdef __LITTLE_ENDIAN__
_tm_("swap signature");
LBSwapWord(&sign,sizeof(UInt32));
#endif*/
	
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
	add_ddesc(arr,2,"scaleref",0,0);
	add_ddesc(arr,2,"unitcoef",1,0);
	add_idesc(arr,2,"sort",_sort);
	add_idesc(arr,2,"passcount",_pssmax);
	add_cdesc(arr,2,"classboundsarray","");
	for(i=1;i<=n;i++){
		add_idesc(arr,3,"classbound",i);
		_tp->fields()->get_constraint(_fld,i,val);
		add_cdesc(arr,3,"classname",val);
	}	
	add_idesc(arr,3,"classbound",i);
	add_cdesc(arr,3,"classname","");
	
// Condition	
	add_cdesc(arr,1,"condition","");
// Nothing else, condition are optional.
	
// StyleRuns
bvDefPaletteQuickStyle* stl;
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
bool bvDefPaletteQuick::read(){
_bTrace_("bvDefPaletteQuick::read",true);
bool	made=false,b=false;
	if(!_root){
		make();
		made=true;
	}
	
_tm_("make passé");
bGenericXMLBaseElement* chld;
bGenericXMLBaseElement* elt;
bvDefPaletteQuickStyle*	stl;
char					val[_values_length_max_];
int						i;
	
	for(;;){
// Identification		
_tm_("Identification");
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
_tm_("Globals");
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
		elt=_gapp->classMgr()->NthElement(chld,1,"passcount");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_pssmax=atoi(val);
		
// Condition		
_tm_("Condition");
		chld=_gapp->classMgr()->NthElement(_root,1,"condition");
		if(!chld){
			break;
		}
		if(chld->countelements()>0){
			elt=chld->getelement(1);
			elt->getclassname(val);
			for(i=1;i<=nsvdef_op_max;i++){
				if(!strcmp(val,nsvdef_op_list[i])){
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
			if(i==nsvdef_op_max+1){
				_cfld=0;
			}
			else{
				_cop=i;
			}
		}
		else{
			_cfld=0;
		}
_tm_("_cop="+_cop);
		
// StyleRuns
_tm_("StyleRuns");
// Load des styles et init du flag cmyk
		for(int i=1;i<=_styles.count();i++){
			_styles.get(i,&stl);
			stl->load(_root);
			if(_cmyk==false){
				_cmyk=(stl->_cc==5);
			}
		}
// Remise de tous les styles sur la même colorimétrie
		for(int i=1;i<=_styles.count();i++){
			_styles.get(i,&stl);
			stl->_cc=(_cmyk)?5:4;
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
bool bvDefPaletteQuick::write(){
_bTrace_("bvDefPaletteQuick::write",true);
	
bArray	arr(sizeof(xmlelt));
char	val[_values_length_max_];
char	buffer[_values_length_max_];
int		ib,i,k,d,n=_tp->fields()->count_constraints(_fld);
double	bnd;
	
	add_cdesc(arr,0,_stylename,"");
	
// Identification			
	add_cdesc(arr,1,"styleidentification","");
	add_cdesc(arr,2,"name",_name);
	sprintf(val,"%.4s",&_sign);
	add_cdesc(arr,2,"sign",val);
	add_cdesc(arr,2,"version",_vers);
	
// Globals		
	add_cdesc(arr,1,"globals","");
	_tp->fields()->get_name(_fld,val);
	add_cdesc(arr,2,"classfield",val);
	add_ddesc(arr,2,"scaleref",_sref,0);
	add_ddesc(arr,2,"unitcoef",_coef,6);
	add_idesc(arr,2,"sort",_sort);
	add_idesc(arr,2,"passcount",_pssmax);
	add_cdesc(arr,2,"classboundsarray","");
	k=_tp->fields()->get_constraints_kind(_fld);
	_tp->fields()->get_decs(_fld,&d);
	for(i=1;i<=n;i++){
		switch(k){
			case _int:
			case _bool:
				_tp->fields()->get_constraint(_fld,i,&ib);
				bnd=ib;
				sprintf(val,"%d",ib);
				break;
			case _double:
			case _date:
			case _time:
				_tp->fields()->get_constraint(_fld,i,&bnd);
				sprintf(val,"%f",bnd);
				break;
			default:
				bnd=i;
				_tp->fields()->get_constraint(_fld,i,buffer);
				xToChar(k,d,buffer,val);
				break;
		}
		add_ddesc(arr,3,"classbound",bnd,6);		
		add_cdesc(arr,3,"classname",val);
	}	
	add_ddesc(arr,3,"classbound",bnd+1,6);
	add_cdesc(arr,3,"classname","");
	
// Conditions	
	add_cdesc(arr,1,"condition","");
	if(_cfld){
		add_cdesc(arr,2,nsvdef_op_list[_cop],"");
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
bvDefPaletteQuickStyle* stl;
	for(int i=1;i<=_styles.count();i++){
		_styles.get(i,&stl);
		stl->dump(arr,0);
	}
	
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	
	free_descs(arr);
	
	return(true);
}

