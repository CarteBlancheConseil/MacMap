//----------------------------------------------------------------------------
// File : bToolAlter.h
// Project : MacMap
// Purpose : Header file : Object modification (geometry) tool class
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
// 30/03/2006 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolAlter__
#define __bToolAlter__

//----------------------------------------------------------------------------

#include <tool_lib/bToolGeomWithJoin.h>

//----------------------------------------------------------------------------

#define	kAlterMessageID	"alter"

enum{
	kToolAlterMovePoint		=1,
	kToolAlterAddPoint		=2,
	kToolAlterRemovePoint	=3
};

//----------------------------------------------------------------------------

class bToolAlter : public bToolGeomWithJoin{
public:		
	bToolAlter								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolAlter						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);

	virtual void update						(	bool global);
	virtual bool set_key					(	int k);
	virtual void set_modifiers				(	int k);
	virtual void drag						(	CGPoint pt);

	virtual void track_obj					(	);
	
	virtual bGenericXMLBaseElement* load	(	);
	bGenericXMLBaseElement* make			(	bArray &arr);

protected:
	virtual int find_vx						(	);
	virtual int insert_vx					(	i2dvertex* vx);
	virtual void put_vxs					(	);
	virtual void remove						(	int idx,
												bool all);// all for polylines

private:
	ivertices*		_vxs;
	int				_mode;
	int				_kind;
	int				_idx;
	CursorRef		_cadd;
	CursorRef		_crmv;
};

//----------------------------------------------------------------------------

#endif
