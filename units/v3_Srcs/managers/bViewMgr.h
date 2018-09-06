//----------------------------------------------------------------------------
// File : bViewMgr.h
// Project : MacMap
// Purpose : Header file : Views management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 02/02/2005 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bViewMgr__
#define __bViewMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericViewMgr.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bViewMgr : public bGenericViewMgr{
	public:
		bViewMgr									(	);
		virtual ~bViewMgr							(	);
		
		virtual int load							(	);
		virtual int unload							(	);

		virtual int count							(	);
		virtual int get_current						(	);
		virtual bool set_current					(	int idx);
		
		virtual bool append							(	);
		virtual bool duplicate						(	int idx);
		virtual bool remove							(	int idx);
		
		virtual void get_name						(	int idx, 
														char* name);
		virtual void get_name						(	char* name);
		virtual bool set_name						(	int idx, 
														const char* name);
		virtual bool set_name						(	const char* name);
		virtual bool save							(	int idx);
		virtual bool save							(	);
		virtual bGenericXMLBaseElement* get_root	(	int idx);
		virtual bGenericXMLBaseElement* get_root	(	);
		virtual bool make							(	);
	protected:
	
	private:
		bool dump									(	);
		int find									(	const char* name);
		static int name_compare						(	const void* a,	
														const void* b);
		
		int		_cur;
		bArray	_elts;
};


//----------------------------------------------------------------------------

#endif
