//----------------------------------------------------------------------------
// File : bContrastesMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Class for management of contrasted object list 
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
// 22/01/2004 creation.
//----------------------------------------------------------------------------

#include "bContrastesMgr.h"
#include "bLayersMgr.h"
#include "bMacMapGeoElement.h"
#include "bMacMapApp.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bContrastesMgr::bContrastesMgr():bObjListMgr(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bContrastesMgr::~bContrastesMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bContrastesMgr::unload(){
//	layersmgr->DrawContrastes(NULL);
	bObjListMgr::flush();
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bContrastesMgr::add(bGenericGeoElement* o){
	if(bObjListMgr::add(o)!=noErr){
		return(-1);
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bContrastesMgr::add(bArray* l){
bMacMapGeoElement*	o;
int					i;
OSStatus			status=noErr;
bArray				*a,*b;
	
//	layersmgr->DrawContrastes(NULL);
	
	a=(bArray*)l;
	b=new bArray(*a);
	for(i=b->count();i>0;i--){
		if(b->get(i,&o)){
			if(o->contrasted()){
				if(!b->rmv(i)){
					status=-1;
					break;
				}
			}
			else{
				o->setcontrast(true);
			}
		}
		else{
			status=-2;
			break;
		}
	}
	if(status){
		delete b;
		return(status);
	}
	
	status=bObjListMgr::add(b);
	
//	layersmgr->DrawContrastes(NULL);

	delete b;

	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bContrastesMgr::rmv(bGenericGeoElement* o){
	if(bObjListMgr::rmv(o)!=noErr){
		return(-1);
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bContrastesMgr::flush(){
int					i,n=count();
bMacMapGeoElement*	o;
	
//	layersmgr->DrawContrastes(NULL);
	for(i=n;i>0;i--){
		if(_elts->get(i,&o)){
			o->setcontrast(false);
		}
	}
	bObjListMgr::flush();
}
