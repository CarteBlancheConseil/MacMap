//----------------------------------------------------------------------------
// File : bXMapAssociation.h
// Project : MacMap
// Purpose : Header file : XMap class, concatenate geometries into the first object of selection
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
// 16/12/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapAssociation__
#define __bXMapAssociation__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapAssociationSignature				'xAso'
#define	kXMapAssociationMessageID				"Association"
#define	kXMapAssociationIndexErr				"IndexErr"
#define	kXMapAssociationCloneObjectErr			"CloneObjectErr"
#define	kXMapAssociationSetVerticesErr			"SetVerticesErr"
#define	kXMapAssociationVerticesErr				"VerticesErr"
#define	kXMapAssociationObjNotAssociateWarning	"ObjNotAssociatedWarning"

enum{
	kXMapAssociationDistID	=4,
	kXMapAssociationUnitID	=5,
	kXMapAssociationKillID	=6
};

//----------------------------------------------------------------------------

class bXMapAssociation : public bStdXMapWithModalIntf{
	public:		
		bXMapAssociation						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapAssociation				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		virtual bool associate					(	);
		virtual bool associate					(	double dmax, 
													bool kill);

		virtual bool associate_points			(	double dmax, 
													bool kill);
		virtual bool associate_polylines		(	double dmax, 
													bool kill);
		virtual bool associate_texts			(	double dmax, 
													bool kill);
		virtual bool associate_polygons			(	double dmax, 
													bool kill);
		virtual bool associate_rasters			(	double dmax, 
													bool kill);

		virtual void clean						(	bArray& arr);
		virtual void puts						(	);

	private:
		virtual int get_lines					(	ivertices* vxsa,
													int loc,
													double dmax,
													bGenericGeoIterator* iter,
													bArray& sel,
													bGenericGeoElement** ob);
		virtual int get_surfs					(	ivertices* vxsa,
													double dmax,
													bGenericGeoIterator* iter,
													bArray& sel,
													bGenericGeoElement** ob);

		static int check_line_dist				(	void *o, 
													void *prm);
		static int check_surf_dist				(	void *o, 
													void *prm);
		association_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
