//----------------------------------------------------------------------------
// File : NSContextualMenuWrappers.h
// Project : MacMap
// Purpose : Header file : NSMenu C wrappers
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
// 25/03/2014 Creation. Can handle 2 levels popup menus.
//----------------------------------------------------------------------------

#ifndef __NSContextualMenuWrappers__
#define __NSContextualMenuWrappers__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

typedef struct menuitem_desc{
	char	name[256];
	long	index;
	bool	disabled;
	bool	checked;
}menuitem_desc;

//----------------------------------------------------------------------------

long	popUpContextMenuWithNSEvent				(	void* event,
													menuitem_desc* desc,
													long n);
long	popUpContextMenuWithCGPointAndNSWindow	(	CGPoint locationInWindow,
													void* window,
													menuitem_desc* desc,
													long n,
                                                    CGFloat unused);
	
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
