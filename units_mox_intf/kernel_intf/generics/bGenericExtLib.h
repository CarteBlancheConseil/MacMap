//----------------------------------------------------------------------------
// File : bGenericExtLib.h
// Project : MacMap
// Purpose : Header file : Root class for extern plugins
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

#ifndef __bGenericExtLib__
#define __bGenericExtLib__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericExt.h>

//----------------------------------------------------------------------------

class bGenericExtLib : public bGenericElement{
	public:
		virtual ~bGenericExtLib		(	){};
	
		virtual int status			(	)=0;
	
		virtual int family			(	)=0;
		virtual int creator			(	)=0;
		virtual void vers			(	char* c)=0;
		virtual void name			(	char* c)=0;
		
		virtual int signature		(	)=0;
		virtual void macmap_vers	(	char* c)=0;

		virtual int num				(	)=0;// A virer
		
		virtual bGenericExt* inst	(	)=0;
};

//----------------------------------------------------------------------------

#endif
