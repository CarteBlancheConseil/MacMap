//----------------------------------------------------------------------------
// File : CWType.h
// Project : MacMap
// Purpose : Header file : bGenericType C wrappers
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
// Array manipluation
//----------------------------------------------------------------------------
// 15/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWType__
#define __CWType__

//----------------------------------------------------------------------------

#include "CWDefs.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// CWType base management
//----------------------------------------------------------------------------

// CWType creation
// Returns a CWType if success, NULL if failed
CWType	CWType_Create					(	CWCallback Callback,	// -> Callback
											CWFileName name,  		// -> Name
											CWPath path,			// -> Path
											int kind,				// -> Type Kind (see kCWTypeKind in	CWDefs.h)
											int tkind);				// -> pass kCWBaseKindLocal (CWDefs.h)
// CWType opening
// Returns a CWType if success, NULL if failed
CWType	CWType_Open						(	CWCallback Callback,	// -> Callback
											CWFileName name, 		// -> Name
											CWPath path,			// -> Path	
											int tkind);				// -> pass kCWBaseKindLocal (CWDefs.h)
// Getting CWType
// Returns a CWType if success, NULL if failed
CWType	CWType_Get						(	CWCallback Callback,	// -> Callback
											int index);				// -> Index in [1..n]

// Getting current CWType
// Returns a CWType or NULL
CWType	CWType_GetCurrent				(	CWCallback Callback);	// -> Callback
								
// Getting name
void	CWType_GetName					(	CWType typ,				// -> Target CWType
											CWCharData data);		// -> Name

// Getting Path
void	CWType_GetPath					(	CWType typ,				// -> Target CWType
											CWPath path);			// -> Path

// Getting ID
double	CWType_GetID					(	CWType typ);			// -> Target CWType

// Getting Index in [1..n]
int		CWType_GetIndex					(	CWType typ);			// -> Target CWType 

// Getting kind 
int		CWType_GetKind					(	CWType typ);			// -> Target CWType

// Getting record count
int		CWType_GetRecordCount			(	CWType typ);			// -> Target CWType

// Getting killed record count
int		CWType_GetRecordKilled			(	CWType typ);			// -> Target CWType

// Getting alive record count
int		CWType_GetRecordAlive			(	CWType typ);			// -> Target CWType

// Getting select count (number objects of type selected)
int		CWType_GetSelectCount			(	CWType typ);			// -> Target CWType

// Getting select count (number objects of type contrasted)
int		CWType_GetContrastCount			(	CWType typ);			// -> Target CWType

//----------------------------------------------------------------------------
// CWType fields management
//----------------------------------------------------------------------------

// Getting field count
// Returns kCWObject_Dir_+User Fields Count. See kCWObject_Dir_ in CWObject.h
int		CWType_GetFieldCount			(	CWType typ);			// -> Target CWType

// Getting field info
// Returns 1 if success, 0 if failed
int		CWType_GetFieldInfo				(	CWType typ,				// -> Target CWType
											int index,				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int* kind,				// <- Field kind
											int* len,				// <- Field signature
											int* decimals,			// <- Field decimals
											CWCharData name);		// <- Field name

// Setting field info
// Returns 1 if success, 0 if failed
int		CWType_SetFieldInfo				(	CWType typ,				// -> Target CWType
											int index,				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int kind,				// -> Field kind
											int len,				// -> Field signature
											int decimals,			// -> Field decimals
											CWCharData name);		// -> Field name

// Adding field
// Returns 1 if success, 0 if failed
int		CWType_AddField					(	CWType typ,				// -> Target CWType
											int kind,				// -> Field kind
											int len,				// -> Field signature
											int decimals,			// -> Field decimals
											CWCharData name);		// -> Field name

// Removing field
// Returns 1 if success, 0 if failed
int		CWType_RemoveField				(	CWType typ,				// -> Target CWType
											int index);				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)

