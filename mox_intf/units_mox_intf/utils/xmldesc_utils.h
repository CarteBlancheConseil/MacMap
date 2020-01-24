//----------------------------------------------------------------------------
// File : xmldesc_utils.h
// Project : MacMap
// Purpose : Header file : XML creation utils
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
// 24/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __xmldesc_utils__
#define __xmldesc_utils__

//----------------------------------------------------------------------------

#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

void add_cdesc	(	bArray& arr, 
					int indent, 
					const char* name, 
					const char* value);
void add_idesc	(	bArray& arr, 
					int indent, 
					const char* name, 
					int value);
void add_ddesc	(	bArray& arr, 
					int indent, 
					const char* name, 
					double value, int decs);
void free_descs	(	bArray& arr);
void dump_descs	(	bArray& arr);

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
