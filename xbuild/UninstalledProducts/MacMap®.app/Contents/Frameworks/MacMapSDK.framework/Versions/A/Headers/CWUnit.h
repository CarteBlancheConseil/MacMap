//----------------------------------------------------------------------------
// File : CWUnit.h
// Project : MacMap
// Purpose : Header file : bGenericUnit C wrappers
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
// Units Manipulation : Distances, Scales
//----------------------------------------------------------------------------
// 20/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWUnit__
#define __CWUnit__

//----------------------------------------------------------------------------

#include "CWDefs.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Getting Unit's name
void		CWUnit_GetName						(	CWUnit unit,		// -> Reference unit
													CWCharData data);	// <- Name

// Getting Unit's short name
void		CWUnit_GetShortName					(	CWUnit unit,		// -> Reference unit
													CWCharData data);	// <- Short Name

// Getting Unit's value
double		CWUnit_GetCoef						(	CWUnit unit);		// -> Reference unit

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
