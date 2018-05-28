//----------------------------------------------------------------------------
// File : bStdNSPalettevDefCocoaStuff.h
// Project : MacMap
// Purpose : Header file : Cocoa floating window vDef controller C wrappers
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

void*	CCWDvDef_Init			(const void* gapp,// Default allocator
								 const void* ext);
	
void	CCWDvDef_Dispose		(const void* instance);
void	CCWDvDef_Close			(const void* instance);
void	CCWDvDef_CheckMMEvt		(const void* instance);
void	CCWDvDef_Idle			(const void* instance);
	
//----------------------------------------------------------------------------
// Sample Allocator
/*
void* myCCWDvDef_Init(const void* gapp, 
					  const void* ext) {
myvDefWindowController	*controller;
NSAutoreleasePool		*localPool;
	
	localPool=[[NSAutoreleasePool alloc] init];   
	controller=[[myvDefWindowController alloc] init];
	[controller setExt:(bStdNSPalettevDef*)ext];
	[localPool release];
	return((void*)controller);
}
*/

//----------------------------------------------------------------------------
	
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------
