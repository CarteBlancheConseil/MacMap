//----------------------------------------------------------------------------
// File : CWObject.h
// Project : MacMap
// Purpose : Header file : bGenericGeoElement C wrappers
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
// Object manipluation
//----------------------------------------------------------------------------
// 15/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWObject__
#define __CWObject__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWGeometry.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// CWObject allocation
// Returns NULL if failed
CWObject	CWObject_Alloc			(	CWCallback Callback,	// -> MacMap CallBack Address
										int type);				// -> Source type index
								
// CWObject duplication								
// Returns NULL if failed
CWObject	CWObject_Dup			(	CWCallback Callback,	// -> MacMap CallBack Address
										CWObject obj);			// -> Source object

// CWObject delete						
// Returns 1 if success, 0 otherwise						
int			CWObject_Delete			(	CWCallback Callback,	// -> MacMap CallBack Address
										CWObject obj);			// -> Target object

// CWObject recover	: undelete an object				
// Returns 1 if success, 0 otherwise						
int			CWObject_Recover		(	CWCallback Callback,	// -> MacMap CallBack Address
										CWObject obj);			// -> Deleted object

// Accessing Type								
int			CWObject_GetType		(	CWObject obj);			// -> Target object

// Accessing ID								
int			CWObject_GetID			(	CWObject obj);			// -> Target object

// Accessing Subtype								
int			CWObject_GetSubtype		(	CWObject obj);			// -> Target object

// Setting Subtype
// Returns 1 if success, 0 otherwise						
int			CWObject_SetSubtype		(	CWObject obj,			// -> Target object
										int data);				// -> SubType number [1..n]

// Accessing Name								
void		CWObject_GetName		(	CWObject obj,			// -> Target object
										CWCharData data);		// <- Name

// Setting Name
// Returns 1 if success, 0 otherwise						
int			CWObject_SetName		(	CWObject obj,			// -> Target object
										CWCharData data);		// -> Name

// Accessing Direction								
int			CWObject_GetDirection	(	CWObject obj);			// -> Target object

// Setting Direction								
// Returns 1 if success, 0 otherwise						
int			CWObject_SetDirection	(	CWObject obj,			// -> Target object
										int data);				// -> Direction [1, 0, -1] or [1, 0]

// Accessing Color								
int			CWObject_GetColor		(	CWObject obj);			// -> Target object

// Setting Color								
// Returns 1 if success, 0 otherwise						
int			CWObject_SetColor		(	CWObject obj,			// -> Target object
										int data);				// -> Color [1..8]

// Accessing Geometry
// Returns the object geometry, so never free this geometry							
CWiGeometry	CWObject_GetGeometry	(	CWObject obj);			// -> Target object

// Setting Geometry								
// Returns 1 if success, 0 otherwise
// data is duplicated, so you can free it after				
int			CWObject_SetGeometry	(	CWObject obj,			// -> Target object
										CWiGeometry data);		// -> Geometry

// Accessing Data								
// Returns 1 if success, 0 otherwise
int			CWObject_GetData		(	CWObject obj,			// -> Target object
										int field,				// -> Field index, can be std field index
										CWCharData data);		// <- Data

// Setting Data								
// Returns 1 if success, 0 otherwise						
int			CWObject_SetData		(	CWObject obj,			// -> Target object
										int field,				// -> Field index, can be std field index
										CWCharData data);		// -> Data

// Accessing Selection status								
// Returns 1 if Selected, 0 otherwise						
int			CWObject_IsSelected		(	CWObject obj);			// -> Target object

// Setting Selection status								
void		CWObject_SetSelected	(	CWObject obj,			// -> Target object
										int data);				// -> Select or not [0, 1]

// Accessing Contrast status								
// Returns 1 if Contrasted, 0 otherwise						
int			CWObject_IsContrasted	(	CWObject obj);			// -> Target object

// Setting Selection status								
void		CWObject_SetContrasted	(	CWObject obj,			// -> Target object
										int data);				// -> Contrast or not [0, 1]

// Accessing Mask status								
// Returns 1 if Masked, 0 otherwise						
int			CWObject_IsMasked		(	CWObject obj);			// -> Target object

// Setting Mask status								
void		CWObject_SetMasked		(	CWObject obj,			// -> Target object
										int data);				// -> Mask or not [0, 1]

// Getting Reference Object (used for modified objects found in events)								
CWObject	CWObject_GetReference	(	CWObject obj);			// -> Mask or not [0, 1]

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
