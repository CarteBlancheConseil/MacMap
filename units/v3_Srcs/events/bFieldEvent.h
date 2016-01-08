//----------------------------------------------------------------------------
// File : bFieldEvent.h
// Project : MacMap
// Purpose : Header file : MacMap field (database) event class
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
// 22/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bFieldEvent__
#define __bFieldEvent__

//----------------------------------------------------------------------------

#include "bMacMapEvent.h"

//----------------------------------------------------------------------------

class bFieldEvent : public bMacMapEvent{
	public:
		bFieldEvent							(	char* msg, 
												int crt, // BigEndian coded
												int act);
		virtual ~bFieldEvent				(	);

// ext intf
		virtual bool add					(	void* elt);
		virtual int find					(	void* elt);
					
	protected:
		virtual int find					(	field_info* f);		
		static int compare					(	const void* a, 
												const void* b);
		
	private:

};

//----------------------------------------------------------------------------

class bFieldCreateEvent : public bFieldEvent{
	public:
		bFieldCreateEvent					(	char* msg, 
												int crt);// BigEndian coded
		virtual ~bFieldCreateEvent			(	);
				
	protected:

	private:

};

//----------------------------------------------------------------------------

class bFieldDeleteEvent : public bFieldEvent{
	public:
		bFieldDeleteEvent					(	char* msg, 
												int crt);// BigEndian coded
		virtual ~bFieldDeleteEvent			(	);
				
	protected:

	private:

};

//----------------------------------------------------------------------------

class bFieldModifyEvent : public virtual bFieldEvent{
	public:
		bFieldModifyEvent					(	char* msg, 
												int crt);// BigEndian coded
		virtual ~bFieldModifyEvent			(	);
				
	protected:

	private:

};

//----------------------------------------------------------------------------

#endif
