//----------------------------------------------------------------------------
// File : bStdUserCalc.h
// Project : MacMap
// Purpose : Header file : Base class for user Calcs
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
// 05/12/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdUserCalc__
#define __bStdUserCalc__

//----------------------------------------------------------------------------

#include <std_ext/bStdCalc.h>

//----------------------------------------------------------------------------

#define kUserCalcSolverName	"fcalcuser"

//----------------------------------------------------------------------------

class bStdUserCalc : public bStdCalc{
public:		
	bStdUserCalc							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdUserCalc					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
			
	virtual void open						(	int* flags);		
	virtual bool process					(	int msg, 
												void* prm);
	virtual int kind						(	);

	virtual void get_params					(	char* name, 
												char* value);

protected:
	virtual void write						(	bStdDBValue* val);
	virtual void read						(	);
	
	char*					_name;	// Calc name
	UInt32					_kind;	// Returned data type

private:
};

//----------------------------------------------------------------------------

#endif
