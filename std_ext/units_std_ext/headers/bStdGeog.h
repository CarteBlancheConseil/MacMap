//----------------------------------------------------------------------------
// File : bStdGeog.h
// Project : MacMap
// Purpose : Header file : Base class for Geogs
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
// 28/01/2005 creation.
// 01/11/2016 remove carbon call (edit(windowref,*rect), hide and show)
//----------------------------------------------------------------------------

#ifndef __bStdGeog__
#define __bStdGeog__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <std_ext/bStdXMLFormulaElement.h>

#include <mox_intf/bGenericGeog.h>

//----------------------------------------------------------------------------

#define	kStdGeogConfigSign	'geog'

//----------------------------------------------------------------------------

class bStdGeog : public virtual bGenericGeog, public bStdExt {
public:		
	bStdGeog								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdGeog						(	);
		
	virtual void open						(	int* flags);
	virtual void close						(	);
	virtual bool nsevent					(	void* nsevt);
	virtual void idle						(	void* prm);
	virtual bool process					(	int msg, 
												void* prm);
	virtual bool test						(	void* prm);
    
	virtual bool edit						(	void* nsview);
    virtual void end_edit					(	);

    virtual void get_params					(	bArray* arr,
												int indent);
	virtual void put_params					(	bGenericXMLBaseElement* root);
	virtual bGenericXMLFormulaElement* root	(	);

protected:
	bStdXMLFormulaElement*	_root;

private:

	
};

//----------------------------------------------------------------------------

#endif

