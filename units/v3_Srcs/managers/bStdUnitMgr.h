//----------------------------------------------------------------------------
// File : bStdUnitMgr.h
// Project : MacMap
// Purpose : Header file : Base class for units manager
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
// 24/11/2004 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bStdUnitMgr__
#define __bStdUnitMgr__

//----------------------------------------------------------------------------

#include "bStdUnit.h"
#include <mox_intf/bGenericUnitMgr.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bStdUnitMgr : public bGenericUnitMgr{
	public:		
		bStdUnitMgr					(	);
		virtual ~bStdUnitMgr		(	);
		
		virtual bStdUnit* allocator (	);	
		
		virtual int load			(	);
		virtual int unload			(	);
		virtual int app_load		(	);
		virtual int app_unload		(	);
		
		virtual int get_current		(	);
		virtual bool set_current	(	int idx);
		
		virtual int count			(	);
		
		virtual bGenericUnit* get	(	);
		virtual bGenericUnit* get	(	int idx);
		
		virtual void set			(	int idx, 
										const char* name, 
										double coef);
		
		virtual bool add			(	const char* name, 
										double coef);
		virtual bool rmv			(	int idx);
		virtual bool make			(	);

	protected:
		virtual int pth_load		(	const char* path);
		virtual int apppth_load		(	const char* path);
		virtual int pth_unload		(	const char* path, 
										bool dispose);
		virtual bool make			(	const char* fpath, 
										const char* fname);
		virtual void sort			(	int(*compare)(const void*,const void*));
		
	private:
		virtual void reset			(	);	
		static bool _load			(	bGenericXMLBaseElement *elt, 
										void *prm,
										int indent);
		static bool _current		(	bGenericXMLBaseElement *elt, 
										void *prm,
										int indent);
		
		bArray		_elts;
		int			_cur;
};

//----------------------------------------------------------------------------

#endif
