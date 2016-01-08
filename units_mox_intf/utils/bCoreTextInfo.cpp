//----------------------------------------------------------------------------
// File : bCoreTextInfo.cpp
// Project : MacMap
// Purpose : C++ source file : CoreText utility C++ class
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

#include "bCoreTextInfo.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/bGenericGraphicContext.h>
								  
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCoreTextInfo::bCoreTextInfo(	const char*	font, 
								double		size, 
								double		gspace, 
								double		wspace, 
								double		lspace,
								const char*	text){
	_font=NULL;
	_text=NULL;
	_utext=NULL;
	_lstrt=NULL;
	_wstrt=NULL;
	_xpos=NULL;
	_ypos=NULL;
	_glyph_xpos=NULL;
	_glyph_ypos=NULL;
	_glyph_angle=NULL;
	_width=NULL;
	_fontref=NULL;
	
	_len=0;
	_nlines=0;
	_nwords=0;
	
	_status=noErr;
	
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,font,kCFStringEncodingMacRoman);	
	_fontref=CTFontCreateWithName(cfs,size,NULL);	
	CFRelease(cfs);
	if(_fontref==0){
		_status=-1;
		return;
	}
	
	_font=strdup(font);
		
	_size=size;
	_gspace=gspace;
	_wspace=wspace;
	_lspace=lspace;
	_just=0;
	_len=strlen(text);
	_text=new char[_len+1];
	memmove(_text,text,_len+1);
	_text[_len]='_';
	
	_lstrt=(int*)malloc(2L*sizeof(int));
	_lstrt[0]=0;
	_lstrt[1]=0;
	_wstrt=(int*)malloc(2L*sizeof(int));
	_wstrt[0]=0;
	_wstrt[1]=0;
	
	_glyphs=NULL;
	_advances=NULL;

	_xpos=NULL;
	_ypos=NULL;
	_width=NULL;
	
	_glyph_xpos=NULL;
	_glyph_ypos=NULL;
	_glyph_angle=NULL;

	_glyph_angle=NULL;
	
	if(_len==0){
		_nlines=0;
		_nwords=0;
	}
	else{
		_nlines=1;
		_nwords=1;
		_lstrt[1]=_len+1;
		_wstrt[1]=_len+1;
	}
	
	convert2unicode();
	makeStyle();
	makeLayout();
	getGlyphInfo();
	
	_text[_len]=0;
	
	_first=0;
	
	_ox=0;
	_oy=0;
	_a=0;
	
	_trunc=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bCoreTextInfo::~bCoreTextInfo(){
	if(_font){
		free(_font);
	}
	if(_text){
		delete[] _text;
	}
	if(_utext){
		free(_utext);
	}
	if(_fontref){
		CFRelease(_fontref);
	}
	
	if(_lstrt){
		free(_lstrt);
	}
	if(_wstrt){
		free(_wstrt);
	}
	if(_glyphs){
		delete[] _glyphs;
	}
	if(_advances){
		delete[] _advances;
	}
	if(_xpos){
		delete[] _xpos;
	}
	if(_ypos){
		delete[] _ypos;
	}
	if(_glyph_xpos){
		delete[] _glyph_xpos;
	}
	if(_glyph_ypos){
		delete[] _glyph_ypos;
	}
	if(_glyph_angle){
        delete[] _glyph_angle;
	}
	if(_width){
		delete[] _width;
	}
}

// ---------------------------------------------------------------------------
// Inutile
// ------------
void bCoreTextInfo::setOrigin(double ox, double oy){
	_ox=ox;
	_oy=oy;
}	

// ---------------------------------------------------------------------------
// Inutile
// ------------
void bCoreTextInfo::setAngle(double a){
	_a=a;
}

// ---------------------------------------------------------------------------
// Inutile
// ------------
void bCoreTextInfo::setFirst(int idx){
	if((idx<0)||(idx>=_len)){
		return;
	}
	_first=idx;
}	

// ---------------------------------------------------------------------------
//
// ------------
int bCoreTextInfo::nGlyphs(){
	return(_len);
}

