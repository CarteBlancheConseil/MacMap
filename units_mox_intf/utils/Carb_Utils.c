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
ControlKeyFilterResult crcatch_filter(	ControlRef c, 
										SInt16* key, 
										SInt16* car, 
										EventModifiers* m){
	if((*car)==13){
fprintf(stderr,"CR13\n");
		return(kControlKeyFilterPassKey);
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
void DataBrowserAdd(ControlRef c){
}

// ---------------------------------------------------------------------------
// 
// -----------
void DataBrowserRemove(ControlRef c){
}

// ---------------------------------------------------------------------------
// 
// -----------
void DataBrowserUpdate(ControlRef c){
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
void GetTextControlValueLength(ControlRef c, int* len){
char	bf[16];
Size	sz;
	GetControlData(c,kControlEditTextPart,kControlEditTextTextTag,16,bf,&sz);
	*len=sz;
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
void GetTextControlSelection(ControlRef c, short* start, short* end){
ControlEditTextSelectionRec	sl={0,0};
Size						sz;
	GetControlData(c,kControlEntireControl,kControlEditTextSelectionTag,sizeof(ControlEditTextSelectionRec),&sl,&sz);
	*start=sl.selStart;
	*end=sl.selEnd;
}

// ---------------------------------------------------------------------------
// 
// -----------
void SetTextControlSelection(ControlRef c, short start, short end){
ControlEditTextSelectionRec	sl={start,end};
	SetControlData(c,kControlEntireControl,kControlEditTextSelectionTag,sizeof(ControlEditTextSelectionRec),&sl);
	Draw1Control(c);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus SetControlTitleWithCString(ControlRef c, const char* str){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman);
OSStatus	status=SetControlTitleWithCFString(c,cfs);
	CFRelease(cfs);
	return(status);
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
void GetPopupControlMenuItemValue(ControlRef c, int item, char* str, int max){
MenuRef		m=GetPopupControlMenu(c);
CFStringRef	cfs;
	CopyMenuItemTextAsCFString(m,item,&cfs);
	CFStringGetCString(cfs,str,max,kCFStringEncodingMacRoman);
}

// ---------------------------------------------------------------------------
// 
// -----------
void GetPopupControlMenuItemCommandID(ControlRef c, int item, MenuCommand* cmd){
MenuRef	m=GetPopupControlMenu(c);
	GetMenuItemCommandID(m,item,cmd);  
}

// ---------------------------------------------------------------------------
// 
// -----------
void SetPopupControlMenuItemValue(ControlRef c, int item, const char* str){
MenuRef		m=GetPopupControlMenu(c);
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman); 
	(void)SetMenuItemTextWithCFString(m,item,cfs);
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
// -----------
int CountPopupControlMenuItems(ControlRef c){
MenuRef	m=GetPopupControlMenu(c);
	return(CountMenuItems(m));
}

// ---------------------------------------------------------------------------
// 
// -----------
void EnablePopupControlMenuItem(ControlRef c, int item){
MenuRef	m=GetPopupControlMenu(c);
	EnableMenuItem(m,item);
}

// ---------------------------------------------------------------------------
// 
// -----------
void DisablePopupControlMenuItem(ControlRef c, int item){
MenuRef	m=GetPopupControlMenu(c);
	DisableMenuItem(m,item);
}

// ---------------------------------------------------------------------------
// 
// -----------
void CheckPopupControlMenuItem(ControlRef c, int item, int check){
MenuRef	m=GetPopupControlMenu(c);
	CheckMenuItem(m,item,check);
}

// ---------------------------------------------------------------------------
// 
// -----------
int IsPopupControlMenuItemChecked(ControlRef c, int item){
MenuRef			m=GetPopupControlMenu(c);
CharParameter	check;
	GetItemMark(m,item,&check);
fprintf(stderr,"->%c\n",check);

	return(check!=0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int IsPopupControlMenuItemEnabled(ControlRef c, int item){
MenuRef			m=GetPopupControlMenu(c);
	return(IsMenuItemEnabled(m,item));
}

// ---------------------------------------------------------------------------
// 
// -----------
int GetPopupControl32BitValue(ControlRef c){
fprintf(stderr,"GetPopupControl32BitValue\n");
ControlKind	k;
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
fprintf(stderr,"k.kind==kControlKindPopupButton\n");
		return(GetControl32BitValue(c));
	}
	else if(k.kind==kControlKindBevelButton){
fprintf(stderr,"k.kind==kControlKindBevelButton\n");
MenuItemIndex	bbmv;
		GetBevelButtonMenuValue(c,&bbmv);
		return(bbmv);
	}
	
int			i,n=CountPopupControlMenuItems(c);
fprintf(stderr,"n=%d\n",n);
	for(i=1;i<=n;i++){
fprintf(stderr,"i=%d\n",i);
		if(IsPopupControlMenuItemChecked(c,i)){
			return(i);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus SetPopupControl32BitValue(ControlRef c, int value){
fprintf(stderr,"SetPopupControl32BitValue\n");
ControlKind	k;
	GetControlKind(c,&k);
	if(k.kind==kControlKindPopupButton){
fprintf(stderr,"k.kind==kControlKindPopupButton\n");
		SetControl32BitValue(c,value);
		return(noErr);
	}
	else if(k.kind==kControlKindBevelButton){
fprintf(stderr,"k.kind==kControlKindBevelButton\n");
		SetBevelButtonMenuValue(c,value);
		return(noErr);
	}
	return(-1);
}

// ---------------------------------------------------------------------------
// 
// -----------
static void SendControlSelfCommandEvent(ControlRef c){
HICommand	cmd;
	(void)GetControlCommandID(c,&cmd.commandID);
	if(cmd.commandID==0){
		return;
	}
EventRef	evt;
	(void)CreateEvent(	kCFAllocatorDefault,
						kEventClassCommand,
						kEventCommandProcess,
						//TicksToEventTime(TickCount()),
                        GetCurrentEventTime(),
						kEventAttributeUserEvent,
						&evt);
	(void)SetEventParameter(	evt,
								kEventParamDirectObject,
								typeHICommand,
								sizeof(HICommand),
								&cmd);
	(void)SendEventToControl(evt,c);
	ReleaseEvent(evt);
}

// ---------------------------------------------------------------------------
// 
// -----------
static OSStatus MMColorWellDrawProc	(	EventHandlerCallRef hdlr,
										EventRef evt,
										void* udt){
OSStatus		status=eventNotHandledErr;
HIRect			hir;
HIViewRef		vw;
CGContextRef	ctx;	
float			clr[6];
int				cmp;
UInt32			sign,clss=GetEventClass(evt);	

	if(clss==kEventClassControl){
		GetEventParameter(evt,kEventParamDirectObject,typeControlRef,NULL,sizeof(HIViewRef),NULL,&vw);
		GetControlCommandID(vw,&sign);
		cmp=MMColorWellGetComponentCount(vw);
		switch(GetEventKind(evt)){
			case kEventControlDraw:
				status=GetEventParameter(	evt,
											kEventParamCGContextRef,
											typeCGContextRef,
											NULL,
											sizeof(CGContextRef),
											NULL,
											&ctx);
				if(status){
					break;
				}
				if(cmp==4){
					MMColorWellGetRGBColor(vw,clr);
				}
				else{
					MMColorWellGetCMYKColor(vw,clr);
				}
				HIViewGetBounds(vw,&hir);
				if(IsControlEnabled(vw)){
					if(cmp==4){
						CGContextSetRGBFillColor(ctx,clr[0],clr[1],clr[2],1);
					}
					else{
						CGContextSetCMYKFillColor(ctx,clr[0],clr[1],clr[2],clr[3],1);
					}
					CGContextSetRGBStrokeColor(ctx,0,0,0,1);
				}
				else{
					if(cmp==4){
						CGContextSetRGBFillColor(ctx,clr[0],clr[1],clr[2],0.5);
					}
					else{
						CGContextSetCMYKFillColor(ctx,clr[0],clr[1],clr[2],clr[3],0.5);
					}
					CGContextSetRGBStrokeColor(ctx,0,0,0,0.5);
				}
				CGContextFillRect(ctx,hir);
				CGContextStrokeRect(ctx,hir);
				break;
			
// A améliorer pour prendre en compte le track
			case kEventControlClick:
				if((IsControlEnabled(vw))&&(sign!=kColorWellLocked)){
					if(cmp==4){
						MMColorWellGetRGBColor(vw,clr);
						if(PickRGBColor(&clr[0],&clr[1],&clr[2])==noErr){
							MMColorWellSetRGBColor(vw,clr);
							SendControlSelfCommandEvent(vw);						
						}
					}
					else{
						MMColorWellGetCMYKColor(vw,clr);
						if(PickCMYKColor(&clr[0],&clr[1],&clr[2],&clr[3])==noErr){
							MMColorWellSetCMYKColor(vw,clr);
							SendControlSelfCommandEvent(vw);						
						}
					}
				}
				status=noErr;
				break;
			
			case kEventControlHit:
				break;
		}
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
void CreateMMColorWellWithHIView(HIViewRef v){
float	clr[6]={0,0,0,1,0,4};
	SetControlProperty(v,'BoB!','Clor',6*sizeof(float),clr);
EventTypeSpec	spec[]={{kEventClassControl,kEventControlDraw},
						{kEventClassControl,kEventControlClick},
						{kEventClassControl,kEventControlHit}};
	InstallEventHandler(GetControlEventTarget(v),
						MMColorWellDrawProc,
						GetEventTypeCount(spec),
						spec,
						v,
						NULL);	
}

// ---------------------------------------------------------------------------
// 
// -----------
int MMColorWellGetComponentCount(HIViewRef v){
float	buf[6]={0,0,0,1,0,4};
	GetControlProperty(v,'BoB!','Clor',6*sizeof(float),NULL,buf);
	return(buf[5]);
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellGetRGBColor(HIViewRef v, float* clr){
float	buf[6]={0,0,0,1,0,4};
	GetControlProperty(v,'BoB!','Clor',6*sizeof(float),NULL,buf);
	if(buf[5]==4){
		clr[0]=buf[0];
		clr[1]=buf[1];
		clr[2]=buf[2];
		clr[3]=buf[3];
	}
	else{
		CMYK2RGB(buf[0],buf[1],buf[2],buf[3],&clr[0],&clr[1],&clr[2]);
		clr[3]=buf[4];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellSetRGBColor(HIViewRef v, float* clr){
float	buf[6]={clr[0],clr[1],clr[2],clr[3],0,4};
	GetControlProperty(v,'BoB!','Clor',6*sizeof(float),NULL,buf);
	if(buf[5]==4){
		buf[0]=clr[0];
		buf[1]=clr[1];
		buf[2]=clr[2];
		buf[3]=clr[3];
	}
	else{// Le colorwell est repassé en RGB
		buf[0]=clr[0];
		buf[1]=clr[1];
		buf[2]=clr[2];
		buf[3]=clr[3];
		buf[4]=1;
		buf[5]=4;
	}	
	SetControlProperty(v,'BoB!','Clor',6*sizeof(float),buf);
	Draw1Control(v);
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellGetCMYKColor(HIViewRef v, float* clr){
float	buf[6]={0,0,0,0,1,5};
	GetControlProperty(v,'BoB!','Clor',6*sizeof(float),NULL,buf);
	if(buf[5]==5){
		clr[0]=buf[0];
		clr[1]=buf[1];
		clr[2]=buf[2];
		clr[3]=buf[3];
		clr[4]=buf[4];
	}
	else{
		RGB2CMYK(buf[0],buf[1],buf[2],&clr[0],&clr[1],&clr[2],&clr[3]);
		clr[4]=buf[3];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellSetCMYKColor(HIViewRef v, float* clr){
float	buf[6]={clr[0],clr[1],clr[2],clr[3],0,5};
	GetControlProperty(v,'BoB!','Clor',6*sizeof(float),NULL,buf);
	if(buf[5]==5){
		buf[0]=clr[0];
		buf[1]=clr[1];
		buf[2]=clr[2];
		buf[3]=clr[3];
		buf[4]=clr[4];
	}
	else{// Le colorwell est repassé en CMYK
		buf[0]=clr[0];
		buf[1]=clr[1];
		buf[2]=clr[2];
		buf[3]=clr[3];
		buf[4]=clr[4];
		buf[5]=5;
	}	
	SetControlProperty(v,'BoB!','Clor',6*sizeof(float),buf);
	Draw1Control(v);
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellGetColor(HIViewRef v, float* clr, int cmyk){
	if(cmyk){
		MMColorWellGetCMYKColor(v,clr);
	}
	else{
		MMColorWellGetRGBColor(v,clr);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMColorWellSetColor(HIViewRef v, float* clr, int cmyk){
	if(cmyk){
		MMColorWellSetCMYKColor(v,clr);
	}
	else{
		MMColorWellSetRGBColor(v,clr);
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
static OSStatus PDFPatternWellDrawProc	(	EventHandlerCallRef hdlr,
											EventRef evt,
											void* udt){
OSStatus			status=eventNotHandledErr;
HIRect				hir,box;
HIViewRef			vw;
CGContextRef		ctx;	
UInt32				clss=GetEventClass(evt);	
CGPDFDocumentRef	pdf;
CGPDFPageRef		pg;
float				color[4]={0,0,0,1};
CGPatternCallbacks	callbacks={0,&PDFPatternPlot,NULL};
CGColorSpaceRef		bspc;
CGColorSpaceRef		pspc;
CGPatternRef		pat;

	if(clss==kEventClassControl){
		GetEventParameter(evt,kEventParamDirectObject,typeControlRef,NULL,sizeof(HIViewRef),NULL,&vw);
		switch(GetEventKind(evt)){
			case kEventControlDraw:
				status=GetEventParameter(	evt,
											kEventParamCGContextRef,
											typeCGContextRef,
											NULL,
											sizeof(CGContextRef),
											NULL,
											&ctx);
				if(status){
					break;
				}
				pdf=PDFPatternWellGetPDF(vw);
				HIViewGetBounds(vw,&hir);
				CGContextSetRGBStrokeColor(ctx,0,0,0,1);
				CGContextStrokeRect(ctx,hir);
				hir=CGRectInset(hir,1,1);
				CGContextSetRGBFillColor(ctx,1,1,1,1);
				CGContextFillRect(ctx,hir);
				if(!pdf){
					break;
				}
				pg=CGPDFDocumentGetPage(pdf,1);
				box=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
				bspc=CGColorSpaceCreateDeviceRGB();
				pspc=CGColorSpaceCreatePattern(bspc);
				CGContextSetFillColorSpace(ctx,pspc);
				CGColorSpaceRelease(pspc);
				CGColorSpaceRelease(bspc);
				pat=CGPatternCreate(pdf, 
									box,
									CGAffineTransformIdentity,
									box.size.width,
									box.size.height,
									kCGPatternTilingConstantSpacing,
									false, 
									&callbacks);   
				CGContextSetFillPattern(ctx,pat,color);
				CGPatternRelease(pat);
				CGContextFillRect(ctx,hir);
				break;
			
// A améliorer pour prendre en compte le track
			case kEventControlClick:
				SendControlSelfCommandEvent(vw);				
				status=noErr;
				break;
			
			case kEventControlHit:
				break;
		}
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
void CreatePDFPatternWellWithHIView(HIViewRef v){
char				nm[256]="";
CGPDFDocumentRef	pdf=NULL;
	SetControlProperty(v,'BoB!','PtNm',256,nm);
	SetControlProperty(v,'BoB!','PtPt',sizeof(CGPDFDocumentRef),&pdf);
EventTypeSpec	spec[]={{kEventClassControl,kEventControlDraw},
						{kEventClassControl,kEventControlClick},
						{kEventClassControl,kEventControlHit}};
	InstallEventHandler(GetControlEventTarget(v),
						PDFPatternWellDrawProc,
						GetEventTypeCount(spec),
						spec,
						v,
						NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void PDFPatternWellGetName(HIViewRef v, char* name){
	GetControlProperty(v,'BoB!','PtNm',256,NULL,name);
}

// ---------------------------------------------------------------------------
// 
// -----------
void PDFPatternWellSetName(HIViewRef v, char* name){
	SetControlProperty(v,'BoB!','PtNm',256,name);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGPDFDocumentRef PDFPatternWellGetPDF(HIViewRef v){
CGPDFDocumentRef	pdf=NULL;
	GetControlProperty(v,'BoB!','PtPt',sizeof(CGPDFDocumentRef),NULL,&pdf);
	return(pdf);
}

// ---------------------------------------------------------------------------
// 
// -----------
void PDFPatternWellSetPDF(HIViewRef v, CGPDFDocumentRef pdf){
CGPDFDocumentRef	bpdf=NULL;
	GetControlProperty(v,'BoB!','PtPt',sizeof(CGPDFDocumentRef),NULL,&bpdf);
	if(bpdf){
		CGPDFDocumentRelease(bpdf);
	}
	SetControlProperty(v,'BoB!','PtPt',sizeof(CGPDFDocumentRef),&pdf);
	Draw1Control(v);
}

// ---------------------------------------------------------------------------
// 
// -----------
void BevelButtonSetImageRef(ControlRef c, CGImageRef img){
ControlButtonContentInfo	info;

	info.contentType=kControlContentCGImageRef;
	info.u.imageRef=img;
	SetBevelButtonContentInfo(c,&info);
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
void TransformHIViewToCG(CGContextRef ctx, HIViewRef theView){
HIRect bounds;
HIViewGetBounds(theView, &bounds);
    CGContextConcatCTM(ctx,CGAffineTransformMake(1,0,0,-1,0,bounds.size.height));
}

// ---------------------------------------------------------------------------
// 
// -----------
void SendCommandToWindow(WindowRef wd, UInt32 sign){
EventRef	evt;
HICommand	cmd;
		
	cmd.commandID=sign;
//    CreateEvent(kCFAllocatorDefault,kEventClassCommand,kEventProcessCommand,TicksToEventTime(TickCount()),kEventAttributeUserEvent,&evt);
    CreateEvent(kCFAllocatorDefault,kEventClassCommand,kEventProcessCommand,GetCurrentEventTime(),kEventAttributeUserEvent,&evt);
	(void)SetEventParameter(evt,kEventParamDirectObject,typeHICommand,sizeof(HICommand),&cmd);
	(void)SendEventToWindow(evt,wd);
	ReleaseEvent(evt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void SendCommandToApplication(UInt32 sign){
EventRef	evt;
HICommand	cmd;
		
	cmd.commandID=sign;
//	CreateEvent(kCFAllocatorDefault,kEventClassCommand,kEventProcessCommand,TicksToEventTime(TickCount()),kEventAttributeUserEvent,&evt);
    CreateEvent(kCFAllocatorDefault,kEventClassCommand,kEventProcessCommand,GetCurrentEventTime(),kEventAttributeUserEvent,&evt);
	(void)SetEventParameter(evt,kEventParamDirectObject,typeHICommand,sizeof(HICommand),&cmd);
	(void)SendEventToApplication(evt);
	ReleaseEvent(evt);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus yn_evt_hdlr(	EventHandlerCallRef hdlr, 
						EventRef evt, 
						void *up){
OSStatus				result=eventNotHandledErr;
HICommand				cmd;
ynrec*					x=(ynrec*)up;

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
//				HideWindow(x->wd);
				QuitAppModalLoopForWindow(x->wd);
				x->ok=true;
				result=noErr;
				break;
			case kHICommandCancel:
//				HideWindow(x->wd);
				QuitAppModalLoopForWindow(x->wd);
				x->ok=false;
				result=noErr;
				break;
		}
	}
	return(result);
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

// ---------------------------------------------------------------------------
// 
// -----------
void MMSetRect(	Rect *  r,
				short   left,
				short   top,
				short   right,
				short   bottom){
	r->left=left;
	r->top=top;
	r->right=right;
	r->bottom=bottom;
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMOffsetRect(	Rect *  r,
					short   h,
					short   v){
	r->left+=h;
	r->top+=v;
	r->right+=h;
	r->bottom+=v;
}

// ---------------------------------------------------------------------------
// 
// -----------
Point HIWindowConvertLocal2Global(	WindowRef wd,
									Point pt){
HIRect	hir;
	HIWindowGetBounds(wd,kWindowStructureRgn,kHICoordSpaceScreenPixel,&hir);
	
	pt.h+=hir.origin.x;
	pt.v+=hir.origin.y;
	
	HIWindowGetBounds(wd,kWindowTitleBarRgn,kHICoordSpaceScreenPixel,&hir);
	if(HIWindowTestAttribute(wd,kHIWindowBitSideTitlebar)){
		pt.h+=hir.size.width;
	}
	else{
		pt.v+=hir.size.height;
	}	
	return(pt);
}

// ---------------------------------------------------------------------------
// 
// -----------
Point HIWindowConvertGlobal2Local(	WindowRef wd,
									Point pt){
HIRect	hir;
	HIWindowGetBounds(wd,kWindowStructureRgn,kHICoordSpaceScreenPixel,&hir);
	
	pt.h-=hir.origin.x;
	pt.v-=hir.origin.y;
	
	HIWindowGetBounds(wd,kWindowTitleBarRgn,kHICoordSpaceScreenPixel,&hir);
	if(HIWindowTestAttribute(wd,kHIWindowBitSideTitlebar)){
		pt.h-=hir.size.width;
	}
	else{
		pt.v-=hir.size.height;
	}	
	return(pt);
}
