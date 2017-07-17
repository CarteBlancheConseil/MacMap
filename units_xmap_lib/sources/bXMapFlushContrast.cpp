//----------------------------------------------------------------------------
// File : bXMapFlushContrast.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, empty contrasts list
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

#include "bXMapFlushContrast.h"
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapFlushContrast	::bXMapFlushContrast(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("flushcontrast");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapFlushContrast::~bXMapFlushContrast(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapFlushContrast::create(bGenericXMLBaseElement* elt){
	return(new bXMapFlushContrast(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushContrast::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->cntMgr()->count()>0){
		FlushContrasts(_gapp);
//		return(true);
	}
//	return(false);
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushContrast::test(void* prm){
	return(_gapp->cntMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushContrast::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
