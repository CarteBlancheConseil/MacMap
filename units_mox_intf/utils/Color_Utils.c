//----------------------------------------------------------------------------
// File : Color_Utils.c
// Project : MacMap
// Purpose : C source file : Color utilities
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
// 03/02/2010 creation.
//----------------------------------------------------------------------------

#include <mox_intf/Color_Utils.h>	
#include <mox_intf/Carb_Utils.h>

// ---------------------------------------------------------------------------
// 
// -----------
enum{	
	kCMYKPickerSignature		='CPck',

	kCMYKCyanCursorID			=2,
	kCMYKCyanCursorCmd			='cCrs',
	kCMYKCyanValueID			=3,
	kCMYKCyanValueCmd			='cVal',
	kCMYKCyanWellID				=4,
	
	kCMYKMagentaCursorID		=6,
	kCMYKMagentaCursorCmd		='mCrs',
	kCMYKMagentaValueID			=7,
	kCMYKMagentaValueCmd		='mVal',
	kCMYKMagentaWellID			=8,

	kCMYKYellowCursorID			=10,
	kCMYKYellowCursorCmd		='yCrs',
	kCMYKYellowValueID			=11,
	kCMYKYellowValueCmd			='yVal',
	kCMYKYellowWellID			=12,

	kCMYKBlackCursorID			=14,
	kCMYKBlackCursorCmd			='kCrs',
	kCMYKBlackValueID			=15,
	kCMYKBlackValueCmd			='kVal',
	kCMYKBlackWellID			=16,

	kCMYKWellID					=17
};

// ---------------------------------------------------------------------------
// 
// -----------
static ControlRef cmyk_get_control(WindowRef wd, UInt32 cid){
ControlID	crid={kCMYKPickerSignature,cid};
ControlRef	c;
	
	GetControlByID(wd,&crid,&c);
	return(c);
}

