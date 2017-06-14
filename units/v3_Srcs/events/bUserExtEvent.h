//----------------------------------------------------------------------------
// File : bUserExtEvent.h
// Project : MacMap
// Purpose : Header file : MacMap user extern event class
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

#ifndef __bUserExtEvent__
#define __bUserExtEvent__

//----------------------------------------------------------------------------

#include "bMacMapEvent.h"

//----------------------------------------------------------------------------

class bUserExtEvent : public bMacMapEvent{
public:
    bUserExtEvent			(	char* msg,
                                int crt,
                                int act);
    virtual ~bUserExtEvent	(	);

// ext intf
    virtual bool add		(	void* elt);
    virtual int find		(	void* elt);
                
protected:
    virtual int find		(	bGenericExt* t);
    static int compare		(	const void* a,
                                const void* b);
    
private:
};

//----------------------------------------------------------------------------

class bUserExtCreateEvent : public bUserExtEvent{
public:
    bUserExtCreateEvent				(	char* msg,
                                        int crt);
    virtual ~bUserExtCreateEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bUserExtDeleteEvent : public bUserExtEvent{
public:
    bUserExtDeleteEvent				(	char* msg,
                                        int crt);
    virtual ~bUserExtDeleteEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

class bUserExtModifyEvent : public virtual bUserExtEvent{
public:
    bUserExtModifyEvent				(	char* msg,
                                        int crt);
    virtual ~bUserExtModifyEvent	(	);
            
protected:

private:

};

//----------------------------------------------------------------------------

#endif
