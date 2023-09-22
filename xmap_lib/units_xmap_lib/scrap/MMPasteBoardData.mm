//----------------------------------------------------------------------------
// File : MMPasteBoardData.mm
// Project : MacMap
// Purpose : Objective C++ source file : paste/cut/copy pasteboard data C wrappers, for MacMap objects
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
// 28/04/2015 Creation.
//----------------------------------------------------------------------------

#import "MMPasteBoardData.h"
#import <MacMapSuite/bTrace.h>
#import <stdlib.h>
#import <stdio.h>

// ---------------------------------------------------------------------------
#define MacMapPasteBoardData        @"MacMapPasteBoardData"
#define MacMapPasteboardDataType    @"com.cbconseil.macmap.pasteboard.data"

// ---------------------------------------------------------------------------
//
// -----------
@implementation MMPasteBoardData



// ---------------------------------------------------------------------------
//
// -----------
+(id)alloc{
_bTrace_("[MMPasteBoardData alloc]",true);
    return [super alloc];
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)dealloc{
_bTrace_("[MMPasteBoardData dealloc]",true);
//    if(_nsdata){
//        [_nsdata release];
//    }
    [super dealloc];
}

// ---------------------------------------------------------------------------
//
// -----------
-(id)initWithData:(const void*)data
             size:(size_t)sz{
//_bTrace_("[MMPasteBoardData initWithData]",true);
    self=[super init];
    if(self){
        _nsdata=[NSData dataWithBytes:data length:sz];
//_tm_("size="+[_nsdata length]);
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// -----------
-(id)initWithCoder:(NSCoder*)coder{
//_bTrace_("[MMPasteBoardData initWithCoder]",true);
    self=[super init];
    if(self){
        _nsdata=[coder decodeObjectForKey:MacMapPasteBoardData];
        [_nsdata retain];// Sinon, désalloué avant qu'on ait eu le temps de le lire
//_tm_("addr="+(void*)_nsdata);
//_tm_("size="+[_nsdata length]);
//_tm_("internal addr="+(void*)[_nsdata bytes]);
    }
    else{
//_te_("self==nil");
    }
    return self;
}

// ---------------------------------------------------------------------------
//
// -----------
-(NSArray*)writableTypesForPasteboard:(NSPasteboard*)pasteboard{
//_bTrace_("[MMPasteBoardData writableTypesForPasteboard]",true);
static NSArray* writableTypes=nil;
    if(!writableTypes){
        writableTypes=[[NSArray alloc] initWithObjects:MacMapPasteboardDataType,nil];
    }
    return writableTypes;
}

// ---------------------------------------------------------------------------
//
// -----------
+(NSArray*)readableTypesForPasteboard:(NSPasteboard*)pasteboard{
//_bTrace_("[MMPasteBoardData readableTypesForPasteboard]",true);
static NSArray* readableTypes=nil;
    if(!readableTypes){
        readableTypes=[[NSArray alloc] initWithObjects:MacMapPasteboardDataType,nil];
    }
    return readableTypes;
}

// ---------------------------------------------------------------------------
//
// -----------
+(NSPasteboardReadingOptions)readingOptionsForType:(NSString *)type
                                        pasteboard:(NSPasteboard *)pboard {
    if([type isEqualToString:MacMapPasteboardDataType]){
        return NSPasteboardReadingAsKeyedArchive;
    }
    return 0;
}

// ---------------------------------------------------------------------------
//
// -----------
- (id)pasteboardPropertyListForType:(NSString*)type{
//_bTrace_("[MMPasteBoardData pasteboardPropertyListForType]",true);
    if([type isEqualToString:MacMapPasteboardDataType]){
        return [NSKeyedArchiver archivedDataWithRootObject:self];
    }
    return nil;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void)encodeWithCoder:(NSCoder*)encoder{
//_bTrace_("[MMPasteBoardData encodeWithCoder]",true);
    [encoder encodeObject:_nsdata forKey:MacMapPasteBoardData];
}

// ---------------------------------------------------------------------------
//
// -----------
//-(const void*)getCData{
////_bTrace_("[MMPasteBoardData getCData]",true);
//    if(_nsdata){
////_tm_((void*)_nsdata);
//        return [_nsdata bytes];
//    }
//    return NULL;
//}

// ---------------------------------------------------------------------------
//
// -----------
//-(size_t)getCDataSize{
////_bTrace_("[MMPasteBoardData getCDataSize]",true);
//    if(_nsdata){
////_tm_((int)[_nsdata length]);
//        return [_nsdata length];
//    }
//    return 0;
//}

// ---------------------------------------------------------------------------
//
// -----------
-(NSData*)getNSData{
    return _nsdata;
}

// ---------------------------------------------------------------------------
//
// -----------
+(BOOL)supportsSecureCoding {
    return YES;
}

// ---------------------------------------------------------------------------
//
// -----------
@end


// ---------------------------------------------------------------------------
//
// ------------
void putPasteBoardData(const void* cdata, size_t cdataSize){
_bTrace_("putPasteBoardData",true);
NSAutoreleasePool*  localPool=[[NSAutoreleasePool alloc] init];
NSPasteboard*       pasteboard=[NSPasteboard generalPasteboard];

    [pasteboard clearContents];
MMPasteBoardData*   nsdata=[[MMPasteBoardData alloc] initWithData:cdata size:cdataSize];
NSArray*            copiedObjects=[NSArray arrayWithObject:nsdata];
    [pasteboard writeObjects:copiedObjects];
    [localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
void* getPasteBoardData(void** cdata, size_t* cdataSize){
_bTrace_("getPasteBoardData",true);
NSAutoreleasePool*  localPool=[[NSAutoreleasePool alloc] init];
NSPasteboard*       pasteboard=[NSPasteboard generalPasteboard];
NSArray*            classes=[[NSArray alloc] initWithObjects:[MMPasteBoardData class], nil];
NSDictionary*       options=[NSDictionary dictionary];
NSArray*            copiedItems=[pasteboard readObjectsForClasses:classes options:options];
NSData*             nsdata=NULL;
    if(copiedItems!=nil){
MMPasteBoardData*   pboarddata=[copiedItems objectAtIndex:0];
        nsdata=[pboarddata getNSData];
        if(nsdata){
            *cdata=(void*)[nsdata bytes];
            *cdataSize=[nsdata length];
        }
        else{
_te_("no data");
            *cdata=NULL;
            *cdataSize=0;
        }
    }
    [localPool release];
    return nsdata;
}

// ---------------------------------------------------------------------------
//
// ------------
bool testPasteBoardData(){
_bTrace_("testPasteBoardData",true);
NSAutoreleasePool*  localPool=[[NSAutoreleasePool alloc] init];
NSPasteboard*       pasteboard=[NSPasteboard generalPasteboard];
NSArray*            classes=[[NSArray alloc] initWithObjects:[MMPasteBoardData class], nil];
NSDictionary*       options=[NSDictionary dictionary];
NSArray*            copiedItems=[pasteboard readObjectsForClasses:classes options:options];
bool                ret=false;
    
    if((copiedItems!=nil)&&([copiedItems count]>0)){
MMPasteBoardData*   pboarddata=[copiedItems objectAtIndex:0];
NSData*             nsdata=[pboarddata getNSData];
        if(nsdata){
            [nsdata release];
            ret=true;
        }
    }
    [localPool release];
    return ret;
}

// ---------------------------------------------------------------------------
//
// ------------
void cleanPasteBoardData(void* nsdata){
_bTrace_("testPasteBoardData",true);
NSAutoreleasePool*  localPool=[[NSAutoreleasePool alloc] init];
NSData*             lnsdata=(NSData*)nsdata;
    if(lnsdata){
        [lnsdata release];
    }
    [localPool release];
}


// ---------------------------------------------------------------------------
//
// -----------
UInt32 nmsz(char* nm){
UInt32	sz=strlen(nm);
    sz++;
UInt32	maj=sz%sizeof(UInt32);
    if(maj){
        sz-=maj;
        sz+=sizeof(UInt32);
    }
    return(sz);
}


// ---------------------------------------------------------------------------
//
// -----------
//-(id)initWithPasteboardPropertyList:(id)propertyList ofType:(NSString*)type{
//_bTrace_("[MMPasteBoardData initWithPasteboardPropertyList]",true);
//    if(self=[self init]){
//        if([type isEqualToString:MacMapPasteboardDatatype]){
//            NSLog(type);
//_tm_("_nsdata="+(void*)_nsdata);
//            _nsdata=[[NSData alloc] initWithPasteboardPropertyList:propertyList ofType:type];
//        }
//        else{
//            [self release];
//            return nil;
//        }
//    }
//    return self;
//}


