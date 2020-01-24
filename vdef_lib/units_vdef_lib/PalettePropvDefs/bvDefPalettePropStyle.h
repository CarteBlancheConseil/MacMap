//----------------------------------------------------------------------------
// File : bvDefPalettePropStyle.h
// Project : MacMap
// Purpose : Header file : Base subclass for proportionnal vDef styles
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
// 02/01/2013 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefPalettePropStyle__
#define __bvDefPalettePropStyle__

//----------------------------------------------------------------------------

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/C_Utils.h>

//----------------------------------------------------------------------------

typedef struct color_range64{
	double	bound;
	char	name[256];
	long	clr_spc;
	double	clrs[5];
	char	pat[256];
}color_range64;

//----------------------------------------------------------------------------

class bvDefPalettePropStyle{
	public:
		bvDefPalettePropStyle			(	bGenericMacMapApp* gapp,
											bGenericType* gtp,
											bool size);
		virtual ~bvDefPalettePropStyle	(	);
		
		virtual bool load				(	bGenericXMLBaseElement* root);
		virtual bool dump				(	bArray& arr, 
											int indent);
		virtual void reset				(	); 
		
		int		_fld;
		int		_clss;

		int		_pass;

		double	_smin;
		double	_smax;
		
		int		_offx;
		int		_offy;
		int		_cc;
		
		double	_qref;
		double	_qmax;
		double	_rref;
		double	_exp;

		bool	_sow;

		int		_subfld;
		bArray	_arr;

		double	_color[5];
	
	
	protected:													
		virtual bool std_load			(	bGenericXMLBaseElement* root);
		virtual bool std_dump			(	bArray& arr, 
											int indent);
		virtual bool prop_load			(	bGenericXMLBaseElement* root,
											double* rat);
		virtual bool prop_dump			(	bArray& arr, 
											int indent,
											double rat);
		virtual bool sub_load			(	bGenericXMLBaseElement* root);
		virtual bool sub_dump			(	bArray& arr, 
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
