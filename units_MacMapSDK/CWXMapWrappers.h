//----------------------------------------------------------------------------
// File : CWXMapWrappers.h
// Project : MacMap
// Purpose : Header file : C wrappers for XMap interface development
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
// XMap Wrappers
//----------------------------------------------------------------------------
// 14/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWXMapWrappers__
#define __CWXMapWrappers__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

typedef void*						CWXMap;

enum{
	kCWXMapNeedNothing				=0, // XMap needs nothing
	kCWXMapNeedIdle					=2, // XMap needs to be called recurringly
	kCWXMapNeedEvents				=4	// XMap needs to 
};

enum{
	kCWXMapProcessCallFromIntf		=1 // XMap called from user interface
};

typedef void	(*CWXMap_OpenPtr)	(	CWXMap,		// -> Target XMap
										int*);		// <- Flags (kCWXMapNeedNothing,
													//    kCWXMapNeedIdle, 
													//	  kCWXMapNeedEvents, or
													//    kCWXMapNeedIdle+kCWXMapNeedEvents)
typedef void	(*CWXMap_ClosePtr)	(	CWXMap);	// -> Target XMap
typedef int		(*CWXMap_EventPtr)	(	CWXMap,		// -> Target XMap
										EventRef);	// -> Carbon Event
typedef void	(*CWXMap_IdlePtr)	(	CWXMap,		// -> Target XMap
										void*);		// -> NULL only for now
typedef int		(*CWXMap_ProcessPtr)(	CWXMap,		// -> Target XMap		
										int,		// -> kCWXMapProcessCallFromIntf only for now
										void*);		// -> NULL only for now
typedef int		(*CWXMap_TestPtr)	(	CWXMap,		// -> Target XMap
										void*);		// -> NULL only for now
typedef int		(*CWXMap_EditPtr)	(	CWXMap,		// -> Target XMap
										void*);		// -> NULL only for now

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
