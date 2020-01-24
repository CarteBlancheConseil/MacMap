//----------------------------------------------------------------------------
// File : NSValueNamePicker.h
// Project : MacMap
// Purpose : Header file : Name and value picker window
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2019 Carte Blanche Conseil.
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
// 10/07/2019 Creation.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

#ifndef __NSValueNamePicker__
#define __NSValueNamePicker__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

bool runNamePicker  (char* name,
                     const char* wtitle,
                     bool cncl);
bool runValuePicker (bGenericType* tp,
                     int field,
                     const char* name,
                     char* value);
    
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
