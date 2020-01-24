//----------------------------------------------------------------------------
// File : bGenericEventMgr.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap events manager
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

#ifndef __bGenericEventMgr__
#define __bGenericEventMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericEvent.h>

//----------------------------------------------------------------------------

// Event classes
/*enum{
	kEventClassMacMap		='Map_'
};

// 
enum{
	kEventMacMapDataBase	='MpDt',
	kEventMacMapUpdateTools ='MpTU'
};*/


// nsevent subtypes for NSEventTypeApplicationDefined
enum{
    NSEventSubtypeDrawMap           =1, // Map Window Refresh
    
    NSEventSubtypeMacMapDataBase	=2, // Database event
    NSEventSubtypeMacMapUpdateTools	=3, // ?

    NSEventSubtypeMacMapCommand     =4, // ?
};

//----------------------------------------------------------------------------

class bGenericEventMgr : public bGenericElement{
	public:
		virtual ~bGenericEventMgr   (	){};
		
// externs intf
		virtual void enable         (	)=0;				// enable event generation
		virtual void disable        (	)=0;				// disable event generation
		virtual bool state          (	)=0;				// true if enabled

		virtual int init_log        (	int creator,		// BigEndian coded
                                        const char* msg)=0;	// returns error code (<0) or 0
		virtual int close_log       (	)=0;				// returns error code (<0) or 0
		virtual void reset_log      (	)=0;
// ajouter un reopen
						
		virtual bArray* events      (	)=0;
		
// only for kernel
		virtual void create         (	field_info* fi)=0;
		virtual void create         (	bGenericGeoElement* o)=0;
		virtual void create         (	bGenericType* t)=0;
		virtual void create         (	bGenericExt* e)=0;
		virtual void kill           (	field_info* fi)=0;
		virtual void kill           (	bGenericGeoElement* o)=0;
		virtual void kill           (	bGenericType* t)=0;
		virtual void kill           (	bGenericExt* e)=0;
		virtual void modify         (	field_info* fi)=0;
		virtual void modify         (	bGenericGeoElement* o,
                                        int field)=0;
		virtual void modify         (	bGenericType* t)=0;
		virtual void modify         (	bGenericExt* e)=0;
};

//----------------------------------------------------------------------------

#endif
