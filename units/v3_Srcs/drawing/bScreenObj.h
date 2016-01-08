//----------------------------------------------------------------------------
// File : bScreenObj.h
// Project : MacMap
// Purpose : Header file : Screen object class (i.e. object appearance for a style)
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

#ifndef __BSCREENOBJCLASSE__
#define __BSCREENOBJCLASSE__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericScreenObj.h>
#include <mox_intf/bGenericGeoElement.h>

//----------------------------------------------------------------------------

class bScreenObj : public bGenericScreenObj{
	public:
		bScreenObj									(	); 
		virtual ~bScreenObj							(	);
		
		virtual void setreference					(	bGenericGeoElement* o);
		virtual void setvalue						(	double d);
		virtual void setclass						(	int k);
		virtual void setbounds						(	float xmin,
														float ymin,
														float xmax,
														float ymax);
		virtual void setbounds						(	float* x,
														float* y,
														int n,
														float hi,
														float vi,
														bool reset);
		virtual bGenericGeoElement* getreference	(	);
		virtual double getvalue						(	);
		virtual int getclass						(	);
		virtual void getbounds						(	float* xmin,
														float* ymin,
														float* xmax,
														float* ymax);
			
		virtual bool visible						(	);
		
		virtual void setflag						(	bool flg);
		virtual bool flag							(	);
		
	protected:
		
	private:
		bGenericGeoElement*	_obj;
		double				_val;
		int					_clss;
		float				_xmin;
		float				_ymin;
		float				_xmax;
		float				_ymax;
		float				_hins;
		float				_vins;
		bool				_flg;
};

//----------------------------------------------------------------------------

#endif
