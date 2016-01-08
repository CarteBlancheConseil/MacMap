//----------------------------------------------------------------------------
// File : bXMapShowAll.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, unmask all masked objects
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
// 01/02/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapShowAll.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapShowAll	::bXMapShowAll(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("showall");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapShowAll::~bXMapShowAll(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapShowAll::create(bGenericXMLBaseElement* elt){
	return(new bXMapShowAll(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowAll::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		_gapp->typesMgr()->get(i)->iterator()->iterate(NULL,process_obj);
	}
	_gapp->mapIntf()->inval();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowAll::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapShowAll::process_obj(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		geo->setmasked(false,false);
	}
	return(0);
}
