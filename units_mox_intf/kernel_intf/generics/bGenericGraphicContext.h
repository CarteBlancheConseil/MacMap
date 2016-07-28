//----------------------------------------------------------------------------
// File : bGenericGraphicContext.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap graphic context
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
// Classe de base pour dessin des objets
// Encodage MacRoman
//----------------------------------------------------------------------------
// 20/05/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericGraphicContext__
#define __bGenericGraphicContext__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericContext.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericScreenObj.h>
#include <mox_intf/bCoreTextInfo.h>

//----------------------------------------------------------------------------

enum{
	_red			=0,
	_green			=1,
	_blue			=2,
	_cyan			=0,
	_magenta		=1,
	_yellow			=2,
	_black			=3,
	_alpha			=4
};

enum{
	_rgb			=0,
	_cmyk			=1
};

enum{
	_stroke			=0,
	_fill			=5,
	_backfill		=10,
	_backstroke		=15
};

enum{
	kv1LineBreak	='#',   // 35
	kv2LineBreak	='¦',   // 166 ?
	kMacLineBreak	=13,    // Carriage return
	kUnixLineBreak	=10,    // Line feed
	kTextSpace		=' ',   // 32
	kTextTrunc		='É'    // 201 ?
};

enum{
//	kCGGraphicContext		='CG__',
	kPDFGraphicContext		='PDF_',
	kPrintGraphicContext	='PRNT',
	kBitMapGraphicContext	='BMP_',
	kKMLGraphicContext		='KML_',
	kCtxGraphicContext		='Ctx_',
    kCGGraphicContext		=kCtxGraphicContext,
    kSVGGraphicContext		='SVG_'
};

//----------------------------------------------------------------------------

class bGenericGraphicContext : public bGenericContext{
	public:	
		virtual ~bGenericGraphicContext				(	){};
				
// Colors		
		virtual void setRed							(	float c)=0;		// get via getColor
		virtual void setGreen						(	float c)=0;		// get via getColor
		virtual void setBlue						(	float c)=0;		// get via getColor
		virtual void setCyan						(	float c)=0;		// get via getColor
		virtual void setMagenta						(	float c)=0;		// get via getColor
		virtual void setYellow						(	float c)=0;		// get via getColor
		virtual void setBlack						(	float c)=0;		// get via getColor
		virtual void setAlpha						(	float c)=0;		// get via getColor
		virtual void setColorSpace					(	int space)=0;	// get via getColor
		virtual void setColorLevel					(	int level)=0;
		virtual void plotColor						(	)=0;
		virtual float* getColor						(	int* space,
														int level)=0;
														
// Size, Width, Shape
		virtual void setWidth						(	float width)=0;
		virtual float getWidth						(	)=0;
		virtual void setSize						(	float size)=0;
		virtual void setHSize						(	float size)=0;
		virtual float getHSize						(	)=0;
		virtual void setVSize						(	float size)=0;
		virtual float getVSize						(	)=0;
		virtual void setMiterLimit					(	float limit)=0;
		virtual float getMiterLimit					(	)=0;
		virtual void setJoin						(	int join)=0;
		virtual int getJoin							(	)=0;
		virtual void setCap							(	int cap)=0;
		virtual int getCap							(	)=0;
		virtual void setAngle						(	float angle)=0;
		virtual float getAngle						(	)=0;
		virtual void setStartAngle					(	float angle)=0;
		virtual float getStartAngle					(	)=0;
		virtual void setEndAngle					(	float angle)=0;
		virtual float getEndAngle					(	)=0;
		virtual void setRoundRadius					(	float radius)=0;
		virtual float getRoundRadius				(	)=0;
		
// Background
		virtual void setBackgroundWidthMaj			(	float maj)=0;
		virtual float getBackgroundWidthMaj			(	)=0;
		virtual void setBackgroundHeightMaj			(	float maj)=0;
		virtual float getBackgroundHeightMaj		(	)=0;
		virtual void setBackgroundWidthDec			(	float dec)=0;
		virtual float getBackgroundWidthDec			(	)=0;
		virtual void setBackgroundHeightDec			(	float dec)=0;
		virtual float getBackgroundHeightDec		(	)=0;
		virtual void setBackgroundWidth				(	float width)=0;
		virtual float getBackgroundWidth			(	)=0;	

// Text		
		virtual void setCharSpacing					(	float	space)=0;
		virtual float getCharSpacing				(	)=0;
		virtual void setWordSpacing					(	float	space)=0;
		virtual float getWordSpacing				(	)=0;
		virtual void setLineSpacing					(	float	space)=0;
		virtual float getLineSpacing				(	)=0;
		virtual void setJustification				(	float	just)=0;
		virtual float getJustification				(	)=0;
		virtual void setFont						(	const char* font)=0;
		virtual char* getFont						(	)=0;
		virtual void setText						(	const char* text)=0;
		virtual bCoreTextInfo* getText				(	)=0;
		
// Icon & Pic
		virtual void setPDF							(	void* data,
														int sz,
														const char* name)=0;
		virtual CGPDFDocumentRef getPDF				(	)=0;
		virtual void setImage						(	void* data, 
														int sz,
														const char* name)=0;
		virtual CGImageRef getImage					(	)=0;

// Dash & Pattern
		virtual void setDash						(	float* dash, 
														int ndash,
														const char* name)=0;
		virtual float* getDash						(	int* ndash)=0;
		virtual void setFillPattern					(	void* data, 
														int sz,
														const char* name)=0;
		virtual CGPDFDocumentRef getFillPattern		(	)=0;
		virtual void setStrokePattern				(	void* data, 
														int sz,
														const char* name)=0;
		virtual CGPDFDocumentRef getStrokePattern	(	)=0;

// Scales		
		virtual void setScaleRef					(	double scaleref)=0;
		virtual double getScaleRef					(	)=0;
		virtual void setUnitCoef					(	float coef)=0;
		virtual float getUnitCoef					(	)=0;		
		virtual void setScale						(	double scale)=0;	// Current scale
		virtual double getScale						(	)=0;				// Current scale
		virtual double getFixConv					(	)=0;		
		
// Geography		
		virtual void getGeography					(	ivertices **vxs, 
														bool *disposeit)=0;
		virtual void setGeography					(	ivertices *vxs, 
														bool disposeit)=0;
		virtual void convertGeography				(	)=0;
		
// Geometry		
		virtual void getGeometry					(	float **xpts, 
														float **ypts, 
														int *npts, 
														int **offsets, 
														int *noffsets)=0;
		virtual void setGeometry					(	float *xpts, 
														float *ypts, 
														int npts, 
														int *offsets, 
														int noffsets)=0;
		virtual void getGeometry					(	float **xpts, 
														float **ypts, 
														float **zpts, 
														int *npts, 
														int **offsets, 
														int *noffsets)=0;
		virtual void setGeometry					(	float *xpts, 
														float *ypts, 
														float *zpts, 
														int npts, 
														int *offsets, 
														int noffsets)=0;		
		
// Object
		virtual bGenericGeoElement* getCurElement	(	)=0;

