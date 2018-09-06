//----------------------------------------------------------------------------
// File : bXMapGroup.h
// Project : MacMap
// Purpose : Header file : XMap class, group geometries of selected object in a complex geometry, then assing it to the first object and destroy others
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
// 09/12/2008 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapGroup__
#define __bXMapGroup__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapGroupSignature			'xGrp'
#define	kXMapGroupMessageID			"Group"
#define kXMapGroupProgressMessageID	"GroupProgress"

//----------------------------------------------------------------------------

class bXMapGroup : public bStdXMap{
	public:		
		bXMapGroup								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapGroup						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		virtual bool test						(	void* prm);
				
	protected:		
		virtual bool group						(	);
		virtual bool group						(	bool kill,
													bool silent);
	private:

};

//----------------------------------------------------------------------------

#endif
