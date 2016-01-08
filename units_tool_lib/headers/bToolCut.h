//----------------------------------------------------------------------------
// File : bToolCut.h
// Project : MacMap
// Purpose : Header file : Object modification (geometry cut, scissors) tool class
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
// 24/10/2006 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolCut__
#define __bToolCut__

//----------------------------------------------------------------------------

#include <tool_lib/bToolGeomWithJoin.h>

//----------------------------------------------------------------------------

#define	kCutMessageID	"cut"

//----------------------------------------------------------------------------

class bToolCut : public bToolGeomWithJoin{
public:		
	bToolCut								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolCut						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);		
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void update						(	bool global);
	virtual bool set_key					(	int k);
	virtual void track_obj					(	);
	virtual void action						(	int count);
	
	virtual bGenericXMLBaseElement* load	(	);
	bGenericXMLBaseElement* make			(	bArray &arr);

protected:
	virtual void cut_line					(	bGenericGeoElement* o);
	virtual void cut_surf					(	bGenericGeoElement* o);

private:

};

//----------------------------------------------------------------------------

#endif
