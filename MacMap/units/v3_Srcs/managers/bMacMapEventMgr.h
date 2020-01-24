//----------------------------------------------------------------------------
// File : bMacMapEventMgr.h
// Project : MacMap
// Purpose : Header file : MacMap events management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2019 Carte Blanche Conseil.
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
// 17/08/2005 creation.
// 20/12/2018 cocoa.
//----------------------------------------------------------------------------

#ifndef __bMacMapEventMgr__
#define __bMacMapEventMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericEventMgr.h>
#include <MacMapSuite/bArray.h>
#include "bMacMapEvent.h"

//----------------------------------------------------------------------------

class bMacMapEventMgr : public bGenericEventMgr{
public:
    bMacMapEventMgr					(	);
    virtual ~bMacMapEventMgr		(	);
    
    virtual void enable				(	);
    virtual void disable			(	);
    virtual bool state				(	);

    virtual int init_log			(	int creator, // BigEndian coded
                                        const char* msg);
    virtual int close_log			(	);
    virtual void reset_log			(	);

    virtual bArray* events			(	);

    virtual void create				(	field_info*	fi);
    virtual void create				(	bGenericGeoElement* o);
    virtual void create				(	bGenericType* t);
    virtual void create				(	bGenericExt* e);
    virtual void kill				(	field_info*	fi);
    virtual void kill				(	bGenericGeoElement* o);
    virtual void kill				(	bGenericType* t);
    virtual void kill				(	bGenericExt* e);
    virtual void modify				(	field_info*	fi);
    virtual void modify				(	bGenericGeoElement* o,
                                        int field);
    virtual void modify				(	bGenericType* t);
    virtual void modify				(	bGenericExt* e);
    
    char* message                   (   ){return _msg;};
    
protected:
    void send						(	);
    bool std_test					(	int kind, 
                                        int action);
    void std_close					(	);
    
private:
    bool			_enabled;
    bool			_logged;
    bMacMapEvent*	_evt;
    bArray			_evts;
    bArray			_tmp;
    
    int				_sign;
    char			_msg[256];
};

//----------------------------------------------------------------------------

#endif
