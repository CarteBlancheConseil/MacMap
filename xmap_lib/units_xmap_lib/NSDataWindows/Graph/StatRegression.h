//----------------------------------------------------------------------------
// File : StatRegression.h
// Project : MacMap
// Purpose : Header file : Regression classes for stats
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
// 23/06/2014 creation.
//----------------------------------------------------------------------------

/*#import <Cocoa/Cocoa.h>
#import <mox_intf/bGenericMacMapApp.h>
#import <MacMapSuite/vx.h>
#import <MacMapSuite/bArray.h>
#import <MacMapStats/MMsAnalysis.h>

//----------------------------------------------------------------------------
enum{
	kRegLinear,
	kRegLog,
};

//----------------------------------------------------------------------------
// Base classe
//----------------------------------------------------------------------------
@interface StatRegression : NSObject{
	double	_a;
	double	_b;
	
	float	_cr; 
	float	_cg; 
	float	_cb;
	float	_ca;
	float	_cs;	

	long	_n;
	long	_lid;
}

//----------------------------------------------------------------------------
-(id)init;
-(id)initWithBivariate:(mmx_biv*)biv;

-(double)y:(double)x;
-(NSString*)equation;

-(long)regKind;

-(void)setRed:(float)r
		green:(float)g
		 blue:(float)b
		alpha:(float)a
		width:(float)s;

-(void)getRed:(float*)r
		green:(float*)g
		 blue:(float*)b
		alpha:(float*)a
		width:(float*)s;

-(void)drawRect:(NSRect)rect
	 dataBounds:(dvx_rect)vbnds
		  xcoef:(double)rx
		  ycoef:(double)ry;

-(void)dump:(bArray&)arr;
-(void)load:(bGenericXMLBaseElement*)root
		app:(bGenericMacMapApp*)gapp;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
@interface LinearStatRegression : StatRegression{
}

@end

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
@interface LogStatRegression : StatRegression{
}

@end

//----------------------------------------------------------------------------
*/