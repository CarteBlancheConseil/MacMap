//----------------------------------------------------------------------------
// File : bXMapAgregateIntoLine.h
// Project : MacMap
// Purpose : Header file : XMap class, make new polyline from selected objects geometry
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
// 20/03/2006 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapAgregateIntoLine__
#define __bXMapAgregateIntoLine__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

#define kXMapAgregateIntoLineSignature	'xAgL'
#define	kXMapAgregateIntoLineMessageID	"AgregateIntoLine"

//----------------------------------------------------------------------------

class bXMapAgregateIntoLine : public bStdXMap{
	public:		
		bXMapAgregateIntoLine					(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapAgregateIntoLine			(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);
		
	protected:		
		
	private:
		virtual void make						(	);
		virtual void make						(	bGenericType* tp);
};

//----------------------------------------------------------------------------

#endif
