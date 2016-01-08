//----------------------------------------------------------------------------
// File : bXMapIGrid.h
// Project : MacMap
// Purpose : Header file : XMap class, import grid and projection (type dispatch)
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
// 24/11/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapIGrid__
#define __bXMapIGrid__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

enum{	
	kXMapIGridSignature		='IGrd',
	kXMapIGridProjID		=7,
	kXMapIGridProjCmd		='Proj',
	kXMapIGridProjNameID	=8
};

//----------------------------------------------------------------------------

class bXMapIGrid : public bStdXMapWithModalIntf{
	public:		
		bXMapIGrid								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapIGrid						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool process					(	int msg, 
													void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		void proj_action						(	);
	
	private:		
		virtual void put_p						(	);		
		virtual void read_p						(	);
		virtual void write_p					(	);
		virtual bGenericXMLBaseElement*	make_p	(	);
		
		import_grid	_prm;
};

//----------------------------------------------------------------------------

#endif
