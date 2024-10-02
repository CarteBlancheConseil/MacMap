//----------------------------------------------------------------------------
// File : vDefPalettePreview.mm
// Project : MacMap
// Purpose : Objective C++ source file : Cocoa vDef preview
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
// 29/06/2011 creation.
//----------------------------------------------------------------------------

#import "vDefPalettePreview.h"
#import <MacMapSuite/bTrace.h> 
#import <Foundation/NSGeometry.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation vDefPalettePreview

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[vDefPalettePreview initWithFrame]",true);
_tm_(_trxysz_(frameRect));
	if((self=[super initWithFrame:frameRect])!=nil){
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
/*-(void)dealloc{
_bTrace_("[vDefPalettePreview dealloc]",true);
	[super dealloc];
}*/

// ---------------------------------------------------------------------------
// 
// ------------
-(void)installController:(bStdNSPalettevDefWindowController*)controller{
	_ctrlr=controller;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawRect:(NSRect)rect{
CGRect              cgr=NSRectToCGRect([self bounds]);
NSGraphicsContext*	nsctx=[NSGraphicsContext currentContext];
CGContextRef		ctx=(CGContextRef)[nsctx graphicsPort];

	CGContextSetRGBFillColor(ctx,1,1,1,1);
	CGContextSetRGBStrokeColor(ctx,0.5,0.5,0.5,1);
	CGContextSetLineWidth(ctx,0.5);
	CGContextAddRect(ctx,cgr);
	CGContextDrawPath(ctx,kCGPathFillStroke);

	[_ctrlr drawPreview:cgr context:ctx];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{
NSPoint	nsp=[event locationInWindow];
	nsp=[self convertPointFromBase:nsp];
NSUInteger	modi=[event modifierFlags];
	[_ctrlr clickPreview:CGPointMake(nsp.x,nsp.y) modifiers:modi];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end





