//----------------------------------------------------------------------------
// File : bXMapMakeSquaring.h
// Project : MacMap
// Purpose : Header file : XMap class, create new squaring (graticule)
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
// 21/06/2007 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapMakeSquaring__
#define __bXMapMakeSquaring__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapMakeSquaringMessageID	"MakeSquaring"

enum{	
	kXMapMakeSquaringSignature		='xMkS',
	kXMapMakeSquaringXID			=1,
	kXMapMakeSquaringYID			=2,
	kXMapMakeSquaringNHID			=3,
	kXMapMakeSquaringNVID			=4,
	kXMapMakeSquaringSizeID			=5,
	kXMapMakeSquaringKindID			=6,
	
	kXMapMakeSquaringXUnitID		=7,
	kXMapMakeSquaringYUnitID		=8,
	kXMapMakeSquaringSzUnitID		=9
};

//----------------------------------------------------------------------------

class bXMapMakeSquaring : public bStdXMapWithModalIntf{
	public:		
		bXMapMakeSquaring						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapMakeSquaring				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		
	private:
		virtual bool make						(	);
		virtual bool make_sqr					(	bGenericType* tp, 
													d2dvertex* o, 
													int nh, 
													int nv,
													double sz);
		virtual bool make_hex					(	bGenericType* tp, 
													d2dvertex* o, 
													int nh, 
													int nv,
													double sz);
		virtual void puts						(	);
		
		makesquaring_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
