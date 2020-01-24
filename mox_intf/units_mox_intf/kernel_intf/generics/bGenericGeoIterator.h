//----------------------------------------------------------------------------
// File : bGenericGeoIterator.h
// Project : MacMap
// Purpose : Header file : Root class for geographic objects indexation
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
// 13/01/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericGeoIterator__
#define __bGenericGeoIterator__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/vx.h>

//----------------------------------------------------------------------------

typedef struct gi_init_p{
	ivx_rect*	bounds;
	int*		status;
	void		(*set_flag)(void*,bool);
	bool		(*get_flag)(void*);
	void		(*get_bounds)(void*,ivx_rect*);
	int			(*obj_comp)(const void*,const void*);
}gi_init_p;

//----------------------------------------------------------------------------

class bGenericGeoIterator : public bGenericElement{
	public:		
		virtual ~bGenericGeoIterator			(	){};
		virtual bool add						(	void* o)=0;
		virtual bool rmv						(	void* o)=0;	
		
		virtual void flush						(	)=0;
		
		virtual int	iterate						(	void *prm,
													int (*process)(void*,void*))=0;
		virtual int	iterate						(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*))=0;											
		virtual int	iterate						(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*))=0;
		
		virtual int	iteratenear					(	void *prm,
													int (*process)(void*,void*))=0;
		virtual int	iteratenear					(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*))=0;											
		virtual int	iteratenear					(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*))=0;	
		
		virtual int	iteratefar					(	void *prm,
													int (*process)(void*,void*))=0;
		virtual int	iteratefar					(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*))=0;											
		virtual int	iteratefar					(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*))=0;	

		virtual void start						(	)=0;
													
		virtual void bounds						(	ivx_rect* r)=0;
		virtual void set_bounds					(	ivx_rect* r)=0;
		
		virtual void objs_in_rect				(	bArray** arr, 
													ivx_rect* r)=0;
		
		virtual int signature					(	)=0;
};

//----------------------------------------------------------------------------

#endif


