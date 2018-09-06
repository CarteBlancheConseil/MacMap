//----------------------------------------------------------------------------
// File : bGenericStyleMgr.h
// Project : MacMap
// Purpose : Header file : Root class for styles manager
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
// 04/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericStyleMgr__
#define __bGenericStyleMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericXMLBaseElement.h>

//----------------------------------------------------------------------------

class bGenericStyleMgr : public bGenericMgr{
	public:
		virtual ~bGenericStyleMgr					(	){};	
		virtual int count							(	)=0;
		
		virtual bool add							(	bGenericXMLBaseElement* root)=0;
		virtual bool duplicate						(	int idx)=0;
		virtual bool remove							(	int idx)=0;
		
		virtual void get_name						(	int idx, 
														char* name)=0;
		virtual bool set_name						(	int idx, 
														const char* name)=0;
		virtual int signature						(	int idx)=0;

		virtual bool save							(	int idx)=0;
		virtual bGenericXMLBaseElement* get_root	(	int idx)=0;
		virtual void set_root						(	int idx, 
														bGenericXMLBaseElement* root)=0;

		virtual int index							(	const char* name)=0;
		virtual int index							(	const char* file_name, 
														bool dummy)=0;
		virtual int index							(	bGenericXMLBaseElement* root)=0;
		
		virtual bool edit							(	int idx,
														int signature)=0;
		virtual bool create							(	int signature)=0;
	
		virtual void set_on_screen					(	int idx, 
														bool b)=0;
		virtual bool get_on_screen					(	int idx)=0;

		virtual void set_on_edit					(	int idx, 
														bool b)=0;
		virtual bool get_on_edit					(	int idx)=0;
	
		virtual bool make							(	)=0;
};

//----------------------------------------------------------------------------

#endif