// Getting Field ID
// Returns field ID if succeed, 0 otherwise
int		CWType_FieldIDWithIndex			(	CWType typ,				// -> Target CWType
											int index);				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)

// Getting Field index from ID
// Returns field index in [1..n] if succeed, 0 otherwise
int		CWType_FieldIndexWithID			(	CWType typ,				// -> Target CWType
											int ID);				// -> Field ID

// Getting Field index from Name
// Returns field index in [1..n] if succeed, 0 otherwise
int		CWType_FieldIndexWithName		(	CWType typ,				// -> Target CWType
											CWCharData name);		// -> Field name

// Getting Field Status Flags
// Returns 1 id succeed, 0 otherwise
int		CWType_GetFieldFlags			(	CWType typ,				// -> Target CWType
											int index,				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int* delprot,			// <- Delete protected [0, 1]
											int* writeprot,			// <- Write protected [0, 1]
											int* hidden);			// <- Hidden [0, 1]

// Setting Field Status Flags
// Returns 1 succeed, 0 otherwise
int		CWType_SetFieldFlags			(	CWType typ,				// -> Target CWType
											int index,				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int delprot,			// -> Delete protected [0, 1]
											int writeprot,			// -> Write protected [0, 1]
											int hidden);			// -> Hidden [0, 1]

// Getting Field constraints count
// Returns constraints count if succeed or -1 if failed
int		CWType_GetFieldConstraintCount	(	CWType typ,				// -> Target CWType
											int index);				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)

// Getting Field constraints count
// Returns kCWTypeFieldKind if success, 0 if failed
int		CWType_GetFieldConstraintKind	(	CWType typ,				// -> Target CWType
											int index);				// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)

// Getting constraints value
// Returns 1 if success, 0 if failed
int		CWType_GetFieldConstraintValue	(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int constraintindex,	// -> Constraint index in [1..n]
											CWCharData data);		// <- Constraint value

// Setting constraints value
// Returns 1 if success, 0 if failed
int		CWType_SetFieldConstraintValue	(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int constraintindex,	// -> Constraint index in [1..n]
											CWCharData data);		// -> Constraint value

// Adding constraints
// Returns 1 if success, 0 if failed
int		CWType_AddFieldConstraint		(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int constraintkind,		// -> Constraint kind (see kCWTypeFieldKind in CWDefs.h)
											CWCharData data);		// -> Constraint value

// Removing constraints
// Returns 1 if success, 0 if failed
int		CWType_RemoveFieldConstraint	(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											int constraintindex);	// -> Constraint index in [1..n]	

// Getting constraints index
// Returns constraint index if success, 0 if failed
int		CWType_GetFieldConstraintIndex	(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											CWCharData data);		// -> Constraint value

// Getting default value
// Returns 1 if success, 0 if failed
int		CWType_FieldHasDefaultValue		(	CWType typ,				// -> Target CWType
											int fieldindex);		// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)

// Getting default value
// Returns 1 if success, 0 if failed
int		CWType_FieldGetDefaultValue		(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											CWCharData data);		// -> Default value

// Setting default value
// Returns 1 if success, 0 if failed
int		CWType_FieldSetDefaultValue		(	CWType typ,				// -> Target CWType
											int fieldindex,			// -> Field index in [1..n], user fields at kCWObject_Dir_+1 (see in CWDefs.h)
											CWCharData data);		// <- Default value
										
//----------------------------------------------------------------------------
// CWType params management
//----------------------------------------------------------------------------

// Getting Param count
// Returns param count
int		CWType_GeParamCount				(	CWType typ,				// -> Target CWType
											CWCharData folder);		// -> Param's folder

// Getting Param Name
// Returns 1 if succeed, 0 otherwise
int		CWType_GeParamName				(	CWType typ,				// -> Target CWType
											CWCharData folder,		// -> Param's folder
											int index,				// -> Param's index in [1..n]
											CWCharData name);		// -> Param's name

