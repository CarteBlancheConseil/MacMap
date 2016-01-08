//----------------------------------------------------------------------------
// File : loader.cpp
// Project : MacMap
// Purpose : C++ source file : Loader library interface
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
// 17/09/2015 creation.
//----------------------------------------------------------------------------

#include "loader.h"
#include "bSplashWd.h"
#include "bExtLibMgr.h"

// ---------------------------------------------------------------------------
//
// ------------
void Loader_SplashAlloc(bGenericMacMapApp* gapp){
    bSplashWd::instance(true,gapp);
}

// ---------------------------------------------------------------------------
//
// ------------
void Loader_SplashRelease(){
    bSplashWd::release_instance();
}

// ---------------------------------------------------------------------------
//
// ------------
bGenericExtLibMgr* Loader_ExtLibMgrAlloc(bGenericMacMapApp* gapp){
    return new bExtLibMgr(gapp);
}

