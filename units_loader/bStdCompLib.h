//----------------------------------------------------------------------------
// File : bStdCompLib.h
// Project : MacMap
// Purpose : Header file : Component plugin class
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
// 17/09/2015 creation.
//----------------------------------------------------------------------------

#ifndef __bStdCompLib__
#define __bStdCompLib__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericCompLib.h>
#include <MacMapSuite/bTrace.h>

//----------------------------------------------------------------------------

typedef void* (*iallocPtr)	(int,void*,void*);
typedef void (*ifreePtr)	(void*);

//----------------------------------------------------------------------------

class bStdCompLib : public bGenericCompLib{
public:
    bStdCompLib					(	CFBundleRef	bndl);
    bStdCompLib                 (	const char* name);
    virtual ~bStdCompLib		(	);
    
    virtual void vers			(	char* c);
    virtual void name			(	char* c);
    
    virtual void* i_allocate	(	int signature,
                                    void* app,
                                    void* prm);
    virtual void i_free			(	void*);

    virtual int status			(	);

protected:
    int				_status;
    
private:
    CFBundleRef _bndl;
    
    CFStringRef _version;
    CFStringRef _name;
    
    iallocPtr   _ialloc;
    ifreePtr    _ifree;
};

//----------------------------------------------------------------------------

#endif