// ---------------------------------------------------------------------------
// 
// -----------
static OSStatus cmyk_wd_handler(EventHandlerCallRef	inCallRef,
								EventRef			inEvent,
								void*				inUserData){
ynrec*				prm=(ynrec*)inUserData;
OSStatus			result=eventNotHandledErr;
HICommand			cmd;
ControlRef			c;
float				clrb[5]={0,0,0,0,1};
float				clr[5]={0,0,0,0,1};
HIViewID			vid={kCMYKPickerSignature,kCMYKCyanWellID};
HIViewRef			vw;
char				str[256];

	if(GetEventClass(inEvent)==kEventClassCommand){
		GetEventParameter(	inEvent,
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kCMYKCyanCursorCmd:
				c=cmyk_get_control(prm->wd,kCMYKCyanCursorID);
				clrb[0]=GetControl32BitValue(c);
				c=cmyk_get_control(prm->wd,kCMYKCyanValueID);
				sprintf(str,"%.0f",clrb[0]);
				SetTextControlValue(c,str);
				clrb[0]/=100.0;				
				vid.id=kCMYKCyanWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[0]=clrb[0];
				MMColorWellSetCMYKColor(vw,clr);
				result=noErr;
				break;
			case kCMYKMagentaCursorCmd:
				c=cmyk_get_control(prm->wd,kCMYKMagentaCursorID);
				clrb[1]=GetControl32BitValue(c);
				c=cmyk_get_control(prm->wd,kCMYKMagentaValueID);
				sprintf(str,"%.0f",clrb[1]);
				SetTextControlValue(c,str);
				clrb[1]/=100.0;				
				vid.id=kCMYKMagentaWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[1]=clrb[1];
				MMColorWellSetCMYKColor(vw,clr);
				result=noErr;
				break;
			case kCMYKYellowCursorCmd:
				c=cmyk_get_control(prm->wd,kCMYKYellowCursorID);
				clrb[2]=GetControl32BitValue(c);
				c=cmyk_get_control(prm->wd,kCMYKYellowValueID);
				sprintf(str,"%.0f",clrb[2]);
				SetTextControlValue(c,str);
				clrb[2]/=100.0;				
				vid.id=kCMYKYellowWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[2]=clrb[2];
				MMColorWellSetCMYKColor(vw,clr);
				result=noErr;
				break;
			case kCMYKBlackCursorCmd:
				c=cmyk_get_control(prm->wd,kCMYKBlackCursorID);
				clrb[3]=GetControl32BitValue(c);
				c=cmyk_get_control(prm->wd,kCMYKBlackValueID);
				sprintf(str,"%.0f",clrb[3]);
				SetTextControlValue(c,str);
				clrb[3]/=100.0;				
				vid.id=kCMYKBlackWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(prm->wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[3]=clrb[3];
				MMColorWellSetCMYKColor(vw,clr);
				result=noErr;
				break;
			default:
				return(yn_evt_hdlr(inCallRef,inEvent,inUserData));
				break;
		}
	}	
	return(result);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus cmyk_key_proc(	EventHandlerCallRef hdlr,
						EventRef evt,
						void* udt){
WindowRef	wd=(WindowRef)udt;
ControlRef	c;
	if(GetKeyboardFocus(wd,&c)){
	}
UInt32			sgn;
float			clrb[5]={0,0,0,0,1};
float			clr[5]={0,0,0,0,1};
HIViewID		vid={kCMYKPickerSignature,kCMYKCyanWellID};
HIViewRef		vw;
char			str[256];

	if(GetControlCommandID(c,&sgn)==noErr){
		switch(sgn){
			case kCMYKCyanValueCmd:
				GetTextControlValue(c,str,256);
				clrb[0]=atof(str)/100.0;
				if(clrb[0]<0){
					clrb[0]=0;
					sprintf(str,"%.0f",clrb[0]*100);
					SetTextControlValue(c,str);
				}
				if(clrb[0]>1){
					clrb[0]=1;
					sprintf(str,"%.0f",clrb[0]*100);
					SetTextControlValue(c,str);
				}
				c=cmyk_get_control(wd,kCMYKCyanCursorID);
				SetControl32BitValue(c,(int)clrb[0]*100);
				vid.id=kCMYKCyanWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[0]=clrb[0];
				MMColorWellSetCMYKColor(vw,clr);
				break;
			case kCMYKMagentaValueCmd:
				GetTextControlValue(c,str,256);
				clrb[1]=atof(str)/100.0;
				if(clrb[1]<0){
					clrb[1]=0;
					sprintf(str,"%.0f",clrb[1]*100);
					SetTextControlValue(c,str);
				}
				if(clrb[1]>1){
					clrb[1]=1;
					sprintf(str,"%.0f",clrb[1]*100);
					SetTextControlValue(c,str);
				}
				c=cmyk_get_control(wd,kCMYKMagentaCursorID);
				SetControl32BitValue(c,(int)clrb[1]*100);
				vid.id=kCMYKMagentaWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[1]=clrb[1];
				MMColorWellSetCMYKColor(vw,clr);
				break;
			case kCMYKYellowValueCmd:
				GetTextControlValue(c,str,256);
				clrb[2]=atof(str)/100.0;
				if(clrb[2]<0){
					clrb[2]=0;
					sprintf(str,"%.0f",clrb[2]*100);
					SetTextControlValue(c,str);
				}
				if(clrb[2]>1){
					clrb[2]=1;
					sprintf(str,"%.0f",clrb[2]*100);
					SetTextControlValue(c,str);
				}
				c=cmyk_get_control(wd,kCMYKYellowCursorID);
				SetControl32BitValue(c,(int)clrb[2]*100);
				vid.id=kCMYKYellowWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[2]=clrb[2];
				MMColorWellSetCMYKColor(vw,clr);
				break;
			case kCMYKBlackValueCmd:
				GetTextControlValue(c,str,256);
				clrb[3]=atof(str)/100.0;
				if(clrb[3]<0){
					clrb[3]=0;
					sprintf(str,"%.0f",clrb[3]*100);
					SetTextControlValue(c,str);
				}
				if(clrb[3]>1){
					clrb[3]=1;
					sprintf(str,"%.0f",clrb[3]*100);
					SetTextControlValue(c,str);
				}
				c=cmyk_get_control(wd,kCMYKBlackCursorID);
				SetControl32BitValue(c,(int)clrb[3]*100);
				vid.id=kCMYKBlackWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellSetCMYKColor(vw,clrb);
				vid.id=kCMYKWellID;
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellGetCMYKColor(vw,clr);
				clr[3]=clrb[3];
				MMColorWellSetCMYKColor(vw,clr);
				break;
			default:
				break;
		}
	}
	else{
	}	
	return(eventNotHandledErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus PickRGBColor(float* r, float* g, float* b){
NColorPickerInfo    info;
OSStatus			status;
	
	memset(&info,0,sizeof(NColorPickerInfo));
	info.placeWhere=kCenterOnMainScreen;
	info.flags=kColorPickerDialogIsMoveable|kColorPickerDialogIsModal;
	info.theColor.color.rgb.red=round((*r)*USHRT_MAX);
	info.theColor.color.rgb.green=round((*g)*USHRT_MAX);
	info.theColor.color.rgb.blue=round((*b)*USHRT_MAX);
	status=NPickColor(&info);
	if(status!=noErr){
		return(status);
	}
	if(!info.newColorChosen){
		return(-1);
	}
	(*r)=((float)info.theColor.color.rgb.red)/((float)USHRT_MAX);
	(*g)=((float)info.theColor.color.rgb.green)/((float)USHRT_MAX);
	(*b)=((float)info.theColor.color.rgb.blue)/((float)USHRT_MAX);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus PickCMYKColor(float* c, float* m, float* y, float* k){
OSStatus		status;
IBNibRef		nib=NULL;
EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
EventTypeSpec	spec[]={{kEventClassKeyboard,kEventRawKeyUp}};
ynrec			wr={NULL,false};
ControlRef		ctl;
char			val[256];
HIViewID		vid={kCMYKPickerSignature,kCMYKCyanWellID};
HIViewRef		vw;
float			clr[5]={*c,*m,*y,*k,1};
float			clrb[5]={0,0,0,0,1};

	for(;;){
		status=CreateNibReferenceWithCFBundle(CFBundleGetMainBundle(),CFSTR("main"),&nib);
		if(status){
			break;
		}
		status=CreateWindowFromNib(nib,CFSTR("CMYKPicker"),&wr.wd);
		if(status){
			break;
		}
EventHandlerUPP hdlr=cmyk_wd_handler;
		InstallWindowEventHandler(wr.wd,hdlr,GetEventTypeCount(kWindowEvents),kWindowEvents,(void*)&wr,NULL);
		
// Cyan
		ctl=cmyk_get_control(wr.wd,kCMYKCyanCursorID);
		SetControl32BitValue(ctl,clr[0]*100);
		
		ctl=cmyk_get_control(wr.wd,kCMYKCyanValueID);
		SetTextControlFilter(ctl,uint_filter);
		InstallEventHandler(GetControlEventTarget(ctl),
							cmyk_key_proc,
							GetEventTypeCount(spec),
							spec,
							wr.wd,
							NULL);
		sprintf(val,"%.0f",clr[0]*100.0);
		SetTextControlValue(ctl,val);	
			
		vid.id=kCMYKCyanWellID;
		HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
		CreateMMColorWellWithHIView(vw);
		clrb[0]=clr[0];
		MMColorWellSetCMYKColor(vw,clrb);
		clrb[0]=0;

// Magenta
		ctl=cmyk_get_control(wr.wd,kCMYKMagentaCursorID);
		SetControl32BitValue(ctl,clr[1]*100);
		
		ctl=cmyk_get_control(wr.wd,kCMYKMagentaValueID);
		SetTextControlFilter(ctl,uint_filter);
		InstallEventHandler(GetControlEventTarget(ctl),
							cmyk_key_proc,
							GetEventTypeCount(spec),
							spec,
							wr.wd,
							NULL);
		sprintf(val,"%.0f",clr[1]*100.0);
		SetTextControlValue(ctl,val);	
			
		vid.id=kCMYKMagentaWellID;
		HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
		CreateMMColorWellWithHIView(vw);
		clrb[1]=clr[1];
		MMColorWellSetCMYKColor(vw,clrb);
		clrb[1]=0;

// Yellow
		ctl=cmyk_get_control(wr.wd,kCMYKYellowCursorID);
		SetControl32BitValue(ctl,clr[2]*100);
		
		ctl=cmyk_get_control(wr.wd,kCMYKYellowValueID);
		SetTextControlFilter(ctl,uint_filter);
		InstallEventHandler(GetControlEventTarget(ctl),
							cmyk_key_proc,
							GetEventTypeCount(spec),
							spec,
							wr.wd,
							NULL);
		sprintf(val,"%.0f",clr[2]*100.0);
		SetTextControlValue(ctl,val);	
			
		vid.id=kCMYKYellowWellID;
		HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
		CreateMMColorWellWithHIView(vw);
		clrb[2]=clr[2];
		MMColorWellSetCMYKColor(vw,clrb);
		clrb[2]=0;

// Black
		ctl=cmyk_get_control(wr.wd,kCMYKBlackCursorID);
		SetControl32BitValue(ctl,clr[3]*100);
		
		ctl=cmyk_get_control(wr.wd,kCMYKBlackValueID);
		SetTextControlFilter(ctl,uint_filter);
		InstallEventHandler(GetControlEventTarget(ctl),
							cmyk_key_proc,
							GetEventTypeCount(spec),
							spec,
							wr.wd,
							NULL);
		sprintf(val,"%.0f",clr[3]*100.0);
		SetTextControlValue(ctl,val);	
			
		vid.id=kCMYKBlackWellID;
		HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
		CreateMMColorWellWithHIView(vw);
		clrb[3]=clr[3];
		MMColorWellSetCMYKColor(vw,clrb);
		clrb[3]=0;

// Composite
		vid.id=kCMYKWellID;
		HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
		CreateMMColorWellWithHIView(vw);
		MMColorWellSetCMYKColor(vw,clr);
		
		ShowWindow(wr.wd);
		status=RunAppModalLoopForWindow(wr.wd);
		if(status){
			break;
		}
		
		if(wr.ok){
			vid.id=kCMYKWellID;
			HIViewFindByID(HIViewGetRoot(wr.wd),vid,&vw);
			MMColorWellGetCMYKColor(vw,clr);
			*c=clr[0];
			*m=clr[1];
			*y=clr[2];
			*k=clr[3];
		}
		else{
			status=-1;
		}
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(wr.wd){
		DisposeWindow(wr.wd);
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
void	RGB2CMYK(	float r, 
					float g, 
					float b,
					float* c, 
					float* m, 
					float* y,
					float* k){
	*c=1.0-r;
	*m=1.0-g;
	*y=1.0-b;
	*k=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void	CMYK2RGB(	float c, 
					float m, 
					float y,
					float k,
					float* r, 
					float* g, 
					float* b){
float	_k,_c,_m,_y;

	_k=k;
	if(_k<0){
		_k+=1.0;
	}
	_c=c;
	if(_c<0){
		_c+=1.0;
	}
	_m=m;
	if(_m<0){
		_m+=1.0;
	}
	_y=y;
	if(_y<0){
		_y+=1.0;
	}
	*r=1.0-((_c+_k)<1.0?(_c+_k):1.0);
	*g=1.0-((_m+_k)<1.0?(_m+_k):1.0);
	*b=1.0-((_y+_k)<1.0?(_y+_k):1.0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void compRGB2CMYK(float* components){
float	buf=components[3];
	RGB2CMYK(	components[0],
				components[1],
				components[2],
				&components[0],
				&components[1],
				&components[2],
				&components[3]);
	components[4]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void compCMYK2RGB(float* components){
float	buf=components[4];
	CMYK2RGB(	components[0],
				components[1],
				components[2],
				components[3],
				&components[0],
				&components[1],
				&components[2]);
	components[3]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetFillColor	(	CGContextRef c, 
								 float* components, 
								 bool cmyk){
	if(cmyk){
		CGContextSetCMYKFillColor(	c,
								  components[0],
								  components[1],
								  components[2],
								  components[3],
								  components[4]);
	}
	else{
		CGContextSetRGBFillColor(	c,
								 components[0],
								 components[1],
								 components[2],
								 components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetStrokeColor	(	CGContextRef c, 
								 float* components, 
								 bool cmyk){
	if(cmyk){
		CGContextSetCMYKStrokeColor(c,
									components[0],
									components[1],
									components[2],
									components[3],
									components[4]);
	}
	else{
		CGContextSetRGBStrokeColor(	c,
								   components[0],
								   components[1],
								   components[2],
								   components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void	RGB2CMYK64(double r, 
				   double g, 
				   double b,
				   double* c, 
				   double* m, 
				   double* y,
				   double* k){
	*c=1.0-r;
	*m=1.0-g;
	*y=1.0-b;
	*k=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void	CMYK2RGB64(double c, 
				   double m, 
				   double y,
				   double k,
				   double* r, 
				   double* g, 
				   double* b){
double	_k,_c,_m,_y;
	
	_k=k;
	if(_k<0){
		_k+=1.0;
	}
	_c=c;
	if(_c<0){
		_c+=1.0;
	}
	_m=m;
	if(_m<0){
		_m+=1.0;
	}
	_y=y;
	if(_y<0){
		_y+=1.0;
	}
	*r=1.0-((_c+_k)<1.0?(_c+_k):1.0);
	*g=1.0-((_m+_k)<1.0?(_m+_k):1.0);
	*b=1.0-((_y+_k)<1.0?(_y+_k):1.0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void compRGB2CMYK64(double* components){
double	buf=components[3];
	RGB2CMYK64(components[0],
			   components[1],
			   components[2],
			   &components[0],
			   &components[1],
			   &components[2],
			   &components[3]);
	components[4]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void compCMYK2RGB64(double* components){
double	buf=components[4];
	CMYK2RGB64(components[0],
			   components[1],
			   components[2],
			   components[3],
			   &components[0],
			   &components[1],
			   &components[2]);
	components[3]=buf;		
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetFillColor64(	CGContextRef c, 
							   double* components, 
							   bool cmyk){
	if(cmyk){
		CGContextSetCMYKFillColor(	c,
								  components[0],
								  components[1],
								  components[2],
								  components[3],
								  components[4]);
	}
	else{
		CGContextSetRGBFillColor(	c,
								 components[0],
								 components[1],
								 components[2],
								 components[3]);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void MMCGContextSetStrokeColor64(	CGContextRef c, 
								 double* components, 
								 bool cmyk){
	if(cmyk){
		CGContextSetCMYKStrokeColor(c,
									components[0],
									components[1],
									components[2],
									components[3],
									components[4]);
	}
	else{
		CGContextSetRGBStrokeColor(c,
								   components[0],
								   components[1],
								   components[2],
								   components[3]);
	}
}

