//----------------------------------------------------------------------------
// File : CWEvent.h
// Project : MacMap
// Purpose : Header file : bGenericEvent C wrappers
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
// MacMap¨ Database Events Manipulation
//----------------------------------------------------------------------------
// 20/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWEvent__
#define __CWEvent__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWArray.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Getting event kin (see kCWEventKinds in CWDefs.h)
int			CWEvent_GetKind			(	CWEvent event); // -> ref event

// Getting event action (see kCWEventActions in CWDefs.h)
int			CWEvent_GetAction		(	CWEvent event); // -> ref event

// Getting event creator
int			CWEvent_GetCreator		(	CWEvent event); // -> ref event

// Getting the associated message
char*		CWEvent_GetMessage		(	CWEvent event); // -> ref event

// Getting an event block
// Returned CWArray contains :
// - CWType for kCWEventKindTypeElement
// - CWfield_info for kCWEventKindDataStruct
// - CWObject for kCWEventKindGeoElement
// - CWXMLTree for kCWEventKindUserExt
CWArray		CWEvent_GetElements		(	CWEvent event);

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif