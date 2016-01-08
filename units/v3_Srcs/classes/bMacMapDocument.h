//----------------------------------------------------------------------------
// File : bMacMapDocument.h
// Project : MacMap
// Purpose : Header file : MacMap document class
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
// 01/12/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bMacMapDocument__
#define __bMacMapDocument__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericDocument.h>
#include <mox_intf/bGenericStyleMgr.h>

//----------------------------------------------------------------------------

#define kStyleFolderName "styles"

//----------------------------------------------------------------------------

class bMacMapDocument : public bGenericDocument{
	public:		
		bMacMapDocument								(	);
		virtual ~bMacMapDocument					(	);
		
		virtual void name							(	char* name);
		
		virtual void location						(	char* path);

		virtual int srid							(	);
		virtual bool set_srid						(	int	srid);

		virtual d2dvertex get_origin				(	);
		virtual bool set_origin						(	d2dvertex vx);
				
		virtual double dist_resolution				(	);
		virtual bGenericUnit* dist_resolution_unit	(	);
		
		virtual int dist_pref_digits				(	);
		virtual void set_dist_pref_digits			(	int digits);
			
		virtual int load							(	const char* path);
		virtual int unload							(	);
				
//----------------------------------------------------------------------------
// High level params management
//----------------------------------------------------------------------------
		virtual bool writeTree						(	bGenericXMLBaseElement* elt, 
														int sign, // BigEndian Coded
														const char* name);
		virtual bool readTree						(	bGenericXMLBaseElement** elt, 
														int sign, // BigEndian Coded
														const char* name);
		virtual bool removeTree						(	int sign, // BigEndian Coded
														const char* name);
//----------------------------------------------------------------------------
// Low level params management
//----------------------------------------------------------------------------
// Getting params count
		virtual int countParams						(	const char* sub);
// Getting params name
		virtual bool getParamName					(	const char* sub,
														int idx,
														char* name);
		virtual bool writeXMLParam					(	bGenericXMLBaseElement* elt, 
														const char* sub,
														const char* name);
		virtual bool readXMLParam					(	bGenericXMLBaseElement** elt, 
														const char* sub,
														const char* name);
		virtual bool writeParam						(	void* buffer,
														int sz,
														const char* sub,
														const char* name);
		virtual bool readParam						(	void** buffer,
														int* sz,
														const char* sub,
														const char* name);
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
		virtual bool make							(	const char* path, 
														int unit, 
														double reso, 
														int srid);
	
		virtual bGenericStyleMgr* styles			(	);

		virtual void set_save_objstate				(	bool prf);
		virtual bool get_save_objstate				(	);

	protected:
		virtual void read_name						(	);
		virtual bool make_package					(	);
		
		virtual bool pref_load						(	);
		virtual bool pref_unload					(	);

	private:
		char				_path[PATH_MAX];
		char				_name[FILENAME_MAX];
		double				_reso_coef;
		double				_ox;
		double				_oy;
		int					_reso_idx;
		int					_digits;
		int					_srid;
		int					_state;
		bGenericStyleMgr*	_styles;
};

//----------------------------------------------------------------------------

#endif
