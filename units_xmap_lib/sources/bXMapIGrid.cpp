//----------------------------------------------------------------------------
// File : bXMapIGrid.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, import grid and projection (type dispatch)
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
// 24/11/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapIGrid.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/type_utils.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapIGrid	::bXMapIGrid(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("igrid");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapIGrid::~bXMapIGrid(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapIGrid::create(bGenericXMLBaseElement* elt){
	return(new bXMapIGrid(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
	read_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::close(){
	bStdXMapWithModalIntf::close();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapIGrid::process(int msg, void* prm){
import_grid*	p;
	switch(msg){
		case kExtProcessCallGetData:
			p=(import_grid*)prm;
			*p=_prm;
			break;
		case kExtProcessCallSetData:
			p=(import_grid*)prm;
			_prm=*p;
			put_p();
			break;
		case kExtProcessCallWithXMLTree:{
char					val[_values_length_max_];
bGenericType*			tp;
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				_prm.srid=atoi(val);
				
				elt=getelement(2);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				_prm.t_point=(tp)?tp->tid():0;

				elt=getelement(3);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				_prm.t_line=(tp)?tp->tid():0;
				
				elt=getelement(4);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				_prm.t_text=(tp)?tp->tid():0;
			
				elt=getelement(5);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				_prm.t_poly=(tp)?tp->tid():0;
				
				elt=getelement(6);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
				_prm.t_raster=(tp)?tp->tid():0;
				
				write_p();
			}break;

			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapIGrid::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				put_p();
				break;
			case kHICommandCancel:
				read_p();
				break;
			case kXMapIGridProjCmd:
				proj_action();
				break;

			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::wd_init(WindowRef wd){
ControlRef	c;
int			k;

	for(int i=kBaseKindPoint;i<=kBaseKindRaster;i++){
		c=get_control(kXMapIGridSignature,i);
		PopulatePopupControlWithType(_gapp,c,i,1);
		switch(i){
			case kBaseKindPoint:
				k=_gapp->typesMgr()->index(_prm.t_point);
				break;
			case kBaseKindPolyline:
				k=_gapp->typesMgr()->index(_prm.t_line);
				break;
			case kBaseKindText:
				k=_gapp->typesMgr()->index(_prm.t_text);
				break;
			case kBaseKindPolygon:
				k=_gapp->typesMgr()->index(_prm.t_poly);
				break;
			case kBaseKindRaster:
				k=_gapp->typesMgr()->index(_prm.t_raster);
				break;
		}
		k++;
		SetControl32BitValue(c,k);
	}

char	name[512];
    GetProjName(_prm.srid,name);
    c=get_control(kXMapIGridSignature,kXMapIGridProjNameID);
    SetTextControlValue(c,name);
    if(_gapp->document()->srid()==-1){
        DisableControl(c);
        c=get_control(kXMapIGridSignature,kXMapIGridProjID);
        DisableControl(c);
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapIGrid::proj_action(){
	if(GetAProj(&_prm.srid,true)){
char		name[512];
		GetProjName(_prm.srid,name);
ControlRef	c=get_control(kXMapIGridSignature,kXMapIGridProjNameID);
		SetTextControlValue(c,name);
		write_p();
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::put_p(){
ControlRef	c;
int			k;

	c=get_control(kXMapIGridSignature,kBaseKindPoint);
	k=GetControl32BitValue(c)-1;
	_prm.t_point=k?_gapp->typesMgr()->get(k)->tid():0;
	
	c=get_control(kXMapIGridSignature,kBaseKindPolyline);
	k=GetControl32BitValue(c)-1;
	_prm.t_line=k?_gapp->typesMgr()->get(k)->tid():0;

	c=get_control(kXMapIGridSignature,kBaseKindPolygon);
	k=GetControl32BitValue(c)-1;
	_prm.t_poly=k?_gapp->typesMgr()->get(k)->tid():0;
	
	c=get_control(kXMapIGridSignature,kBaseKindText);
	k=GetControl32BitValue(c)-1;
	_prm.t_text=k?_gapp->typesMgr()->get(k)->tid():0;
	
	c=get_control(kXMapIGridSignature,kBaseKindRaster);
	k=GetControl32BitValue(c)-1;
	_prm.t_raster=k?_gapp->typesMgr()->get(k)->tid():0;
	
	write_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::read_p(){
_bTrace_("bXMapIGrid::read_p",false);
char	name[_names_length_max_];
	getclassname(name);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,GetSignature(this),name);
char					val[_values_length_max_];
	
	_prm.t_point=0;
	_prm.t_line=0;
	_prm.t_poly=0;
	_prm.t_text=0;
	_prm.t_raster=0;
	_prm.srid=-1;

	if((!b)||(!root)){
		write_p();
		/*b=*/(void)_gapp->document()->readTree(&root,GetSignature(this),name);
		return;
	}
	if((!b)||(!root)){
_te_("((!b)||(!root)) for "+name);
		return;
	}
	elt=_gapp->classMgr()->NthElement(root,1,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_prm.t_point=matof(val);
	
	elt=_gapp->classMgr()->NthElement(root,2,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_prm.t_line=matof(val);
	
	elt=_gapp->classMgr()->NthElement(root,3,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_prm.t_poly=matof(val);
	
	elt=_gapp->classMgr()->NthElement(root,4,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_prm.t_text=matof(val);
	
	elt=_gapp->classMgr()->NthElement(root,5,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_prm.t_raster=matof(val);

	elt=_gapp->classMgr()->NthElement(root,1,"int");
	if(elt){
		elt->getvalue(val);
		_prm.srid=atoi(val);
	}
	else{
		_prm.srid=-1;
	}
		
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapIGrid::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	getclassname(name);

	if(!_gapp->document()->writeTree(root,GetSignature(this),name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapIGrid::make_p(){
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

	add_ddesc(arr,1,"float",_prm.t_point,0);
	add_ddesc(arr,1,"float",_prm.t_line,0);
	add_ddesc(arr,1,"float",_prm.t_poly,0);
	add_ddesc(arr,1,"float",_prm.t_text,0);
	add_ddesc(arr,1,"float",_prm.t_raster,0);
	add_idesc(arr,1,"int",_prm.srid);

bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}
