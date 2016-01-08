//----------------------------------------------------------------------------
// File : CWXMap.h
// Project : MacMap
// Purpose : Header file : C wrappers for XMap developpement
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
// XMap Intf
//----------------------------------------------------------------------------
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWXMap__
#define __CWXMap__

//----------------------------------------------------------------------------

#include "CWXMapWrappers.h"
#include "CWDefs.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// CWXMap allocation
CWXMap		CWXMap_Alloc		(	CWCallback Callback,		// -> Callback Address
									CFBundleRef bndl,			// -> Plugin's bundle
									CWXMap_OpenPtr open,		// -> Open proc, can be NULL
									CWXMap_ClosePtr close,		// -> Close proc, can be NULL
									CWXMap_EventPtr event,		// -> Event proc, can be NULL
									CWXMap_IdlePtr idle,		// -> Idle proc, can be NULL
									CWXMap_ProcessPtr process,	// -> Process proc, can be NULL
									CWXMap_TestPtr test,		// -> Test proc, can be NULL
									CWXMap_EditPtr edit,		// -> Edit proc, can be NULL
									CWCharData name);			// -> Name, used in xml tree, so be carrefull !!

// CWXMap free								
void		CWXMap_Free			(	CWXMap xmap);				// -> Target XMap

// Accessing CallBack
// Returns the CallBack Address
CWCallback	CWXMap_GetCallBack	(	CWXMap xmap);				// -> Target XMap

// Accessing bundle
// Returns the Plug-in's BundleRef
CFBundleRef	CWXMap_GetBundle	(	CWXMap xmap);				// -> Target XMap

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
