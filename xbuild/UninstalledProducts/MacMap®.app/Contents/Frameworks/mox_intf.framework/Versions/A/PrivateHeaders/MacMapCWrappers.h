//----------------------------------------------------------------------------
// File : MacMapCWrappers.h
// Project : MacMap
// Purpose : Header file : C wrappers around MacMap
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
// 03/09/2014 creation.
//----------------------------------------------------------------------------

#ifndef __MacMapCWrappers__
#define __MacMapCWrappers__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Status Window
//----------------------------------------------------------------------------
void	WriteMessage	(void* xapp, 
						 const char* msg);
void	WriteScale		(void* xapp);

//----------------------------------------------------------------------------
// Tools
//----------------------------------------------------------------------------
void*	CurTool			(void* xapp);

//----------------------------------------------------------------------------
// Selection & Contrastes
//----------------------------------------------------------------------------
void	FlushSelection	(void* xapp);
void	FlushContrasts	(void* xapp);
	

//----------------------------------------------------------------------------
// Styles (in current view)
//----------------------------------------------------------------------------
long	CountLayers		(void* xapp);
void*	GetIndLayer		(void* xapp,
						 long i);
void*	CurLayer		(void* xapp);

//----------------------------------------------------------------------------
// Types
//----------------------------------------------------------------------------
void*	CurType			(void* xapp);

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
