//----------------------------------------------------------------------------
// File : bToolMove.h
// Project : MacMap
// Purpose : Header file : Object modification (geometry translation) tool class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 16/08/2005 creation.
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolMove__
#define __bToolMove__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolGeom.h>

//----------------------------------------------------------------------------

#define	kMoveMessageID	"move"

//----------------------------------------------------------------------------

class bToolMove : public bStdToolGeom{
public:		
	bToolMove								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolMove						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void set_modifiers				(	int k);
	virtual void drag						(	CGPoint pt);

	virtual void track_obj					(	);
	virtual void report						(	);

	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);
		
protected:
	void move								(	);
	
private:
	ivertices*		_vxs;
	CursorRef		_ccopy;
	bool			_copy;
	bool			_srep;
};

//----------------------------------------------------------------------------

#endif
