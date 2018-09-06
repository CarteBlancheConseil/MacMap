//----------------------------------------------------------------------------
// File : CWArray.h
// Project : MacMap
// Purpose : Header file : bArray C wrappers
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
// Array manipluation, can be used outside of MacMap¨'s context
//----------------------------------------------------------------------------
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWArray__
#define __CWArray__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

typedef void* CWArray; // Opaque CWArray

//----------------------------------------------------------------------------

// CWArray allocation
CWArray	CWArray_Alloc		(	int eltsz,		// -> Elements size (must be > 0)
								int eltcount);	// -> Element count (can be set to 0)
								
// CWArray duplication								
CWArray	CWArray_Dup			(	CWArray arr);	// -> Target Array

// CWArray free								
void	CWArray_Free		(	CWArray arr);	// -> Target Array

// Getting elements
// Returns 0 if err, 1 otherwise
int		CWArray_Get			(	CWArray arr,	// -> Target Array
								int idx,		// -> Element index in [1..n]
								void* elt);		// <- Pointer to element
// Setting elements							
// Returns 0 if err, 1 otherwise
int		CWArray_Put			(	CWArray arr,	// -> Target Array
								int idx,		// -> Element index in [1..n]
								void* elt);		// -> Pointer to element
// Adding elements							
// Returns 0 if err, 1 otherwise
int		CWArray_Add			(	CWArray arr,	// -> Target Array
								void* elt);		// -> Pointer to element

// Inserting elements								
// Returns 0 if err, 1 otherwise
int		CWArray_Insert		(	CWArray arr,	// -> Target Array
								int idx,		// -> Element index in [1..n]
								void* elt);		// -> Pointer to element 
// Getting element count
// Returns elements count								
int		CWArray_Count		(	CWArray arr);	// -> Target Array

// Setting element count to 0. You have to dealloc pointers if needed
void	CWArray_Reset		(	CWArray arr);	// -> Target Array

// Sorting elements
void	CWArray_Sort		(	CWArray arr,	// -> Target Array
								int(*compare)	// -> Comparison function
								(const void*,const void*));
								
// Searching elements
// Returns element index if found ([1..n], 0 otherwise)
int		CWArray_Search		(	CWArray arr,	// -> Target Array
								const void* elt,// -> Searched Element
								int(*compare)	// -> Comparison function
								(const void*,const void*));

// Array concatenation : content of b is added to a. 
// a and b must have same element size
// Returns 0 if err, 1 otherwise
int		CWArray_Cat			(	CWArray a,		// -> Target Array
								CWArray b);		// -> Second Array

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif