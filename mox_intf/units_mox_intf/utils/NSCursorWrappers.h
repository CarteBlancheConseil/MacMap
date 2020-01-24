//----------------------------------------------------------------------------
// File : NSCursorWrappers.h
// Project : MacMap
// Purpose : Header file : NSCursor C wrappers
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
// 11/07/2007 Creation.
//----------------------------------------------------------------------------

#ifndef __NSCursorWrappers__
#define __NSCursorWrappers__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

typedef void *CursorRef;

//----------------------------------------------------------------------------

CursorRef	CreateCocoaCursor	(	CGImageRef cgImageRef, 
									float hotSpotX, 
									float hotSpotY);
void		ReleaseCocoaCursor	(	CursorRef cursor);
void		SetCocoaCursor		(	CursorRef cursor);
void		HideCocoaCursor		(	);
void		ShowCocoaCursor		(	);

//----------------------------------------------------------------------------

void        MMBeep              (   );
    
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
