//----------------------------------------------------------------------------
// File : bGenericExt.h
// Project : MacMap
// Purpose : Header file : Root class for externs
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
// 14/12/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericExt__
#define __bGenericExt__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericXMLBaseElement.h>

//----------------------------------------------------------------------------

enum{
	kExtProcessCallFromIntf		= 1,	// parameters set with intf
	kExtProcessCallWithParams	= 2,	// call with prm, assume structure is known
	kExtProcessCallWithXMLTree	= 3,	// params describes as xml tree (ie bGenericXMLBaseElement)
	kExtProcessCallGetData		= 32,	// read params
	kExtProcessCallSetData		= 33	// write params
};

//----------------------------------------------------------------------------

class bGenericExt : public virtual bGenericXMLBaseElement{
public:
	virtual ~bGenericExt		(	){};
	virtual void open			(	int* flags)=0;
	virtual void close			(	)=0;
/*	virtual bool event			(	EventRef evt)=0;*/
	virtual bool nsevent		(	void* nsevt)=0;
	virtual void idle			(	void* prm)=0;
	virtual bool process		(	int msg, 
									void* prm)=0;
	virtual bool test			(	void* prm)=0;
	virtual bool edit			(	void* prm)=0;
};

//----------------------------------------------------------------------------

#endif
