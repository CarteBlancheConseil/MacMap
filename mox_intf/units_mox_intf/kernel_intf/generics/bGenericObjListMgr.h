//----------------------------------------------------------------------------
// File : bGenericObjListMgr.h
// Project : MacMap
// Purpose : Header file : Root class for objects lists managers (i.e. selection & contrasts)
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
// 30/01/2004 creation.
//----------------------------------------------------------------------------

#ifndef __BGENERICOBJLISTMGR__
#define __BGENERICOBJLISTMGR__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/bGenericGeoElement.h>

//----------------------------------------------------------------------------

class bGenericObjListMgr : public bGenericMgr{
	public:
		virtual ~bGenericObjListMgr			(	){};	
	
		virtual int load					(	)=0;
		virtual int unload					(	)=0;		
		
		virtual long count					(	)=0;
		virtual int getState				(	)=0;
		virtual void incState				(	)=0;
		
		virtual int add						(	bGenericGeoElement* o)=0;
		virtual int add						(	bArray* l)=0;
		virtual int rmv						(	bGenericGeoElement* o)=0;
		
		virtual void flush					(	)=0;
		
		virtual bArray*	elements			(	)=0;
		
		virtual bGenericGeoElement*	next	(	)=0;
		virtual bGenericGeoElement*	prev	(	)=0;
		
		virtual int type_count				(	)=0;
		virtual bool type_unique			(	)=0;
};

//----------------------------------------------------------------------------

#endif
