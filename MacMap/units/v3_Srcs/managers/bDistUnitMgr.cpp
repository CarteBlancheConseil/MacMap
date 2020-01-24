//----------------------------------------------------------------------------
// File : bDistUnitMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Distance units manager class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004-2018 Carte Blanche Conseil.
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
// 07/12/2004 creation.
// 07/11/2018 64bits
//----------------------------------------------------------------------------

#include "mm_config.h"
#include "bMacMapApp.h"
#include "bDistUnitMgr.h"
#include "bClassesMgr.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define _fname	"dist_units"
#define _fpath	"dist_units.xml"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDistUnitMgr	::bDistUnitMgr()
				:bStdUnitMgr(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDistUnitMgr::~bDistUnitMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bDistUnitMgr::load(){
_bTrace_("bDistUnitMgr::load",_DEVELOPMENT_VERS_);
	return(pth_load(_fpath));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bDistUnitMgr::unload(){
_bTrace_("bDistUnitMgr::unload",_DEVELOPMENT_VERS_);
	return(pth_unload(_fpath,true));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bDistUnitMgr::app_load(){
_bTrace_("bDistUnitMgr::app_load()",_DEVELOPMENT_VERS_);
	return(apppth_load(_fname));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDistUnitMgr::make(){
	return(bStdUnitMgr::make(_fpath,_fname));
}

