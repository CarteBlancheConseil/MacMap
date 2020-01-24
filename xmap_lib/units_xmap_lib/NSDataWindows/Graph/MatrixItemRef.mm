//----------------------------------------------------------------------------
// File : MatrixItemRef.mm
// Project : MacMap
// Purpose : Objective C++ source file : Matrix items classes (Geographics objects, columns)
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
// 07/07/2014 creation.
//----------------------------------------------------------------------------

/*#import "MatrixItemRef.h"
#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/C_Utils.h>
#import <mox_intf/xmldesc_utils.h>

// ---------------------------------------------------------------------------
// 
// -----------
@implementation MatrixItemRef

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithValue:(double)val
			 index:(long)idx{
	self=[super init];
	_value=val;
	_index=idx;
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)dealloc{
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)value{
	return _value;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)index{
	return _index;
}

// ---------------------------------------------------------------------------
// 
// -----------
NSInteger itemSort(id item1, 
				   id item2, 
				   void *context){
MatrixItemRef* v1=item1;
MatrixItemRef* v2=item2;
	
	if([v1 value]<[v2 value])
        return NSOrderedAscending;
    else if([v1 value]>[v2 value])
        return NSOrderedDescending;
    else
        return NSOrderedSame;
}

// ---------------------------------------------------------------------------
// 
// -----------
NSInteger itemSortInv(id item1, 
					  id item2, 
					  void *context){
	return -itemSort(item1,item2,context);
}

// ---------------------------------------------------------------------------
// 
// -----------
@end


// ---------------------------------------------------------------------------
// 
// -----------
@implementation MatrixObjectRef

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithValue:(double)val
			 index:(long)idx
			   ref:(bGenericGeoElement*)geo{
	self=[super initWithValue:val index:idx];
	_geo=geo;
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(bGenericGeoElement*)object{
	return _geo;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end



// ---------------------------------------------------------------------------
// 
// -----------
@implementation MatrixColumnRef

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithValue:(double)val
			 index:(long)idx
			   ref:(BrowserColumn*)col
		   highRed:(CGFloat)hr
		 highGreen:(CGFloat)hg
		  highBlue:(CGFloat)hb
		 highAlpha:(CGFloat)ha
			lowRed:(CGFloat)lr
		  lowGreen:(CGFloat)lg
		   lowBlue:(CGFloat)lb
		  lowAlpha:(CGFloat)la{
	self=[super initWithValue:val index:idx];
	_col=col;
	_hr=hr;
	_hg=hg;
	_hb=hb;
	_ha=ha;
	_lr=lr;
	_lg=lg;
	_lb=lb;
	_la=la;
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setHighColor{
NSColor*	clr=[NSColor colorWithDeviceRed:_hr green:_hg blue:_hb alpha:_ha];
	[clr setFill];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setLowColor{
NSColor*	clr=[NSColor colorWithDeviceRed:_lr green:_lg blue:_lb alpha:_la];
	[clr setFill];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BrowserColumn*)column{
	return _col;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setHighColorRed:(CGFloat)r
				 green:(CGFloat)g
				  blue:(CGFloat)b
				 alpha:(CGFloat)a{
	_hr=r;
	_hg=g;
	_hb=b;
	_ha=a;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setLowColorRed:(CGFloat)r
				green:(CGFloat)g
				 blue:(CGFloat)b
				alpha:(CGFloat)a{
	_lr=r;
	_lg=g;
	_lb=b;
	_la=a;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)highColorRed:(CGFloat*)r
			  green:(CGFloat*)g
			   blue:(CGFloat*)b
			  alpha:(CGFloat*)a{
	*r=_hr;
	*g=_hg;
	*b=_hb;
	*a=_ha;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)lowColorRed:(CGFloat*)r
			 green:(CGFloat*)g
			  blue:(CGFloat*)b
			 alpha:(CGFloat*)a{
	*r=_lr;
	*g=_lg;
	*b=_lb;
	*a=_la;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end*/

