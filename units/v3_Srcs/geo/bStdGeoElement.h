//----------------------------------------------------------------------------
// File : bStdGeoElement.h
// Project : MacMap
// Purpose : Header file : Standart MacMap object class
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
// Std geographic elements class
//----------------------------------------------------------------------------
// 18/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdGeoElement__
#define __bStdGeoElement__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------

class bStdGeoElement : public bGenericGeoElement{
	public:
		bStdGeoElement							(	);
		virtual ~bStdGeoElement					(	);
		
		virtual void kill						(	);
		virtual void unkill						(	);
		virtual bool killed						(	);
		
		virtual bool selected					(	);
		virtual void setselect					(	bool b);
		virtual void setselect					(	bool b, 
													bool unused);
		
		virtual bool contrasted					(	);
		virtual void setcontrast				(	bool b);
		virtual void setcontrast				(	bool b, 
													bool unused);
		
		virtual bool masked						(	);
		virtual void setmasked					(	bool b, 
													bool report);
		
		virtual bool atscreen					(	);
		virtual void setatscreen				(	bool b);
		
		virtual bool needupdate					(	);
		virtual void setneedupdate				(	bool b);
		
		virtual bool handled					(	);
		virtual void sethandled					(	bool b);

		virtual int getID						(	);
		virtual int getType						(	);
		virtual int getOffset					(	);
		virtual void getBounds					(	ivx_rect	*vxr);
		
		virtual void getVertices				(	ivertices	**vxs);
		virtual bool setVertices				(	ivertices	*vxs);
				
		virtual int getSubType					(	);
		virtual bool setSubType					(	int	subtype);

		virtual int getColor					(	);
		virtual bool setColor					(	int	color);
		
		virtual int getDirection				(	);
		virtual bool setDirection				(	
													int	direction);
		
		virtual void getName					(	char *name);
		virtual bool setName					(	char *name);
				
		virtual bool getValue					(	int fieldIdx, 
													void *value);
		virtual bool getValue					(	char *fieldName, 
													void *value);
		virtual bool getValue					(	int fieldIdx, char *value);
		virtual bool getValue					(	char *fieldName, char *value);
		virtual bool getValue					(	int fieldIdx, int *value);
		virtual bool getValue					(	char *fieldName, int *value);
		virtual bool getValue					(	int fieldIdx, double *value);
		virtual bool getValue					(	char *fieldName, double *value);
		
		virtual bool setValue					(	int fieldIdx, void *value);
		virtual bool setValue					(	char *fieldName, void *value);
		virtual bool setValue					(	int fieldIdx, char *value);
		virtual bool setValue					(	char *fieldName, char *value);
		virtual bool setValue					(	int fieldIdx, int value);
		virtual bool setValue					(	char *fieldName, int value);
		virtual bool setValue					(	int fieldIdx, double value);
		virtual bool setValue					(	char *fieldName, double value);

		virtual bool flag1						(	);
		virtual void set_flag1					(	bool b);
		virtual bool flag2						(	);
		virtual void set_flag2					(	bool b);
		virtual bool flag3						(	);
		virtual void set_flag3					(	bool b);
		virtual bool flag4						(	);
		virtual void set_flag4					(	bool b);
		
		virtual bool is_lock					(	);
		virtual void set_lock					(	bool b);

		virtual void inval						(	int idx);
		
		virtual bGenericGeoElement* get_ref		(	);

	protected:
		int				_offset;

	private:
	
};

//----------------------------------------------------------------------------

#endif
