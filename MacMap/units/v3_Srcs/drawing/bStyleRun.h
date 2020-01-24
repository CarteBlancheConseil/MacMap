//----------------------------------------------------------------------------
// File : bStyleRun.h
// Project : MacMap
// Purpose : Header file : Style param set for a class of object
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 06/02/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bStyleRun__
#define __bStyleRun__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGenericXMLRenderingElement.h>

//----------------------------------------------------------------------------

class bStyleRun : public bGenericElement{
public:
    bStyleRun							(	); 
    virtual ~bStyleRun   				(	);
    
    virtual void setroot				(	void* elt);
    virtual void setvalidity			(	void* elt);
    virtual void setrender				(	void* elt);
    virtual void setgeometry			(	void* elt);
    
    virtual void setscalemin			(	double d);
    virtual void setscalemax			(	double d);
    virtual void setclassmin			(	long idx);
    virtual void setclassmax			(	long idx);
    virtual void setpass				(	long idx);
    
    virtual bool good					(	double scale, 
                                            long clss,
                                            long pass);
    
    virtual bool applyforclass			(	bGenericGraphicContext* ctx);
    virtual bool applyforobject			(	bGenericGraphicContext* ctx);
    virtual bool applygeometryforclass	(	bGenericGraphicContext* ctx);
    virtual bool applygeometryforobject	(	bGenericGraphicContext* ctx);
    
    virtual bGenericXMLBaseElement* root(	);
    
protected:

    
private:
    bGenericXMLRenderingElement*    _root;
    bGenericXMLRenderingElement*    _validity;
    bGenericXMLRenderingElement*    _render;
    bGenericXMLRenderingElement*    _geometry;
    
    double                          _scalemin;
    double                          _scalemax;
    long                            _classmin;
    long                            _classmax;
    long                            _pass;
};

//----------------------------------------------------------------------------

#endif
