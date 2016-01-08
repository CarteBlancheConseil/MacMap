//----------------------------------------------------------------------------
// File : bGenericCompLib.h
// Project : MacMap
// Purpose : Header file : Root class for component plugins
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
// 13/12/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericCompLib__
#define __bGenericCompLib__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>

//----------------------------------------------------------------------------

class bGenericCompLib : public bGenericElement{
	public:			
		virtual ~bGenericCompLib	(	){};
		virtual void vers			(	char* c)=0;
		virtual void name			(	char* c)=0;
				
		virtual void* i_allocate	(	int signature, 
										void* app, 
										void* prm)=0;
		virtual void i_free			(	void* lib)=0;
};

//----------------------------------------------------------------------------

#endif
