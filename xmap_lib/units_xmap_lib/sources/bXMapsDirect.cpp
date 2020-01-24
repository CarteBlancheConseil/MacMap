//----------------------------------------------------------------------------
// File : bXMapsDirect.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classes, sets object's direction flag (-1, 0, 1)
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
// 06/04/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapsDirect.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_association.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDirect::bXMapDirect(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	_dir=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDirect::~bXMapDirect(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDirect::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
bArray				arr(*(_gapp->selMgr()->elements()));
bGenericGeoElement* o;
bEventLog			log(_gapp,
						getbundle(),
						kXMapsDirMessageID,
						GetSignature(this));

	for(int i=1;i<=arr.count();i++){
		arr.get(i,&o);
		o->setDirection(_dir);
		o->inval(kOBJ_Vertices_);
	}
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDirect::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDirect::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDir::bXMapDir(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		:bXMapDirect(elt,gapp,bndl){
	setclassname("dirdir");
	_dir=1;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDir::~bXMapDir(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDir::create(bGenericXMLBaseElement* elt){
	return(new bXMapDir(elt,_gapp,elt->getbundle()));
}



// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapInv::bXMapInv(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		:bXMapDirect(elt,gapp,bndl){
	setclassname("dirinv");
	_dir=-1;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapInv::~bXMapInv(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapInv::create(bGenericXMLBaseElement* elt){
	return(new bXMapInv(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapInv::test(void* prm){
	if(_gapp->selMgr()->count()==0){
		return(false);
	}
bGenericType*	tp;
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->nb_sel()>0){
			if(	(tp->kind()==kBaseKindPoint)||
				(tp->kind()==kBaseKindText)	||
				(tp->kind()==kBaseNoKind)	){
				return(false);
			}
		}
	}
	return(true);
}



// ---------------------------------------------------------------------------
// Constructeur
// -----------
bXMapNoDir::bXMapNoDir(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		:bXMapDirect(elt,gapp,bndl){
	setclassname("dirno");
	_dir=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapNoDir::~bXMapNoDir(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapNoDir::create(bGenericXMLBaseElement* elt){
	return(new bXMapNoDir(elt,_gapp,elt->getbundle()));
}


