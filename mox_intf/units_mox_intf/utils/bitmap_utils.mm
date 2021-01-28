//----------------------------------------------------------------------------
// File : bitmap_utils.mm
// Project : MacMap
// Purpose : Objective C++ source file : CGBitmapContext and CGImageRef utilities
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
// 24/03/2015 Elimination des appels GraphicsExportComponent.
//----------------------------------------------------------------------------

#include "bitmap_utils.h"
#include "ext_utils.h"

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bStdFile.h>

/*#include <QuickTime/ImageCompression.h>
#include <QuickTime/QuickTimeComponents.h>*/

// ---------------------------------------------------------------------------
// 
// ------------
bool GetBitMap32BitPixel(BMI* bi, size_t i, size_t j, UInt32* px){
	i-=bi->owidth;
	j-=bi->oheight;
	if(i>bi->width){
		return(false);
	}
	if(j>bi->height){
		return(false);
	}
unsigned char*	bu=(unsigned char*)bi->data+((((j-1)*bi->width)+i)-1)*4L;
	if((((((j-1)*bi->width)+i)-1)*4)>bi->size){
		return(false);
	}
	memcpy(px,bu,sizeof(UInt32));
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool SetBitMap32BitPixel(BMI* bi, size_t i, size_t j, UInt32 px){
	i-=bi->owidth;
	j-=bi->oheight;
	if(i>bi->width){
		return(false);
	}
	if(j>bi->height){
		return(false);
	}
unsigned char*	bu=(unsigned char*)bi->data+((((j-1)*bi->width)+i)-1)*4;
	if((((((j-1)*bi->width)+i)-1)*4)>bi->size){
		return(false);
	}
	memcpy(bu,&px,sizeof(UInt32));
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGContextRef CreateBitmapContextWithImage(CGImageRef img){
_bTrace_("CreateBitmapContextWithImage",true);
CGFloat			width=CGImageGetWidth(img);
CGFloat			height=CGImageGetHeight(img);
size_t			bitsPerComponent=8;
CGColorSpaceRef	colorspace=CGColorSpaceCreateDeviceRGB();
size_t			bytesPerRow=(width*4);
void*			data=malloc(bytesPerRow*height);
CGBitmapInfo	bitmapInfo=kCGImageAlphaPremultipliedLast;
    
	if(data==NULL){
_te_("data==NULL");
        CGColorSpaceRelease(colorspace);
		return(NULL);
	}
	memset(data,0,bytesPerRow*height);
CGContextRef	ctx=CGBitmapContextCreate(data,
										  width,
										  height,
										  bitsPerComponent,
										  bytesPerRow,
										  colorspace,
										  bitmapInfo);
	if(ctx==NULL){
_te_("CGBitmapContextCreate==NULL");
        CGColorSpaceRelease(colorspace);
		return(NULL);
	}
CGRect	cgr={{0,0},{width,height}};
	CGContextDrawImage(ctx,cgr,img);
	return(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGContextRef CreateBitmapContextWithSize(size_t width, 
										 size_t height){
_bTrace_("CreateBitmapContextWithSize",false);
size_t			bitsPerComponent=8;
CGColorSpaceRef	colorspace=CGColorSpaceCreateDeviceRGB();
size_t			bytesPerRow=(width*4);
void*			data=malloc(bytesPerRow*height);
CGBitmapInfo	bitmapInfo=kCGImageAlphaPremultipliedLast;
	if(data==NULL){
_te_("data==NULL");
        CGColorSpaceRelease(colorspace);
		return(NULL);
	}
	memset(data,0,bytesPerRow*height);
CGContextRef	ctx=CGBitmapContextCreate(data,
										  width,
										  height,
										  bitsPerComponent,
										  bytesPerRow,
										  colorspace,
										  bitmapInfo);
	return(ctx);
}

// ---------------------------------------------------------------------------
//
// ------------
CGImageRef CreateImageFromBitmapContext(CGContextRef ctx){
_bTrace_("CreateImageFromBitmapContext",false);
void*               imgdt=CGBitmapContextGetData(ctx);
size_t              imgsz=CGBitmapContextGetHeight(ctx)*CGBitmapContextGetBytesPerRow(ctx);
CGDataProviderRef   dataProvider=CGDataProviderCreateWithData(NULL,imgdt,imgsz,NULL);
    if(dataProvider==NULL){
_te_("CGDataProviderCreateWithData failed");
        return NULL;
    }
CGImageRef          cgImage=CGImageCreate(CGBitmapContextGetWidth(ctx),
                                          CGBitmapContextGetHeight(ctx),
                                          CGBitmapContextGetBitsPerComponent(ctx),
                                          CGBitmapContextGetBitsPerPixel(ctx),
                                          CGBitmapContextGetBytesPerRow(ctx),
                                          CGBitmapContextGetColorSpace(ctx),
                                          CGBitmapContextGetBitmapInfo(ctx),
                                          dataProvider,
                                          NULL,
                                          false,
                                          kCGRenderingIntentDefault);
    if(cgImage==NULL){
_te_("CGImageCreate failed");
    }
    CGDataProviderRelease(dataProvider);
    return(cgImage);
}

// ---------------------------------------------------------------------------
//
// ------------
CFDictionaryRef MakeImageProperties(int res, int q){
_bTrace_("MakeImageProperties",false);
CFDictionaryRef options;
CFStringRef     keys[3];
CFTypeRef       values[3];
CGFloat         compression;
CGFloat         resolution=res;
    
   switch(q){
        case 1:
            compression=0;
            break;
        case 2:
            compression=256;
            break;
        case 3:
            compression=512;
            break;
        case 4:
            compression=768;
            break;
        case 5:
            compression=1023;
            break;
        case 6:
            compression=1024;
            break;
        default:
            compression=512;
            break;
    }
    compression/=1024.0;
    
    keys[0]=kCGImageDestinationLossyCompressionQuality;
    values[0]=CFNumberCreate(kCFAllocatorDefault,kCFNumberCGFloatType,&compression);
    keys[1]=kCGImagePropertyDPIHeight;
    values[1]=CFNumberCreate(kCFAllocatorDefault,kCFNumberCGFloatType,&resolution);
    keys[2]=kCGImagePropertyDPIWidth;
    values[2]=CFNumberCreate(kCFAllocatorDefault,kCFNumberCGFloatType,&resolution);
    options=CFDictionaryCreate(kCFAllocatorDefault,
                               (const void **)keys,
                               (const void **)values,
                               3,
                               &kCFTypeDictionaryKeyCallBacks,
                               &kCFTypeDictionaryValueCallBacks);
    CFRelease(values[0]);
    CFRelease(values[1]);
    CFRelease(values[2]);
    if(options==NULL){
_te_("CFDictionaryCreate failed");
    }
    return options;
}

// ---------------------------------------------------------------------------
//
// ------------
CFStringRef ImageType(UInt32 codec){
_bTrace_("ImageType",false);
CFStringRef imgType;
    switch(codec){
        case kFileTypeJPEG:
            imgType=kUTTypeJPEG;
_tm_("kUTTypeJPEG");
            break;
        case kFileTypeJPEG2000:
            imgType=kUTTypeJPEG2000;
_tm_("kUTTypeJPEG2000");
            break;
        case kFileTypeTIFF:
            imgType=kUTTypeTIFF;
_tm_("kUTTypeTIFF");
            break;
        case kFileTypePNG:
            imgType=kUTTypePNG;
_tm_("kUTTypePNG");
            break;
        default:
            imgType=kUTTypeJPEG;
_tm_("kUTTypeJPEG");
            break;
    }
    return imgType;
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus CGBitmapContextExport(CGContextRef ctx, 
							   const char* path, 
							   const char* name, 
							   UInt32 codec, 
							   int res, 
							   int q){
_bTrace_("CGBitmapContextExport",true);
bStdUTF8Directory	dr(path);
	if(dr.status()){
_te_("dr.status()="+dr.status());
		return(dr.status());
	}
    
OSStatus                status=-1;
CGImageRef              cgImage=NULL;
CFStringRef             cfstr=NULL;
CFURLRef                url=NULL;
CGImageDestinationRef   destination=NULL;
CFDictionaryRef         options=NULL;

    for(;;){
// Making CGImage from CGBitmapContext
        cgImage=CreateImageFromBitmapContext(ctx);
        if(cgImage==NULL){
_te_("CreateImageFromBitmapContext failed");
           break;
        }

// Setting file URL
        cfstr=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingUTF8);
        if(cfstr==NULL){
_te_("CFStringCreateWithCString failed");
            break;
        }
        url=CFURLCreateWithFileSystemPath(kCFAllocatorDefault,cfstr,kCFURLPOSIXPathStyle,false);
        if(url==NULL){
_te_("CFURLCreateWithFileSystemPath failed");
            break;
        }

// Making destination
        destination=CGImageDestinationCreateWithURL(url,ImageType(codec),1,NULL);
        if(destination==NULL){
_te_("CGImageDestinationCreateWithURL failed");
            break;
        }
        
// Add CGImage to destination with options
        options=MakeImageProperties(res,q);
        if(options==NULL){
_te_("MakeImageProperties failed");
            break;
        }
        CGImageDestinationAddImage(destination,cgImage,options);

// Write (in fact, at CFRelease(destination))
        if(!CGImageDestinationFinalize(destination)){
_te_("CGImageDestinationFinalize failed");
            break;
        }
        status=0;
        break;
    }

// Memory release
    if(destination){
        CFRelease(destination);
    }
    if(options){
        CFRelease(options);
    }
    if(cgImage){
        CGImageRelease(cgImage);
    }
    if(cfstr){
        CFRelease(cfstr);
    }
    if(url){
        CFRelease(url);
    }
    return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus CGBitmapContextGetOutputData(CGContextRef ctx, 
									  UInt32 codec, 
									  int res, 
									  int q, 
									  void** data, 
									  size_t* sz){
_bTrace_("CGBitmapContextGetOutputData",true);
OSStatus                status=-1;
CGImageRef              cgImage=NULL;
CFMutableDataRef        cfData=NULL;
CGImageDestinationRef   destination=NULL;
CFDictionaryRef         options=NULL;
    
    for(;;){
// Making CGImage from CGBitmapContext
        cgImage=CreateImageFromBitmapContext(ctx);
        if(cgImage==NULL){
_te_("CreateImageFromBitmapContext failed");
            break;
        }
        
// Creating DataRef
        cfData=CFDataCreateMutable(kCFAllocatorDefault,0);
        if(cfData==NULL){
_te_("CFDataCreateMutable failed");
            break;
        }

// Making destination
        destination=CGImageDestinationCreateWithData(cfData,ImageType(codec),1,NULL);
        if(destination==NULL){
_te_("CGImageDestinationCreateWithURL failed");
            break;
        }
        
// Add CGImage to destination with options
        options=MakeImageProperties(res,q);
        if(options==NULL){
_te_("MakeImageProperties failed");
            break;
        }
        CGImageDestinationAddImage(destination,cgImage,options);
        
// Write (in fact, at CFRelease(destination))
        if(!CGImageDestinationFinalize(destination)){
_te_("CGImageDestinationFinalize failed");
            break;
        }
        
        *sz=CFDataGetLength(cfData);
        (*data)=malloc((*sz));
        if(!data){
_te_("malloc failed");
            break;
        }
        CFDataGetBytes(cfData,CFRangeMake(0,(*sz)),(UInt8*)(*data));
        status=0;
        break;
    }
    
// Memory release
    if(destination){
        CFRelease(destination);
    }
    if(options){
        CFRelease(options);
    }
    if(cgImage){
        CGImageRelease(cgImage);
    }
    if(cfData){
        CFRelease(cfData);
    }
    return(status);
}
