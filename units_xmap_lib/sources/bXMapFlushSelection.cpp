//----------------------------------------------------------------------------
// File : bXMapFlushSelection.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, empty selection list
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
// 30/06/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapFlushSelection.h"
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapFlushSelection	::bXMapFlushSelection(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
	setclassname("flushselection");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapFlushSelection::~bXMapFlushSelection(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapFlushSelection::create(bGenericXMLBaseElement* elt){
	return(new bXMapFlushSelection(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushSelection::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->selMgr()->count()>0){
		FlushSelection(_gapp);
//		return(true);
	}
//	return(false);
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushSelection::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFlushSelection::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
