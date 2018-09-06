//----------------------------------------------------------------------------
// File : bvDefPaletteRangeStyle.h
// Project : MacMap
// Purpose : Header file : Base subclass for range vDef styles
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
// 20/08/2011 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPaletteRangeStyle__
#define __bvDefPaletteRangeStyle__

//----------------------------------------------------------------------------

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/C_Utils.h>

//----------------------------------------------------------------------------

class bvDefPaletteRangeStyle{
	public:
		bvDefPaletteRangeStyle			(	bGenericMacMapApp* gapp,
											bGenericType* gtp,
											int clss);
		bvDefPaletteRangeStyle			(	bvDefPaletteRangeStyle* stl,
											int clss);
		virtual ~bvDefPaletteRangeStyle	(	);
		
		virtual bool load				(	bGenericXMLBaseElement* root);
		virtual bool dump				(	bArray& arr, 
											int indent);
		
		int		_clss;

		int		_pass;

		double	_smin;
		double	_smax;
		
		int		_offx;
		int		_offy;
		int		_cc;
		
		char	_fld[256];
		double	_vmin;
		double	_vmax;
		double	_fcolor[5];
		double	_tcolor[5];
		bool	_visible;
		
	protected:													
		virtual bool std_load			(	bGenericXMLBaseElement* root);
		virtual bool std_dump			(	bArray& arr, 
											int indent);
		virtual bool off_load			(	bGenericXMLBaseElement* root);
		virtual bool off_dump			(	bArray& arr, 
											int indent);
		virtual bool color_load			(	bGenericXMLBaseElement* root,
											double* clr);
		virtual bool color_dump			(	bArray& arr, 
											int indent,
											double* clr);
		virtual bool prop_load			(	bGenericXMLBaseElement* root);
		virtual bool prop_dump			(	bArray& arr, 
											int indent);
		
		virtual void find_stylerun		(	bGenericXMLBaseElement* root);
		
		bGenericXMLBaseElement*	_gstl;
		bGenericMacMapApp*		_gapp;
		bGenericType*			_gtp;
		
	private:
		static bool test_stylerun		(	bGenericXMLBaseElement *root, 
											void *prm,
											int indent);
};

//----------------------------------------------------------------------------

#endif
