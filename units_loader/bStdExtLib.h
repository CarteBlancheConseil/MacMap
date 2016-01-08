//----------------------------------------------------------------------------
// File : bStdExtLib.h
// Project : MacMap
// Purpose : Header file : Extern plugin class
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

#ifndef __bStdExtLib__
#define __bStdExtLib__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExtLib.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bTrace.h>

//----------------------------------------------------------------------------

typedef void (*initPtr)		(bGenericExt**,bGenericMacMapApp*,CFBundleRef);
typedef void (*destroyPtr)	(bGenericExt*);

//----------------------------------------------------------------------------

class bStdExtLib : public bGenericExtLib{
public:
    bStdExtLib					(	CFBundleRef	bndl,
                                    bGenericMacMapApp* gapp);
    virtual ~bStdExtLib			(	);
    
    virtual int status			(	);

    virtual int family			(	);
    virtual int creator			(	);
    virtual void vers			(	char* c);
    virtual void name			(	char* c);
    
    virtual int signature		(	);
    virtual void macmap_vers	(	char* c);

    virtual int num				(	){return -1;};
    
    virtual bGenericExt* inst	(	);
		
protected:
    int				_status;
    
private:
    CFBundleRef		_bndl;
    
    UInt32			_family;
    UInt32			_creator;
    CFStringRef		_version;
    CFStringRef		_name;
    
    UInt32			_sign;
    CFStringRef		_mmvers;
    
    initPtr			_init;
    destroyPtr		_destroy;
    
    bGenericExt*	_inst;
};

//----------------------------------------------------------------------------

#endif
