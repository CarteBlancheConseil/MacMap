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
//----------------------------------------------------------------------------

#include "Carb_Utils.h"	
#include "Color_Utils.h"	
#include <limits.h>	
#include <AppKit/NSGraphics.h>

// ---------------------------------------------------------------------------
//
// ------------
typedef struct selected_p{
	DataBrowserItemID	item;
	int					count;
}selected_p;

#define kColorWellLocked	'Lock'

// ---------------------------------------------------------------------------
// 
// -----------
static void db_count_selected(	DataBrowserItemID item, 
								DataBrowserItemState state, 
								void *clientData){
selected_p *p=(selected_p*)clientData;
	p->count++;
	if(p->item==0){
		p->item=item;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
static void db_is_selected(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void *clientData){
selected_p *p=(selected_p*)clientData;
	if(p->item==item){
		p->count++;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult uint_filter(	ControlRef c, 
									SInt16* key, 
									SInt16* car, 
									EventModifiers* m){
	if((*car)<48||(*car)>57){
		switch(*car){
			case kBackspaceCharCode:
			case kLeftArrowCharCode:
			case kRightArrowCharCode:
			case kUpArrowCharCode:
			case kDownArrowCharCode:
			case kSpaceCharCode:
			case kDeleteCharCode:
				return(kControlKeyFilterPassKey);
				break;
  		}
		return(kControlKeyFilterBlockKey);
	}
	return(kControlKeyFilterPassKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult int_filter(	ControlRef c, 
									SInt16* key, 
									SInt16* car, 
									EventModifiers* m){
	if((*car)<48||(*car)>57){
		switch(*car){
			case 43://+
			case 45://-
			case kBackspaceCharCode:
			case kLeftArrowCharCode:
			case kRightArrowCharCode:
			case kUpArrowCharCode:
			case kDownArrowCharCode:
			case kSpaceCharCode:
			case kDeleteCharCode:
				return(kControlKeyFilterPassKey);
				break;
  		}
		return(kControlKeyFilterBlockKey);
	}
	return(kControlKeyFilterPassKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult float_filter(	ControlRef c, 
										SInt16* key, 
										SInt16* car, 
										EventModifiers* m){
	if((*car)<48||(*car)>57){
		switch(*car){
			case 44://,
				*car=46;
			case 46://.
			case 43://+
			case 45://-
			case kBackspaceCharCode:
			case kLeftArrowCharCode:
			case kRightArrowCharCode:
			case kUpArrowCharCode:
			case kDownArrowCharCode:
			case kSpaceCharCode:
			case kDeleteCharCode:
				return(kControlKeyFilterPassKey);
				break;
	  		}
		return(kControlKeyFilterBlockKey);
	}
	return(kControlKeyFilterPassKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
ControlKeyFilterResult ufloat_filter(	ControlRef c, 
										SInt16* key, 
										SInt16* car, 
										EventModifiers* m){
	if((*car)<48||(*car)>57){
		switch(*car){
			case 44://,
				*car=46;
			case 46://.
			case kBackspaceCharCode:
			case kLeftArrowCharCode:
			case kRightArrowCharCode:
			case kUpArrowCharCode:
			case kDownArrowCharCode:
			case kSpaceCharCode:
			case kDeleteCharCode:
				return(kControlKeyFilterPassKey);
				break;
	  		}
		return(kControlKeyFilterBlockKey);
	}
	return(kControlKeyFilterPassKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
void DataBrowserInit(	ControlRef c, 
						DataBrowserItemDataUPP data, 
						DataBrowserItemNotificationUPP notifier,
						long RefCon){
DataBrowserCallbacks 	dbc;
	dbc.version=kDataBrowserLatestCallbacks;
	InitDataBrowserCallbacks(&dbc);
	if(data){
		dbc.u.v1.itemDataCallback=data;
	}
	if(notifier){
		dbc.u.v1.itemNotificationCallback=notifier;
	}
	(void)SetDataBrowserCallbacks(c,&dbc);
	SetControlReference(c,RefCon);
}

// ---------------------------------------------------------------------------
// 
// -----------
void DataBrowserClose(	ControlRef c){
DataBrowserCallbacks 	dbc;
	(void)GetDataBrowserCallbacks(c,&dbc);
	dbc.u.v1.itemDataCallback=NULL;
	dbc.u.v1.itemNotificationCallback=NULL;
	(void)SetDataBrowserCallbacks(c,&dbc);
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 DataBrowserCountSelected(ControlRef c){
selected_p			p={0,0};
DataBrowserItemUPP	upp=db_count_selected;
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&p);
	return(p.count);
}

// ---------------------------------------------------------------------------
// 
// -----------
DataBrowserItemID DataBrowserSelectedItem(ControlRef c){
selected_p			p={0,0};
DataBrowserItemUPP	upp=db_count_selected;
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&p);
	if(p.count!=1){
		p.item=0;
	}
	return(p.item);
}

// ---------------------------------------------------------------------------
// 
// -----------
int DataBrowserIsSelectedItem(ControlRef c, DataBrowserItemID item){
selected_p			p={item,0};
DataBrowserItemUPP	upp=db_is_selected;
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,upp,&p);
	return(p.count==1);
}

// ---------------------------------------------------------------------------
// 
// -----------
void DataBrowserReset(ControlRef c, UInt32 n, DataBrowserPropertyID* pid){
UInt32	i;
	for(i=0;i<n;i++){
		RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,pid[i]);
	}
	SetDataBrowserScrollPosition(c,0,0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void SetControlAbility(ControlRef c, Boolean b){
	if(b){
		EnableControl(c);
	}
	else{
		DisableControl(c);
	}
	Draw1Control(c);
}
												
// ---------------------------------------------------------------------------
// 
// -----------
void GetTextControlValue(ControlRef c, char* val, int max){
Size	sz;
	GetControlData(c,kControlEditTextPart,kControlEditTextTextTag,max,val,&sz);
	if(sz<max){
		val[sz]=0;
	}
	else{
		val[max-1]=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void SetTextControlValue(ControlRef c, const char* val){
if(!c){
fprintf(stderr,"SetTextControlValue::c==NULL\n");
	return;
}
	SetControlData(c,kControlEditTextPart,kControlEditTextTextTag,strlen(val),val);
	Draw1Control(c);
}

// ---------------------------------------------------------------------------
// 
// -----------
void SetTextControlFilter(ControlRef c, ControlKeyFilterUPP flt){
if(!c){
fprintf(stderr,"SetTextControlFilter::c==NULL\n");
	return;
}
	if(	(flt==uint_filter)	||
		(flt==int_filter)	||
		(flt==ufloat_filter)||
		(flt==float_filter)	){
ControlFontStyleRec		stl;
		stl.flags=kControlUseAllMask;
		GetControlData(c,kControlEditTextPart,kControlEditTextStyleTag,sizeof(ControlFontStyleRec),&stl,NULL);
		stl.flags+=kControlUseJustMask;
		stl.just=teFlushLeft;
		SetControlData(c,kControlEditTextPart,kControlEditTextStyleTag,sizeof(ControlFontStyleRec),&stl);
	}
	SetControlData(c,kControlEditTextPart,kControlEditTextKeyFilterTag,sizeof(ControlKeyFilterUPP),&flt);
}

// ---------------------------------------------------------------------------
// 
// -----------
MenuRef GetPopupControlMenu(ControlRef c){
MenuRef	m=GetControlPopupMenuRef(c);
	if(!m){
		(void)GetBevelButtonMenuRef(c,&m);
	}
	return(m);
}

// ---------------------------------------------------------------------------
// 
// -----------
void ClearPopupControl(ControlRef c, int from){
MenuRef		m=GetPopupControlMenu(c);
ControlKind	k;
int			i,n=CountMenuItems(m)-(from-1);
	for(i=1;i<=n;i++){
		DeleteMenuItem(m,from);
	}
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void AddMenuItemValue(MenuRef m, const char* str){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman); 
	AppendMenuItemTextWithCFString(m,cfs,0,0,NULL);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void AddPopupControlMenuItemValue(ControlRef c, const char* str){
MenuRef		m=GetPopupControlMenu(c);
ControlKind	k;

	if(m==NULL){
//fprintf(stderr,"NULL Menu\n");
	}
//fprintf(stderr,"%s\n",str);
	AddMenuItemValue(m,str);
	
//fprintf(stderr,"n=%d\n",CountMenuItems(m));
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
		SetControl32BitMaximum(c,CountMenuItems(m));
	}
}

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
}

// ---------------------------------------------------------------------------
// 
// -----------
void SendCommandToApplication(UInt32 sign){
EventRef	evt;
HICommand	cmd;
		
	cmd.commandID=sign;
    CreateEvent(kCFAllocatorDefault,kEventClassCommand,kEventProcessCommand,GetCurrentEventTime(),kEventAttributeUserEvent,&evt);
	(void)SetEventParameter(evt,kEventParamDirectObject,typeHICommand,sizeof(HICommand),&cmd);
	(void)SendEventToApplication(evt);
	ReleaseEvent(evt);
}


// ---------------------------------------------------------------------------
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
}
