//----------------------------------------------------------------------------
// File : bExtLibMgr.h
// Project : MacMap
// Purpose : Header file : Plugins manager/loader class
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
// Gestionnaire chargé de l'ouverture des plugins (tous confondus)
//----------------------------------------------------------------------------
// 17/09/2015 creation.
//----------------------------------------------------------------------------

#ifndef __bExtLibMgr__
#define __bExtLibMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExtLibMgr.h>
#include <mox_intf/bGenericCompLib.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bExtLibMgr : public bGenericExtLibMgr{
public:
    bExtLibMgr								(	bGenericMacMapApp* gapp);
    virtual ~bExtLibMgr						(	);
            
    virtual int load						(	);
    virtual int unload						(	);
    
    virtual int getlibs						(	bArray& arr, 
                                                UInt32 family);
    
    virtual bGenericCompLib* get_component	(	const char* name);
    
protected:

private:
    virtual int load						(	CFArrayRef bundles);
    static int comp							(	const void *a,
                                                const void *b);

    bGenericMacMapApp*  _gapp;
    bArray              _elts;
    bArray              _acls;
    bArray              _rez;
		
};

//----------------------------------------------------------------------------

#endif
