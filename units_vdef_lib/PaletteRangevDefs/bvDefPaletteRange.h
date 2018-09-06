//----------------------------------------------------------------------------
// File : bvDefPaletteRange.h
// Project : MacMap
// Purpose : Header file : Base subclass for range vDefs
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
// 20/08/2011 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPaletteRange__
#define __bvDefPaletteRange__

//----------------------------------------------------------------------------

#include <std_ext/bStdNSPalettevDef.h>
#include <Carbon/Carbon.h>

//----------------------------------------------------------------------------

class bvDefPaletteRange : public bStdNSPalettevDef{
public:		
	bvDefPaletteRange						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bvDefPaletteRange				(	);
	
	virtual void fld_put					(	int fld){_fld=fld;};

	virtual bArray&	limits					(	){return(_limits);};

	virtual void rebuild_limits				(	);

	virtual void stl_alloc					(	int idx);

protected:
	
	virtual void make						(	);
	virtual bool read						(	);
	virtual bool write						(	);
	
	bArray		_limits;
	
private:
	
	
};

//----------------------------------------------------------------------------

#endif
