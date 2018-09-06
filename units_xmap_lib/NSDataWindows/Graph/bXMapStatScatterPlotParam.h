//----------------------------------------------------------------------------
// File : bXMapStatScatterPlotParam.h
// Project : MacMap
// Purpose : Header file : Params class for scatter plot graphs
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
// 22/04/2008 creation.
//----------------------------------------------------------------------------

/*#ifndef __bXMapStatScatterPlotParam__
#define __bXMapStatScatterPlotParam__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

typedef struct StatObject{
	int					clss;
	bGenericGeoElement*	geo;
}StatObject;

typedef struct itrun{
	int		k;
	
	float	fr;
	float	fg;
	float	fb;
	float	fa;
	
	float	sr;
	float	sg;
	float	sb;
	float	sa;
	
	float	h;
	float	v;
	float	s;
}itrun;

//----------------------------------------------------------------------------

class bXMapStatScatterPlotParam{
public:		
	bXMapStatScatterPlotParam				(	bArray& elts,
												int grp,
												bool mapupdt);
	virtual ~bXMapStatScatterPlotParam		(	);
	
	virtual void SetGroupField				(	int f);
	virtual int GetGroupField				(	){return _grp;};

	virtual void SaveGroups					(	bGenericMacMapApp* gapp,
												const char* path,
												const char* name);
	virtual void RecoverGroups				(	bGenericMacMapApp* gapp,
												const char* path,
												const char* name);
	virtual void SizeGroups					(	long n);
	
	bArray			_objects;
	bArray			_colors;
	itrun			_vv;
	int				_grp;
	
protected:
	
private:
	static bool put_grp						(	bGenericXMLBaseElement *elt, 
												void *prm, 
												int indent);

};

//----------------------------------------------------------------------------

#endif*/
