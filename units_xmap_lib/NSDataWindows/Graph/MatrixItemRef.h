//----------------------------------------------------------------------------
// File : MatrixItemRef.h
// Project : MacMap
// Purpose : Header file : Matrix items classes (Geographics objects, columns)
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

/*#import <Cocoa/Cocoa.h>
#import "BrowserColumn.h"

//----------------------------------------------------------------------------
// Base classe
//----------------------------------------------------------------------------
@interface MatrixItemRef : NSObject{
	double	_value;
	int		_index;	
}

//----------------------------------------------------------------------------
-(id)initWithValue:(double)val
			 index:(long)idx;
-(double)value;
-(long)index;

//----------------------------------------------------------------------------
NSInteger itemSort		(id item1, 
						 id item2, 
						 void* context);

NSInteger itemSortInv	(id item1, 
						 id item2, 
						 void* context);

//----------------------------------------------------------------------------
@end

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
@interface MatrixObjectRef : MatrixItemRef{
	bGenericGeoElement*	_geo;
}

//----------------------------------------------------------------------------
-(id)initWithValue:(double)val
			 index:(long)idx
			   ref:(bGenericGeoElement*)geo;
-(bGenericGeoElement*)object;

//----------------------------------------------------------------------------
@end


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
@interface MatrixColumnRef : MatrixItemRef{
	BrowserColumn*	_col;
	CGFloat			_hr;
	CGFloat			_hg;
	CGFloat			_hb;
	CGFloat			_ha;
	CGFloat			_lr;
	CGFloat			_lg;
	CGFloat			_lb;
	CGFloat			_la;
}

//----------------------------------------------------------------------------
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
		  lowAlpha:(CGFloat)la;

-(void)setHighColor;
-(void)setLowColor;
-(BrowserColumn*)column;
-(void)setHighColorRed:(CGFloat)r
				 green:(CGFloat)g
				  blue:(CGFloat)b
				 alpha:(CGFloat)a;
-(void)setLowColorRed:(CGFloat)r
				green:(CGFloat)g
				 blue:(CGFloat)b
				alpha:(CGFloat)a;
-(void)highColorRed:(CGFloat*)r
			  green:(CGFloat*)g
			   blue:(CGFloat*)b
			  alpha:(CGFloat*)a;
-(void)lowColorRed:(CGFloat*)r
			 green:(CGFloat*)g
			  blue:(CGFloat*)b
			 alpha:(CGFloat*)a;

//----------------------------------------------------------------------------
@end*/

