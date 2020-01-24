//----------------------------------------------------------------------------
// File : NSEventWrappers.m
// Project : MacMap
// Purpose : Objective C source file : NSEvent C wrappers
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

#import <Cocoa/Cocoa.h>
#import "NSEventWrappers.h"

// ---------------------------------------------------------------------------
// 
// ------------
long NSEvent_type(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt type]);
}

// ---------------------------------------------------------------------------
//
// ------------
long NSEvent_subtype(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
    return([evt subtype]);
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSEvent_modifierFlags(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt modifierFlags]);
}

// ---------------------------------------------------------------------------
// 
// ------------
void* NSEvent_window(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return((__bridge void*)[evt window]);
}

// ---------------------------------------------------------------------------
// 
// ------------
WindowRef NSEvent_windowAsWindowRef(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return((WindowRef)[[evt window] windowRef]);
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSEvent_clickCount(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt clickCount]);
}

// ---------------------------------------------------------------------------
// 
// ------------
long NSEvent_buttonNumber(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt buttonNumber]);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPoint NSEvent_locationInWindow(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return(NSPointToCGPoint([evt locationInWindow]));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool NSEvent_isARepeat(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt isARepeat]);
}

// ---------------------------------------------------------------------------
// 
// ------------
unsigned short NSEvent_keyCode(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
	return([evt keyCode]);
}

// ---------------------------------------------------------------------------
//
// ------------
char NSEvent_characters(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
NSString*   nsstr=[evt characters];
    
    return([nsstr characterAtIndex:0]);
}

// ---------------------------------------------------------------------------
//
// ------------
char NSEvent_charactersIgnoringModifiers(void* nsevt){
NSEvent*	evt=(__bridge NSEvent*)nsevt;
NSString*   nsstr=[evt charactersIgnoringModifiers];
    
    return([nsstr characterAtIndex:0]);
}
