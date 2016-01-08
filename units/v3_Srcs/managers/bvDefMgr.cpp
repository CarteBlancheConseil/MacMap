//----------------------------------------------------------------------------
// File : bvDefMgr.cpp
// Project : MacMap
// Purpose : C++ source file : vDefs management class
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#include "bvDefMgr.h"
#include <mox_intf/bGenericvDef.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefMgr::bvDefMgr()
		:bStdExtMgr(kXMLSubClassExtvDef,false,false,false,0){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefMgr::~bvDefMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefMgr::edit(bGenericExt* ext, void* prm){
_bTrace_("bvDefMgr::edit",true);
vdef_edit*	p=(vdef_edit*)prm;
	return(((bGenericvDef*)(void*)ext)->edit(&p->root,p->tp));
}
