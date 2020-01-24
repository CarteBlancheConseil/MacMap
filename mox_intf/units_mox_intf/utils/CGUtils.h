//----------------------------------------------------------------------------
// File : CGUtils.h
// Project : MacMap
// Purpose : Header file : C utils around CGGeometry, CGImage and CGContext 
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
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
// 01/01/2002 creation.
// 30/11/2018 64bits.
//----------------------------------------------------------------------------

#ifndef __CGUTILS__
#define __CGUTILS__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

CGFloat CGPointsDist							(	CGPoint *a,
													CGPoint *b);

void XYFloat2RectWithCGRect						(	CGRect*	inRect,
													float xmin, 
													float ymin, 
													float xmax, 
													float ymax, 
													Rect* r);

CGFloat XYDist									(	float ax,
													float ay, 
													float bx, 
													float by);
double XYsDist									(	float* x,
													float* y,
													long n);
	
CGFloat XYDist2Vec								(	float x,	
													float y, 
													float ax, 
													float ay, 
													float bx, 
													float by);
CGFloat XYDist2Vecs								(	float x,
													float y,
													float *xpts,
													float *ypts,
													long npts);
CGFloat XYDistMax2Vecs							(	float x,
													float y,
													float *xpts,
													float *ypts,
													long npts);

int XYInBounds									(	float x, 
													float y, 
													float xmin, 
													float ymin, 
													float xmax, 
													float ymax);

void XYBounds									(	float* xmin,
													float* ymin,
													float* xmax,
													float* ymax,
													float* xpts,
													float* ypts,
													long npts);
									
int XYInClosedPath								(	float x,
													float y,
													float* xpts,
													float* ypts,
													long npts,
													long limit);

void CentroidizeClosedPath						(	float* x,
													float* y,
													float* xpts,
													float* ypts,
													long npts);

void CentroidizeLinePath						(	float* x,
													float* y,
													float* xpts,
													float* ypts,
													long npts);

void Centroidize								(	float* x,
													float* y,
													float* xpts,
													float* ypts,
													long npts);

CGFloat CGPointVec2Angle						(	CGPoint* pt1,
													CGPoint* pt2);

CGFloat XYVec2Angle								(	float x1,
													float y1, 
													float x2, 
													float y2);
									
void CGPointNewPos								(	CGPoint* o, 
													float d, 
													float angle, 
													CGPoint* r);
									
void XYNewPos									(	float ox,
													float oy,
													float d, 
													float angle, 
													float* rx,
													float* ry);

void XYDecal									(	float*	xpts, 
													float*	ypts, 
													long	npts,
													float	dst,
													float**	xnew, 
													float**	ynew,
													int* 	nnew);

CGFloat XYProjOnSeg								(	float	inx,
													float	iny,
													float	xa,
													float	ya,
													float	xb,
													float	yb,
													float*	outx,
													float*	outy,
													double* r);
CGFloat XYProjOnPath							(	float	inx,
													float	iny,
													float*	pathx,
													float*	pathy,
													long	pathn,
													float*	outx,
													float*	outy,
													int*	idx,	// in [0..n-1]
													float*	r);		// if <=0 or >=1, vr exists in vs
																								
long XYSIAM										(	float x0,
													float y0, 
													float x1, 
													float y1, 
													float x2, 
													float y2);

long XYLegible									(	float*	xpts,
													float*	ypts, 
													long	npts);
												
CGRect CGScaleRect								(	CGRect bound, 
													CGRect r);
					
CGImageRef CGImageCreateFromData				(	void* data, 
													int sz,
													OSType unused);
OSStatus GetImageInfoFromData					(	void* data, 
													int sz, 
													OSType unused, 
													int* hsz,
													int* vsz,
													double* hr, 
													double* vr);

CGPDFDocumentRef PDFCreate						(	void* data, 
													int sz);
OSStatus PDFConvert								(	void* data, 
													int sz, 
													UInt32 codec,
													void** outdata, 
													size_t* outsz);

void CGContextAddRoundedRect					(	CGContextRef c,
													CGRect rect,
													float radius);

void CGContextShowStringAtPoint					(	CGContextRef c,
													CGFloat	x,
													CGFloat	y,
													const char* str,
													const char* font,
													CGFloat size);

void CGContextSetFillColorWithComponents64		(	CGContextRef c, 
													double* components, 
													bool cmyk);
void CGContextSetStrokeColorWithComponents64	(	CGContextRef c, 
													double* components, 
													bool cmyk);
void CGContextSetEmptyStyle						(	CGContextRef c, 
													CGRect rect);
    
void PDFPatternPlot                             (	void* info,
                                                    CGContextRef ctx);
CGImageRef LoadPNGImageFromBundle               (	CFBundleRef bndl,
                                                    CFStringRef name);
    
void MacRoman2UTF8                              (	char* str,
                                                    int sz);
void UTF82MacRoman                              (	char* str,
                                                    int sz);

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
