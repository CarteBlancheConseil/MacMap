//----------------------------------------------------------------------------
// File : ExtWindowController.h
// Project : MacMap
// Purpose : Header file : Base window controller class for externs panels (Macro, Calcs, etc...)
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
// 20/01/2015 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <std_ext/bStdNSXBoxWindowController.h>

//----------------------------------------------------------------------------

#define kXMapExtWdDeleteMsgID	"delete"
#define kXMapExtWdCloneMsgID	"clone"

//----------------------------------------------------------------------------
// Juste pour avoir la classe en local pour IB

@interface ExtPanel : bStdNSXBoxPanel{
}

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

@interface ExtWindowController : bStdNSXBoxWindowController{	
    IBOutlet NSButton*      _add_btn;
    IBOutlet NSButton*      _rmv_btn;
    IBOutlet NSButton*      _edt_btn;
    IBOutlet NSButton*      _dup_btn;
    
    IBOutlet NSTableView*	_mgr_viw;
    
    bGenericExtMgr*	_mgr;
    int				_xsign;
}

//----------------------------------------------------------------------------
// Actions
-(IBAction)doAdd:(id)sender;
-(IBAction)doRemove:(id)sender;
-(IBAction)doEdit:(id)sender;
-(IBAction)doDuplicate:(id)sender;
-(void)doubleClick:(id)nid;

//----------------------------------------------------------------------------
// Gestion table
-(NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView;
-(id)               tableView:(NSTableView *)aTableView
    objectValueForTableColumn:(NSTableColumn*)aTableColumn
                          row:(NSInteger)rowIndex;
-(void)tableViewSelectionDidChange:(NSNotification*)notification;

//----------------------------------------------------------------------------
// Gestion Interface
-(void)updateUI;

//----------------------------------------------------------------------------
// Interface XMapBox
-(void)prepareForClose;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
