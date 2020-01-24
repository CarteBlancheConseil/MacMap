//----------------------------------------------------------------------------
// File : bvDefPaletteProp.h
// Project : MacMap
// Purpose : Header file : Base subclass for proportionnal vDefs
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
// 02/01/2013 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPaletteProp__
#define __bvDefPaletteProp__

//----------------------------------------------------------------------------

#include <std_ext/bStdNSPalettevDef.h>
#include <Carbon/Carbon.h>

//----------------------------------------------------------------------------

enum{
	kvDefPropOutOfRangeEmpty			=1,
	kvDefPropOutOfRangeInvisible		=2
};

//----------------------------------------------------------------------------

class bvDefPaletteProp : public bStdNSPalettevDef{
public:		
	bvDefPaletteProp						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bvDefPaletteProp				(	);
	
	virtual void fld_put					(	int fld){_fld=fld;};

	virtual void stl_alloc					(	);

	virtual int oor_get						(	){return(_oor);};
	virtual void oor_put					(	int oor){_oor=oor;};
	virtual int subfld_get					(	){return(_subfld);};
	virtual void subfld_put					(	int subfld){_subfld=subfld;};
	virtual double cmin_get					(	){return(_cmin);};
	virtual void cmin_put					(	double cmin){_cmin=cmin;};
	virtual double cmax_get					(	){return(_cmax);};
	virtual void cmax_put					(	double cmax){_cmax=cmax;};

protected:

	virtual void make						(	);
	virtual bool read						(	);
	virtual bool write						(	);
	
private:
	int		_oor;
	int		_subfld;
	double	_cmin;
	double	_cmax;

	
};

//----------------------------------------------------------------------------

#endif
