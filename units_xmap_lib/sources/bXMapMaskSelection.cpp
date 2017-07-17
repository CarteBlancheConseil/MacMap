//----------------------------------------------------------------------------
// File : bXMapMaskSelection.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, masks selected objects
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
// 31/01/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapMaskSelection.h"
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapMaskSelection	::bXMapMaskSelection(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("maskselection");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapMaskSelection::~bXMapMaskSelection(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapMaskSelection::create(bGenericXMLBaseElement* elt){
	return(new bXMapMaskSelection(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMaskSelection::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->selMgr()->count()>0){
bArray				sel(*(_gapp->selMgr()->elements()));
bGenericGeoElement*	o;
bool				b=(sel.count()<30);
	
		_gapp->selMgr()->flush();	
		for(int i=sel.count();i>0;i--){
			if(sel.get(i,&o)){
				o->setmasked(true,b);
			}
		}
		if(!b){
			_gapp->mapIntf()->inval();
		}
//		return(true);
	}
//	return(false);
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMaskSelection::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapMaskSelection::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
