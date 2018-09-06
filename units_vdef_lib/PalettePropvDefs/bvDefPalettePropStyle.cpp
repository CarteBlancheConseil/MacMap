//----------------------------------------------------------------------------
// File : bvDefPalettePropStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Base subclass for proportionnal vDef styles
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

#include "bvDefPalettePropStyle.h"
#include <limits.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefPalettePropStyle	::bvDefPalettePropStyle(bGenericMacMapApp* gapp,
												bGenericType* gtp,
												bool size)
						:_arr(sizeof(color_range64)){
	_gapp=gapp;
	_gtp=gtp;
	_gstl=NULL;
	
	_fld=kOBJ_SubType_;
	_clss=1;
	
	_qref=1;
	_qmax=1;
	_rref=5;
	_exp=1;
	
	_smin=1;
	_smax=LONG_MAX;
	_pass=1;
	
	_offx=0;
	_offy=0;
	
	_color[0]=0;
	_color[1]=0;
	_color[2]=0;
	_color[3]=1;
	_color[4]=1;
	_cc=4;
	
	_sow=size;
	
	_subfld=kOBJ_SubType_;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefPalettePropStyle::~bvDefPalettePropStyle(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::load(bGenericXMLBaseElement* root){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::dump(bArray& arr, int indent){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefPalettePropStyle::reset(){
	_arr.reset();
//	_subfld=0;
	_subfld=kOBJ_SubType_;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::std_load(bGenericXMLBaseElement* root){
	_gstl=NULL;
	find_stylerun(root);
	if(!_gstl){
		return(false);
	}
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
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
bool bvDefPalettePropStyle::std_dump(bArray& arr, int indent){
	add_cdesc(arr,indent+1,"stylerun","");
	add_cdesc(arr,indent+2,"validity","");
	add_idesc(arr,indent+3,"classmin",1);
	add_idesc(arr,indent+3,"classmax",2);
	add_ddesc(arr,indent+3,"scalemin",_smin,6);
	add_ddesc(arr,indent+3,"scalemax",(_smax==LONG_MAX)?_smax:_smax+1,6);
	add_idesc(arr,indent+3,"pass",_pass);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::prop_load(bGenericXMLBaseElement* root, double* rat){
bGenericXMLBaseElement*		chld;
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	if(_sow){
		chld=_gapp->classMgr()->NthElement(_gstl,1,"propsize");
	}
	else{
		chld=_gapp->classMgr()->NthElement(_gstl,1,"propwidth");
	}
	if(!chld){
		return(true);
	}
	elt=_gapp->classMgr()->NthElement(chld,1,"field");
	if(!elt){
		_fld=kOBJ_SubType_;
	}
	else{
		elt->getvalue(val);
		_fld=_gtp->fields()->get_index(val);
		if(!_fld){
			_fld=kOBJ_SubType_;
		}
	}
	
	elt=_gapp->classMgr()->NthElement(chld,1,"value");
	if(elt){
		elt->getvalue(val);
		_qref=matof(val);
		if(_qref==0){
			_qref=1;
		}
	}
	elt=_gapp->classMgr()->NthElement(chld,2,"value");
	if(elt){
		elt->getvalue(val);
		_qmax=matof(val);
		if(_qmax==0){
			_qmax=1;
		}
	}
	elt=_gapp->classMgr()->NthElement(chld,3,"value");
	if(elt){
		elt->getvalue(val);
		_rref=matof(val);
		if(_rref==0){
			_rref=1;
		}
	}
	elt=_gapp->classMgr()->NthElement(chld,4,"value");
	if(elt){
		elt->getvalue(val);
		_exp=matof(val);
		if(_exp==0){
			_exp=1;
		}
	}
	(*rat)=1;
	elt=_gapp->classMgr()->NthElement(chld,5,"value");
	if(elt){
		elt->getvalue(val);
		(*rat)=matof(val);
		if((*rat)<=0){
			(*rat)=1;
		}
	}	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::prop_dump(bArray& arr, int indent, double rat){
_bTrace_("bvDefPalettePropStyle::prop_dump]",true);
char	nm[256];
int		d,k;
	
	_gtp->fields()->get_name(_fld,nm);
	_gtp->fields()->get_kind(_fld,&k);
	if(k==_char){
		d=6;
	}
	else{
		_gtp->fields()->get_decs(_fld,&d);
		d++;
	}
	if(_sow){
		add_cdesc(arr,indent+1,"propsize","");
	}
	else{
		add_cdesc(arr,indent+1,"propwidth","");
	}
	add_cdesc(arr,indent+2,"field",nm);
	
	add_ddesc(arr,indent+2,"value",_qref,d);
	add_ddesc(arr,indent+2,"value",_qmax,d);
	add_ddesc(arr,indent+2,"value",_rref,2);
	add_ddesc(arr,indent+2,"value",_exp,15);
	if(rat!=1){
		add_ddesc(arr,indent+2,"value",rat,2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::sub_load(bGenericXMLBaseElement* root){
bGenericXMLBaseElement*		chld;
bGenericXMLBaseElement*		sub;
bGenericXMLBaseElement*		clr;
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
int							i;
color_range64				stl;
	
	stl.name[0]=0;
	stl.clr_spc=4;
	stl.clrs[0]=0;
	stl.clrs[1]=0;
	stl.clrs[2]=0;
	stl.clrs[3]=0;
	stl.clrs[4]=1;
	stl.pat[0]=0;	
	
	sub=_gapp->classMgr()->NthElement(_gstl,1,"substyle");
	if(!sub){
		return(false);
	}
	elt=_gapp->classMgr()->NthElement(sub,1,"subclassfield");
	if(!elt){
		_subfld=kOBJ_SubType_;
	}
	else{
		elt->getvalue(val);
		_subfld=_gtp->fields()->get_index(val);
		if(!_subfld){
			_subfld=kOBJ_SubType_;
		}	
	}
	
	chld=_gapp->classMgr()->NthElement(sub,1,"subclassboundsarray");
	if(!chld){
		return(false);
	}
	i=0;
	do{	
		i++;
		elt=_gapp->classMgr()->NthElement(chld,i,"subclassbound");
		if(elt){
			elt->getvalue(val);
			stl.bound=matof(val);
			_arr.add(&stl);
		}
	}while(elt);
	i=0;
	do{	
		i++;
		chld=_gapp->classMgr()->NthElement(sub,i,"substylerun");
		if(chld){
			if(!_arr.get(i,&stl)){
				break;
			}
			if(_sow){
				clr=_gapp->classMgr()->NthElement(chld,1,"fill");
				if(clr){
					elt=_gapp->classMgr()->NthElement(clr,1,"red");
					if(elt){
						elt->getvalue(val);
						stl.clrs[0]=matof(val);
						_cc=stl.clr_spc=4;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"green");
					if(elt){
						elt->getvalue(val);
						stl.clrs[1]=matof(val);
						_cc=stl.clr_spc=4;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"blue");
					if(elt){
						elt->getvalue(val);
						stl.clrs[2]=matof(val);
						_cc=stl.clr_spc=4;
					}
					
					elt=_gapp->classMgr()->NthElement(clr,1,"cyan");
					if(elt){
						elt->getvalue(val);
						stl.clrs[0]=matof(val);
						_cc=stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"magenta");
					if(elt){
						elt->getvalue(val);
						stl.clrs[1]=matof(val);
						_cc=stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"yellow");
					if(elt){
						elt->getvalue(val);
						stl.clrs[2]=matof(val);
						_cc=stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"black");
					if(elt){
						elt->getvalue(val);
						stl.clrs[3]=matof(val);
						_cc=stl.clr_spc=5;
					}
					
					elt=_gapp->classMgr()->NthElement(clr,1,"alpha");
					if(elt){
						if(_cc==4){
							elt->getvalue(val);
							stl.clrs[3]=matof(val);
						}
						else{
							elt->getvalue(val);
							stl.clrs[4]=matof(val);						
						}
					}
				}
				else{
					clr=_gapp->classMgr()->NthElement(chld,1,"fillpattern");
					if(clr){
						clr->getvalue(stl.pat);
					}
				}
			}
			else{
				clr=_gapp->classMgr()->NthElement(chld,1,"stroke");
				if(clr){
					elt=_gapp->classMgr()->NthElement(clr,1,"red");
					if(elt){
						elt->getvalue(val);
						stl.clrs[0]=matof(val);
						_cc=stl.clr_spc=4;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"green");
					if(elt){
						elt->getvalue(val);
						stl.clrs[1]=matof(val);
						_cc=stl.clr_spc=4;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"blue");
					if(elt){
						elt->getvalue(val);
						stl.clrs[2]=matof(val);
						_cc=stl.clr_spc=4;
					}
					
					elt=_gapp->classMgr()->NthElement(clr,1,"cyan");
					if(elt){
						elt->getvalue(val);
						stl.clrs[0]=matof(val);
						_cc=stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"magenta");
					if(elt){
						elt->getvalue(val);
						stl.clrs[1]=matof(val);
						stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"yellow");
					if(elt){
						elt->getvalue(val);
						stl.clrs[2]=matof(val);
						_cc=stl.clr_spc=5;
					}
					elt=_gapp->classMgr()->NthElement(clr,1,"black");
					if(elt){
						elt->getvalue(val);
						stl.clrs[3]=matof(val);
						_cc=stl.clr_spc=5;
					}
					
					elt=_gapp->classMgr()->NthElement(clr,1,"alpha");
					if(elt){
						if(stl.clr_spc==4){
							elt->getvalue(val);
							stl.clrs[3]=matof(val);
						}
						else{
							elt->getvalue(val);
							stl.clrs[4]=matof(val);						
						}
					}
				}
				else{
					clr=_gapp->classMgr()->NthElement(chld,1,"strokepattern");
					if(clr){
						clr->getvalue(stl.pat);
					}
				}
			}
			_arr.put(i,&stl);
		}
	}while(chld);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::sub_dump(bArray& arr, int indent){
	_bTrace_("bvDefPalettePropStyle::sub_dump",true);
char			val[256];
color_range64	stl;
	
	add_cdesc(arr,indent+1,"substyle","");
	add_cdesc(arr,indent+2,"subglobals","");
	_gtp->fields()->get_name(_subfld,val);
	add_cdesc(arr,indent+3,"subclassfield",val);
	add_cdesc(arr,indent+3,"subclassboundsarray",val);
	for(int i=1;i<=_arr.count();i++){
		_arr.get(i,&stl);
		add_ddesc(arr,indent+4,"subclassbound",stl.bound,10);
		_tm_(i+"->"+stl.bound);
	}
	for(int i=1;i<_arr.count();i++){
		_arr.get(i,&stl);
		add_cdesc(arr,indent+2,"substylerun","");
		if(strlen(stl.pat)==0){
			if(_sow){
				add_cdesc(arr,indent+3,"fill","");
			}
			else{
				add_cdesc(arr,indent+3,"stroke","");
			}
			if(_cc==4){
				add_ddesc(arr,indent+4,"red",stl.clrs[0],2);
				add_ddesc(arr,indent+4,"green",stl.clrs[1],2);
				add_ddesc(arr,indent+4,"blue",stl.clrs[2],2);
				add_ddesc(arr,indent+4,"alpha",stl.clrs[3],2);
			}
			else{
				add_ddesc(arr,indent+4,"cyan",stl.clrs[0],2);
				add_ddesc(arr,indent+4,"magenta",stl.clrs[1],2);
				add_ddesc(arr,indent+4,"yellow",stl.clrs[2],2);
				add_ddesc(arr,indent+4,"black",stl.clrs[3],2);			
				add_ddesc(arr,indent+4,"alpha",stl.clrs[4],2);			
			}
		}
		else{
			if(_sow){
				add_cdesc(arr,indent+3,"fillpattern",stl.pat);
			}
			else{
				add_cdesc(arr,indent+3,"strokepattern",stl.pat);
			}
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefPalettePropStyle::off_load(bGenericXMLBaseElement* root){
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
bool bvDefPalettePropStyle::off_dump(bArray& arr, int indent){
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
void bvDefPalettePropStyle::find_stylerun(bGenericXMLBaseElement* root){
	(void)root->dotoall(this,0,test_stylerun);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefPalettePropStyle::test_stylerun(bGenericXMLBaseElement* root, void* prm, int indent){
bvDefPalettePropStyle*		stl=(bvDefPalettePropStyle*)prm;	
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
