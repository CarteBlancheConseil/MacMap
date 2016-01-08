//----------------------------------------------------------------------------
// File : bStyleRun.h
// Project : MacMap
// Purpose : Header file : Style param set for a class of object
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
// 06/02/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStyleRun__
#define __bStyleRun__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGenericXMLRenderingElement.h>

//----------------------------------------------------------------------------

class bStyleRun : public bGenericElement{
	public:
		bStyleRun							(	); 
		virtual ~bStyleRun   				(	);
		
		virtual void setroot				(	void* elt);
		virtual void setvalidity			(	void* elt);
		virtual void setrender				(	void* elt);
		virtual void setgeometry			(	void* elt);
		
		virtual void setscalemin			(	double d);
		virtual void setscalemax			(	double d);
		virtual void setclassmin			(	int idx);
		virtual void setclassmax			(	int idx);
		virtual void setpass				(	int idx);
		
		virtual bool good					(	double scale, 
												int clss, 
												int pass);
		
		virtual bool applyforclass			(	bGenericGraphicContext* ctx);
		virtual bool applyforobject			(	bGenericGraphicContext* ctx);
		virtual bool applygeometryforclass	(	bGenericGraphicContext* ctx);
		virtual bool applygeometryforobject	(	bGenericGraphicContext* ctx);
		
		virtual bGenericXMLBaseElement* root(	);
		
	protected:

		
	private:
		bGenericXMLRenderingElement*		_root;
		bGenericXMLRenderingElement*		_validity;
		bGenericXMLRenderingElement*		_render;
		bGenericXMLRenderingElement*		_geometry;
		
		double								_scalemin;
		double								_scalemax;
		int									_classmin;
		int									_classmax;
		int									_pass;
		
};

//----------------------------------------------------------------------------

#endif
