//----------------------------------------------------------------------------
// File : StatRegression.mm
// Project : MacMap
// Purpose : Objective C++ source file : Regression classes for stats
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
/*
#import "StatRegression.h"
#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/C_Utils.h>
#import <mox_intf/xmldesc_utils.h>

// ---------------------------------------------------------------------------
// 
// -----------
@implementation StatRegression

// ---------------------------------------------------------------------------
// 
// -----------
-(id)init{
	self=[super init];
	_a=0;
	_b=0;
	_cr=0;
	_cg=0;
	_cb=0;
	_ca=1;
	_cs=1;
	_n=2;
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithBivariate:(mmx_biv*)biv{
	self=[super init];
	_a=0;
	_b=0;
	_cr=0;
	_cg=0;
	_cb=0;
	_ca=1;
	_cs=1;
	_n=2;
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setRed:(float)r
		green:(float)g
		 blue:(float)b
		alpha:(float)a
		width:(float)s{
	_cr=r;
	_cg=g;
	_cb=b;
	_ca=a;
	if(s<=0){
		s=1;
	}
	_cs=s;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)getRed:(float*)r
		green:(float*)g
		 blue:(float*)b
		alpha:(float*)a
		width:(float*)s{
	*r=_cr;
	*g=_cg;
	*b=_cb;
	*a=_ca;
	*s=_cs;	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawRect:(NSRect)rect
	 dataBounds:(dvx_rect)vbnds
		  xcoef:(double)rx
		  ycoef:(double)ry{
double			sx=(vbnds.xmax-vbnds.xmin)/(double)(_n-1);
double			x=vbnds.xmin;
NSBezierPath*	path=[NSBezierPath bezierPath];

	for(long i=0;i<_n;i++){
		if(i==0){
			[path moveToPoint:NSMakePoint(NSMinX(rect)+(x-vbnds.xmin)*rx,NSMinY(rect)+([self y:x]-vbnds.ymin)*ry)];
		}
		else{
			[path lineToPoint:NSMakePoint(NSMinX(rect)+(x-vbnds.xmin)*rx,NSMinY(rect)+([self y:x]-vbnds.ymin)*ry)];			
		}
		x+=sx;
	}
	[[NSColor colorWithDeviceRed:_cr green:_cg blue:_cb alpha:_ca] setStroke];
	[path setLineWidth:_cs];
	[path stroke];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dump:(bArray&)arr{
	add_cdesc(arr,1,"container","");

	add_idesc(arr,2,"int",[self regKind]);

	add_ddesc(arr,2,"float",_a,15);
	add_ddesc(arr,2,"float",_b,15);
	
	add_ddesc(arr,2,"float",_cr,2);
	add_ddesc(arr,2,"float",_cg,2);
	add_ddesc(arr,2,"float",_cb,2);
	add_ddesc(arr,2,"float",_ca,2);
	add_ddesc(arr,2,"float",_cs,2);
	add_idesc(arr,2,"int",_n);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)load:(bGenericXMLBaseElement*)root
		app:(bGenericMacMapApp*)gapp{
bGenericXMLBaseElement* elt;
char					val[_values_length_max_];
	
	elt=gapp->classMgr()->NthElement(root,1,"float");
	if(elt){
		elt->getvalue(val);
		_a=matof(val);
	}
	
	elt=gapp->classMgr()->NthElement(root,2,"float");
	if(elt){
		elt->getvalue(val);
		_b=matof(val);
	}
	
	elt=gapp->classMgr()->NthElement(root,3,"float");
	if(elt){
		elt->getvalue(val);
		_cr=matof(val);
	}
	elt=gapp->classMgr()->NthElement(root,4,"float");
	if(elt){
		elt->getvalue(val);
		_cg=matof(val);
	}
	elt=gapp->classMgr()->NthElement(root,5,"float");
	if(elt){
		elt->getvalue(val);
		_cb=matof(val);
	}
	elt=gapp->classMgr()->NthElement(root,6,"float");
	if(elt){
		elt->getvalue(val);
		_ca=matof(val);
	}
	elt=gapp->classMgr()->NthElement(root,7,"float");
	if(elt){
		elt->getvalue(val);
		_cs=matof(val);
	}

	elt=gapp->classMgr()->NthElement(root,2,"int");
	if(elt){
		elt->getvalue(val);
		_n=atoi(val);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
@end



// ---------------------------------------------------------------------------
// 
// -----------
@implementation LinearStatRegression

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithBivariate:(mmx_biv*)biv{
	self=[super init];
	linear_reg(biv,&_a,&_b);
	_n=2;
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)regKind{
	return kRegLinear;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)y:(double)x{
	return(_a*x+_b);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSString*)equation{
	if(_b==0){
		return [NSString stringWithFormat:@"y=%fx",_a];
	}
	else if(_b<0){
		return [NSString stringWithFormat:@"y=%fx - %f",_a,-_b];
	}
	else{
		return [NSString stringWithFormat:@"y=%fx + %f",_a,_b];
	}	
	return nil;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end




// ---------------------------------------------------------------------------
// 
// -----------
@implementation LogStatRegression

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithBivariate:(mmx_biv*)biv{
	self=[super init];
	logarithmic_reg(biv,&_a,&_b);
	_n=100;
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)regKind{
	return kRegLog;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)y:(double)x{
	return(_a*log(x)+_b);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSString*)equation{
	if(_b==0){
		return [NSString stringWithFormat:@"y=%fln(x)",_a];
	}
	else if(_b<0){
		return [NSString stringWithFormat:@"y=%fln(x) - %f",_a,-_b];
	}
	else{
		return [NSString stringWithFormat:@"y=%fln(x) + %f",_a,_b];
	}	
	return nil;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

*/
