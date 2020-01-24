//----------------------------------------------------------------------------
// File : bGenericExtMgr.h
// Project : MacMap
// Purpose : Header file : Root class for externs
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
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
// 14/12/2004 creation.
// 09/11/2018 64Bits.
//----------------------------------------------------------------------------

#ifndef __bGenericExtMgr__
#define __bGenericExtMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericExt.h>

//----------------------------------------------------------------------------

class bGenericExtMgr : public bGenericMgr{
public:		
    virtual ~bGenericExtMgr			(	){};

    virtual void open				(	)=0;
    virtual void close				(	)=0;
    
    virtual bool event				(	void* nsevt)=0;
    virtual void idle				(	)=0;

    virtual bool edit				(	bGenericExt* ext, 
                                        void* prm)=0;
    virtual bool edit				(	const char* name, 
                                        UInt32 signature, 
                                        void* prm)=0;
            
    virtual void set_current		(	long index)=0;
    virtual long get_current		(	)=0;
    
    virtual long count				(	)=0;
    
    virtual bGenericExt* get		(	)=0;
    virtual bGenericExt* get		(	long idx)=0;
    virtual bGenericExt* find		(	const char* name)=0;
    virtual bGenericExt* find		(	UInt32 signature)=0;
    
    virtual void ext_name			(	long idx,
                                        char* name)=0;
    virtual UInt32 ext_signature	(	long idx)=0;
    
    virtual bool add				(	bGenericXMLBaseElement* root)=0;
    virtual bool remove				(	long idx)=0;
    virtual bool modify				(	long idx, 
                                        bGenericXMLBaseElement* root)=0;

    virtual long ext_index			(	UInt32 sign)=0;
    virtual long ext_index			(	const char* name)=0;

};

//----------------------------------------------------------------------------

#endif
