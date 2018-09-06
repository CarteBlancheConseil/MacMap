//----------------------------------------------------------------------------
// File : vdef_utils.h
// Project : MacMap
// Purpose : Header file : vDefs utilities
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
// 03/04/2007 creation.
//----------------------------------------------------------------------------

#ifndef __vdef_utils__
#define __vdef_utils__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapStats/MMsDiscretization.h>
#include <MacMapSuite/C_Utils.h>

//----------------------------------------------------------------------------

#define kvDefMaxPrecs	(1.0/1000000.0)

//----------------------------------------------------------------------------
// New Cocoa vDefs
void	getMin		(bGenericMacMapApp* gapp,
					 bGenericType* tp,
					 int field,
					 int nth,
					 double* max);
void	getMax		(bGenericMacMapApp* gapp,
					 bGenericType* tp,
					 int field,
					 int nth,
					 double* max);
void	getBornes	(bGenericMacMapApp* gapp,
					 bGenericType* tp,
					 int field,
					 double* min,
					 double* max);
void	getMatrix	(bGenericMacMapApp* gapp,
					 bGenericType* tp,
					 int field,
					 double min,
					 double max,
					 MMsMatrix** mx);

int		countColors	(bGenericMacMapApp* gapp);
void	getColorName(bGenericMacMapApp* gapp,
					 int index,
					 char* name);
void	getColor	(bGenericMacMapApp* gapp,
					 int index,
					 double* cmin,
					 double* cmax,
					 int cmyk);
int		getDashIndex(bGenericType* tp, 
					 char* dsh);

//----------------------------------------------------------------------------

#endif
