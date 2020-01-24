//----------------------------------------------------------------------------
// File : bGenericMenuMgr.h
// Project : MacMap
// Purpose : Header file : Root class for menus manager
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
// 29/11/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericMenuMgr__
#define __bGenericMenuMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExtMgr.h>

//----------------------------------------------------------------------------

#define kMenuMgrMenuFileID		1
#define kMenuMgrMenuEditID		2
#define kMenuMgrMenuPalettesID	6

//----------------------------------------------------------------------------

class bGenericMenuMgr : public virtual bGenericMgr{
public:
    virtual ~bGenericMenuMgr	(	){};
		
    virtual int add_item		(	int menu,
                                    const char* name,
                                    UInt32 cmd)=0;
    virtual bool rmv_item		(	int menu,
                                    UInt32 cmd)=0;
    virtual bool ren_item		(	int menu,
                                    const char* name,
                                    UInt32 cmd)=0;
    
    virtual bool is_enabled		(	UInt32 cmd)=0;
    virtual void check_menus    (	)=0;

};

//----------------------------------------------------------------------------

#endif
