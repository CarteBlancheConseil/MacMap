//----------------------------------------------------------------------------
// File : bScaleMgr.h
// Project : MacMap
// Purpose : Header file : Scale units manager class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 08/01/2004 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bScaleMgr__
#define __bScaleMgr__

//----------------------------------------------------------------------------

#include "bStdUnitMgr.h"

//----------------------------------------------------------------------------

class bScaleMgr : public bStdUnitMgr{
	public:
	 	bScaleMgr 						(	);
		virtual ~bScaleMgr  			(	);
		
		virtual bStdUnit* allocator  	(	);	
			
		virtual int load				(	);
		virtual int unload				(	);
		
		virtual bool set_current		(	int idx);
		
		virtual void set				(	int idx, 
											const char* name, 
											double coef);
		virtual bool add				(	const char* name, 
											double coef);
		virtual bool rmv				(	int idx);
		virtual bool make				(	);
};

//----------------------------------------------------------------------------

#endif

