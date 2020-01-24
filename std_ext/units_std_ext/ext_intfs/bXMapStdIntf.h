//----------------------------------------------------------------------------
// File : bXMapStdIntf.h
// Project : MacMap
// Purpose : Header file : XMap params
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
// 07/11/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapStdIntf__
#define __bXMapStdIntf__

//----------------------------------------------------------------------------

#include <MacMapSuite/MacMapSuiteCPP.h>

#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------
// Join Family
//------------
typedef struct join_prm{
	i2dvertex			vx;		//  -> Vertex to proj
	i2dvertex			prev;	//  -> Previous vertex to for angle proj
	ivertices*			ref;	//  -> Ref segs
	
	double				dmax;	// <-> Dist max
	bool				status; // <-> XMap on/off
	bArray*				arr;	// <-> Array of activated types

	i2dvertex			res;	// <-  Result vertex
	int					cur;	// <-  Index of result in ref
	double				d;		// <-  Dist of result from ref
	
}join_prm;

//----------------------------------------------------------------------------
// Transfert de type
//------------
typedef struct transtype_prm{
	int		type_dest;
	bool	keep_objects;
	bool	create_fields;
	bArray*	objects;
}transtype_prm;

//----------------------------------------------------------------------------
// Translation
//------------
enum{
	kXMapTranslationWithOffset		=1,
	kXMapTranslationToCoord			=2,
};

typedef struct translation_prm{
	double	x;
	double	y;
	int		kind;
}translation_prm;

//----------------------------------------------------------------------------
// Rotation
//------------
enum{
	kXMapRotationRetro			=1,
	kXMapRotationDir			=2,
	kXMapRotationOnSelf			=1,
	kXMapRotationOnCoord		=2
};

typedef struct rotation_prm{
	double	x;
	double	y;
	double	rad;
}rotation_prm;

//----------------------------------------------------------------------------
// Dilatation
//------------
enum{
	kXMapScaleOnSelf		=1,
	kXMapScaleOnCoord		=2
};

typedef struct scale_prm{
	double	x;
	double	y;
	double	cx;
	double	cy;
}scale_prm;

//----------------------------------------------------------------------------
// Shake
//------------
typedef double shake_prm;

//----------------------------------------------------------------------------
// Association
//------------
typedef struct association_prm{
	double			dmax;		// -> max dist
	bool			kill;		// -> kill objects
}association_prm;

//----------------------------------------------------------------------------
// Association with field
//------------
typedef struct associationwithfield_prm{
	double			lmin;		// -> min length
	double			lmax;		// -> max length
	int				needhier;	// -> 0 (do not use subtype) or 1 (use subtype)
	int				field;		// -> name field
}associationwithfield_prm;

//----------------------------------------------------------------------------
// Paint
//------------
typedef int paint_prm;

//----------------------------------------------------------------------------
// Import
//------------
typedef struct import_grid{
	double	t_point;
	double	t_line;
	double	t_poly;
	double	t_text;
	double	t_raster;
	int		srid;
}import_grid;

typedef struct import_prm{
	import_grid		grid;
	char			path[PATH_MAX];
	char			name[FILENAME_MAX];
	bool			ctin;
    
    long            typErr;
    long            objErr;
    long            filErr;
}import_prm;

typedef struct importpts_prm{
	import_grid		grid;
	char			path[PATH_MAX];
	char			name[FILENAME_MAX];
	bool			ctin;
	int				fx;
	int				fy;

    long            typErr;
    long            objErr;
    long            filErr;
}importpts_prm;

//----------------------------------------------------------------------------
// ConvexHull
//------------
typedef bGenericType* convexhull_prm;

//----------------------------------------------------------------------------
// PolygonalPath
//------------
typedef bGenericType* polygonal_prm;

//----------------------------------------------------------------------------
// SplitToPoints
//------------
typedef bGenericType* splittopoints_prm;

//----------------------------------------------------------------------------
// AgregateIntoLine
//------------
typedef bGenericType* agregateintoline;

//----------------------------------------------------------------------------
// AgregateIntoPoly
//------------
typedef bGenericType* agregateintopoly;

//----------------------------------------------------------------------------
// MakeRect
//------------
typedef struct makerect_prm{
	int				justo;
	d2dvertex		o;
	double			w;
	double			h;
	bGenericType*	tp;
	int				sui;
}makerect_prm;

//----------------------------------------------------------------------------
// MakePoly
//------------
typedef struct makepoly_prm{
	int				justo;
	d2dvertex		o;
	int				nb;
	double			sz;
	bGenericType*	tp;
	int				sui;
}makepoly_prm;

//----------------------------------------------------------------------------
// MakeSquaring
//------------
enum{
	kMakeSquaringSqr		=1,
	kMakeSquaringHex		=2
};

typedef struct makesquaring_prm{
	int				justo;
	d2dvertex		o;
	int				nh;
	int				nv;
	double			sz;
	int				kind;
	bGenericType*	tp;
}makesquaring_prm;

//----------------------------------------------------------------------------
// Pickers
//------------
typedef struct picker_prm{
	bGenericType*	tp;					// -> type used
	char			name[FILENAME_MAX];	// <- rsrc choosen (empty if cancel)
}picker_prm;

//----------------------------------------------------------------------------
// DataJoin
//------------
enum{
	kDataJoinActionIgnore	=1,
	kDataJoinActionSearch	=2,
	kDataJoinActionFill		=3,
	kDataJoinActionCreate	=4
};

