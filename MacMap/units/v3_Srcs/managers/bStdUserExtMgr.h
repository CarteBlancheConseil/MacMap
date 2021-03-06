//----------------------------------------------------------------------------
// File : bStdUserExtMgr.h
// Project : MacMap
// Purpose : Header file : Base class for user externs management
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2018 Carte Blanche Conseil.
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
// 05/12/2005 creation.
// 09/11/2018 64Bits
//----------------------------------------------------------------------------

#ifndef __bStdUserExtMgr__
#define __bStdUserExtMgr__

//----------------------------------------------------------------------------

#include "bStdExtMgr.h"
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bStdUserExtMgr : public bStdExtMgr{
public:		
	virtual int load				(	);
	virtual void open				(	);
	virtual void close				(	);
	virtual long count				(	);
	virtual bGenericExt* get		(	long idx);
	virtual bGenericExt* find		(	const char* name);
	virtual bGenericExt* find		(	UInt32 signature);
	virtual void ext_name			(	long idx,
										char* name);
	virtual UInt32 ext_signature	(	long idx);

	virtual bool add				(	bGenericXMLBaseElement* root);
	virtual bool remove				(	long idx);
	virtual bool modify				(	long idx,
										bGenericXMLBaseElement* root);

	virtual long ext_index			(	UInt32 sign);
	virtual long ext_index			(	const char* name);

protected:
	bStdUserExtMgr					(	UInt32 sign, 
										bool use_current, 
										bool need_idle, 
										bool need_events, 
										long index,
										const char* container);
	virtual ~bStdUserExtMgr			(	);
	
	virtual int usr_load			(	);
	virtual int usr_unload			(	);
	
	static int comp_uname			(	const void* a, 
										const void* b);
	static int comp_usignature		(	const void* a, 
										const void* b);

	
private:
	bArray			_uelts;
	char			_container[_names_length_max_];

};

//----------------------------------------------------------------------------

#endif
