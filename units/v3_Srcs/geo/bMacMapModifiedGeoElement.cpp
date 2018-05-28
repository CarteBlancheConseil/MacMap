//----------------------------------------------------------------------------
// File : bMacMapModifiedGeoElement.cpp
// Project : MacMap
// Purpose : C++ source file : (events and undo/redo maangement)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005Carte Blanche Conseil.
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
// 17/08/2005 creation.
//----------------------------------------------------------------------------

#include "bMacMapModifiedGeoElement.h"
#include "bMacMapApp.h"

//size_t   _AllocCount=0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapModifiedGeoElement	::bMacMapModifiedGeoElement(bGenericGeoElement* ref,
														int field)
							:bStdGeoElement(){
//_bTrace_("bMacMapModifiedGeoElement::bMacMapModifiedGeoElement",false);_tm_("alloc "+field);
	_ref=ref;
	_buffer=NULL;
	_buffersz=0;
	_field=field;
	_fieldsgn=0;
 	replicate();
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapModifiedGeoElement::~bMacMapModifiedGeoElement(){
//_bTrace_("bMacMapModifiedGeoElement::~bMacMapModifiedGeoElement()",false);_tm_("delete "+_field);
	if(_buffer){
		switch(_fieldsgn){
			case _ivxs2:
            case _ivxs3:{
//_tm_("free vxs");
				ivs_free(*((ivertices**)_buffer));
                }break;
		}
//_tm_("free _buffer");
		free(_buffer);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapModifiedGeoElement::getType(){
	return(_ref->getType());
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapModifiedGeoElement::getBounds(ivx_rect* vxr){
	if(_field!=kOBJ_Vertices_){
		vxr->top=0;
		vxr->left=0;
		vxr->right=0;
		vxr->bottom=0;
	}
	else{
		ivs_bounds((*(ivertices**)_buffer),vxr);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapModifiedGeoElement::getVertices(ivertices** vxs){
	if(_field!=kOBJ_Vertices_){
		*vxs=NULL;
		return;
	}
	*vxs=(*(ivertices**)_buffer);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapModifiedGeoElement::getSubType(){
	if(_field!=kOBJ_SubType_){
		return(-1);
	}
	return(*((int*)_buffer));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapModifiedGeoElement::getColor(){
	if(_field!=kOBJ_Color_){
		return(-1);
	}
	return(*((int*)_buffer));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapModifiedGeoElement::getDirection(){
	if(_field!=kOBJ_Dir_){
		return(-2);
	}
	return(*((int*)_buffer));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapModifiedGeoElement::getName(char *name){
	if(_field!=kOBJ_Name_){
		name[0]=-1;
		return;
	}
	strcpy(name,(char*)_buffer);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapModifiedGeoElement::getValue(int fieldIdx, void *value){
	if(fieldIdx<0){
		(*((int*)value))=_field;
		return(true);
	}
	if(fieldIdx!=_field){
		return(false);
	}
	if(fieldIdx>kOBJ_Dir_){
		memcpy(value,_buffer,_buffersz);
	}
	else{
		switch(fieldIdx){
			case kOBJ_Name_:	
				getName((char*)value);
				break;
			case kOBJ_SubType_:
				(*((int*)value))=getSubType();
				break;
			case kOBJ_Color_:
				(*((int*)value))=getColor();
				break;
			case kOBJ_Dir_:
				(*((int*)value))=getDirection();
				break;
			default:
				return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bMacMapModifiedGeoElement::getValue(char *fieldName, void *value){
int	idx=typesmgr->get(getType())->fields()->get_index(fieldName);
	if(idx<=0){
		return(false);
	}
	return(getValue(idx,value));
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bMacMapModifiedGeoElement::get_ref(){
	return(_ref);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapModifiedGeoElement::replicate(){
//_bTrace_("bMacMapModifiedGeoElement::replicate",true);
bGenericType*	tp=typesmgr->get(_ref->getType());
int				status;

	tp->fields()->get_size(_field,&_buffersz);
	tp->fields()->get_kind(_field,&_fieldsgn);
    
    switch(_fieldsgn){
        case _ivxs2:
        case _ivxs3:
            _buffersz=sizeof(ivertices*);
            break;
        default:
            break;
    }
    
	_buffer=malloc(_buffersz);
//_tm_("buffer size ="+_buffersz);
	if((status=tp->fields()->read(_ref->getOffset(),_field,_buffer))){
//_te_("tp->fields()->read failed for "+_field+" with status "+status);
	}
}