		virtual void getCurElementBounds			(	float	*xmin,
														float	*ymin,
														float	*xmax,	
														float	*ymax)=0;
		virtual void setCurElementBounds			(	float	xmin,
														float	ymin,
														float	xmax,	
														float	ymax)=0;

// Draw procs
		virtual void drawIcon						(	)=0;	
		virtual void drawCircle						(	)=0;
		virtual void drawRect						(	)=0;	
		virtual void drawText						(	)=0;
		virtual void drawLine						(	)=0;
		virtual void drawSurf						(	)=0;
		virtual void drawRaster						(	)=0;
		
// Dist procs		
		virtual float distToIcon					(	CGPoint cgp)=0;
		virtual float distToCircle					(	CGPoint cgp)=0;
		virtual float distToRect					(	CGPoint cgp)=0;
		virtual float distToText					(	CGPoint cgp)=0;
		virtual float distToLine					(	CGPoint cgp)=0;
		virtual float distToSurf					(	CGPoint cgp)=0;
		virtual float distToRaster					(	CGPoint cgp)=0;
		
// Bound procs		
		virtual void boundsForIcon					(	)=0;
		virtual void boundsForCircle				(	)=0;
		virtual void boundsForRect					(	)=0;
		virtual void boundsForText					(	)=0;
		virtual void boundsForLine					(	)=0;
		virtual void boundsForSurf					(	)=0;
		virtual void boundsForRaster				(	)=0;
		
// Stuff
		virtual void setDrawingFlag					(	bool drawflg)=0;
		virtual void beginDraw						(	)=0;
		virtual void endDraw						(	)=0;
		virtual void reset							(	)=0;
		virtual void flush							(	)=0;
		virtual void setElement						(	bGenericScreenObj* scr)=0;
		virtual void set_background					(	float* clr,
														int space)=0;// a remonter
		
		virtual int signature						(	)=0;
		
		virtual void set_box						(	CGRect box)=0;
		virtual CGRect get_box						(	)=0;

		virtual void set_context					(	CGContextRef ctx)=0;
		virtual void set_window						(	WindowRef wd)=0;//CC
		virtual void set_path						(	const char* path)=0;
		virtual void set_bitmap_info				(	void *data, 
														size_t width, 
														size_t height, 
														size_t bitsPerComponent, 
														size_t bytesPerRow, 
														CGColorSpaceRef colorspace, 
														CGBitmapInfo bitmapInfo,
														CGContextRef* outbm)=0;
	
};

//----------------------------------------------------------------------------

#endif
