//----------------------------------------------------------------------------
// File : bXMapDashPicker.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, dash picker (used by vDefs)
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

#include "bXMapDashPicker.h"
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
//			if(!strcmp(ext,"XML")){
//				return(true);
//			}
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
bXMapDashPicker	::bXMapDashPicker(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bXMapStdPicker(elt,gapp,bndl){
	setclassname("dashpicker");
	strcpy(_sub,"dashes");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDashPicker::~bXMapDashPicker(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDashPicker::create(bGenericXMLBaseElement* elt){
	return(new bXMapDashPicker(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDashPicker::draw(CGContextRef ctx){
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
	
int		n;
float*	dsh=GetDash(_gapp,_prm.tp,_prm.name,&n);
	if(!dsh){
_te_("GetDash==NULL");
		return;
	}
	CGContextSetLineDash(ctx,0,dsh,n);
	CGContextMoveToPoint(	ctx,
							cgr.origin.x,
							cgr.origin.y+cgr.size.height/2.0);
	CGContextAddLineToPoint(ctx,
							cgr.origin.x+cgr.size.width,
							cgr.origin.y+cgr.size.height/2.0);
	CGContextStrokePath(ctx);
	delete dsh;
}

// ---------------------------------------------------------------------------
// 
// ------------
//void bXMapDashPicker::import(NavObjectFilterUPP filter){
//_bTrace_("bXMapDashPicker::import",true);
//	bXMapStdPicker::import(imp_file_filter);
//}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapDashPicker::import(const char** pnsft,
                                int nnsft){
const char*		nsft[1]={"xml"};
    pnsft=nsft;
    nnsft=1;
    bXMapStdPicker::import(pnsft,nnsft);
}

