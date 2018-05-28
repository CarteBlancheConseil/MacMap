//----------------------------------------------------------------------------
// File : bGenericEvent.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap events
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
// 16/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericEvent__
#define __bGenericEvent__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericExt.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

// event actions
enum{
	kEventActionCreate		='crea',
	kEventActionModify		='modi',
	kEventActionDestroy		='kill'
};

// event kinds
enum{
	kEventKindGeoElement	='gobj',
	kEventKindDataStruct	='data',
	kEventKindTypeElement	='type',
	kEventKindUserExt		='uExt'
};

// struct for field events
typedef struct field_info{
	int				fidx;
	int				fid;
	int				kind;
	int				len;
	int				decs;
	char			name[256];
	bGenericType*	tp;
}field_info;

//----------------------------------------------------------------------------

class bGenericEvent : public bGenericElement{
	public:
		virtual ~bGenericEvent			(	){};
// stuff
		virtual long eid				(	)=0;			// event id
		virtual int kind				(	)=0;			// event kind
		virtual int action				(	)=0;			// event action
		virtual int creator				(	)=0;			// creator function signature (BigEndian coded)
		virtual char* message			(	)=0;			// associated message
		
		virtual bool is_undo			(	)=0;			// true if event come from undo
		virtual bool is_redo			(	)=0;			// true if event come from redo
		
		virtual bool add				(	void* elt)=0;	// adding element, return true if ok

		virtual void close				(	)=0;			// lock the event

		virtual bArray* elements		(	)=0;			// elements
		// Array contains :
		// - bGenericType* for kEventKindTypeElement
		// - field_info for kEventKindDataStruct
		// - bGenericGeoElement* for kEventKindGeoElement
		// - bGenericExt* for kEventKindMacroElement, kEventKindCalcElement, 
		//   and kEventKindScriptElement

};

//----------------------------------------------------------------------------

#endif
