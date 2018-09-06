//----------------------------------------------------------------------------
// File : bXMapJoinFamily.h
// Project : MacMap
// Purpose : Header file : XMap base class for join utilities (geometry)
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
// 12/07/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapJoinFamily__
#define __bXMapJoinFamily__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define	kXMapJoinFamilyConfigSign	'nioj'
#else
#define	kXMapJoinFamilyConfigSign	'join'
#endif

//----------------------------------------------------------------------------

class bXMapJoinFamily : public bStdXMap{
public:
    bXMapJoinFamily							(   bGenericXMLBaseElement* elt,
                                                bGenericMacMapApp* gapp,
                                                CFBundleRef bndl);
    virtual ~bXMapJoinFamily				(   );
    virtual bGenericXMLBaseElement* create	(   bGenericXMLBaseElement* elt);
    
    virtual void open						(   int* flags);
    virtual void close						(   );
    virtual bool process					(   int msg,
                                                void* prm);
    virtual bool test						(   void* prm);
    
protected:		
    void read_p								(   );
    void write_p							(   );
    bGenericXMLBaseElement* make_p			(   );
    virtual void info                       (   long far,
                                                long joined);
    double	_dmax;
    
private:
};

//----------------------------------------------------------------------------

#endif
