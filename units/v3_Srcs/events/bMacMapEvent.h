//----------------------------------------------------------------------------
// File : bMacMapEvent.h
// Project : MacMap
// Purpose : Header file : MacMap event management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
//----------------------------------------------------------------------------

#ifndef __bMacMapEvent__
#define __bMacMapEvent__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericEvent.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericType.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bMacMapEvent : public bGenericEvent{
public:
    bMacMapEvent				(	char* msg,
                                    int crt, // BigEndian coded
                                    int knd,
                                    int act,
                                    int esz);
    virtual ~bMacMapEvent		(	);

// ext intf
    virtual int eid             (	){return (int)this;};
    virtual int kind			(	);
    virtual int action			(	);
    virtual int creator			(	); // BigEndian coded
    virtual char* message		(	);
    
    virtual bool is_undo		(	);
    virtual bool is_redo		(	);

    virtual bool add			(	void* elt);
    virtual bool add			(	void* elt,
                                    int prm);
    virtual int find			(	void* elt);
    
    virtual void close			(	);

    virtual bArray* elements	(	);
        
protected:
    
    bool		_closed;
    bArray		_elts;
    
private:
    int			_creator;
    int			_kind;
    int			_action;
};

//----------------------------------------------------------------------------

#endif
