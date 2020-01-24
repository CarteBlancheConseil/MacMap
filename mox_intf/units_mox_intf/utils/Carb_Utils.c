//----------------------------------------------------------------------------
// File : Carb_Utils.c
// Project : MacMap
// Purpose : C source file : Carbon interface utilities
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
// 12/11/2018 Remove Carbon UI calls for 64 bits.
//----------------------------------------------------------------------------

#include "Carb_Utils.h"	
/*
// ---------------------------------------------------------------------------
// 
// ------------
void PDFPatternPlot(void* info, CGContextRef ctx){
CGPDFDocumentRef	pat=(CGPDFDocumentRef)info;
CGPDFPageRef		pg=CGPDFDocumentGetPage(pat,1);
CGRect				cgr=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
	CGContextDrawPDFDocument(ctx,cgr,pat,1);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGImageRef LoadPNGImageFromBundle(CFBundleRef bndl, CFStringRef name){
//fprintf(stderr,"LoadPNGImageFromBundle\n");
CGImageRef	image=NULL;
CFURLRef	url=CFBundleCopyResourceURL(bndl,name,NULL,NULL);
    if(url!=NULL){
CGDataProviderRef	provider=CGDataProviderCreateWithURL(url);
		image=CGImageCreateWithPNGDataProvider(provider,NULL,false,kCGRenderingIntentDefault);
//fprintf(stderr,"CGImageCreateWithPNGDataProvider\n");
		if(!image){
//fprintf(stderr,"Pad d'image\n");			
		}
		CGDataProviderRelease(provider);
		CFRelease(url);
    }
	else{
//fprintf(stderr,"Pad d'URL\n");				
	}
    return(image);
}*/

// ---------------------------------------------------------------------------
// Moved to NSUIUtils.h
// -----------
/*void SendCommandToApplication(UInt32 sign){
}*/

/*// ---------------------------------------------------------------------------
// 
// -----------
void MacRoman2UTF8(char* str, int sz){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman);
	CFStringGetCString(cfs,str,sz,kCFStringEncodingUTF8);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void UTF82MacRoman(char* str, int sz){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingUTF8);
	CFStringGetCString(cfs,str,sz,kCFStringEncodingMacRoman);
	CFRelease(cfs);
}*/
