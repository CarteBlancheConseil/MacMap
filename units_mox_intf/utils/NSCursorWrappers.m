//----------------------------------------------------------------------------
// File : NSCursorWrappers.m
// Project : MacMap
// Purpose : Objective C source file : NSCursor C wrappers
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
// 11/07/2007 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import "NSCursorWrappers.h"

// ---------------------------------------------------------------------------
//	From "Cocoa Drawing Guide: Working with Images"
// ------------
NSImage	*CreateNSImageFromCGImage(CGImageRef image){
NSRect			imageRect=NSMakeRect(0.0, 0.0, 0.0, 0.0);

// Get the image dimensions.
	imageRect.size.height=CGImageGetHeight(image);
	imageRect.size.width=CGImageGetWidth(image);

// Create a new image to receive the Quartz image data.
NSImage	*newImage=[[NSImage alloc] initWithSize:imageRect.size]; 
	[newImage lockFocus];

// Get the Quartz context and draw.
CGContextRef	imageContext=(CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
	CGContextDrawImage(imageContext,*(CGRect*)&imageRect,image);
	[newImage unlockFocus];
	return(newImage);
}

// ---------------------------------------------------------------------------
//	C-based style wrapper routines around NSCursor
// ------------
CursorRef CreateCocoaCursor(CGImageRef cgImageRef, 
							float hotSpotX, 
							float hotSpotY){
static BOOL	firstTime=YES;
	
	if(firstTime){
// Must first call [[[NSWindow alloc] init] release] to get the NSWindow 
// machinery set up so that NSCursor can use a window to cache the cursor image
		[[[NSWindow alloc] init] release];
		firstTime=NO;
	}
	
NSImage		*nsImage=CreateNSImageFromCGImage(cgImageRef);
NSCursor	*cursor=[[NSCursor alloc] initWithImage:nsImage hotSpot:NSMakePoint(hotSpotX,hotSpotY)];
	
	[nsImage release];
	return((CursorRef)cursor);
}

// ---------------------------------------------------------------------------
//	
// ------------
void ReleaseCocoaCursor(CursorRef cursor){
	[(NSCursor*)cursor release];
}

// ---------------------------------------------------------------------------
//	
// ------------
void SetCocoaCursor(CursorRef cursor){
	[(NSCursor*)cursor set];
}

// ---------------------------------------------------------------------------
//	
// ------------
void HideCocoaCursor(){
	[NSCursor hide];
}

// ---------------------------------------------------------------------------
//	
// ------------
void ShowCocoaCursor(){
	[NSCursor unhide];
}

// ---------------------------------------------------------------------------
//
// ------------
void MMBeep(){
    NSBeep();
}

