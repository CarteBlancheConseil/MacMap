//----------------------------------------------------------------------------
// File : bGenericLocConverter.h
// Project : MacMap
// Purpose : Header file : Root class for coordinates conversion
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
// 23/11/2004 creation.
// 04/09/2014 suppression des geometries QD.
//----------------------------------------------------------------------------

#ifndef __bGenericLocConverter__
#define __bGenericLocConverter__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <MacMapSuite/vx_lib.h>

//----------------------------------------------------------------------------

enum{
	kTransformationNone,
	kTransformationSimil,
	kTransformationAffinity
};

//----------------------------------------------------------------------------

class bGenericLocConverter : public bGenericElement{
	public:
		virtual ~bGenericLocConverter	(	){};

// Conversion
		virtual bool convert			(	i2dvertex* lt,	
											i2dvertex* lf)=0;
		virtual bool convert			(	i2dvertex* lt,
											d2dvertex* lf)=0;
		virtual bool convert			(	i2dvertex* lt,	
											CGPoint* lf)=0;
		
		virtual bool convert			(	d2dvertex* lt,	
											i2dvertex* lf)=0;
		virtual bool convert			(	d2dvertex* lt,	
											d2dvertex* lf)=0;
		virtual bool convert			(	d2dvertex* lt,	
											CGPoint* lf)=0;

		virtual bool convert			(	CGPoint* lt,	
											i2dvertex* lf)=0;
		virtual bool convert			(	CGPoint* lt,	
											d2dvertex* lf)=0;
		virtual bool convert			(	CGPoint* lt,	
											CGPoint* lf)=0;
		
// Lock Up
		virtual void set_lockup			(	dvertices* fich,	
											dvertices* click,	
											int method)=0;
		virtual void transform			(	i2dvertex* fich,	
											i2dvertex* click)=0;
		virtual void transform			(	d2dvertex* fich,	
											d2dvertex* click)=0;
		virtual void transform			(	CGPoint* fich,
											CGPoint* click)=0;

// Don't touch !
		virtual void init				(	ivx_rect* vxr)=0;
		virtual void reset				(	ivx_rect* vxr)=0;
};

//----------------------------------------------------------------------------

#endif
