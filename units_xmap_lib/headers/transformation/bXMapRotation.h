//----------------------------------------------------------------------------
// File : bXMapRotation.h
// Project : MacMap
// Purpose : Header file : XMap class, rotate selected geometries with angle
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

#ifndef __bXMapRotation__
#define __bXMapRotation__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#define	kXMapRotationSignature	'xRot'
#define	kXMapRotationMessageID	"rotation"

enum{	
	kXMapRotationDegID			=4,
	kXMapRotationMinID			=6,
	kXMapRotationPopDirID		=8,
	kXMapRotationPopKindID		=9,
	kXMapRotationPopKindCmd		='PopK',
	kXMapRotationXXID			=10,
	kXMapRotationXID			=11,
	kXMapRotationUnitXID		=12,
	kXMapRotationYYID			=13,
	kXMapRotationYID			=14,
	kXMapRotationUnitYID		=15
};

//----------------------------------------------------------------------------

class bXMapRotation : public bStdXMapWithModalIntf{
	public:		
		bXMapRotation							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapRotation					(	);
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
		virtual bool rotate						(	);
		virtual bool rotate						(	double x,
													double y,
													double rad);
		virtual void puts						(	);
		virtual void center						(	i2dvertex* c);
		virtual void showhide					(	);
		
		rotation_prm	_prm;
		int				_dir;
		int				_kind;
};

//----------------------------------------------------------------------------

#endif
