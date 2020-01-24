//----------------------------------------------------------------------------
// File : Color_Utils.c
// Project : MacMap
// Purpose : C source file : Color utilities
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
// 03/02/2010 creation.
//----------------------------------------------------------------------------

#include "Color_Utils.h"
#include "Carb_Utils.h"

// ---------------------------------------------------------------------------
// 
// -----------
void	RGB2CMYK64(double r, 
				   double g, 
				   double b,
				   double* c, 
				   double* m, 
				   double* y,
				   double* k){
	*c=1.0-r;
	*m=1.0-g;
	*y=1.0-b;
	*k=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void	CMYK2RGB64(double c, 
				   double m, 
				   double y,
				   double k,
				   double* r, 
				   double* g, 
				   double* b){
double	_k,_c,_m,_y;
	
	_k=k;
	if(_k<0){
		_k+=1.0;
	}
	_c=c;
	if(_c<0){
		_c+=1.0;
	}
	_m=m;
	if(_m<0){
		_m+=1.0;
	}
	_y=y;
	if(_y<0){
		_y+=1.0;
	}
	*r=1.0-((_c+_k)<1.0?(_c+_k):1.0);
	*g=1.0-((_m+_k)<1.0?(_m+_k):1.0);
	*b=1.0-((_y+_k)<1.0?(_y+_k):1.0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void compRGB2CMYK64(double* components){
double	buf=components[3];
	RGB2CMYK64(components[0],
			   components[1],
			   components[2],
			   &components[0],
			   &components[1],
			   &components[2],
			   &components[3]);
	components[4]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void compCMYK2RGB64(double* components){
double	buf=components[4];
	CMYK2RGB64(components[0],
			   components[1],
			   components[2],
			   components[3],
			   &components[0],
			   &components[1],
			   &components[2]);
	components[3]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetFillColor64(	CGContextRef c, 
							   double* components, 
							   bool cmyk){
	if(cmyk){
		CGContextSetCMYKFillColor(	c,
								  components[0],
								  components[1],
								  components[2],
								  components[3],
								  components[4]);
	}
	else{
		CGContextSetRGBFillColor(	c,
								 components[0],
								 components[1],
								 components[2],
								 components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetStrokeColor64(	CGContextRef c, 
								 double* components, 
								 bool cmyk){
	if(cmyk){
		CGContextSetCMYKStrokeColor(c,
									components[0],
									components[1],
									components[2],
									components[3],
									components[4]);
	}
	else{
		CGContextSetRGBStrokeColor(c,
								   components[0],
								   components[1],
								   components[2],
								   components[3]);
	}
}

