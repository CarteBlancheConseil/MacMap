//----------------------------------------------------------------------------
// File : Color_Utils.h
// Project : MacMap
// Purpose : Header file : Color utilities
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

#ifndef _Color_Utils_
#define _Color_Utils_

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Code pour carbon	A VIRER
OSStatus	PickRGBColor				(	float* r, 
											float* g, 
											float* b);

OSStatus	PickCMYKColor				(	float* c, 
											float* m, 
											float* y,
											float* k);

//----------------------------------------------------------------------------
// 32 bits
void		RGB2CMYK					(	float r, 
											float g, 
											float b,
											float* c, 
											float* m, 
											float* y,
											float* k);
void		CMYK2RGB					(	float c, 
											float m, 
											float y,
											float k,
											float* r, 
											float* g, 
											float* b);		
void		compRGB2CMYK				(	float* components);
void		compCMYK2RGB				(	float* components);
void		MMCGContextSetFillColor		(	CGContextRef c, 
											float* components, 
											bool cmyk);
void		MMCGContextSetStrokeColor	(	CGContextRef c, 
											float* components, 
											bool cmyk);
	


//----------------------------------------------------------------------------
// 64 bits
void		RGB2CMYK64					(	double r, 
											double g, 
											double b,
											double* c, 
											double* m, 
											double* y,
											double* k);
void		CMYK2RGB64					(	double c, 
											double m, 
											double y,
											double k,
											double* r, 
											double* g, 
											double* b);
void		compRGB2CMYK64				(	double* components);
void		compCMYK2RGB64				(	double* components);
void		MMCGContextSetFillColor64	(	CGContextRef c, 
											double* components, 
											bool cmyk);
void		MMCGContextSetStrokeColor64	(	CGContextRef c, 
											double* components, 
											bool cmyk);
	
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
