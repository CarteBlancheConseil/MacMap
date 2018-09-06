//----------------------------------------------------------------------------
// File : CWApp.h
// Project : MacMap
// Purpose : Header file : bGenericMacMapApp C wrappers
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
// Application globals
//----------------------------------------------------------------------------
// 19/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWApp__
#define __CWApp__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWArray.h"
#include "CWGeometry.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Status Window
//----------------------------------------------------------------------------

// Status Window Write Proc
void	CWApp_WriteStatus			(	CWCallback Callback,	// -> App Callback
										CWCharData data);		// -> Message

//----------------------------------------------------------------------------
// Selection
//----------------------------------------------------------------------------

// Getting selection
// Return the selection's CWArray (array of CWObjects), never release it
CWArray CWApp_GetSelection			(	CWCallback Callback);	// -> App Callback

// Adding CWObjects to selection
void	CWApp_AddToSelection		(	CWCallback Callback,	// -> App Callback
										CWArray objects);		// -> Array of CWObjects
										
// Realeasing selection (i.e. resets the selection's CWarray and 
// set CWObjects selection flag to 0)
void	CWApp_ResetSelection		(	CWCallback Callback);	// -> App Callback

// Getting selection count : each time selection is modified
// the SelectionCount is set to SelectionCount+1
// Usable in Idle proc to know if selection has changed
// Returns the SelectionCount
int		CWApp_GetSelectionCount		(	CWCallback Callback);	// -> App Callback

//----------------------------------------------------------------------------
// Contrasts
//----------------------------------------------------------------------------

// Getting Contrasts
// Return the contrasts's CWArray (array of CWObjects), never release it
CWArray CWApp_GetContrasts			(	CWCallback Callback);	// -> App Callback

// Adding CWObjects to contrasts
void	CWApp_AddToContrasts		(	CWCallback Callback,	// -> App Callback
										CWArray objects);		// -> Array of CWObjects

// Realeasing contrasts (i.e. resets the contrast's CWarray and 
// set CWObjects contrasts flag to 0)
void	CWApp_ResetContrasts		(	CWCallback Callback);	// -> App Callback

// Getting contrasts count : each time contrasts are modified
// the ContrastsCount is set to ContrastsCount+1
// Usable in Idle proc to know if contrasts has changed
// Returns the ContrastsCount
int		CWApp_GetContrastsCount		(	CWCallback Callback);	// -> App Callback

//----------------------------------------------------------------------------
// Scales
//----------------------------------------------------------------------------

// Getting the scale count
// Returns the number of scale in the current document
int		CWApp_CountScales			(	CWCallback Callback);	// -> App Callback

// Getting scale
// Returns a scale, or NULL if failed
CWUnit	CWApp_GetScale				(	CWCallback Callback,	// -> App Callback
										int index);				// -> Scale index in [1..n]

// Setting scale informations
// Returns 1 if success, 0 if failed
int		CWApp_SetScale				(	CWCallback Callback,	// -> App Callback
										int index,				// -> Scale index in [1..n]
										CWCharData name,		// -> Scale Name
										double coef);			// -> Scale Value (i.e. 25000)
										
// Adding a scale
// Returns 1 if success, 0 if failed										
int		CWApp_AddScale				(	CWCallback Callback,	// -> App Callback
										CWCharData name,		// -> Scale Name
										double coef);			// -> Scale Value (i.e. 25000)
										
// Removing and destroying a scale										
// Returns 1 if success, 0 if failed										
int		CWApp_RemoveScale			(	CWCallback Callback,	// -> App Callback
										int index);				// -> Scale index in [1..n]
										
// Getting the current scale index									
// Returns scale index in [1..n]										
int		CWApp_GetCurrentScale		(	CWCallback Callback);	// -> App Callback

// Setting the current scale index									
// Returns 1 if success, 0 if failed										
int		CWApp_SetCurrentScale		(	CWCallback Callback,	// -> App Callback
										int index);				// -> Scale index in [1..n]

//----------------------------------------------------------------------------
// Dist units
//----------------------------------------------------------------------------

// Getting the unit count
// Returns the number of unit in the current document
int		CWApp_CountDistUnits		(	CWCallback Callback);	// -> App Callback

// Getting unit
// Returns a unit, or NULL if failed
CWUnit	CWApp_GetDistUnit			(	CWCallback Callback,	// -> App Callback
										int index);				// -> Unit index in [1..n]

// Getting the current unit index (defined in document's peferences)						
// Returns unit index in [1..n]										
int		CWApp_GetCurrentDistUnit	(	CWCallback Callback);	// -> App Callback

// Setting the current unit index									
// Returns 1 if success, 0 if failed										
int		CWApp_SetCurrentDistUnit	(	CWCallback Callback,	// -> App Callback
										int index);				// -> Unit index in [1..n]

//----------------------------------------------------------------------------
// Events
//----------------------------------------------------------------------------

// Opening a new event session
// Each Database action is reported in the Event Log after that call
// Warning : events can'n be nested
void	CWApp_InitEventLog			(	CWCallback Callback,	// -> App Callback
										CWCharData message,		// -> Message
										int big_endian_creator);// -> Event Creator (i.e. plugin's signature), BigEndian coded

// Closing current event session
void	CWApp_CloseEventLog			(	CWCallback Callback);	// -> App Callback

// Releasing current event session (=abort)
// Every events of the current session are released
void	CWApp_ResetEventLog			(	CWCallback Callback);	// -> App Callback

// Getting Events
// Returns a CWArray of CWEvents (see in CWEvent.h)
CWArray	CWApp_GetEvents				(	CWCallback Callback);	// -> App Callback

//----------------------------------------------------------------------------
// Drawing
//----------------------------------------------------------------------------

// Drawing current view in CGrafPtr, for a specified area
//void	CWApp_DrawInPort			(	CWCallback Callback,	// -> App Callback
//										CGrafPtr port,			// -> The graph port (from a window, offscreen, ...)
//										CWiRect bounds);		// -> The area to draw

// Changing the current drawing context
// Default graphic context is kCWGraphicContextCG. Don't forget to set it back after...
void	CWApp_SwitchContext			(	CWCallback Callback,	// -> App Callback
										int signature,			// -> Context's signature (see kCWGraphicContexts in CWDefs.h)
										CWPath path);			// -> Path if needed, NULL otherwise

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
