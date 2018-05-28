//----------------------------------------------------------------------------
// File : bXMapJoinOnVertex.h
// Project : MacMap
// Purpose : Header file : XMap class, join on vertices
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

#ifndef __bXMapJoinOnVertex__
#define __bXMapJoinOnVertex__

//----------------------------------------------------------------------------

#include "bXMapJoinFamily.h"

//----------------------------------------------------------------------------

#define	kXMapJoinOnVertexSignature	'JAng'
#define	kXMapJoinOnVertexMessageID	"join on vertex"

//----------------------------------------------------------------------------

class bXMapJoinOnVertex : public bXMapJoinFamily{
public:
    bXMapJoinOnVertex						(   bGenericXMLBaseElement* elt,
                                                bGenericMacMapApp* gapp,
                                                CFBundleRef bndl);
    virtual ~bXMapJoinOnVertex				(   );
    virtual bGenericXMLBaseElement* create	(   bGenericXMLBaseElement* elt);
    
    virtual bool process					(   int msg,
                                                void* prm);
    virtual bool edit						(   void* prm);
    
protected:

private:
    bool join								(	i2dvertex* vx,
                                                ivertices* ref,
                                                double	dmax,
                                                i2dvertex* res,
                                                int* cur,
                                                double *d);
    int sm_dst								(	i2dvertex* a, 
                                                i2dvertex* b);
};

//----------------------------------------------------------------------------

#endif
