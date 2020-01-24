//----------------------------------------------------------------------------
// File : bTypeEvent.h
// Project : MacMap
// Purpose : Header file : MacMap type event class
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
// 22/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bTypeEvent__
#define __bTypeEvent__

//----------------------------------------------------------------------------

#include "bMacMapEvent.h"

//----------------------------------------------------------------------------

class bTypeEvent : public bMacMapEvent{
public:
    bTypeEvent              (	char* msg,
                                int crt,
                                int act);
    virtual ~bTypeEvent		(	);

// ext intf
    virtual bool add		(	void* elt);
    virtual int find		(	void* elt);
                
protected:
    virtual int find		(	bGenericType* t);
    static int compare		(	const void* a,
                                const void* b);
    
private:
};

//----------------------------------------------------------------------------

class bTypeCreateEvent : public bTypeEvent{
public:
    bTypeCreateEvent			(	char* msg,
                                    int crt);
    virtual ~bTypeCreateEvent   (	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bTypeDeleteEvent : public bTypeEvent{
public:
    bTypeDeleteEvent			(	char* msg,
                                    int crt);
    virtual ~bTypeDeleteEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bTypeModifyEvent : public virtual bTypeEvent{
public:
    bTypeModifyEvent			(	char* msg,
                                    int crt);
    virtual ~bTypeModifyEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

#endif
