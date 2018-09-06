//----------------------------------------------------------------------------
// File : CWGeometry.h
// Project : MacMap
// Purpose : Header file : MacMap geometry utilities
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
// Geometry manipluation, can be used outside of MacMap¨'s context
//----------------------------------------------------------------------------
// 15/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWGeometry__
#define __CWGeometry__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Integer Geometry
typedef struct CWiCoord{
	int		h;
	int		v;
}CWiCoord;

typedef	struct	CWiRect{ 
	int		top;
	int		left;
	int		bottom;
	int		right;
}CWiRect;

typedef void* CWiGeometry;

// Double Geometry
typedef struct CWdCoord{
	double	x;
	double	y;
}CWdCoord;

//----------------------------------------------------------------------------

// CWiGeometry allocation
// Returns NULL if failed
CWiGeometry		CWiGeometry_Alloc				(	int ncoord,			// -> CWiCoord count (>=0)
													int npart);			// -> Part count ((>=0)&&(<=ncoord))

// CWiGeometry free								
void			CWiGeometry_Free				(	CWiGeometry geom);	// -> Reference geometry

// Getting total coord count
// Returns total coord count of the geometry
int				CWiGeometry_CoordCount			(	CWiGeometry geom);	// -> Reference geometry

// Getting part count
// Returns number of part of the geometry
int				CWiGeometry_PartCount			(	CWiGeometry geom);	// -> Reference geometry

// Getting coord count for part
// Returns the coord count for a part of the geometry
int				CWiGeometry_CoordCountForPart	(	CWiGeometry geom,	// -> Reference geometry
													int pindex);			// -> Part number in [1..n]

// Getting a coord (total count)
// Returns 1 if success, 0 if failed
int				CWiGeometry_GetCoord			(	CWiGeometry geom,	// -> Reference geometry
													int cindex,			// -> Coord index in [1..n]
													CWiCoord* coord);	// -> Coord

// Setting a coord (total count)
// Returns 1 if success, 0 if failed
int				CWiGeometry_SetCoord			(	CWiGeometry geom,	// -> Reference geometry
													int coordidx,		// -> Coord index in [1..n]
													CWiCoord coord);	// <- Coord

// Getting a coord in a part
// Returns 1 if success, 0 if failed
int				CWiGeometry_GetCoordForPart		(	CWiGeometry geom,	// -> Reference geometry
													int pindex,			// -> Part index in [1..n]
													int cindex,			// -> Coord index in [1..n]
													CWiCoord* coord);	// -> Coord

// Setting a coord in a part
// Returns 1 if success, 0 if failed
int				CWiGeometry_SetCoordForPart		(	CWiGeometry geom,	// -> Reference geometry
													int pindex,         // -> Part index in [1..n]
													int cindex,         // -> Coord index in [1..n]
													CWiCoord coord);	// <- Coord

// Adding a coord
// Returns realloc of geom if success, NULL if failed
CWiGeometry		CWiGeometry_AddCoord			(	CWiGeometry geom,	// -> Reference geometry
													CWiCoord coord);	// -> Coord

// Removing a coord
// Returns realloc of geom if success, NULL if failed
CWiGeometry		CWiGeometry_RemoveCoord			(	CWiGeometry geom,	// -> Reference geometry
													int cindex);		// -> Coord index in [1..n]

// Geometry concatenation
// Returns realloc of geoma if success, NULL if failed
CWiGeometry		CWiGeometry_Cat					(	CWiGeometry geoma,	// -> Reference geometry
													CWiGeometry geomb);	// -> Second geometry

// Grouping geometry (i.e. adding a part)
// Returns realloc of Geom if success, NULL if failed
CWiGeometry		CWiGeometry_Group				(	CWiGeometry geoma,	// -> Reference geometry
													CWiGeometry geomb);

// Getting geometry bounds
CWiRect			CWiGeometry_Bounds				(	CWiGeometry geom);	// -> Reference geometry

// Conversion iCoord -> dCoord
void			CWiCoord2CWdCoord				(	CWdCoord* dcoord,	// <- Target coord
													CWiCoord* icoord,	// -> Source coord
													CWdCoord* origin,	// -> Document origin
													double reso);		// -> Document resolution

// Conversion dCoord -> iCoord
void			CWdCoord2CWiCoord				(	CWiCoord* icoord,	// <- Target coord
													CWdCoord* dcoord,	// -> Source coord
													CWdCoord* origin,	// -> Document origin
													double reso);		// -> Document resolution

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif