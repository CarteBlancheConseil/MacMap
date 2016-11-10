//----------------------------------------------------------------------------
// File : CWMapWindow.h
// Project : MacMap
// Purpose : Header file : Map window (bGenericMapIntf) C wrappers
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
// MapWindow
//----------------------------------------------------------------------------
// 22/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWMapWindow__
#define __CWMapWindow__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWGeometry.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Getting the Map Window
CWMapWindow	CWMapWindow_Get						(	CWCallback Callback);	// -> Callback

// Getting window bounds
void		CWMapWindow_GetBounds				(	CWMapWindow mapwd,		// -> MapWindow target
													CWiRect* data);			// <- bounds

// Setting window map center
void		CWMapWindow_SetScreenCenter			(	CWMapWindow mapwd,		// -> MapWindow target
													CWiCoord data);			// -> Wanted center

// Getting the WindowRef
// Returns the main WindowRef
//WindowRef	CWMapWindow_GetWindowRef			(	CWMapWindow mapwd);		// -> MapWindow target

// Refreshing the main window
void		CWMapWindow_Inval					(	CWMapWindow mapwd);		// -> MapWindow target

// Screen To Geom conversion
// Returns a CWiCoord
CWiCoord	CWMapWindow_Screen2Geom				(	CWCallback Callback,	// -> Callback
													CGPoint cgp);			// -> Screen point

// Geom To Screen conversion
// Returns a CGPoint
CGPoint		CWMapWindow_Geom2Screen				(	CWCallback Callback,	// -> Callback
													CWiCoord coord);		// -> Coord

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
