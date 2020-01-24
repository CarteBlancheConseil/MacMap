//----------------------------------------------------------------------------
// File : Carb_Utils.h
// Project : MacMap
// Purpose : Header file : Carbon interface utilities
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2018 Carte Blanche Conseil.
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

#ifndef _Carb_Utils_
#define _Carb_Utils_

//----------------------------------------------------------------------------

#include <mox_intf/Color_Utils.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

/*void SendCommandToApplication		(	UInt32 sign);// dans NSUIUtils
void PDFPatternPlot					(	void* info,
                                        CGContextRef ctx);
CGImageRef LoadPNGImageFromBundle	(	CFBundleRef bndl,
                                        CFStringRef name);
void MacRoman2UTF8					(	char* str,
                                        int sz);
void UTF82MacRoman					(	char* str,
                                        int sz);*/

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
