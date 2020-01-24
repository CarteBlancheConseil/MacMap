//----------------------------------------------------------------------------
// File : bMacMapModifiedGeoElement.h
// Project : MacMap
// Purpose : Header file : MacMap modified object class (events and undo/redo maangement)
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
// Modified geographic elements class for event manager
// Only a part of get functions are implemented
//----------------------------------------------------------------------------
// 17/08/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bMacMapModifiedGeoElement__
#define __bMacMapModifiedGeoElement__

//----------------------------------------------------------------------------

#include "bStdGeoElement.h"

//----------------------------------------------------------------------------

class bMacMapModifiedGeoElement : public bStdGeoElement{
public:
    bMacMapModifiedGeoElement	 		(	bGenericGeoElement* ref,
                                            int field);
    virtual ~bMacMapModifiedGeoElement  (	);
    
    virtual int getType					(	);
    virtual void getBounds				(	ivx_rect	*vxr);
    
    virtual void getVertices			(	ivertices	**vxs);
    
    virtual int getSubType				(	);

    virtual int getColor				(	);
    
    virtual int getDirection			(	);
    
    virtual void getName				(	char *name);
            
    virtual bool getValue				(	int fieldIdx,
                                            void *value);
    virtual bool getValue				(	char *fieldName,
                                            void *value);
    
    virtual bGenericGeoElement* get_ref	(	);

protected:
    
private:
    void replicate						(	);

    bGenericGeoElement*	_ref;
    void*				_buffer;
    int					_buffersz;
    int					_field;
    int					_fieldsgn;
};

//----------------------------------------------------------------------------

#endif
