//----------------------------------------------------------------------------
// File : bStdUserVar.h
// Project : MacMap
// Purpose : Header file : Base class for user Variables/Constants
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bStdUserVar__
#define __bStdUserVar__

//----------------------------------------------------------------------------

#include <std_ext/bStdVar.h>

//----------------------------------------------------------------------------

#define kUserVarSolverName	"fvaruser"

//----------------------------------------------------------------------------

class bStdUserVar : public bStdVar{
public:		
	bStdUserVar								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdUserVar					(	);
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
