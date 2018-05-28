//----------------------------------------------------------------------------
// File : CWDocument.h
// Project : MacMap
// Purpose : Header file : bGenericDocument C wrappers
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
// Document Data
//----------------------------------------------------------------------------
// 19/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWDocument__
#define __CWDocument__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWGeometry.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Getting the current CWDocument
// Returns NULL if no document opened
CWDocument	CWDocument_Get						(	CWCallback Callback);	// -> Callback

// Getting document's name
void		CWDocument_GetName					(	CWDocument document,	// -> Current document
													CWFileName data);		// <- Document's name

// Getting document's path
void		CWDocument_GetPath					(	CWDocument document,	// -> Current document
													CWPath data);			// <- Document's path

// Getting document's SRID
// Returns epsg projection ID associated to the current document.
int			CWDocument_GetSRID					(	CWDocument document);	// -> Current document

// Setting document's SRID
// Return's 1 if success, 0 if failed. Document SRID can be set only once.
int			CWDocument_SetSRID					(	CWDocument document,	// -> Current document
													int data);				// -> srid

// Getting document's origin
// Returns the documents origin.
CWdCoord	CWDocument_GetOrigin				(	CWDocument document);	// -> Current document

// Setting document's origin
// Return's 1 if success, 0 if failed.
int			CWDocument_SetOrigin				(	CWDocument document,	// -> Current document
													CWdCoord data);			// -> origin

// Getting digits.
int			CWDocument_GetDistDigits			(	CWDocument document);	// -> Current document

// Setting digits
// Returns 1 if success, 0 if failed
int			CWDocument_SetDistDigits			(	CWDocument document,	// -> Current document
													int data);				// -> Digits number

// Getting document's resolution
double		CWDocument_GetDistResolution		(	CWDocument document);	// -> Current document

// Getting document's unit
// Returns a CWUnit
CWUnit		CWDocument_GetDistResolutionUnit	(	CWDocument document);	// -> Current document

// Getting param count for a subdirectory
// Returns in [1..n], 0 if no params
int			CWDocument_GetParamCount			(	CWDocument document,	// -> Current document
													char* sub);				// -> Sub directory

// Getting param name in a subdirectory
// Returns 1 if succeed, 0 if failed
int			CWDocument_GetParamName				(	CWDocument document,	// -> Current document
													char* sub,				// -> Sub directory
													int index,				// -> Param's index in [1..n]
													CWCharData name);		// <- Param's name

// Getting a param
// Returns 1 if succeed, 0 if failed
int			CWDocument_GetParam					(	CWDocument document,	// -> Current document
													char* sub,				// -> Sub directory
													CWFileName name,		// -> Param name
													void** data,			// <- Param
													int* datasize);			// <- Param's Size

// Adding a param
// Returns 1 if succeed, 0 if failed
int			CWDocument_SetParam					(	CWDocument document,	// -> Current document
													char* sub,				// -> Sub directory
													CWFileName name,		// -> Param name
													void* data,				// <- Param
													int datasize);			// <- Param's Size

// Getting an XML param
// Returns 1 if succeed, 0 if failed
int			CWDocument_GetXMLParam				(	CWDocument document,	// -> Current document
													char* sub,				// -> Sub directory
													CWFileName name,		// -> Param name
													CWXMLTree* data);		// <- Param

// Adding an XML param
// Returns 1 if succeed, 0 if failed
int			CWDocument_SetXMLParam				(	CWDocument document,	// -> Current document
													char* sub,				// -> Sub directory
													CWFileName name,		// -> Param name
													CWXMLTree data);		// -> Param. Pass NULL to remove an existing param.

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
