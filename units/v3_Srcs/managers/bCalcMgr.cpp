//----------------------------------------------------------------------------
// File : bCalcMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Calcs management class
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
// 29/11/2005 creation.
//----------------------------------------------------------------------------

#include "bCalcMgr.h"
#include "bMacMapApp.h"
//#include "bStdExtLib.h"
#include <mox_intf/endian.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCalcMgr	::bCalcMgr()
			:bStdUserExtMgr(kXMLSubClassExtCalc,false,false,false,0,kUserCalcName){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bCalcMgr::~bCalcMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCalcMgr::add(bGenericXMLBaseElement* root){
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
bool bCalcMgr::modify(int idx, bGenericXMLBaseElement* root){
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
