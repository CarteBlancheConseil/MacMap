//----------------------------------------------------------------------------
// File : bXMapIconPicker.h
// Project : MacMap
// Purpose : Header file : XMap class, icon picker (used by vDefs)
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
// 31/05/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapIconPicker__
#define __bXMapIconPicker__

//----------------------------------------------------------------------------

#include <xmap_lib/bXMapStdPicker.h>

//----------------------------------------------------------------------------

class bXMapIconPicker : public bXMapStdPicker{
public:		
    bXMapIconPicker							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapIconPicker				(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
            
protected:															
    virtual void draw						(	CGContextRef ctx);
//    virtual void import						(	NavObjectFilterUPP filter);
    virtual void import						(	const char** pnsft,
                                                int nnsft);
    
private:

};

//----------------------------------------------------------------------------

#endif
