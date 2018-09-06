//----------------------------------------------------------------------------
// File : bitmap_utils.h
// Project : MacMap
// Purpose : Header file : CGBitmapContext and CGImageRef utilities
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
// 04/08/2010 Création.
// 24/03/2015 Ajout CreateImageFromBitmapContext, MakeImageProperties et ImageType.
//----------------------------------------------------------------------------

#ifndef __bitmap_utils__
#define __bitmap_utils__

//----------------------------------------------------------------------------

typedef struct BMI{
	size_t	width;
	size_t	height;
	size_t	owidth;
	size_t	oheight;
	size_t	wscale;
	size_t	hscale;	
	size_t	size;
	void*	data;
}BMI;

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

bool			GetBitMap32BitPixel             (	BMI* bi,
                                                    size_t i,
                                                    size_t j, 
                                                    UInt32* px);	
bool			SetBitMap32BitPixel             (	BMI* bi,
                                                    size_t i,
                                                    size_t j,
                                                    UInt32 px);
CGContextRef	CreateBitmapContextWithImage    (	CGImageRef img);
CGContextRef	CreateBitmapContextWithSize     (	size_t width,
                                                    size_t height);
CGImageRef      CreateImageFromBitmapContext    (   CGContextRef ctx);
CFDictionaryRef MakeImageProperties             (   int res,
                                                    int q);
CFStringRef     ImageType                       (   UInt32 codec);
    
OSStatus		CGBitmapContextExport           (	CGContextRef ctx,
                                                    const char* path,
                                                    const char* name, 
                                                    UInt32 codec, 
                                                    int res, 
                                                    int q);
OSStatus		CGBitmapContextGetOutputData    (	CGContextRef ctx,
                                                    UInt32 codec,
                                                    int res, 
                                                    int q, 
                                                    void** data, 
                                                    size_t* sz);
					
//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
