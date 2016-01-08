//----------------------------------------------------------------------------
// File : bvDefScaleRef.h
// Project : MacMap
// Purpose : Header file : Scale reference vDef utility class
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
// 28/04/2010 creation.
//----------------------------------------------------------------------------

#ifndef __bvDefScaleRef__
#define __bvDefScaleRef__

//----------------------------------------------------------------------------

#include <std_ext/bStdvDef.h>

//----------------------------------------------------------------------------

enum{
	kvDefScaleRefViewSignature			='View',
	
	kvDefScaleRefTypeNameLegendID		=1,
	kvDefScaleRefTypeNameID				=2,
	kvDefScaleRefPopupID				=3,
	kvDefScaleRefPopupCmd				='Pop!'
};

//----------------------------------------------------------------------------

class bvDefScaleRef : public bStdvDef{
public:
    bvDefScaleRef							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bvDefScaleRef					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual void open						(	int* flags);
    virtual void close						(	);
    virtual bool process					(	int msg,	
                                                void* prm);
    virtual bool test						(	bGenericType* tp);
    virtual bool edit						(	bGenericXMLBaseElement** root, 
                                                bGenericType* tp);
    virtual bool wd_event					(	EventRef evt, 
                                                WindowRef wd);
    virtual void wd_init					(	WindowRef wd);
            
    virtual void make						(	);
        
protected:													
    virtual bool read						(	);
    virtual bool write						(	);		
    
private:
    double		_scale;
    int			_idx;
    char		_set;
};

//----------------------------------------------------------------------------

#endif
