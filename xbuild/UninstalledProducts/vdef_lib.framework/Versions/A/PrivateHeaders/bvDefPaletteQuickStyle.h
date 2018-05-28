//----------------------------------------------------------------------------
// File : bvDefPaletteQuickStyle.h
// Project : MacMap
// Purpose : Header file : Base subclass for quick (standard) vDef styles
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
// 02/06/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPaletteQuickStyle__
#define __bvDefPaletteQuickStyle__

//----------------------------------------------------------------------------

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/C_Utils.h>

//----------------------------------------------------------------------------

class bvDefPaletteQuickStyle{
	public:
		bvDefPaletteQuickStyle			(	bGenericMacMapApp* gapp,
											bGenericType* gtp,
											int clss);
		virtual ~bvDefPaletteQuickStyle	(	);
		
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
		
	protected:													
		virtual bool std_load			(	bGenericXMLBaseElement* root);
		virtual bool std_dump			(	bArray& arr, 
											int indent);
		virtual bool off_load			(	bGenericXMLBaseElement* root);
		virtual bool off_dump			(	bArray& arr, 
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
