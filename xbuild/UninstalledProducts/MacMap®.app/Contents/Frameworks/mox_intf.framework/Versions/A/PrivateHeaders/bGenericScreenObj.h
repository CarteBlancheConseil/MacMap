//----------------------------------------------------------------------------
// File : bGenericScreenObj.h
// Project : MacMap
// Purpose : Header file : Root class for screen objects
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

#ifndef __bGenericScreenObj__
#define __bGenericScreenObj__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericGeoElement.h>

//----------------------------------------------------------------------------

class bGenericScreenObj : public bGenericElement{
	public:
		virtual ~bGenericScreenObj					(	){};	
		virtual void setreference					(	bGenericGeoElement* o)=0;
		virtual void setvalue						(	double d)=0;
		virtual void setclass						(	int k)=0;
		virtual void setbounds						(	float xmin,
														float ymin,
														float xmax,
														float ymax)=0;
		virtual void setbounds						(	float* x,
														float* y,
														int n,
														float hi,
														float vi,
														bool reset)=0;
		virtual bGenericGeoElement* getreference	(	)=0;
		virtual double getvalue						(	)=0;
		virtual int getclass						(	)=0;
		virtual void getbounds						(	float* xmin,
														float* ymin,
														float* xmax,
														float* ymax)=0;
			
		virtual bool visible						(	)=0;
		
		virtual void setflag						(	bool flg)=0;
		virtual bool flag							(	)=0;
};

//----------------------------------------------------------------------------

#endif
