//----------------------------------------------------------------------------
// File : bPrintMgr.h
// Project : MacMap
// Purpose : Header file : Print utility class
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
// 22/03/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bPrintMgr__
#define __bPrintMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericPrintMgr.h>

//----------------------------------------------------------------------------

class bPrintMgr : public bGenericPrintMgr{
	public:		
		bPrintMgr								(	);
		virtual ~bPrintMgr						(	);
		virtual int load						(	);
		virtual int unload						(	);		
		virtual ivx_rect* get_print_area		(	);
		virtual void set_print_area				(	ivx_rect* area);
		virtual OSStatus get_page_format		(	PMPageFormat* pf, 
													char* name);
		virtual OSStatus set_page_format		(	PMPageFormat pf, 
													char* name);
		virtual void set_print_context			(	CGContextRef ctx);
		virtual CGContextRef get_print_context	(	);

	protected:
		virtual bool read					();
		virtual bool write					();
		
	private:
		ivx_rect		_area;
		CGContextRef	_ctx;
};


//----------------------------------------------------------------------------

#endif
