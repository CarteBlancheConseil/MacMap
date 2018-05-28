//----------------------------------------------------------------------------
// File : bStdSolver.h
// Project : MacMap
// Purpose : Header file : Base class for solvers (used for formulas execution)
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
// 08/03/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStdSolver__
#define __bStdSolver__

//----------------------------------------------------------------------------

#include <MacMapSuite/bArray.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bStdDBValue.h>
#include <mox_intf/bStdDBValue.h>
#include <mox_intf/bBoolDBValue.h>
#include <mox_intf/bCharDBValue.h>
#include <mox_intf/bDateDBValue.h>
#include <mox_intf/bDoubleDBValue.h>
#include <mox_intf/bIntDBValue.h>
#include <mox_intf/bTimeDBValue.h>

//----------------------------------------------------------------------------

class bStdSolver{
	public:
		bStdSolver 								(   void* gapp);
		virtual ~bStdSolver 					(   );
		
		virtual int count						(   );
		virtual bool add						(   bStdDBValue* elt);
		virtual bool rmv						(   int idx);
		virtual bStdDBValue* get				(   int idx);
		virtual void reset						(   );
		
		virtual void set_type					(   bGenericType* tp);
		virtual bGenericType* get_type          (   );
		virtual void set_object                 (   bGenericGeoElement* o);
		virtual bGenericGeoElement* get_object  (   );
		
		virtual bool result						(   bStdDBValue** elt);
				 
	protected:
		void*		_gapp;

	private:
		bGenericGeoElement*		_obj;
		bGenericType*			_tp;
		bArray					_elts;// DBValues array
		
};

//----------------------------------------------------------------------------

#endif
