//----------------------------------------------------------------------------
// File : CGUtils.h
// Project : MacMap
// Purpose : Header file : C utils around CGGeometry, CGImage and CGContext 
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
// 01/01/2002 creation.
//----------------------------------------------------------------------------

#ifndef __CGUTILS__
#define __CGUTILS__

//----------------------------------------------------------------------------

typedef struct intRect{
	int	left;
	int	top;
	int	right;
	int	bottom;
}intRect;

typedef struct intPoint{
	int	h;
	int	v;
}intPoint;

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

/*void CGPoint2PointWithRect						(	Rect *inRect,
													CGPoint *cgp, 
													Point *p);//CC*/

float CGPointsDist								(	CGPoint *a, 
													CGPoint *b);

/*void XYFloat2RectWithRect						(	Rect *inRect,
													float xmin, 
													float ymin, 
													float xmax, 
													float ymax, 
													Rect *r);*/
void XYFloat2RectWithCGRect						(	CGRect*	inRect, 
													float xmin, 
													float ymin, 
													float xmax, 
													float ymax, 
													Rect* r);

float XYDist									(	float ax, 
													float ay, 
													float bx, 
													float by);
double XYsDist									(	float* x,
													float* y,
													int n);
	
float XYDist2Vec								(	float x,	
													float y, 
													float ax, 
													float ay, 
													float bx, 
													float by);
float XYDist2Vecs								(	float	x, 
													float	y, 
													float	*xpts, 
													float	*ypts, 
													int		npts);
float XYDistMax2Vecs							(	float	x,	
													float	y, 
													float	*xpts, 
													float	*ypts, 
													int		npts);

int XYInBounds									(	float x, 
													float y, 
													float xmin, 
													float ymin, 
													float xmax, 
													float ymax);

void XYBounds									(	float*	xmin, 
													float*	ymin, 
													float*	xmax, 
													float*	ymax,
													float*	xpts, 
													float*	ypts, 
													int		npts);
									
int XYInClosedPath								(	float	x, 
													float	y, 
													float*	xpts, 
													float*	ypts, 
													int		npts,
													int		limit);

void CentroidizeClosedPath						(	float*	x, 
													float*	y, 
													float*	xpts, 
													float*	ypts, 
													int		npts);

void CentroidizeLinePath						(	float*	x, 
													float*	y, 
													float*	xpts, 
													float*	ypts, 
													int		npts);

void Centroidize								(	float*	x, 
													float*	y, 
													float*	xpts, 
													float*	ypts, 
													int		npts);

float CGPointVec2Angle							(	CGPoint* pt1,
													CGPoint* pt2);

float XYVec2Angle								(	float x1, 
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
													int		npts, 
													float	dst,
													float**	xnew, 
													float**	ynew,
													int* 	nnew);

float XYProjOnSeg								(	float	inx,
													float	iny,
													float	xa,
													float	ya,
													float	xb,
													float	yb,
													float*	outx,
													float*	outy,
													double* r);
float XYProjOnPath								(	float	inx,
													float	iny,
													float*	pathx,
													float*	pathy,
													int		pathn,
													float*	outx,
													float*	outy,
													int*	idx,	// in [0..n-1]
													float*	r);		// if <=0 or >=1, vr exists in vs
																								
int XYSIAM										(	float x0, 
													float y0, 
													float x1, 
													float y1, 
													float x2, 
													float y2);

int XYLegible									(	float*	xpts, 
													float*	ypts, 
													int		npts);
												
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
													int* outsz);

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
	
	
//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
