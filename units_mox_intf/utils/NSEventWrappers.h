//----------------------------------------------------------------------------
// File : NSEventWrappers.h
// Project : MacMap
// Purpose : Header file : NSEvent C wrappers
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
// 26/03/2014 creation.
//----------------------------------------------------------------------------

#ifndef __NSEventWrappers__
#define __NSEventWrappers__

//----------------------------------------------------------------------------

/* various types of events */
enum {        
	NSEvent_typeLeftMouseDown             = 1,            
	NSEvent_typeLeftMouseUp               = 2,
	NSEvent_typeRightMouseDown            = 3,
	NSEvent_typeRightMouseUp              = 4,
	NSEvent_typeMouseMoved                = 5,
	NSEvent_typeLeftMouseDragged          = 6,
	NSEvent_typeRightMouseDragged         = 7,
	NSEvent_typeMouseEntered              = 8,
	NSEvent_typeMouseExited               = 9,
	NSEvent_typeKeyDown                   = 10,
	NSEvent_typeKeyUp                     = 11,
	NSEvent_typeFlagsChanged              = 12,
	NSEvent_typeAppKitDefined             = 13,
	NSEvent_typeSystemDefined             = 14,
	NSEvent_typeApplicationDefined        = 15,
	NSEvent_typePeriodic                  = 16,
	NSEvent_typeCursorUpdate              = 17,
	NSEvent_typeScrollWheel               = 22,
	NSEvent_typeTabletPoint               = 23,
	NSEvent_typeTabletProximity           = 24,
	NSEvent_typeOtherMouseDown            = 25,
	NSEvent_typeOtherMouseUp              = 26,
	NSEvent_typeOtherMouseDragged         = 27
};

typedef NSUInteger NSEventType;

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

/* these messages are valid for all events */
long			NSEvent_type                        (void* nsevt);
long			NSEvent_modifierFlags               (void* nsevt);
void*			NSEvent_window                      (void* nsevt);
WindowRef		NSEvent_windowAsWindowRef           (void* nsevt);
	
/* these messages are valid for all mouse down/up/drag events */
long			NSEvent_clickCount                  (void* nsevt);
long			NSEvent_buttonNumber                (void* nsevt);

/* These messages are valid for all mouse down/up/drag events */
/* These messages are also valid for NSTabletPoint events on 10.4 or later */
CGPoint			NSEvent_locationInWindow            (void* nsevt);

/* these messages are valid for keyup and keydown events */
bool			NSEvent_isARepeat                   (void* nsevt);
/* these messages are valid for keyup, keydown and flagschanged events */
unsigned short	NSEvent_keyCode                     (void* nsevt);
char            NSEvent_characters                  (void* nsevt);
char            NSEvent_charactersIgnoringModifiers (void* nsevt);
    
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
