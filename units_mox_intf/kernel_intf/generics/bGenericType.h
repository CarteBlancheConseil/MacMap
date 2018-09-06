//----------------------------------------------------------------------------
// File : bGenericType.h
// Project : MacMap
// Purpose : Header file : Root class for types
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
// 16/01/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericType__
#define __bGenericType__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bGenericStyleMgr.h>
#include <MacMapSuite/bStdTable.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

class bGenericType : public bGenericElement{
	public:
		virtual ~bGenericType					(	){};	
		virtual bool load						(	)=0;
		virtual void save						(	)=0;
		
		virtual void pack						(	)=0;
		virtual void name						(	char* name)=0;
		virtual void set_name					(	const char* name)=0;
		virtual double tid						(	)=0;
		virtual int index						(	)=0;
		virtual int kind						(	)=0;
		virtual int nb_rec						(	)=0;
		virtual int nb_live						(	)=0;
		virtual int nb_killed					(	)=0;

		virtual bool load_object				(	int o,
													bool *b)=0;
		virtual bool load_object				(	int o,	
													bGenericGeoElement** f)=0;
		virtual bool new_object					(	bGenericGeoElement** f)=0;
		virtual bool clone_object				(	bGenericGeoElement* fin, 
													bGenericGeoElement** fout)=0;
		virtual int append						(	)=0;
		virtual bool kill_object				(	bGenericGeoElement* f)=0;
		virtual bool unkill_object				(	bGenericGeoElement* f)=0;
				
		virtual int nb_sel						(	)=0;
		virtual void inc_sel					(	int n)=0;
		
		virtual int nb_cont						(	)=0;
		virtual void inc_cont					(	int n)=0;
		
		virtual void location					(	char* path)=0;
		
		virtual bool is_lock					(	)=0;
		virtual void set_lock					(	bool b)=0;

		virtual bool is_remove					(	)=0;
		virtual void set_remove					(	bool b)=0;
		
		virtual bStdBaseAccessor* fields		(	)=0;
		virtual bGenericGeoIterator* iterator	(	)=0;
		virtual bGenericStyleMgr* styles		(	)=0;
};

//----------------------------------------------------------------------------

#endif