// ---------------------------------------------------------------------------
//
// ------------
int bCoreTextInfo::nWords(){
	return(_nwords);
}

// ---------------------------------------------------------------------------
//
// ------------
int bCoreTextInfo::nLines(){
	return(_nlines);
}

// ---------------------------------------------------------------------------
//
// ------------
int bCoreTextInfo::wordStart(int idx, int* len){
	if(idx>=_nwords){
		*len=0;
		return(0);
	}
	*len=(_wstrt[idx+1]-_wstrt[idx])-1;
	return(_wstrt[idx]);
}

// ---------------------------------------------------------------------------
//
// ------------
int bCoreTextInfo::lineStart(int idx, int* len){
	if(idx>=_nlines){
		*len=0;
		return(0);
	}
	*len=(_lstrt[idx+1]-_lstrt[idx])-1;
	return(_lstrt[idx]);
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::lineWidth(int idx){
int	x,sz;
	x=lineStart(idx,&sz);
	if(sz<=0){
		return(0);
	}
double	w=0;
	for(int i=x;i<x+sz;i++){
		w+=_width[i];
	}

#warning COMMENT QU'ON FAIT ?
/*ATSGlyphIdealMetrics atsm;
	
	ATSUGlyphGetIdealMetrics(_style,1,&(_ginfo->glyphs[x].glyphID),0,&atsm);
	w+=fabs(atsm.sideBearing.x);
	ATSUGlyphGetIdealMetrics(_style,1,&(_ginfo->glyphs[(x+sz)-1].glyphID),0,&atsm);
	w+=fabs(atsm.otherSideBearing.x);*/
	return(w);
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::textWidth(){
	if(_len==0){
		return(0);
	}
double	w=0,wmax=0;	
	if(wmax==0){
		wmax=w;
	}
	if(wmax<w){
		wmax=w;
	}
	for(int i=0;i<_nlines;i++){
		w=lineWidth(i);
		if(wmax<w){
			wmax=w;
		}
	}
	return(wmax);
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::textHeight(){
	return(((double)_nlines)*(_size+_lspace));
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::textOrigin(double *x,
								double *y){
int		sz,k;
double	ox,oy;
	
	*x=SHRT_MAX;
	*y=SHRT_MAX;
	for(long i=0;i<nLines();i++){
		k=lineStart(i,&sz);
		if(sz<=0){
			continue;
		}
		relativeGlyphPos(&ox,&oy,k);
		if(ox<(*x)){
			(*x)=ox;
		}
		if(oy<(*y)){
			(*y)=oy;
		}
	}
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::boundsOrigin(double *x,
								 double *y){
int		sz,k;
double	ox,oy;
	
	*x=SHRT_MAX;
	*y=SHRT_MAX;
	for(long i=0;i<nLines();i++){
		k=lineStart(i,&sz);
		if(sz<=0){
			continue;
		}
		relativeGlyphPos(&ox,&oy,k);
#warning COMMENT QU'ON FAIT ?
//		ATSUGlyphGetIdealMetrics(_style,1,&(_ginfo->glyphs[k].glyphID),0,&atsm);
//		ox-=atsm.sideBearing.x;
		if(ox<(*x)){
			(*x)=ox;
		}
		if(oy<(*y)){
			(*y)=oy;
		}
	}
}

// ---------------------------------------------------------------------------
//
// ------------
char* bCoreTextInfo::text(){
	return(_text);
}

// ---------------------------------------------------------------------------
//
// ------------
unsigned short* bCoreTextInfo::glyphs(){
	return/*(NULL);*/(_glyphs);
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::trunc(int idx){
	if((idx<0)||(idx>=_len)){
		return;
	}
	_text[idx]=kTextTrunc;
	_len=idx+1;
	_trunc=true;
}

// ---------------------------------------------------------------------------
//
// ------------
unsigned short bCoreTextInfo::glyph(int idx){
	if((idx<0)||(idx>=_len)){
		return(0);
	}
	return(_glyphs[idx]);
}

// ---------------------------------------------------------------------------
//
// ------------
char bCoreTextInfo::character(int idx){
	if((idx<0)||(idx>=_len)){
		return(0);
	}
	return(_text[idx]);
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::glyphWidth(int idx){
	if((idx<0)||(idx>=_len)){
		return(0);
	}
	return(_width[idx]);
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::relativeGlyphPos(double *xshift, 
									  double *yshift, 
									  int idx){
	if((idx<0)||(idx>=_len)){
		(*xshift)=0;
		(*yshift)=0;
		return;
	}
	(*xshift)=_xpos[idx];
	(*yshift)=_ypos[idx];
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCoreTextInfo::absoluteGlyphPos(double *xpos, 
									  double *ypos, 
									  int idx, 
									  double dcf){
	if((idx<0)||(idx>=_len)){
		(*xpos)=0;
		(*ypos)=0;
		return;
	}
//double	d=sqrt(sqr(_xpos[idx]-_xpos[_first])+sqr(_ypos[idx]-_ypos[_first]));
double	d=XYDist(_xpos[_first],_ypos[_first],_xpos[idx],_ypos[idx]);
	d*=dcf;
	*xpos=_ox+(d*cos(_a));
	*ypos=_oy+(d*sin(_a));
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::ascent(){
	return(CTFontGetAscent(_fontref));
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::descent(){
	return(-CTFontGetDescent(_fontref));// Négatif pour être cohérent avec ATSUI
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::leading(){
	return(CTFontGetLeading(_fontref));
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::setAlign(double just){
	if(_just==just){
		return;
	}
	_just=just;
	
double	w=0;
double	wl;
int		i,j;
int		g,sz;
	
	if((_just>=0)&&(_just<=1)){
		w=textWidth();
		for(i=0;i<_nlines;i++){
			wl=lineWidth(i);
			if((wl>0)&&(wl!=w)){
				wl=(w-wl)*_just;
				g=lineStart(i,&sz);
				for(j=g;j<g+sz;j++){
					_xpos[j]+=wl;
				}
			}
		}
	}
	else if(_just==2){
double	maj;
double	wg;
		w=textWidth();
		for(i=0;i<_nlines;i++){
			wl=lineWidth(i);
			if((wl>0)&&(wl!=w)){
				g=lineStart(i,&sz);
				wg=_width[(g+sz)-1];
				maj=((w-wg)-(wl-wg))/(double)(sz-1);
				for(j=g;j<g+sz;j++){
					_xpos[j]+=(((double)(j-g))*maj);
					if(j<g+sz-1){
						_width[j]+=maj;
					}
				}
			}
		}
	}
	
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::setGlyphPos(float*	x,
								float*	y,
								float*	angle){
	if(_glyph_xpos){
		delete[] _glyph_xpos;
		_glyph_xpos=NULL;
	}
	if(_glyph_ypos){
		delete[] _glyph_ypos;
		_glyph_ypos=NULL;
	}
	if(_glyph_angle){
        delete[] _glyph_angle;
		_glyph_angle=NULL;
	}
	_glyph_xpos=new float[_len];
	if(_glyph_xpos){
		memmove(_glyph_xpos,x,_len*sizeof(float));
	}
	_glyph_ypos=new float[_len];
	if(_glyph_ypos){
		memmove(_glyph_ypos,y,_len*sizeof(float));
	}
	_glyph_angle=new float[_len];
	if(_glyph_angle){
		memmove(_glyph_angle,angle,_len*sizeof(float));
	}
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::getGlyphPos(float**	x,
								float**	y,
								float**	a){
	*x=_glyph_xpos;
	*y=_glyph_ypos;
	*a=_glyph_angle;
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::convert2unicode(){
ByteCount	clen;
	
	for(long i=0;i<_len;i++){
		if(	(_text[i]==kv1LineBreak)	||
			(_text[i]==kv2LineBreak)	||
			(_text[i]==kUnixLineBreak)	){
			_text[i]=kMacLineBreak;
		}
		if(_text[i]==kMacLineBreak){
			_lstrt[_nlines]=i+1;
			_nlines++;
			_lstrt=(int*)realloc(_lstrt,(_nlines+1)*sizeof(int));
			_lstrt[_nlines]=_len+1;
			
			_wstrt[_nwords]=i+1;
			_nwords++;
			_wstrt=(int*)realloc(_wstrt,(_nwords+1)*sizeof(int));
			_wstrt[_nwords]=_len+1;
		}
		else if(_text[i]==kTextSpace){
			_wstrt[_nwords]=i+1;
			_nwords++;
			_wstrt=(int*)realloc(_wstrt,(_nwords+1)*sizeof(int));
			_wstrt[_nwords]=_len+1;
		}
	}
	
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,_text,kCFStringEncodingMacRoman);
	clen=strlen(_text)*3+1;
	_utext=(UniChar*)malloc(sizeof(UniChar)*clen);
	CFStringGetCString(cfs,(char*)_utext,clen,kCFStringEncodingUnicode);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::makeStyle(){
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::makeLayout(){
ByteCount	len=_len/*+1*/;
	_glyphs=new CGGlyph[len];
	if(_glyphs==NULL){
		_status=-1;
		return;
	}
	if(CTFontGetGlyphsForCharacters(_fontref,
									_utext,
									_glyphs,
									len)){
		_status=-2;
	}		
}

// ---------------------------------------------------------------------------
//
// ------------
void bCoreTextInfo::getGlyphInfo(){
//_bTrace_("bCoreTextInfo::getGlyphInfo",true);	
	_advances=new CGSize[_len+1];
	(void)CTFontGetAdvancesForGlyphs(_fontref,
									 kCTFontDefaultOrientation,
									 _glyphs,
									 _advances,
									 _len);
	
	_xpos=new double[_len+1];
	_ypos=new double[_len+1];
	_width=new double[_len];
	
double					l=0;
double					x=0;
double					w=0;
unsigned long			i;
	
	_xpos[0]=0;
	_ypos[0]=0;	
	for(i=0;i<_len;i++){
		switch(_text[i]){
			case kMacLineBreak:
				w=0;
				x=0;
				l-=(_lspace+_size);
				break;
			case kTextSpace:
				w=(_wspace!=0)?_wspace:_advances[i].width;
				break;
			default:
				if(_advances[i].width!=0){
					w=_gspace+_advances[i].width;
				}
				else{
fprintf(stderr,"bCoreTextInfo::getGlyphInfo : Ligature!\n");
					// Ligatures !!!! -> Il faut corriger pour retrouver les bonnes largeurs et positions des caractères concernés
					// Y a t'il des cas ou advance.x==0 et ou on n'est pas dans une ligature ?
char				buf[4]={_text[i-1],0,0,0};
bCoreTextInfo		tmpa(_font,_size,_gspace,_wspace,_lspace,buf);// Le caractère précédent est également pourri
					buf[0]=_text[i];
bCoreTextInfo		tmpb(_font,_size,_gspace,_wspace,_lspace,buf);// Le caractère pourri
					w=_gspace+tmpb.glyphWidth(0);
					_width[i-1]=_gspace+tmpa.glyphWidth(0);
					_xpos[i]=_xpos[i-1]+_width[i-1];
					x=_xpos[i];
				}
				break;
		}
		_xpos[i+1]=x+w;
		_ypos[i+1]=l;
		_width[i]=w;
		x=_xpos[i+1];
	}
	if(	((_just>0)&&(_just<=1))	||
		(_just==2)				){
		setAlign(_just);
	}
}

// ---------------------------------------------------------------------------
//
// ------------
double bCoreTextInfo::vec2angle(int idx){
double	x=_xpos[idx]-_ox;
double	y=_ypos[idx]-_oy;
double	a;
	
	if(x==0){
		a=M_PI_2;
		if(y<0){
			a+=M_PI;
		}
	}
	else{
	 	a=atan(y/x);
	 	if(x<0){
	 		a+=M_PI;
	 	}
	} 
	if(a<0){
		a+=2.0*M_PI;
	}
	return(a);
}
