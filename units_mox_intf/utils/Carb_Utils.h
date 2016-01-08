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

typedef char CExtension[10];

typedef struct ynrec{
	WindowRef	wd;
	bool		ok;
}ynrec;
	
//----------------------------------------------------------------------------
    
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
ControlKeyFilterResult	crcatch_filter		(	ControlRef c, 
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
void DataBrowserAdd							(	ControlRef c);
void DataBrowserRemove						(	ControlRef c);
void DataBrowserUpdate						(	ControlRef c);
void DataBrowserReset						(	ControlRef c, 
												UInt32 n, 
												DataBrowserPropertyID* pid);

void SetControlAbility						(	ControlRef c,
												Boolean	b);
												
void GetTextControlValue					(	ControlRef c,
												char* val,
												int	max);
void GetTextControlValueLength				(	ControlRef c, 
												int* len);
void SetTextControlValue					(	ControlRef c,
												const char* val);
void SetTextControlFilter					(	ControlRef c,
												ControlKeyFilterUPP flt);
void GetTextControlSelection				(	ControlRef c, 
												short* start, 
												short* end);
void SetTextControlSelection				(	ControlRef c, 
												short start, 
												short end);
OSStatus SetControlTitleWithCString			(	ControlRef c,
												const char* str);
MenuRef GetPopupControlMenu					(	ControlRef c);
void ClearPopupControl						(	ControlRef c,
												int from);
void AddMenuItemValue						(	MenuRef m, 
												const char* str);
void GetPopupControlMenuItemValue			(	ControlRef c, 
												int item, 
												char* str, 
												int max);
void GetPopupControlMenuItemCommandID		(	ControlRef c, 
												int item, 
												MenuCommand* cmd);
void SetPopupControlMenuItemValue			(	ControlRef c, 
												int item, 
												const char* str);
void AddPopupControlMenuItemValue			(	ControlRef c, 
												const char* str);
int CountPopupControlMenuItems				(	ControlRef c);
void EnablePopupControlMenuItem				(	ControlRef c, 
												int item);
void DisablePopupControlMenuItem			(	ControlRef c, 
												int item);
void CheckPopupControlMenuItem				(	ControlRef c, 
												int item,
												int enabled);
int IsPopupControlMenuItemChecked			(	ControlRef c, 
												int item);
int IsPopupControlMenuItemEnabled			(	ControlRef c, 
												int item);
int GetPopupControl32BitValue				(	ControlRef c);
OSStatus SetPopupControl32BitValue			(	ControlRef c, 
												int value);

void CreateMMColorWellWithHIView			(	HIViewRef c);
int MMColorWellGetComponentCount			(	HIViewRef c);

void MMColorWellGetRGBColor					(	HIViewRef c,
												float* clr);
void MMColorWellSetRGBColor					(	HIViewRef c,
												float* clr);

void MMColorWellGetCMYKColor				(	HIViewRef c,
												float* clr);
void MMColorWellSetCMYKColor				(	HIViewRef c,
												float* clr);

void MMColorWellGetColor					(	HIViewRef c,
												float* clr,
												int cmyk);
void MMColorWellSetColor					(	HIViewRef c,
												float* clr,
												int cmyk);

void CreatePDFPatternWellWithHIView			(	HIViewRef c);
void PDFPatternWellGetName					(	HIViewRef c,
												char* name);
void PDFPatternWellSetName					(	HIViewRef c,
												char* name);
CGPDFDocumentRef PDFPatternWellGetPDF		(	HIViewRef c);
void PDFPatternWellSetPDF					(	HIViewRef c,
												CGPDFDocumentRef pdf);
void PDFPatternPlot							(	void* info, 
												CGContextRef ctx);

void BevelButtonSetImageRef					(	ControlRef c,
												CGImageRef img);
CGImageRef LoadPNGImageFromBundle			(	CFBundleRef bndl, 
												CFStringRef name);
void TransformHIViewToCG					(	CGContextRef ctx, 
												HIViewRef vw);
void SendCommandToWindow					(	WindowRef wd, 
												UInt32 sign);
void SendCommandToApplication				(	UInt32 sign);
OSStatus yn_evt_hdlr						(	EventHandlerCallRef hdlr, 
												EventRef evt, 
												void *up);
void MacRoman2UTF8							(	char* str,
												int sz);
void UTF82MacRoman							(	char* str,
												int sz);

void MMSetRect								(	Rect *  r,
												short   left,
												short   top,
												short   right,
												short   bottom);//CC
void MMOffsetRect							(	Rect *  r,
												short   h,
												short   v);//CC
		
Point HIWindowConvertLocal2Global			(	WindowRef wd,
												Point pt);//CC
Point HIWindowConvertGlobal2Local			(	WindowRef wd,
												Point pt);//CC
	
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
