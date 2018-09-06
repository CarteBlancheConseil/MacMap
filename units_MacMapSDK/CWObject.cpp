//----------------------------------------------------------------------------
// File : CWObject.cpp
// Project : MacMap
// Purpose : C++ source file : bGenericGeoElement C wrappers
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
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWObject.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWObject CWObject_Alloc(CWCallback Callback, int type){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bGenericType*		tp=gapp->typesMgr()->get(type);
	if(tp==NULL){
		return(NULL);
	}
bGenericGeoElement*	geo;
	if(!tp->new_object(&geo)){
		return(NULL);
	}
	return((CWObject)geo);
}
								
// ---------------------------------------------------------------------------
// 
// ------------
CWObject CWObject_Dup(CWCallback Callback, CWObject obj){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
bGenericType*		tp=gapp->typesMgr()->get(geo->getType());
	if(tp==NULL){
		return(NULL);
	}
bGenericGeoElement*	res;
	if(!tp->clone_object(geo,&res)){
		return(NULL);
	}
	return((CWObject)res);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_Delete(CWCallback Callback, CWObject obj){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
bGenericType*		tp=gapp->typesMgr()->get(geo->getType());
	if(tp==NULL){
		return(NULL);
	}
	return(tp->kill_object(geo));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_Recover(CWCallback Callback, CWObject obj){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
bGenericType*		tp=gapp->typesMgr()->get(geo->getType());
	if(tp==NULL){
		return(NULL);
	}
	return(tp->unkill_object(geo));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetType(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getType());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetID(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getID());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetSubtype(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getSubType());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetSubtype(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setSubType(data));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWObject_GetName(CWObject obj, CWCharData data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	geo->getName(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetName(CWObject obj, CWCharData data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setName(data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetDirection(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getDirection());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetDirection(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setDirection(data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetColor(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getColor());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetColor(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setColor(data));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWObject_GetGeometry(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
ivertices*			vxs=NULL;
	geo->getVertices(&vxs);
	return((CWiGeometry)vxs);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetGeometry(CWObject obj, CWiGeometry data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setVertices((ivertices*)data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_GetData(CWObject obj, int field, CWCharData data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->getValue(field,(char*)data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_SetData(CWObject obj, int field, CWCharData data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->setValue(field,(char*)data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_IsSelected(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->selected());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWObject_SetSelected(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	geo->setselect(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_IsContrasted(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->contrasted());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWObject_SetContrasted(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	geo->setcontrast(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWObject_IsMasked(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->masked());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWObject_SetMasked(CWObject obj, int data){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	geo->setmasked(data,true);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWObject CWObject_GetReference(CWObject obj){
bGenericGeoElement*	geo=(bGenericGeoElement*)obj;
	return(geo->get_ref());
}

