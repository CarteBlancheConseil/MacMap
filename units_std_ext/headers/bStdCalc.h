//----------------------------------------------------------------------------
// File : bStdCalc.h
// Project : MacMap
// Purpose : Header file : Base class for Calcs
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
// 29/11/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdCalc__
#define __bStdCalc__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <mox_intf/bGenericCalc.h>
#include <std_ext/bStdXMLFormulaElement.h>

//----------------------------------------------------------------------------

class bStdCalc : public virtual bGenericCalc, public bStdExt{
public:		
	bStdCalc								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdCalc						(	);
			
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
	
	UInt32					_sign;	// Calc signature (id)
	bStdXMLFormulaElement*	_root;	// Root of the formula
	
private:
	
};

//----------------------------------------------------------------------------

#endif

