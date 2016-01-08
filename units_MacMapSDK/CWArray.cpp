//----------------------------------------------------------------------------
// File : CWArray.cpp
// Project : MacMap
// Purpose : C++ source file : bArray C wrappers
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
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWArray.h"
#include <MacMapSuite/bArray.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWArray_Alloc(int eltsz, int eltcount){
	return(new bArray(eltsz,eltcount));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWArray_Dup(CWArray arr){
	return(new bArray(*((bArray*)arr)));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWArray_Free(CWArray arr){
	return(delete (bArray*)arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Get(CWArray arr, int idx, void* elt){
bArray* barr=(bArray*)arr;
	return(barr->get(idx,elt));
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Put(CWArray arr, int idx, void* elt){
bArray* barr=(bArray*)arr;
	return(barr->put(idx,elt));
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Add(CWArray arr, void* elt){
bArray* barr=(bArray*)arr;
	return(barr->add(elt));
}

// ---------------------------------------------------------------------------
// PAS DANS L'INTERFACE POUR L'INSTANT
// ------------
int	CWArray_Push(CWArray arr, int idx, int offset){
bArray* barr=(bArray*)arr;
	return(barr->push(idx,offset));
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Insert(CWArray arr, int idx, void* elt){
bArray* barr=(bArray*)arr;
	return(barr->insert(idx,elt));
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Count(CWArray arr){
bArray* barr=(bArray*)arr;
	return(barr->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
void	CWArray_Reset(CWArray arr){
bArray* barr=(bArray*)arr;
	barr->reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
void	CWArray_Sort(CWArray arr, int(*compare)(const void*,const void*)){
bArray* barr=(bArray*)arr;
	barr->sort(compare);
}

// ---------------------------------------------------------------------------
// 
// ------------
int	CWArray_Search(	CWArray arr, const void* elt, 
					int(*compare)(const void*,const void*)){
bArray* barr=(bArray*)arr;
	return(barr->search(elt,compare));
}

// ---------------------------------------------------------------------------
// PAS DANS L'INTERFACE POUR L'INSTANT
// ------------
void CWArray_SetSorted(CWArray arr, int sorted){
bArray* barr=(bArray*)arr;
	return(barr->setsorted(sorted));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWArray_Cat(CWArray a, CWArray b){
bArray* ba=(bArray*)a;
bArray* bb=(bArray*)b;
	return((*ba)+(*bb));
}
