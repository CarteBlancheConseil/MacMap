//----------------------------------------------------------------------------
// File : bXMapShowSelectOnly.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, mask all unselected objects
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

#include "bXMapShowSelectOnly.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapShowSelectOnly	::bXMapShowSelectOnly(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("showselectonly");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapShowSelectOnly::~bXMapShowSelectOnly(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapShowSelectOnly::create(bGenericXMLBaseElement* elt){
	return(new bXMapShowSelectOnly(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowSelectOnly::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->selMgr()->count()>0){
		for(int i=1;i<=_gapp->typesMgr()->count();i++){
			_gapp->typesMgr()->get(i)->iterator()->iterate(NULL,process_obj);
		}
		_gapp->mapIntf()->inval();
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowSelectOnly::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowSelectOnly::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapShowSelectOnly::process_obj(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(!geo->masked()&&!geo->selected()){
		geo->setmasked(true,false);
	}
	return(0);
}
