//----------------------------------------------------------------------------
// File : bXMapPatternPicker.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, pattern picker (pdf, used by vDefs)
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
// 31/05/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapPatternPicker.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bTrace.h>
#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPatternPicker	::bXMapPatternPicker(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapStdPicker(elt,gapp,bndl){
	setclassname("patternpicker");
	strcpy(_sub,"patterns");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPatternPicker::~bXMapPatternPicker(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPatternPicker::create(bGenericXMLBaseElement* elt){
	return(new bXMapPatternPicker(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPatternPicker::draw(CGContextRef ctx){
_bTrace_("bXMapStdPicker::draw",false);
HIRect	cgr;
  	HIViewGetBounds(get_control(kXMapStdPickerSignature,
								kXMapStdPickerViewID),
					&cgr);

	TransformHIViewToCG(ctx,get_control(kXMapStdPickerSignature,
										kXMapStdPickerViewID));
	
	CGContextSetRGBFillColor(ctx,1,1,1,1);
	CGContextFillRect(ctx,cgr);
	CGContextSetRGBStrokeColor(ctx,0,0,0,1);
	CGContextStrokeRect(ctx,cgr);

CGPDFDocumentRef	pdf=GetPDFPattern(_gapp,_prm.tp,_prm.name);
	if(pdf==NULL){
_te_("pdf==NULL");
		return;
	}


CGPDFPageRef		pg=CGPDFDocumentGetPage(pdf,1);
CGRect				box=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
float				color[4]={0,0,0,1 };
CGPatternCallbacks	callbacks={0,&PDFPatternPlot,NULL};
CGColorSpaceRef		bspc=CGColorSpaceCreateDeviceRGB();
CGColorSpaceRef		pspc=CGColorSpaceCreatePattern(bspc);
    CGContextSetFillColorSpace(ctx,pspc);
    CGColorSpaceRelease(pspc);
    CGColorSpaceRelease(bspc);
CGPatternRef		pat=CGPatternCreate(pdf, 
										box,
										CGAffineTransformIdentity,
										box.size.width,
										box.size.height,
										kCGPatternTilingConstantSpacing,
										false, 
										&callbacks);   
    CGContextSetFillPattern(ctx,pat,color);
    CGPatternRelease(pat);
	cgr=CGRectInset(cgr,1,1);
	CGContextFillRect(ctx,cgr);
	CGPDFDocumentRelease(pdf);
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapPatternPicker::import(const char** pnsft,
                                int nnsft){
const char*		nsft[1]={"pdf"};
    pnsft=nsft;
    nnsft=1;
    bXMapStdPicker::import(pnsft,nnsft);
}
