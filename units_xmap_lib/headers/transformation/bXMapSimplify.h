//----------------------------------------------------------------------------
// File : bXMapSimplify.h
// Project : MacMap
// Purpose : Header file : XMap class, simplify selected geometries
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
// 08/11/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapSimplify__
#define __bXMapSimplify__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapSimplifySignature	'xSim'
#define	kXMapSimplifyMessageID	"Simplify"

enum{	
	kXMapSimplifyID			=5,
	kXMapSimplifyUnitID		=6
};

//----------------------------------------------------------------------------

class bXMapSimplify : public bStdXMapWithModalIntf{
	public:		
		bXMapSimplify							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapSimplify					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		
	private:
		virtual bool simplify					(	);
		virtual bool simplify					(	double a);
		virtual void puts						(	);
		
		double		_a;
};

//----------------------------------------------------------------------------

#define	kXMapSimplifyToDeathSignature	'xSiD'
#define	kXMapSimplifyToDeathMessageID	"SimplifyToDeath"

//----------------------------------------------------------------------------

class bXMapSimplifyToDeath : public bStdXMap{
	public:		
		bXMapSimplifyToDeath					(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapSimplifyToDeath			(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);
		
	protected:		
		
	private:
		virtual bool simplify					(	);
		virtual bool simplify					(	double a);
		
		double		_d;

};

//----------------------------------------------------------------------------

#endif