// Getting Param. Recommanded to use CWType_GetXMLParam instead.
// Returns 1 if succeed, 0 otherwise
int		CWType_GetParam					(	CWType typ,				// -> Target CWType
											CWCharData folder,		// -> Param's folder
											CWCharData name,		// -> Param's name
											void** data,			// <- Param
											int* datasize);			// <- Param's size

// Setting Param. Recommanded to use CWType_SetXMLParam instead.
// Returns 1 if succeed, 0 otherwise
int		CWType_SetParam					(	CWType typ,				// -> Target CWType
											CWCharData folder,		// -> Param's folder
											CWCharData name,		// -> Param's name
											void* data,				// -> Param. Pass NULL to remove an existing param
											int datasize);			// -> Param's size

// Getting XML Param
// Returns 1 if succeed, 0 otherwise
int		CWType_GetXMLParam				(	CWCallback Callback,	// -> Callback
											CWType typ,				// -> Target CWType
											CWCharData folder,		// -> Param's folder
											CWCharData name,		// -> Param's name
											CWXMLTree* data);		// <- Param

// Setting XML Param
// Returns 1 if succeed, 0 otherwise
int		CWType_SetXMLParam				(	CWType typ,				// -> Target CWType
											CWCharData folder,		// -> Param's folder
											CWCharData name,		// -> Param's name
											CWXMLTree data);		// -> Param, pass NULL to remove an existing param

//----------------------------------------------------------------------------
// CWType object iteration
//----------------------------------------------------------------------------

// Iterate type
// Returns 1 if succeed, 0 otherwise
int		CWType_Iterate					(	CWType typ,				// -> Target CWType
											void* data,				// -> UserData
											int(*proc)				// -> Function
											(CWObject,				// -> CWObject
											void*));				// -> UserData passed upper

//----------------------------------------------------------------------------
// CWType styles
//----------------------------------------------------------------------------

// Getting number of styles for a type
// return style count in [1..n]
int			CWType_GetStyleCount			(	CWType typ);			// -> Target CWType

// Getting a style name
// return 1 if succeed, 0 if failed
int			CWType_GetStyleName				(	CWType typ,				// -> Target CWType
												int index,				// -> Style index in [1..n]
												CWCharData data);		// -> Style name

// Setting a style name
// return 1 if succeed, 0 if failed
int			CWType_SetStyleName				(	CWType typ,				// -> Target CWType
												int index,				// -> Style index in [1..n]
												CWCharData data);		// <- Style name

// Getting style creator
// 
int			CWType_GetStyleCreator			(	CWType typ,				// -> Target CWType
												int index);				// -> Style index in [1..n]

// Getting style root
// return CWXMLTree if succeed, NULL if failed
CWXMLTree	CWType_GetStyleRoot				(	CWType typ,				// -> Target CWType
												int index);				// -> Style index in [1..n]

// Getting style index
// return index in [1..n] if succeed, 0 if failed
int			CWType_GetStyleIndexFromName	(	CWType typ,				// -> Target CWType
												CWCharData data);		// -> Style name

// Getting style index
// return index in [1..n] if succeed, 0 if failed
int			CWType_GetStyleIndexFromRoot	(	CWType typ,				// -> Target CWType
												CWXMLTree root);		// -> Style root

// Adding a style
// return 1 succeed, 0 if failed
int			CWType_AddStyle					(	CWType typ,				// -> Target CWType
												CWXMLTree root);		// -> Style root

// Removing a style
// return 1 succeed, 0 if failed
int			CWType_RemoveStyle				(	CWType typ,				// -> Target CWType
												int index);				// -> Style index in [1..n]

// Creating a style
// return 1 succeed, 0 if failed
int			CWType_CreateStyle				(	CWType typ,				// -> Target CWType
												int signature);			// -> Style creator signature

// Editing a style
// return 1 succeed, 0 if failed
int			CWType_EditStyle				(	CWType typ,				// -> Target CWType
												int index,				// -> Style index in [1..n]
												int signature);			// -> Style creator signature

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
