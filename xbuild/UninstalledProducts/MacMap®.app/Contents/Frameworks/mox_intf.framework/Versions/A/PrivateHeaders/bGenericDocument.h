//----------------------------------------------------------------------------
// File : bGenericDocument.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap document
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

#ifndef __bGenericDocument__
#define __bGenericDocument__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericUnit.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericStyleMgr.h>
#include <MacMapSuite/vx.h>

//----------------------------------------------------------------------------

class bGenericDocument : public bGenericElement{
	public:		
		virtual ~bGenericDocument					(	){};

		virtual void name							(	char* name)=0;
		
		virtual void location						(	char* path)=0;
		
		virtual int srid							(	)=0;
		virtual bool set_srid						(	int	srid)=0;

		virtual d2dvertex get_origin				(	)=0;
		virtual bool set_origin						(	d2dvertex vx)=0;

		virtual double dist_resolution				(	)=0;
		virtual bGenericUnit* dist_resolution_unit	(	)=0;
		
		virtual int dist_pref_digits				(	)=0;
		virtual void set_dist_pref_digits			(	int digits)=0;
		
		virtual int load							(	const char* path)=0;
		virtual int unload							(	)=0;
		
		virtual bool writeTree						(	bGenericXMLBaseElement* elt, 
														int sign, // BigEndian coded
														const char* name)=0;
		virtual bool readTree						(	bGenericXMLBaseElement** elt, 
														int sign, // BigEndian coded
														const char* name)=0;
		virtual bool removeTree						(	int sign, // BigEndian coded
														const char* name)=0;

		virtual int countParams						(	const char* sub)=0;
		virtual bool getParamName					(	const char* sub,
														int idx,
														char* name)=0;
		virtual bool writeXMLParam					(	bGenericXMLBaseElement* elt, 
														const char* sub,
														const char* name)=0;
		virtual bool readXMLParam					(	bGenericXMLBaseElement** elt, 
														const char* sub,
														const char* name)=0;
		virtual bool writeParam						(	void* buffer,
														int sz,
														const char* sub,
														const char* name)=0;
		virtual bool readParam						(	void** buffer,
														int* sz,
														const char* sub,
														const char* name)=0;
		virtual bool make							(	const char* path,
														int unit, 
														double reso, 
														int srid)=0;
	
		virtual bGenericStyleMgr* styles			(	)=0;
		virtual void set_save_objstate				(	bool prf)=0;
		virtual bool get_save_objstate				(	)=0;

	private:

};

//----------------------------------------------------------------------------

#endif
