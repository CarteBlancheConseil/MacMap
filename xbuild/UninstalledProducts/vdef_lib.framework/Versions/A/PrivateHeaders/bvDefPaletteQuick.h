//----------------------------------------------------------------------------
// File : bvDefPaletteQuick.h
// Project : MacMap
// Purpose : Header file : Base subclass for quick (standard) vDefs
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPaletteQuick__
#define __bvDefPaletteQuick__

//----------------------------------------------------------------------------

#include <std_ext/bStdNSPalettevDef.h>
#include <Carbon/Carbon.h>

//----------------------------------------------------------------------------

class bvDefPaletteQuick : public bStdNSPalettevDef{
public:		
	bvDefPaletteQuick						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bvDefPaletteQuick				(	);
		
protected:
	
	virtual void make						(	);
	virtual bool read						(	);
	virtual bool write						(	);
		
private:
	
	
};

//----------------------------------------------------------------------------

#endif
