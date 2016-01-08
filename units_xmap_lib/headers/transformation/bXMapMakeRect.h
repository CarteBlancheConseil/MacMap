//----------------------------------------------------------------------------
// File : bXMapMakeRect.h
// Project : MacMap
// Purpose : Header file : XMap class, create new rectangle
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
// 15/03/2006 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapMakeRect__
#define __bXMapMakeRect__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapMakeRectMessageID	"MakeRect"

enum{	
	kXMapMakeRectSignature		='xMkR',
	kXMapMakeRectXID			=1,
	kXMapMakeRectYID			=2,
	kXMapMakeRectWID			=3,
	kXMapMakeRectHID			=4,
	kXMapMakeRectXUnitID		=5,
	kXMapMakeRectYUnitID		=6,
	kXMapMakeRectWUnitID		=7,
	kXMapMakeRectHUnitID		=8	
};

//----------------------------------------------------------------------------

class bXMapMakeRect : public bStdXMapWithModalIntf{
	public:		
		bXMapMakeRect							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapMakeRect					(	);
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
		virtual bool make						(	bGenericType* tp, 
													d2dvertex* o, 
													double w, 
													double h,
													int sui,
													int justo);
		virtual void puts						(	);
		
		makerect_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
