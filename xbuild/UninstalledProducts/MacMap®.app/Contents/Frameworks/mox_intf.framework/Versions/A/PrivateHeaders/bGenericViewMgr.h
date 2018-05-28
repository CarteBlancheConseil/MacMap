//----------------------------------------------------------------------------
// File : bGenericViewMgr.h
// Project : MacMap
// Purpose : Header file : Root class for views managers
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
// 02/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericViewMgr__
#define __bGenericViewMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericXMLBaseElement.h>

//----------------------------------------------------------------------------

class bGenericViewMgr : public bGenericMgr{
	public:		
		virtual ~bGenericViewMgr					(	){};	
		virtual int count							(	)=0;
		virtual int get_current						(	)=0;
		virtual bool set_current					(	int idx)=0;
		
		virtual bool append							(	)=0;
		virtual bool duplicate						(	int idx)=0;
		virtual bool remove							(	int idx)=0;
		
		virtual void get_name						(	int idx, 
														char* name)=0;
		virtual void get_name						(	char* name)=0;
		virtual bool set_name						(	int idx, 
														const char* name)=0;
		virtual bool set_name						(	const char* name)=0;
		virtual bool save							(	int idx)=0;
		virtual bool save							(	)=0;
		virtual bGenericXMLBaseElement* get_root	(	int idx)=0;
		virtual bGenericXMLBaseElement* get_root	(	)=0;
		
		virtual bool make							(	)=0;
};

//----------------------------------------------------------------------------

#endif
