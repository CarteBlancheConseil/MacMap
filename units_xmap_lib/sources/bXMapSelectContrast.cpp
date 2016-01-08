//----------------------------------------------------------------------------
// File : bXMapSelectContrast.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, replace selection with contrasted objects
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

#include "bXMapSelectContrast.h"
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSelectContrast	::bXMapSelectContrast(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("selectcontrast");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSelectContrast::~bXMapSelectContrast(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSelectContrast::create(bGenericXMLBaseElement* elt){
	return(new bXMapSelectContrast(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectContrast::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->cntMgr()->count()>0){
		FlushSelection(_gapp);
		_gapp->selMgr()->add(_gapp->cntMgr()->elements());
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectContrast::test(void* prm){
	return(_gapp->cntMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectContrast::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
