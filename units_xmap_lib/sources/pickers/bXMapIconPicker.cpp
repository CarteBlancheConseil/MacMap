//----------------------------------------------------------------------------
// File : bXMapIconPicker.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, icon picker (used by vDefs)
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

#include "bXMapIconPicker.h"
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
// 
// ------------
//static Boolean imp_file_filter(AEDesc* item, 
//							   void* xinfo, 
//							   void* cba, 
//							   NavFilterModes mode){
//NavFileOrFolderInfo*	info=(NavFileOrFolderInfo*)xinfo;
//OSErr					err;
//FSRef					ref;
//CFURLRef				url=CFURLCreateFromFSRef(kCFAllocatorDefault,&ref);
//CFStringRef				cfs;
//char					ext[FILENAME_MAX];
//	
//	if(item->descriptorType==typeFSRef) {
//		if(!info->isFolder){
//			if((err=AEGetDescData(item,&ref,sizeof(FSRef)))){
//				return(false);
//			}
//			url=CFURLCreateFromFSRef(kCFAllocatorDefault,&ref);
//			cfs=CFURLCopyPathExtension(url);
//			CFRelease(url);
//			if(!cfs){
//				return(false);
//			}
//			CFStringGetCString(cfs,ext,FILENAME_MAX,kCFStringEncodingUTF8);
//			CFRelease(cfs);
//			strupper(ext);
//			if(!strcmp(ext,"JPG")){
//				return(true);
//			}
//			else if(!strcmp(ext,"JPEG")){
//				return(true);
//			}
//			else if(!strcmp(ext,"TIF")){
//				return(true);
//			}
//			else if(!strcmp(ext,"TIFF")){
//				return(true);
//			}
//			else if(!strcmp(ext,"PNG")){
//				return(true);
//			}
//			else if(!strcmp(ext,"PDF")){
//				return(true);
//			}
//			
//			return(false);
//		}
//		else{
//			return(true);
//		}
//	}
//	return(false);
//}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapIconPicker	::bXMapIconPicker(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bXMapStdPicker(elt,gapp,bndl){
	setclassname("iconpicker");
	strcpy(_sub,"icons");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapIconPicker::~bXMapIconPicker(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapIconPicker::create(bGenericXMLBaseElement* elt){
	return(new bXMapIconPicker(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapIconPicker::draw(CGContextRef ctx){
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
	
	if(strlen(_prm.name)==0){
		return;
	}
	
OSType	tp=GetImageKind(_prm.name);	
CGRect	box;

	if(tp==kQTFileTypePDF){
_tm_("GetPDFIcon");
CGPDFDocumentRef	pdf=GetPDFIcon(_gapp,_prm.tp,_prm.name);
		if(pdf==NULL){
_te_("pdf==NULL");
			return;
		}
_tm_("CGPDFDocumentGetPage");
CGPDFPageRef	pg=CGPDFDocumentGetPage(pdf,1);
		if(!pg){
_te_("pg==NULL");
			return;
		}
_tm_("CGPDFPageGetDrawingTransform");
CGAffineTransform mx=CGPDFPageGetDrawingTransform(pg,kCGPDFMediaBox,cgr,0,true);
_tm_("CGPDFPageGetBoxRect");
		box=CGPDFPageGetBoxRect(pg,kCGPDFMediaBox);
_tm_("CGContextConcatCTM");
		CGContextConcatCTM(ctx,mx);
_tm_("CGContextClipToRect");
		CGContextClipToRect(ctx,box);
_tm_("CGContextDrawPDFPage");
		CGContextDrawPDFPage(ctx,pg);
		//CGPDFPageRelease(pg);
_tm_("CGPDFDocumentRelease");
		CGPDFDocumentRelease(pdf);
	}
	else{
CGImageRef	img=GetImageIcon(_gapp,_prm.tp,_prm.name);
		if(img==NULL){
_te_("img==NULL");
			return;
		}
		box.size.width=CGImageGetWidth(img);
		box.size.height=CGImageGetHeight(img);
		box.origin.x=0;
		box.origin.y=0;
		box=CGScaleRect(cgr,box);
		box=CGRectInset(box,1,1);
		CGContextDrawImage(ctx,box,img);
		CGImageRelease(img);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
//void bXMapIconPicker::import(NavObjectFilterUPP filter){
///*UInt32	sgn[]={	kQTFileTypeJPEG,
//				kQTFileTypeGIF,
//				kQTFileTypePNG,
//				kQTFileTypeTIFF,
//				kQTFileTypePDF,
//				kQTFileTypePicture,
//				kQTFileTypeBMP,
//				kQTFileTypeJFIF,
//				kQTFileTypePhotoShop,
//				kQTFileTypeFlash,
//				kQTFileTypeJPEG2000};*/
//	bXMapStdPicker::import(imp_file_filter);
//}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapIconPicker::import(const char** pnsft,
                             int nnsft){
const char*		nsft[6]={"pdf","tif","tiff","jpg","jpeg","png"};
    pnsft=nsft;
    nnsft=6;
    bXMapStdPicker::import(pnsft,nnsft);
}
