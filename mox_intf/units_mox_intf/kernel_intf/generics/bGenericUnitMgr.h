//----------------------------------------------------------------------------
// File : bGenericUnitMgr.h
// Project : MacMap
// Purpose : Header file : Root class for unit manager
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

#ifndef __BGENERICUNITMGR__
#define __BGENERICUNITMGR__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericUnit.h>

//----------------------------------------------------------------------------

class bGenericUnitMgr : public bGenericMgr{
public:
    virtual ~bGenericUnitMgr	(	){};	
    virtual long get_current	(	)=0;
    virtual bool set_current	(	long idx)=0;
    
    virtual long count			(	)=0;
    
    virtual bGenericUnit* get	(	)=0;
    virtual bGenericUnit* get	(	long idx)=0;
    
    virtual void set			(	long idx,
                                    const char* name, 
                                    double coef)=0;
    
    virtual bool add			(	const char* name, 
                                    double coef)=0;
    virtual bool rmv			(	long idx)=0;
    
    virtual bool make			(	)=0;
    
    virtual int app_load		(	)=0;
    virtual int app_unload		(	)=0;
};

//----------------------------------------------------------------------------

#endif
