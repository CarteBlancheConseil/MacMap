//----------------------------------------------------------------------------
// File : bStdNSAppModalWindowController.h
// Project : MacMap
// Purpose : Header file : Base class for Cocoa modal window based XMap
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
// 15/06/2011 creation.
// 31/01/2017 tool modal window.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <std_ext/bStdExt.h>

//----------------------------------------------------------------------------

@interface bStdNSAppModalWindowController : NSWindowController{
	bStdExt*            _ext;
	long				_code;

	IBOutlet NSButton*	_okbtn;	
	IBOutlet NSButton*	_cancelbtn;	
}

//----------------------------------------------------------------------------
// Std 
-(id)initWithExt:(bStdExt*)ext;
-(void)runAppModal:(long*)code;

//----------------------------------------------------------------------------
// Validation Dialogue 
-(IBAction)validDialog:(id)sender;
-(IBAction)cancelDialog:(id)sender;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

@interface bStdNSAppToolModalWindowController : bStdNSAppModalWindowController{
}

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
