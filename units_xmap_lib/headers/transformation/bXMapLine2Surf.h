//----------------------------------------------------------------------------
// File : bXMapLine2Surf.h
// Project : MacMap
// Purpose : Header file : XMap class, create polygon from polylines
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

#ifndef __bXMapLine2Surf__
#define __bXMapLine2Surf__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapLine2SurfMessageID	"line2surf"

enum{	
	kXMapLine2SurfSignature			='xL2S',
	kXMapLine2SurfPopLineID			=1,
	kXMapLine2SurfPopSurfCmd		='PopS',
	kXMapLine2SurfPopSurfID			=2,
	kXMapLine2SurfPopSubtypeID		=3
};

//----------------------------------------------------------------------------

class bXMapLine2Surf : public bStdXMapWithModalIntf{
	public:		
		bXMapLine2Surf							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapLine2Surf					(	);
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
		bGenericType*	_tl;
		bGenericType*	_ts;
		int				_stp;
};

//----------------------------------------------------------------------------

#endif
