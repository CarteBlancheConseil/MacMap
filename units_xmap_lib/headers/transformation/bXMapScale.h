//----------------------------------------------------------------------------
// File : bXMapScale.h
// Project : MacMap
// Purpose : Header file : XMap class, scale selected geometries by factor
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
// 07/11/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapScale__
#define __bXMapScale__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapScaleSignature	'xScl'
#define	kXMapScaleMessageID	"scale"

enum{	
	kXMapScaleCXID			=4,
	kXMapScaleCYID			=6,
	kXMapScalePopKindID		=9,
	kXMapScalePopKindCmd	='PopK',
	kXMapScaleXXID			=10,
	kXMapScaleXID			=11,
	kXMapScaleUnitXID		=12,
	kXMapScaleYYID			=13,
	kXMapScaleYID			=14,
	kXMapScaleUnitYID		=15
};

//----------------------------------------------------------------------------

class bXMapScale : public bStdXMapWithModalIntf{
	public:		
		bXMapScale								(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl);
		virtual ~bXMapScale						();
		virtual bGenericXMLBaseElement* create	(bGenericXMLBaseElement* elt);
		
		virtual void open						(int* flags);
		virtual bool process					(int msg, void* prm);
		virtual bool test						(void* prm);
		
		virtual bool wd_event					(EventRef evt, WindowRef wd);
		virtual void wd_init					(WindowRef wd);
		
	protected:		
		
	private:
		virtual bool scale						(	);
		virtual bool scale						(	double x,
													double y,
													double cx,
													double cy);
		virtual void puts						();
		virtual void center						(	i2dvertex* c);
		virtual void showhide					();
		
		scale_prm	_prm;
		int			_kind;
};

//----------------------------------------------------------------------------

#endif
