//----------------------------------------------------------------------------
// File : bCoreTextInfo.h
// Project : MacMap
// Purpose : Header file : CoreText utility C++ class
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
// Utilitaire Text
//----------------------------------------------------------------------------
// 01/04/2011 creation.
//----------------------------------------------------------------------------

#ifndef __bCoreTextInfo__
#define __bCoreTextInfo__

//----------------------------------------------------------------------------

class bCoreTextInfo{
public:
    bCoreTextInfo						(	const char*	font, 
                                            double		size, 
                                            double		gspace, 
                                            double		wspace, 
                                            double		lspace, 
                                            const char*	text);
    virtual ~bCoreTextInfo				(	);

    virtual void setOrigin				(	double ox, 
                                            double oy);
    virtual void setAngle				(	double a);
    virtual void setFirst				(	int idx);

    virtual int nGlyphs					(	);
    virtual int nWords					(	);
    virtual int nLines					(	);

    virtual int wordStart				(	int idx, 
                                            int* len);
    virtual int lineStart				(	int idx, 
                                            int* len);

    virtual double lineWidth			(	int idx);
    virtual double textWidth			(	);
    virtual double textHeight			(	);
    virtual void textOrigin				(	double *x,
                                            double *y);
    virtual void boundsOrigin			(	double *x,
                                            double *y);

    virtual char* text					(	);
    virtual CGGlyph/*unsigned short*/* glyphs		(	);
    virtual void trunc					(	int idx);
    virtual CGGlyph/*unsigned short*/ glyph		(	int idx);
    virtual char character				(	int idx);
    virtual double glyphWidth			(	int idx);
    virtual void relativeGlyphPos		(	double*	xshift, 
                                            double*	yshift, 
                                            int		idx);
    virtual void absoluteGlyphPos		(	double*	xpos, 
                                            double*	ypos, 
                                            int idx,
                                            double dcf);
    virtual double ascent				(	);
    virtual double descent				(	);
    virtual double leading				(	);

    virtual void setAlign				(	double just);

    virtual void setGlyphPos			(	float*	x,
                                            float*	y,
                                            float*	angle);
    virtual void getGlyphPos			(	float**	x,
                                            float**	y,
                                            float**	angle);
    /**/
    virtual bool truncated				(	){return(_trunc);};
    /**/
protected:
    void convert2unicode				(	);
    void makeStyle						(	);
    void makeLayout						(	);
    void getGlyphInfo					(	);
    double vec2angle					(	int idx);

private:
    OSStatus							_status;
    char*								_font;
    
    CTFontRef 							_fontref;
    
    ByteCount							_len;
    int									_nlines;
    int									_nwords;

    double								_size;
    double								_gspace;
    double								_wspace;
    double								_lspace;
    double								_just;

    char*								_text;
    UniChar*							_utext;

    long                                _unused;

    CGGlyph*							_glyphs;
    
    CGSize*								_advances;
    
    int									_first;

    long*								_lstrt;
    long*								_wstrt;

    double								_ox;
    double								_oy;
    double								_a;

    double*								_xpos;
    double*								_ypos;
    double*								_width;

    float*								_glyph_xpos;
    float*								_glyph_ypos;
    float*								_glyph_angle;

    bool								_trunc;

};

//----------------------------------------------------------------------------

#endif