typedef struct datajoin_act{
	int		k;			// Action kind
	int		fidx;		// Field index in database
}datajoin_act;

typedef struct datajoin_prm{
	bArray*	cols;		// ->array of datajoin_act
	int		tpi;		// ->type index
	wtable	tbl;		// ->input table
	bool	use_msk;	// ->use masked objects
	bool	unique;		// ->fill only one per key
}datajoin_prm;

//----------------------------------------------------------------------------
// NetClean
//------------
typedef struct netcleancfg_prm{
	bArray*			nodes;		// ->array of bGenericType
	bArray*			edges;		// ->array of bGenericType
	
	bGenericType*	tnod;		// ->type for node creation
	int				stnod;		// ->subtype for node creation
	char			name[256];	// ->name for node creation

	bGenericType*	tbdg;		// ->type for edges
	int				stbdg;		// ->subtype for edges

	double			dnod;		// ->dist max for nodes
	double			dbdg;		// ->dist max for edges

	bool			autoclean;	// ->autoclean status
}netcleancfg_prm;

typedef struct netcleanact_prm{
	bool	create_nodes;
	bool	cut_edges;
	bool	join_on_nodes;
	bool	join_on_edges;
	bool	check_nodes;
	bool	check_edges;
}netcleanact_prm;

// Flags
enum{
	_kNodeCreated	=1,
	_kNodeJoined	=2,
	_kNodeAlone		=3,
	_kNodeDoubled	=4,
	_kNodeDeadEnd	=6,
	_kNodeNot3		=7
};

enum{
	_kEdgeCreated	=1,
	_kEdgeJoined	=2,
	_kEdgeRing		=3,
	_kEdgeDeadEnd	=4,
	_kEdgeAlone		=5,
	_kEdgeNotJoined	=6
};

//----------------------------------------------------------------------------
// TopoCheck
//------------
typedef struct topocheck_prm{
	bArray*			types;		// ->array of bGenericType
	bool			activated;	// ->status
}topocheck_prm;

//----------------------------------------------------------------------------
// Intersection
//------------
typedef bGenericType* intersection_prm;

//----------------------------------------------------------------------------
// Buffer
//------------
typedef struct buffer_prm{
	bGenericType*	tp;			// ->type for buffer creation
	double			d;			// ->size of buffer
}buffer_prm;

// ---------------------------------------------------------------------------
// RastersExports
// ------------
typedef struct raster_export_prm{
	UInt32		codec;				// ->used codec
	char		path[PATH_MAX];		// ->file path
	char		name[FILENAME_MAX];	// ->file name
	int			nh;					// ->nb columns
	int			nv;					// ->nb rows
	int			q;					// ->compression quality
	int			r;					// ->resolution quality
	int			gr;					// ->georeferencing informations (0/1)
	double		mrg;				// ->search margin
}raster_export_prm;

// ---------------------------------------------------------------------------
// FExp
// ------------
typedef struct fexp_prm{
	int			tidx;			// ->type index
	char*		name;			// ->param name (can be NULL) unused actually
	bArray*		flds;			// <->array of fexp_field
	int			srid;			// <-pojection srid
}fexp_prm;

typedef struct fexp_field{
	int			fid;			// ->field ID
	char		fname[256];		// ->field Name for export
}fexp_field;


// ---------------------------------------------------------------------------
// CloneDetector
// ------------
typedef struct clonedetector_prm{
	bool	name;
	bool	subtype;
	bool	color;
	bool	direction;
	bool	reversegeom;
	bool	userfields;
}clonedetector_prm;

//----------------------------------------------------------------------------
// Field Export
//------------
typedef struct fieldexport_prm{
	bGenericType*	tp;
	char			path[PATH_MAX];
	char			name[FILENAME_MAX];
}fieldexport_prm;

//----------------------------------------------------------------------------
// Route Manager
//------------
enum{	
	kXMapRouteManagerHiliteLinks		=1,
	kXMapRouteManagerHiliteNodes		=2,
	kXMapRouteManagerHiliteAll			=3	
};

typedef struct routemgr_prm{
	bool			autobuild;
	int 			hmode;
}routemgr_prm;

//----------------------------------------------------------------------------
// String Processing
//------------
enum{
	kStrProcessCaseDontChange	=1,
	kStrProcessCaseLower		=2,
	kStrProcessCaseUpper		=3,
	kStrProcessCaseFUpper		=4,
	kStrProcessCaseFWUpper		=5
};

typedef struct strprocess_prm{
	int				tp;
	bArray*			arr;
	char			sep[256];
	int				cas;
	char			srch[256];
	char			repl[256];
	char			fmt[256];
	int				target;
}strprocess_prm;

//----------------------------------------------------------------------------
// Sortcut
//------------
typedef struct shortcut_prm{
	UInt32	xtrn;
	UInt32	modi;
	UInt32	key;
	char	code;
	bool	inmenu;
	char	name[128];
}shortcut_prm;

//----------------------------------------------------------------------------
// MakeConstraints
//------------
typedef struct MakeConstraints_prm{
    bGenericType*	tp;
    int				field;
    int				srcfield;
    int				srcuse;
}MakeConstraints_prm;

enum{
    kMakeConstraintsUseValue=1,
    kMakeConstraintsUseLabel=2
};

//----------------------------------------------------------------------------



#endif
