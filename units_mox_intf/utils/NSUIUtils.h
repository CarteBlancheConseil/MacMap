//----------------------------------------------------------------------------
// File : NSUIUtils.h
// Project : MacMap
// Purpose : Header file : Cocoa interface utils
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
// 31/03/2011 Creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <mox_intf/bGenericType.h>
#import <mox_intf/bGenericUnit.h>
#import <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

@interface NSControl (NSControlAdditions)

//----------------------------------------------------------------------------

-(void)setCharValue:(const char*)aString;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
	
//----------------------------------------------------------------------------
	
void		NSPopupButtonRemoveAllItems					(	NSPopUpButton* c);
void		NSPopupButtonRemoveItemsFrom				(	NSPopUpButton* c, 
															NSInteger from);
long		NSPopupButtonGetMenuItemValue				(	NSPopUpButton* c, 
															long idx,
															char* str, 
															size_t max);
void		NSPopupButtonAddMenuItemValue				(	NSPopUpButton* c, 
															const char* str);

void		NSPopupButtonPopulateWithFields				(	NSPopUpButton* c, 
                                                            bGenericType* tp,
															long start,				// First field index
															long current);			// Checked field in [1..n]
void		NSPopupButtonPopulateWithTypes				(	NSPopUpButton* c, 
															bGenericMacMapApp* gapp,
															long kind,				// Type kind
															long current);			// Checked type in [1..n]
void		NSPopupButtonPopulateWithScales				(	NSPopUpButton* c, 
															bGenericMacMapApp* gapp,
															long current);			// Checked scale in [1..n]
void		NSPopupButtonPopulateWithUnits				(	NSPopUpButton* c,
                                                            bGenericMacMapApp* gapp,
                                                            long current);			// Checked unit in [1..n]
void		NSPopupButtonPopulateWithConstrainedFields	(	NSPopUpButton* c,
															bGenericType* tp, 
															long start, 
															long current);
void		NSPopupButtonPopulateWithDashes				(	NSPopUpButton* c,	
															bGenericType* tp, 
															long current);
	
void		NSPopupButtonMenuItemEnable					(	NSPopUpButton* c, 
															long index);
void		NSPopupButtonMenuItemDisable				(	NSPopUpButton* c, 
															long index);
long		NSPopupButtonMenuItemGetTag					(	NSPopUpButton* c, 
															long index);
void		NSPopupButtonMenuItemSetTitle				(	NSPopUpButton* c, 
															long index,
															const char* title);
	
long		NSTextFieldGetValue							(	NSTextField* c, 
															char* str, 
															size_t max);
void		NSTextFieldSetValue							(	NSTextField* c, 
															const char* str);

NSImage*	GetNSImagePattern							(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															const char* name);

CGRect		NSMainSreenBounds							(	);

CGFloat		NSMenuBarHeight								(	);
		
//----------------------------------------------------------------------------
	
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

