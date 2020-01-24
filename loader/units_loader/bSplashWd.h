//----------------------------------------------------------------------------
// File : bSplashWd.h
// Project : MacMap
// Purpose : Header file : Splash screen class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2019 Carte Blanche Conseil.
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
// 17/09/2015 creation.
// 06/08/2019 Cocoa.
//----------------------------------------------------------------------------

#ifndef __bSplashWd__
#define __bSplashWd__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

class bSplashWd{
public:
    static bSplashWd* instance		(	bool about,
                                        bGenericMacMapApp* gapp);
    static void release_instance	(	);
    virtual bool set_progress		(	const char* str);

protected:
    bSplashWd						(	bool about,
                                        bGenericMacMapApp* gapp);
    virtual ~bSplashWd				(	);

private:
    static bSplashWd*	_splashinst;
    clock_t				_timer;
    void*               _ctrlr;
};

//----------------------------------------------------------------------------

#endif
