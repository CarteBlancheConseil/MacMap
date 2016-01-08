//----------------------------------------------------------------------------
// File : bXMapLineUp.h
// Project : MacMap
// Purpose : Header file : XMap class, order selected points into columns and lines
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
// 16/11/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapLineUp__
#define __bXMapLineUp__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapLineUpMessageID	"lineup"

enum{	
	kXMapLineUpSignature		='xLUp',
	kXMapLineUpPopKindID		=10,
	kXMapLineUpNbObjectLegID	=11,
	kXMapLineUpNbObjectID		=12,
	kXMapLineUpHDistID			=21,
	kXMapLineUpHDistLegID		=22,
	kXMapLineUpVDistID			=24,
	kXMapLineUpVDistLegID		=25,
	kXMapLineUpUseSurfID		=3,
	kXMapLineUpUseSurfCmd		='UseS'
};

enum{	
	kXMapLineUpKindLine			=1,
	kXMapLineUpKindColumn		=2
};

//----------------------------------------------------------------------------

class bXMapLineUp : public bStdXMapWithModalIntf{
	public:		
		bXMapLineUp								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapLineUp					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:		
		virtual void make						(	);
		
	private:
		int		_kind;
		int		_nb;
		double	_hd;
		double	_vd;
		bool	_use_surf;
};

//----------------------------------------------------------------------------

#endif
