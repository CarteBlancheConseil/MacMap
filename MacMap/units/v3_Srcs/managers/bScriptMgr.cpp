//----------------------------------------------------------------------------
// File : bScriptMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Scripts management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 02/12/2005 creation.
//----------------------------------------------------------------------------

#include "bScriptMgr.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bScriptMgr	::bScriptMgr()
			:bStdUserExtMgr(kXMLSubClassExtScript,false,false,false,0,kUserScriptName){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bScriptMgr::~bScriptMgr(){
}

