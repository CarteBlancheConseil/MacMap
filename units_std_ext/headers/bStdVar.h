//----------------------------------------------------------------------------
// File : bStdVar.h
// Project : MacMap
// Purpose : Header file : Base class for Variables/Constants
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
// 04/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bStdVar__
#define __bStdVar__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <mox_intf/bGenericCalc.h>
#include <std_ext/bStdXMLFormulaElement.h>

//----------------------------------------------------------------------------

class bStdVar : public virtual bGenericCalc, public bStdExt{
public:		
	bStdVar									(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdVar						(	);
			
	virtual void open						(	int* flags);
	virtual void close						(	);
	virtual bool event						(	EventRef evt);
	virtual bool nsevent					(	void* nsevt);
	virtual void idle						(	void* prm);
	virtual bool process					(	int msg, 
												void* prm);
	virtual bool test						(	void* prm);
	virtual bool edit						(	void* prm);
	
	virtual bGenericXMLFormulaElement* root	(	);
	virtual int kind						(	);
	virtual bStdDBValue* solve				(	bGenericGeoElement* o);
	virtual void get_params					(	char* name, 
												char* value);

protected:
	virtual void write						(	bStdDBValue* val);
	virtual bool exec						(	);
	
	UInt32					_sign;	// Var signature (id)
	bStdXMLFormulaElement*	_root;	// Root of the formula
	
private:
	
};

//----------------------------------------------------------------------------

#endif

