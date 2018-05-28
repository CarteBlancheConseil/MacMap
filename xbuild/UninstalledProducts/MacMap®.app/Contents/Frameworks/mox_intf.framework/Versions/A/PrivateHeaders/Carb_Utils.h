//----------------------------------------------------------------------------
// File : Carb_Utils.h
// Project : MacMap
// Purpose : Header file : Carbon interface utilities
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
// => A VIRER
//----------------------------------------------------------------------------

typedef char CExtension[10];

typedef struct ynrec{
	WindowRef	wd;
	bool		ok;
}ynrec;
	
//----------------------------------------------------------------------------
// A supprimer
ControlKeyFilterResult	uint_filter			(	ControlRef c,
												SInt16* key, 
												SInt16* car, 
												EventModifiers *m);
ControlKeyFilterResult	int_filter			(	ControlRef c,
												SInt16* key, 
												SInt16* car, 
												EventModifiers *m);
ControlKeyFilterResult	float_filter		(	ControlRef c,
												SInt16* key, 
												SInt16* car, 
												EventModifiers *m);
ControlKeyFilterResult	ufloat_filter		(	ControlRef c,
												SInt16* key, 
												SInt16* car, 
												EventModifiers *m);

void DataBrowserInit						(	ControlRef c, 
												DataBrowserItemDataUPP data, 
												DataBrowserItemNotificationUPP notifier,
												long RefCon);										
void DataBrowserClose						(	ControlRef c);										
UInt32 DataBrowserCountSelected				(	ControlRef c);
DataBrowserItemID DataBrowserSelectedItem	(	ControlRef c);
int DataBrowserIsSelectedItem				(	ControlRef c, 
												DataBrowserItemID item);
void DataBrowserReset						(	ControlRef c,
												UInt32 n, 
												DataBrowserPropertyID* pid);

void SetControlAbility						(	ControlRef c,
												Boolean	b);
												
void GetTextControlValue					(	ControlRef c,
												char* val,
												int	max);
void SetTextControlValue					(	ControlRef c,
												const char* val);
void SetTextControlFilter					(	ControlRef c,
												ControlKeyFilterUPP flt);
MenuRef GetPopupControlMenu					(	ControlRef c);
void ClearPopupControl						(	ControlRef c,
												int from);
void AddMenuItemValue						(	MenuRef m, 
												const char* str);
void AddPopupControlMenuItemValue			(	ControlRef c,
												const char* str);

void SendCommandToApplication				(	UInt32 sign);
    
    
//----------------------------------------------------------------------------
// A transférer dans d'autres librairies
void PDFPatternPlot							(	void* info,
                                                CGContextRef ctx);

CGImageRef LoadPNGImageFromBundle			(	CFBundleRef bndl,
                                                CFStringRef name);

void MacRoman2UTF8							(	char* str,
												int sz);
void UTF82MacRoman							(	char* str,
												int sz);
    
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
