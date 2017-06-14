//----------------------------------------------------------------------------
// File : bObjectEvent.h
// Project : MacMap
// Purpose : Header file : MacMap object event class
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
// 22/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bObjectEvent__
#define __bObjectEvent__

//----------------------------------------------------------------------------

#include "bMacMapEvent.h"

//----------------------------------------------------------------------------

class bObjectEvent : public bMacMapEvent{
public:
    bObjectEvent			(	char* msg,
                                int crt,// BigEndian coded
                                int act);
    virtual ~bObjectEvent	(	);

// ext intf
    virtual bool add		(	void* elt);
    virtual int find		(	void* elt);
            
protected:
    virtual int find		(	bGenericGeoElement* o);
    static int compare		(	const void* a, 
                                const void* b);

private:
};

//----------------------------------------------------------------------------

class bObjectCreateEvent : public bObjectEvent{
public:
    bObjectCreateEvent			(	char* msg,
                                    int crt);// BigEndian coded
    virtual ~bObjectCreateEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bObjectDeleteEvent : public bObjectEvent{
public:
    bObjectDeleteEvent			(	char* msg,
                                    int crt);// BigEndian coded
    virtual ~bObjectDeleteEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bObjectModifyEvent : public virtual bObjectEvent{
public:
    bObjectModifyEvent			(	char* msg,
                                    int crt);// BigEndian coded
    virtual ~bObjectModifyEvent	(	);
    
    virtual bool add			(	void* elt);

    virtual bool add			(	void* elt,
                                    int fld);
    virtual int find			(	void* elt);
    
protected:
    virtual int find			(	bGenericGeoElement* o);
    static int compare			(	const void* a,
                                    const void* b);
private:

};

//----------------------------------------------------------------------------

#endif
