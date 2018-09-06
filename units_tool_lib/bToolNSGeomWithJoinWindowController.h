//----------------------------------------------------------------------------
// File : bToolNSGeomWithJoinWindowController.h
// Project : MacMap
// Purpose : Header file : Tool Create window controller
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2017 Carte Blanche Conseil.
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
// 28/03/2017 creation.
//----------------------------------------------------------------------------

#import <std_ext/bStdNSAppModalWindowController.h>

//----------------------------------------------------------------------------
@interface bToolNSGeomWithJoinWindowController : bStdNSAppToolModalWindowController{
    IBOutlet NSButton*      _nea_chk;
    IBOutlet NSTextField*   _nea_txt;
    IBOutlet NSButton*      _dir_chk;
    IBOutlet NSTextField*   _dir_txt;
    IBOutlet NSButton*      _ang_chk;
    IBOutlet NSTextField*   _ang_txt;
    IBOutlet NSButton*      _ext_chk;
    IBOutlet NSTextField*   _ext_txt;

    IBOutlet NSTableView*	_typ_viw;
}

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
