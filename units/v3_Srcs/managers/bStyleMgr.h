//----------------------------------------------------------------------------
// File : bStyleMgr.h
// Project : MacMap
// Purpose : Header file : Styles management class
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
// 07/02/2005 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bStdStyleMgr__
#define __bStdStyleMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericStyleMgr.h>
#include <mox_intf/bGenericType.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bStdStyleMgr : public bGenericStyleMgr{
	public:		
		bStdStyleMgr								(	int *status);
		virtual ~bStdStyleMgr						(	);
		
		virtual int load							(	);
		virtual int unload							(	);
		virtual int count							(	);

		virtual bool add							(	bGenericXMLBaseElement* root);
		virtual bool duplicate						(	int idx);
		virtual bool remove							(	int idx);
		
		virtual void get_name						(	int idx, 
														char* name);
		virtual bool set_name						(	int idx, 
														const char* name);
		virtual int signature						(	int idx);
		virtual bool save							(	int idx);
		virtual bGenericXMLBaseElement* get_root	(	int idx);
		virtual void set_root						(	int idx, 
														bGenericXMLBaseElement* root);

		virtual int index							(	const char* name);
		virtual int index							(	const char* file_name, 
														bool dummy);
		virtual int index							(	bGenericXMLBaseElement* root);
		
		virtual bool edit							(	int idx,
														int signature);
		virtual bool create							(	int signature);
		
		virtual void set_on_screen					(	int idx, 
														bool b);
		virtual bool get_on_screen					(	int idx);
	
		virtual void set_on_edit					(	int idx, 
														bool b);
		virtual bool get_on_edit					(	int idx);
	
		virtual bool make							(	);

	protected:
		bArray			_elts;
	
	private:
		static int name_compare						(	const void* a, 
														const void* b);
		static int fname_compare					(	const void* a, 
														const void* b);
		static int root_compare						(	const void* a, 
														const void* b);
		
};

//----------------------------------------------------------------------------

class bStyleMgr : public bStdStyleMgr{
	public:		
		bStyleMgr									(	bGenericType* tp, 
														int *status);
		virtual ~bStyleMgr							(	);
		
		virtual int load							(	);
		virtual bool add							(	bGenericXMLBaseElement* root);
		virtual bool duplicate						(	int idx);
		virtual bool remove							(	int idx);

		virtual bool edit							(	int idx,
														int signature);
		virtual bool create							(	int signature);

	protected:
	
	private:
		bGenericType*	_tp;
};

//----------------------------------------------------------------------------

class bVirtualStyleMgr : public bStdStyleMgr{
	public:		
		bVirtualStyleMgr							(	int *status);
		virtual ~bVirtualStyleMgr					(	);
		
		virtual int load							(	);
		virtual bool add							(	bGenericXMLBaseElement* root);
		virtual bool duplicate						(	int idx);
		virtual bool remove							(	int idx);

		virtual bool edit							(	int idx,
														int signature);
		virtual bool create							(	int signature);

	protected:
	
	private:

};

//----------------------------------------------------------------------------

#endif
