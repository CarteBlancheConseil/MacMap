//----------------------------------------------------------------------------
// File : bXMapPrint.h
// Project : MacMap
// Purpose : Header file : XMap class, printing
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
// 22/11/2006 creation.
// 26/07/2016 cocoa intf.
//----------------------------------------------------------------------------

#ifndef __bXMapPrint__
#define __bXMapPrint__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

class bXMapPrint : public bStdXMap{
public:		
    bXMapPrint								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapPrint						(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool test						(	void* prm);
    virtual bool edit						(	void* prm);
    
protected:
    virtual OSStatus print_loop				(	PMPrintSession ps, 
                                                PMPageFormat pf,
                                                PMPrintSettings pt,
                                                UInt32 nh,
                                                UInt32 nv);
private:
    
};

//----------------------------------------------------------------------------

#endif
