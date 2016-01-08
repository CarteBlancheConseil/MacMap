//----------------------------------------------------------------------------
// File : bXMapsGISIOUtils.h
// Project : MacMap
// Purpose : Header file : C wrappers utilities for GIS import/export
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
// 07/07/2010 creation.
// 10/02/2015 NAV to NS.
//----------------------------------------------------------------------------

#ifndef __bXMapsGISIOUtils__
#define __bXMapsGISIOUtils__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

void*   GISIOImport_getAccessoryViewController      (   bool hideCreateIfNeeded);
void*   GISIOImport_getAccessoryViewFromController  (   void* ctrlr);
bool    GISIOImport_getCreateIfNeededFromController (   void* ctrlr);
void    GISIOImport_releaseController               (   void* ctrlr);

void*   GISIOExport_getAccessoryViewController      (   );
void*   GISIOExport_getAccessoryViewFromController  (   void* ctrlr);
void    GISIOExport_releaseController               (   void* ctrlr);

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
