//----------------------------------------------------------------------------
// File : bVarMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Constants/Variables management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#include "bVarMgr.h"
#include "bMacMapApp.h"
#include <mox_intf/endian.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVarMgr	::bVarMgr()
		:bStdUserExtMgr(kXMLSubClassExtVar,false,false,false,0,kUserVarName){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bVarMgr::~bVarMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVarMgr::add(bGenericXMLBaseElement* root){
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,2,"sign");
	if(!elt){
		return(false);
	}
#ifdef __LITTLE_ENDIAN__
char	tmp[16];
	elt->getvalue(tmp);
	LBSwapWord(tmp,sizeof(UInt32));
	elt->setvalue(tmp);
#endif
	return(bStdUserExtMgr::add(root));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bVarMgr::modify(int idx, bGenericXMLBaseElement* root){
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,2,"sign");
	if(!elt){
		return(false);
	}
#ifdef __LITTLE_ENDIAN__
char	tmp[16];
	elt->getvalue(tmp);
	LBSwapWord(tmp,sizeof(UInt32));
	elt->setvalue(tmp);
#endif
	return(bStdUserExtMgr::modify(idx,root));
}
