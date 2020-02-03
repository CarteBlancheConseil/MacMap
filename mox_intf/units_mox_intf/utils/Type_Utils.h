//----------------------------------------------------------------------------
// File : Type_Utils.h
// Project : MacMap
// Purpose : Header file : C utils around MacMap types 
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
// 28/11/2005 creation.
//----------------------------------------------------------------------------

#ifndef __Type_Utils__
#define __Type_Utils__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/wbase.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Returns 0 if selection is not mono type
// Returns type index if selection is mono type
int		SelectionIsMonoType				(	bGenericMacMapApp* gapp);

// Returns 0 if contrastes are not mono type
// Returns type index if contrastes are mono type
int		ContrastesAreMonoType			(	bGenericMacMapApp* gapp);

// Returns kNoKind_ if selection is not mono gender
// Returns gender (kBaseKindPoint, kBaseKindPolyline, kBaseKindText, kBaseKindPolygon, or kBaseKindRaster)
// if selection is mono type
int		SelectionIsMonoGender			(	bGenericMacMapApp* gapp);

// Returns 0 if field kind is equal to field constraint
int		FieldKindEqualToFieldConstraint	(	bGenericType* tp, int field);

// Put an XML param in type "tp". Returns 0 if failed 
int		SetXMLParamToType				(	bGenericType* tp,
											const char* sub, 
											const char* name,
											bGenericXMLBaseElement* root);
											
// Get an XML param from type "tp". Retruns NULL if failed 
bGenericXMLBaseElement*
		GetXMLParamFromType				(	bGenericMacMapApp* gapp,
											bGenericType* tp,
											const char* sub, 
											const char* name);
	
// Getting objects from IDs
bool	RecoverObjects					(	bGenericType* tp,
											bArray& oids,
											bArray& objects);

// Adding fields from descriptors
void	add_fields						(	wbase bse, 
											field_desc* flds, 
											int n);

// Returns 0 if field kind is equal to field constraint
int		TypeIsExportProtected			(	bGenericType* tp);
											
//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
