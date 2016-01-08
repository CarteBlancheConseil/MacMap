//----------------------------------------------------------------------------
// File : bStdUnit.h
// Project : MacMap
// Purpose : Header file : Standard class for units
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
// 24/11/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStdUnit__
#define __bStdUnit__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericUnit.h>

//----------------------------------------------------------------------------

class bStdUnit : public bGenericUnit{
	public:		
		bStdUnit					(	);
		virtual~bStdUnit			(	);
		
		virtual void name_id		(	char* name);
		virtual void long_name		(	char* name);
		virtual void short_name		(	char* name);
		virtual double coef			(	);
		virtual double conv			(	);
		
		virtual void set			(	const char* name, 
										double coef);
			
	protected:
		char*	_id;
		char*	_shrt_name;
		char*	_long_name;
		double	_coef;
		double	_conv;

	private:
};

//----------------------------------------------------------------------------

#endif
