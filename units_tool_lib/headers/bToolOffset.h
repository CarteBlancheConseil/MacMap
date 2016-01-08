//----------------------------------------------------------------------------
// File : bToolOffset.h
// Project : MacMap
// Purpose : Header file : Object modification (offset by field) tool class
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
// 29/11/2006 creation.
//----------------------------------------------------------------------------
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolOffset__
#define __bToolOffset__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolPres.h>

//----------------------------------------------------------------------------

#define	kOffsetMessageID	"offset"

//----------------------------------------------------------------------------

class bToolOffset : public bStdToolPres{
public:		
	bToolOffset								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolOffset					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void drag						(	CGPoint pt);
	
protected:

private:
	int				_fx;
	int				_fy;
	d2dvertex		_orig;
	bGenericStyle*	_styl;
};

//----------------------------------------------------------------------------

#endif
