//----------------------------------------------------------------------------
// File : CWLayer.h
// Project : MacMap
// Purpose : Header file : Layers C wrappers & utilities
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
// Layer manipulation. A layer is a style embbed in the current view.
// All indexes are relative to the current view, so never use them in 
// CWType's styles management procs !!!
//----------------------------------------------------------------------------
// 22/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWLayer__
#define __CWLayer__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWArray.h"
#include "CWGeometry.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Callback procs
//----------------------------------------------------------------------------

// Getting number of layer
// Returns layer index in [1..n], but can be 0 for an empty view
int			CWLayer_GetLayerCount			(	CWCallback Callback);		// -> Callback

// Getting a layer
// Returns a CWLayer if succeed, NULL if failed
CWLayer		CWLayer_Get						(	CWCallback Callback,		// -> Callback
												int index);					// -> Layer index in [1..n]
// Getting the current layer
// Returns a CWLayer, can be NULL for an empty view
CWLayer		CWLayer_GetCurrent				(	CWCallback Callback);		// -> Callback

// Setting the current layer
// Returns 1 if succeed, 0 othwise
int			CWLayer_SetCurrent				(	CWCallback Callback,		// -> Callback
												int index);					// -> Layer index in [1..n]

// Setting the current layer visibility
void		CWLayer_SetCurLayerVisibility	(	CWCallback Callback,		// -> Callback
												int data);					// -> in [0, 1]

// Setting the current layer selectability
void		CWLayer_SetCurLayerSelectability(	CWCallback Callback,		// -> Callback
												int data);					// -> in [0, 1]


//----------------------------------------------------------------------------
// Direct Layer Access
//----------------------------------------------------------------------------

// Getting layer's name (equivalent to CWType_GetStyleName, see in CWType.h)
void		CWLayer_GetName					(	CWLayer layer,				// -> Target layer
												CWCharData data);			// -> Name	

// Getting layer's type
CWType		CWLayer_GetType					(	CWLayer layer);				// -> Target layer

// Getting layer's root
// Returns a CWXMLTree. Usable with CWType_GetStyleIndexFromRoot (see in CWType.h)
CWXMLTree	CWLayer_GetRoot					(	CWLayer layer);				// -> Target layer

// Getting layer's visibility status
// Returns visibility in [0, 1]
int			CWLayer_GetVisibility			(	CWLayer layer);				// -> Target layer

// Getting layer's selectability status
// Returns selectability in [0, 1]
int			CWLayer_GetSelectability		(	CWLayer layer);				// -> Target layer

// Getting objects at point
// Allocates a CWArray of CWObjects, or NULL if failed
CWArray		CWLayer_GetCWObjectsAtPoint		(	CWLayer layer,				// -> Target layer
												CGPoint cgp,				// -> Point
												int all);					// -> In [0,1], get all objects if 1, only first if 0

// Getting objects in rect
// Allocates a CWArray of CWObjects, or NULL if failed
CWArray		CWLayer_GetCWObjectsInRect		(	CWLayer layer,				// -> Target layer
												CGRect cgr,					// -> Rect
												int strict);				// -> In [0,1], get only fully included objects 1

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
