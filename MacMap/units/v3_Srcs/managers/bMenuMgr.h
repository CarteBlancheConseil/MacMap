//----------------------------------------------------------------------------
// File : bMenuMgr.h
// Project : MacMap
// Purpose : Header file : Menu management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2019 Carte Blanche Conseil.
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
// 29/11/2005 creation.
// 07/11/2018 portage Cocoa.
//----------------------------------------------------------------------------

#ifndef __bMenuMgr__
#define __bMenuMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMenuMgr.h>

//----------------------------------------------------------------------------

class bMenuMgr : public bGenericMenuMgr{
public:
    bMenuMgr					(	);
    virtual ~bMenuMgr			(	);
    
    virtual int load			(	);
    virtual int unload			(	);
    virtual int add_item		(	int menu,
                                    const char* name,
                                    UInt32 cmd);
    virtual bool rmv_item		(	int menu,
                                    UInt32 cmd);
    virtual bool ren_item		(	int menu,
                                    const char* name,
                                    UInt32 cmd);
    virtual bool is_enabled     (	UInt32 cmd);
    virtual void check_menus    (	);

protected:
    void checkMenu              (   NSMenu *subMenu);
    bGenericExt* find_ext       (	UInt32 cmd);

private:
		
};


//----------------------------------------------------------------------------

#endif
