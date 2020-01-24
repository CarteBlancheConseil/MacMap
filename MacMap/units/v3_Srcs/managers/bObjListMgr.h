//----------------------------------------------------------------------------
// File : bObjListMgr.h
// Project : MacMap
// Purpose : Header file : Base class for object lists management (i.e. selection & contrats)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 22/01/2004 creation.
//----------------------------------------------------------------------------

#ifndef __BOBJLISTMGR__
#define __BOBJLISTMGR__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericObjListMgr.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bObjListMgr : public bGenericObjListMgr{
public:
    bObjListMgr 						(	);
    virtual ~bObjListMgr  				(	);
            
    virtual int load					(	);
    virtual int unload					(	);		
    
    virtual long count					(	);
    virtual int getState				(	);
    virtual void incState				(	);
    
    virtual int add						(	bGenericGeoElement* o);
    virtual int add						(	bArray* l);
    virtual int rmv						(	bGenericGeoElement* o);
    
    virtual void flush					(	);
    
    virtual bArray*	elements			(	);
    
    virtual bGenericGeoElement*	next	(	);
    virtual bGenericGeoElement*	prev	(	);
    
    virtual int type_count				(	);
    virtual bool type_unique			(	);

protected:		
    bArray*		_elts;
    int			_state;	
    long		_curs;
    bool		_serial;	
};

//----------------------------------------------------------------------------

#endif
