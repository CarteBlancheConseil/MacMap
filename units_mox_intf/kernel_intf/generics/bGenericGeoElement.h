//----------------------------------------------------------------------------
// File : bGenericGeoElement.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap geographic objects
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
// Classe de base objets géographiques
//----------------------------------------------------------------------------
// 20/05/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericGeoElement__
#define __bGenericGeoElement__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <MacMapSuite/vx_lib.h>

//----------------------------------------------------------------------------

class bGenericGeoElement : public bGenericElement{
	public:	
		virtual ~bGenericGeoElement				(	){};

		virtual void kill						(	)=0;
		virtual void unkill						(	)=0;
		virtual bool killed						(	)=0;
		
		virtual bool selected					(	)=0;
		virtual void setselect					(	bool b)=0;
		virtual void setselect					(	bool b, 
													bool unused)=0;
		
		virtual bool contrasted					(	)=0;
		virtual void setcontrast				(	bool b)=0;
		virtual void setcontrast				(	bool b, 
													bool unused)=0;
		
		virtual bool masked						(	)=0;
		virtual void setmasked					(	bool b, 
													bool report)=0;
		
		virtual bool atscreen					(	)=0;
		virtual void setatscreen				(	bool b)=0;
		
		virtual bool needupdate					(	)=0;
		virtual void setneedupdate				(	bool b)=0;
		
		virtual bool handled					(	)=0;
		virtual void sethandled					(	bool b)=0;
		
		virtual int getID						(	)=0;
		virtual int getType						(	)=0;
		virtual int getOffset					(	)=0;
		virtual void getBounds					(	ivx_rect *vxr)=0;
		
		virtual void getVertices				(	ivertices **vxs)=0;
		virtual bool setVertices				(	ivertices *vxs)=0;
				
		virtual int getSubType					(	)=0;
		virtual bool setSubType					(	int	subtype)=0;
		
		virtual int getColor					(	)=0;
		virtual bool setColor					(	int	color)=0;
		
		virtual int getDirection				(	)=0;
		virtual bool setDirection				(	int	direction)=0;
		
		virtual void getName					(	char *name)=0;
		virtual bool setName					(	char *name)=0;
		
		virtual	bool getValue					(	int fieldIdx, 
													void *value)=0;
		virtual bool getValue					(	char *fieldName,
													void *value)=0;
		virtual bool getValue					(	int fieldIdx, 
													char *value)=0;
		virtual bool getValue					(	char *fieldName, 
													char *value)=0;
		virtual bool getValue					(	int fieldIdx, 
													int *value)=0;
		virtual bool getValue					(	char *fieldName, 
													int *value)=0;
		virtual bool getValue					(	int fieldIdx, 
													double *value)=0;
		virtual bool getValue					(	char *fieldName, 
													double *value)=0;

		virtual bool setValue					(	int fieldIdx, 
													void *value)=0;
		virtual bool setValue					(	char *fieldName, 
													void *value)=0;
		virtual bool setValue					(	int fieldIdx, 
													char *value)=0;
		virtual bool setValue					(	char *fieldName, 
													char *value)=0;
		virtual bool setValue					(	int fieldIdx, 
													int value)=0;
		virtual bool setValue					(	char *fieldName, 
													int value)=0;
		virtual bool setValue					(	int fieldIdx, 
													double value)=0;
		virtual bool setValue					(	char *fieldName, 
													double value)=0;

		virtual bool flag1						(	)=0;
		virtual void set_flag1					(	bool b)=0;
		virtual bool flag2						(	)=0;
		virtual void set_flag2					(	bool b)=0;
		virtual bool flag3						(	)=0;
		virtual void set_flag3					(	bool b)=0;
		virtual bool flag4						(	)=0;
		virtual void set_flag4					(	bool b)=0;
		
		virtual bool is_lock					(	)=0;
		virtual void set_lock					(	bool b)=0;
		
		virtual void inval						(	int idx)=0;

		virtual bGenericGeoElement* get_ref		(	)=0;
		
};

//----------------------------------------------------------------------------

#endif
