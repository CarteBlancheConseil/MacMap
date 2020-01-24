//----------------------------------------------------------------------------
// File : MMPasteBoardData.h
// Project : MacMap
// Purpose : Header file : paste/cut/copy pasteboard data C wrappers, for MacMap objects
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

#import <Cocoa/Cocoa.h>

//----------------------------------------------------------------------------
#define kScrapFlavorTypeMacMap      'MapT'
#define kScrapMacMapTagType         'Type'
#define kScrapMacMapTagField        'Fld_'

//----------------------------------------------------------------------------
typedef struct tag{
    UInt32	sign;
    UInt32	snsz;
    UInt32	cnsz;
    void*	val;
}tag;

//----------------------------------------------------------------------------
@interface MMPasteBoardData : NSObject <NSCoding, NSPasteboardWriting, NSPasteboardReading> {
    NSData* _nsdata;
}

//----------------------------------------------------------------------------
-(id)initWithData:(const void*)data size:(size_t)sz;

//----------------------------------------------------------------------------
@end

//----------------------------------------------------------------------------
void putPasteBoardData  (const void* cdata,
                        size_t cdataSize);
void* getPasteBoardData (void** cdata,
                         size_t* cdataSize);
void cleanPasteBoardData(void* nsdata);
bool testPasteBoardData (   );
UInt32 nmsz             (char* nm);

//----------------------------------------------------------------------------
