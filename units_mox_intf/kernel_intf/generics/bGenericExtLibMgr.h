//----------------------------------------------------------------------------
// File : bGenericExtLibMgr.h
// Project : MacMap
// Purpose : Header file : Root class for extern plugins manager
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
// 09/11/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericExtLibMgr__
#define __bGenericExtLibMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericCompLib.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#define kComponentGeoIterator	"GIterator"
#define kComponentGraphics		"Graphics"
#define kComponentStatusWd		"StatusWd"
#define kComponentMapWd			"MapWd"

//----------------------------------------------------------------------------

class bGenericExtLibMgr : public bGenericMgr{
	public:
		virtual ~bGenericExtLibMgr				(	){};
		virtual int getlibs						(	bArray& arr,
													UInt32 family)=0;
		virtual bGenericCompLib* get_component	(	const char* name)=0;
};

//----------------------------------------------------------------------------

#endif
