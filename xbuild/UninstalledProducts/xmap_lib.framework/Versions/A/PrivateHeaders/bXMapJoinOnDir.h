//----------------------------------------------------------------------------
// File : bXMapJoinOnDir.h
// Project : MacMap
// Purpose : Header file : XMap class, join in the direction
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

#ifndef __bXMapJoinOnDir__
#define __bXMapJoinOnDir__

//----------------------------------------------------------------------------

#include "bXMapJoinFamily.h"

//----------------------------------------------------------------------------

#define	kXMapJoinOnDirSignature	'JDir'
#define	kXMapJoinOnDirMessageID	"join on dir"

//----------------------------------------------------------------------------

class bXMapJoinOnDir : public bXMapJoinFamily{
public:
    bXMapJoinOnDir							(   bGenericXMLBaseElement* elt,
                                                bGenericMacMapApp* gapp,
                                                CFBundleRef bndl);
    virtual ~bXMapJoinOnDir					(   );
    virtual bGenericXMLBaseElement* create	(   bGenericXMLBaseElement* elt);
    
    virtual bool process					(   int msg,
                                                void* prm);
    virtual bool edit						(   void* prm);
    
protected:

private:
    bool join								(	i2dvertex* vx,
                                                i2dvertex* pvx,
                                                ivertices* ref,
                                                double	dmax,
                                                i2dvertex* res,
                                                int* cur,
                                                double*	d);
};

//----------------------------------------------------------------------------

#endif
