//----------------------------------------------------------------------------
// File : bXMapTranslation.h
// Project : MacMap
// Purpose : Header file : XMap class, translate selected geometries in x and y
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
// 19/10/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapTranslation__
#define __bXMapTranslation__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapTranslationSignature	'xMov'
#define	kXMapTranslationMessageID	"translation"

enum{	
	kXMapTranslationPopKindID		=1,
	kXMapTranslationPopKindCmd		='kPop',
	kXMapTranslationUnitID			=3,
	kXMapTranslationXID				=5,
	kXMapTranslationYID				=7
};

//----------------------------------------------------------------------------

class bXMapTranslation : public bStdXMapWithModalIntf{
	public:		
		bXMapTranslation						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp,
													CFBundleRef bndl);
		virtual ~bXMapTranslation				(	);
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
		virtual bool translation_with_offset	(	);
		virtual bool translation_with_offset	(	double dx, 
													double dy);
		virtual bool translation_to_coord		(	);
		virtual bool translation_to_coord		(	double x, 
													double y);
		virtual void puts						(	);
		
		translation_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
