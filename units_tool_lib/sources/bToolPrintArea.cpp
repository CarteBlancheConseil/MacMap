//----------------------------------------------------------------------------
// File : bToolPrintArea.cpp
// Project : MacMap
// Purpose : C++ source file : Print area tool class
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
// 22/03/2006 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolPrintArea.h"
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct paper{
	char	name[50];
	float	w;
	float	h;
}paper;

paper	ppr[]={
{"A4",21,29.7},
{"A3",29.7,42},
{"A5",14.85,21},
{"B5",17.60,25.01},
{"US Letter",21.59,27.94},
{"US Legal",21.59,35.56}
};

#define _nppr		6
#define kUnitPixel	"px"
#define _d_(a,b,d) ((fabs(b-a))*d)

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolPrintArea	::bToolPrintArea(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				: bStdToolPres(elt,gapp,bndl){
	setclassname("printarea");
	_draw=true;
	_pprnat=1;
	_pprkind=1;
	_sizekind=kPrintAreaPopKindMenuGeo;
	ivrset(&_vxr,0,0,0,0);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolPrintArea::~bToolPrintArea(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolPrintArea::create(bGenericXMLBaseElement* elt){
	return(new bToolPrintArea(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::open(int* flags){
	set_use_trace(true);
    _draw=true;
    _pprnat=1;
    _pprkind=1;
    _sizekind=kPrintAreaPopKindMenuGeo;

    bStdToolPres::open(flags);

	_cmove=load_curs("MoveCursor.png");
	_cxyresize=load_curs("XYResizeCursor.png");
	_cxresize=load_curs("XResizeCursor.png");
	_cyresize=load_curs("YResizeCursor.png");
	
	_vxr=*(_gapp->printMgr()->get_print_area());
	_startmove=false;
	_dragmode=kPrintAreaDragModeSet;

	set_use_drag(false);
	set_use_track(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::close(){
	bStdToolPres::close();
	if(_cmove){
		ReleaseCocoaCursor(_cmove);
	}
	if(_cxyresize){
		ReleaseCocoaCursor(_cxyresize);
	}
	if(_cxresize){
		ReleaseCocoaCursor(_cxresize);
	}
	if(_cyresize){
		ReleaseCocoaCursor(_cyresize);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::pop(cocoaMenuPrm prm){
	set_mnu_cmd(0);
	bStdToolPres::pop(prm);
	clearTempPathContext(true);
	switch(get_mnu_cmd()){
		case kPrintAreaMenuCommandCenter:
			center_to_screen();
			break;
		case kPrintAreaMenuCommandWindow:
			fit_to_screen();
			break;
		case kPrintAreaMenuCommandSelection:
			fit_to_objs(_gapp->selMgr()->elements());
			break;
		case kPrintAreaMenuCommandContrasts:
			fit_to_objs(_gapp->cntMgr()->elements());
			break;
	}
	_gapp->printMgr()->set_print_area(&_vxr);
	update(true);
	validTempPathContext();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::clic(i2dvertex loc, int count){
	bStdToolPres::clic(loc,count);

ivx_rect		vr=*(_gapp->printMgr()->get_print_area());
bGenericUnit*	u=_gapp->scaleMgr()->get();
i2dvertex		vxa={vr.left,vr.top};
	
	if(	(_d_(vr.bottom,loc.v,u->conv())<3)	&&
		(_d_(vr.right,loc.h,u->conv())<3)	){
		_dragmode=kPrintAreaDragModeFullResize;
		set_clic(&vxa);
		set_cur(&vxa);
	}
	else if(_d_(vr.bottom,loc.v,u->conv())<3){
		_dragmode=kPrintAreaDragModeBottomResize;
		set_clic(&vxa);
		set_cur(&vxa);
	}
	else if(_d_(vr.right,loc.h,u->conv())<3){
		_dragmode=kPrintAreaDragModeRightResize;
		set_clic(&vxa);
		set_cur(&vxa);
	}
	else if((_d_(vr.top,loc.v,u->conv())<3)	||
			(_d_(vr.left,loc.h,u->conv())<3)){
		_dragmode=kPrintAreaDragModeMove;
		_startmove=true;
	}
	else{
		_dragmode=kPrintAreaDragModeSet;
	}
	
	set_use_drag(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::end_clic(){
	bStdToolPres::end_clic();
	_dragmode=kPrintAreaDragModeSet;
	set_use_drag(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolPrintArea::event(EventRef evt){
UInt32	clss=GetEventClass(evt);
	if(clss==kEventClassWindow){
UInt32	kind=GetEventKind(evt);
		switch(kind){
			case kEventWindowDrawContent:
			case kEventWindowUpdate:
				update(false);
				return(true);
				break;
		}
	}
	return(bStdToolPres::event(evt));
}


// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::activate(){
	bStdToolPres::activate();
	switch(_dragmode){
		case kPrintAreaDragModeSet:
			break;
		case kPrintAreaDragModeMove:
			set_curs(_cmove);
			break;
		case kPrintAreaDragModeBottomResize:
			set_curs(_cyresize);
			break;
		case kPrintAreaDragModeRightResize:
			set_curs(_cxresize);
			break;
		case kPrintAreaDragModeFullResize:
			set_curs(_cxyresize);
			break;
		default:
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	if(_draw||get_on_drag()){
ivx_rect	vxr=*(_gapp->printMgr()->get_print_area());
		if(ivr_ok(&vxr)){
CGContextRef	ctx=getTempPathContext();
			if(ctx){
CGFloat	dsh[2]={5,5};
				CGContextSaveGState(ctx);
				CGContextSetLineDash(ctx,0,dsh,2);
				hilite_rect(ctx,ivr2cgr(_gapp,vxr),false,true);
				CGContextRestoreGState(ctx);
			}
		}	
	}
	if(!global){
		validTempPathContext();
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::drag(CGPoint pt){
	set_use_drag(false);
	bStdToolPres::drag(pt);
	set_use_drag(true);
	
i2dvertex	a,b;
ivx_rect	vr=*(_gapp->printMgr()->get_print_area());
ivx_rect	scr;
	_gapp->mapIntf()->screenBounds(&scr);
	
	get_clic(&a);
	if(!ivx2_in_ivr(&scr,&a)){
		return;
	}
	get_last(&a);
	if(!ivx2_in_ivr(&scr,&a)){
		return;
	}
	get_cur(&a);
	if(!ivx2_in_ivr(&scr,&a)){
		return;
	}
	
	get_last(&a);
	get_cur(&b);
	
	switch(_dragmode){
		case kPrintAreaDragModeSet:
		case kPrintAreaDragModeFullResize:
			get_clic(&a);
			get_last(&b);
			vr.left=(a.h<b.h)?a.h:b.h;
			vr.top=(a.v<b.v)?a.v:b.v;
			vr.right=(a.h>b.h)?a.h:b.h;
			vr.bottom=(a.v>b.v)?a.v:b.v;
			
			break;
		case kPrintAreaDragModeMove:
			vr.left+=(b.h-a.h);
			vr.right+=(b.h-a.h);
			vr.top+=(b.v-a.v);
			vr.bottom+=(b.v-a.v);
			break;
		case kPrintAreaDragModeBottomResize:
			vr.bottom=b.v;
			break;
		case kPrintAreaDragModeRightResize:
			vr.right=b.h;
			break;
		default:
			break;
	}
	
	if(	(is_modifiers(shiftKey)							&&
		(	(_dragmode==kPrintAreaDragModeBottomResize)	||
			(_dragmode==kPrintAreaDragModeRightResize)	))){
			 
ivx_rect	bkvr=*(_gapp->printMgr()->get_print_area());
double		rh=(double)(vr.right-vr.left)/(double)(bkvr.right-bkvr.left);
double		rv=(double)(vr.bottom-vr.top)/(double)(bkvr.bottom-bkvr.top);

		  switch(_dragmode){
			  case kPrintAreaDragModeSet:
			  case kPrintAreaDragModeFullResize:
			  case kPrintAreaDragModeMove:
				  break;
			  case kPrintAreaDragModeBottomResize:
				  vr.right=vr.left+(vr.right-vr.left)*rv;
				  break;
			  case kPrintAreaDragModeRightResize:
				  vr.bottom=vr.top+(vr.bottom-vr.top)*rh;
				  break;
			  default:
				  break;
		  }
	}
	
	_gapp->printMgr()->set_print_area(&vr);

	clearTempPathContext(true);
	update(true);
	validTempPathContext();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::track_draw(){
	if(!get_active()){
		return;
	}
i2dvertex	b;
ivx_rect	vr=*(_gapp->printMgr()->get_print_area());
	get_cur(&b);

	if(!get_on_drag()){
bGenericUnit*	u=_gapp->scaleMgr()->get();
		
		if(	(_d_(vr.bottom,b.v,u->conv())<3)&&
			(_d_(vr.right,b.h,u->conv())<3)	){
			set_curs(_cxyresize);
		}
		else if((_d_(vr.bottom,b.v,u->conv())<3)&&((vr.left<b.h)&&(b.h<vr.right))){
			set_curs(_cyresize);
		}
		else if((_d_(vr.right,b.h,u->conv())<3)&&((vr.top<b.v)&&(b.v<vr.bottom))){
			set_curs(_cxresize);
		}
		else if((_d_(vr.top,b.v,u->conv())<3)	||
				(_d_(vr.left,b.h,u->conv())<3)	){
			set_curs(_cmove);
		}
		else{
			set_curs();
		}
		return;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolPrintArea::edit_event(EventRef evt, WindowRef wd){
HICommand	cmd;

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				puts(wd);
				break;
			case kHICommandCancel:
				break;
			case kPrintAreaPopKindCmd:
				pop_kind_action(wd);
				break;
			default:
				break;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::edit_init(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};

	_vxr=*(_gapp->printMgr()->get_print_area());
	
i2dvertex	ivxa={_vxr.left,_vxr.top};
i2dvertex	ivxb={_vxr.right,_vxr.bottom};
d2dvertex	dvxa,dvxb;
char		val[256];

	Convert(&dvxa,&ivxa);
	Convert(&dvxb,&ivxb);

	cid.id=kPrintAreaXID;
	GetControlByID(wd,&cid,&c);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),dvxa.x);
	SetTextControlValue(c,val);

	cid.id=kPrintAreaYID;
	GetControlByID(wd,&cid,&c);
	SetTextControlFilter(c,float_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),dvxa.y);
	SetTextControlValue(c,val);

	cid.id=kPrintAreaPopKindID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_sizekind);

	cid.id=kPrintAreaWID;
	GetControlByID(wd,&cid,&c);
	SetTextControlFilter(c,ufloat_filter);

	cid.id=kPrintAreaHID;
	GetControlByID(wd,&cid,&c);
	SetTextControlFilter(c,ufloat_filter);
	
	cid.id=kPrintAreaPopSizeID;
	GetControlByID(wd,&cid,&c);
	ClearPopupControl(c,1);
	for(int i=0;i<_nppr;i++){
		AddPopupControlMenuItemValue(c,ppr[i].name);
	}
	SetControl32BitValue(c,_pprkind);

	cid.id=kPrintAreaOrientationID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_pprnat);
	
	switch(_sizekind){
		case kPrintAreaPopKindMenuGeo:
			cid.id=kPrintAreaFrameStandardSizeID;
			set_as_geo(wd);
			break;
		case kPrintAreaPopKindMenuPix:
			cid.id=kPrintAreaFrameStandardSizeID;
			set_as_pix(wd);
			break;
		case kPrintAreaPopKindMenuCustom:
			cid.id=kPrintAreaFrameStandardSizeID;
			set_as_ppsz(wd);
			break;
		case kPrintAreaPopKindMenuStandard:
			cid.id=kPrintAreaFrameCustomSizeID;
			break;
	}
	GetControlByID(wd,&cid,&c);
	HideControl(c);
	
bGenericUnit*	u=_gapp->distMgr()->get();
	u->short_name(val);
	for(int i=kPrintAreaXUnitID;i<=kPrintAreaYUnitID;i++){
		cid.id=i;
		GetControlByID(wd,&cid,&c);
		SetTextControlValue(c,val);
	}

	cid.id=kPrintAreaDrawAreaID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_draw);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::pop_kind_action(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};
int			k;

	put_area(wd);

	cid.id=kPrintAreaPopKindID;
	GetControlByID(wd,&cid,&c);
	k=GetControl32BitValue(c);
	switch(k){
		case kPrintAreaPopKindMenuGeo:
			set_as_geo(wd);
			cid.id=kPrintAreaFrameCustomSizeID;
			GetControlByID(wd,&cid,&c);
			ShowControl(c);
			cid.id=kPrintAreaFrameStandardSizeID;
			GetControlByID(wd,&cid,&c);
			HideControl(c);
			break;
		case kPrintAreaPopKindMenuPix:
			set_as_pix(wd);
			cid.id=kPrintAreaFrameCustomSizeID;
			GetControlByID(wd,&cid,&c);
			ShowControl(c);
			cid.id=kPrintAreaFrameStandardSizeID;
			GetControlByID(wd,&cid,&c);
			HideControl(c);
			break;
		case kPrintAreaPopKindMenuCustom:
			set_as_ppsz(wd);
			cid.id=kPrintAreaFrameCustomSizeID;
			GetControlByID(wd,&cid,&c);
			ShowControl(c);
			cid.id=kPrintAreaFrameStandardSizeID;
			GetControlByID(wd,&cid,&c);
			HideControl(c);
			break;
		case kPrintAreaPopKindMenuStandard:
			cid.id=kPrintAreaFrameCustomSizeID;
			GetControlByID(wd,&cid,&c);
			HideControl(c);
			cid.id=kPrintAreaFrameStandardSizeID;
			GetControlByID(wd,&cid,&c);
			ShowControl(c);
			
			cid.id=kPrintAreaPopSizeID;
			GetControlByID(wd,&cid,&c);
			SetControl32BitValue(c,_pprkind);
			break;
	}
	_sizekind=k;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::set_as_geo(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};
i2dvertex	ivxa={_vxr.left,_vxr.top};
i2dvertex	ivxb={_vxr.right,_vxr.bottom};
d2dvertex	dvxa,dvxb;
char		val[256];

	Convert(&dvxa,&ivxa);
	Convert(&dvxb,&ivxb);

	cid.id=kPrintAreaWID;
	GetControlByID(wd,&cid,&c);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),dvxb.x-dvxa.x);
	SetTextControlValue(c,val);

	cid.id=kPrintAreaHID;
	GetControlByID(wd,&cid,&c);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),dvxa.y-dvxb.y);
	SetTextControlValue(c,val);

bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);
	for(int i=kPrintAreaWUnitID;i<=kPrintAreaHUnitID;i++){
		cid.id=i;
		GetControlByID(wd,&cid,&c);
		SetTextControlValue(c,val);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::set_as_ppsz(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};
i2dvertex	ivxa={_vxr.left,_vxr.bottom};
i2dvertex	ivxb={_vxr.right,_vxr.top};
CGPoint		pa,pb;
char		val[256];
float		m;

	Convert(&pa,&ivxa);
	Convert(&pb,&ivxb);

	cid.id=kPrintAreaWID;
	GetControlByID(wd,&cid,&c);
	m=pb.x-pa.x;
	m*=(2.54/72.0);
	sprintf(val,"%.2f",m);
	SetTextControlValue(c,val);

	cid.id=kPrintAreaHID;
	GetControlByID(wd,&cid,&c);
	m=pb.y-pa.y;
	m*=(2.54/72.0);
	sprintf(val,"%.2f",m);
	SetTextControlValue(c,val);

	message_string(kMsgCentimetersAB,val,0);
	for(int i=kPrintAreaWUnitID;i<=kPrintAreaHUnitID;i++){
		cid.id=i;
		GetControlByID(wd,&cid,&c);
		SetTextControlValue(c,val);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::set_as_pix(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};
i2dvertex	ivxa={_vxr.left,_vxr.top};
i2dvertex	ivxb={_vxr.right,_vxr.bottom};
CGPoint		pa,pb;
char		val[256];

	Convert(&pa,&ivxa);
	Convert(&pb,&ivxb);

	cid.id=kPrintAreaWID;
	GetControlByID(wd,&cid,&c);
	sprintf(val,"%.0f",round(pb.x-pa.x));
	SetTextControlValue(c,val);

	cid.id=kPrintAreaHID;
	GetControlByID(wd,&cid,&c);
	sprintf(val,"%.0f",round(pa.y-pb.y));
	SetTextControlValue(c,val);

	for(int i=kPrintAreaWUnitID;i<=kPrintAreaHUnitID;i++){
		cid.id=i;
		GetControlByID(wd,&cid,&c);
		SetTextControlValue(c,kUnitPixel);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::puts(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};

	put_area(wd);
	cid.id=kPrintAreaDrawAreaID;
	GetControlByID(wd,&cid,&c);
bool	vis=GetControl32BitValue(c);
	if(vis!=_draw){
		clearTempPathContext(true);
		_draw=vis;
		_gapp->printMgr()->set_print_area(&_vxr);
		update(true);
		validTempPathContext();
	}
	else if(_draw){
		clearTempPathContext(true);
		_gapp->printMgr()->set_print_area(&_vxr);
		update(true);
		validTempPathContext();
	}
	else{
		_gapp->printMgr()->set_print_area(&_vxr);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::put_area(WindowRef wd){
ControlRef	c;
ControlID	cid={kPrintAreaSignature,0};
char		val[256];
i2dvertex	ivx;
d2dvertex	dvxa,dvxb;
CGPoint		pa,pb;

	cid.id=kPrintAreaXID;
	GetControlByID(wd,&cid,&c);
	GetTextControlValue(c,val,256);
	dvxa.x=matof(val);
	
	cid.id=kPrintAreaYID;
	GetControlByID(wd,&cid,&c);
	GetTextControlValue(c,val,256);
	dvxa.y=matof(val);
	
	switch(_sizekind){
		case kPrintAreaPopKindMenuGeo:
			cid.id=kPrintAreaWID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			dvxb.x=dvxa.x+matof(val);
			cid.id=kPrintAreaHID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			dvxb.y=dvxa.y-matof(val);
			break;
		case kPrintAreaPopKindMenuPix:
			Convert(&pa,&dvxa);
			cid.id=kPrintAreaWID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			pb.x=pa.x+matof(val);
			cid.id=kPrintAreaHID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			pb.y=pa.y-matof(val);
			Convert(&dvxb,&pb);
			break;
		case kPrintAreaPopKindMenuCustom:
			Convert(&pa,&dvxa);
			cid.id=kPrintAreaWID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			pb.x=pa.x+((72.0/2.54)*matof(val));
			cid.id=kPrintAreaHID;
			GetControlByID(wd,&cid,&c);
			GetTextControlValue(c,val,256);
			pb.y=pa.y-((72.0/2.54)*matof(val));
			Convert(&dvxb,&pb);		
			break;
		case kPrintAreaPopKindMenuStandard:
			Convert(&pa,&dvxa);
			cid.id=kPrintAreaPopSizeID;
			GetControlByID(wd,&cid,&c);
			_pprkind=GetControl32BitValue(c);
			cid.id=kPrintAreaOrientationID;
			GetControlByID(wd,&cid,&c);
			_pprnat=GetControl32BitValue(c);
			if(_pprnat==1){
				pb.x=pa.x+((72.0/2.54)*ppr[_pprkind-1].w);
				pb.y=pa.y-((72.0/2.54)*ppr[_pprkind-1].h);
			}
			else{
				pb.x=pa.x+((72.0/2.54)*ppr[_pprkind-1].h);
				pb.y=pa.y-((72.0/2.54)*ppr[_pprkind-1].w);			
			}
			Convert(&dvxb,&pb);		
			break;
	}
	
	Convert(&ivx,&dvxa);		
	_vxr.left=ivx.h;
	_vxr.top=ivx.v;
	Convert(&ivx,&dvxb);		
	_vxr.right=ivx.h;
	_vxr.bottom=ivx.v;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::center_to_screen(){
i2dvertex	svx,avx;

	_vxr=*(_gapp->printMgr()->get_print_area());
	ivr_mid(&_vxr,&avx);
	_gapp->mapIntf()->screenCenter(&svx);
	
	_vxr.left+=(svx.h-avx.h);
	_vxr.right+=(svx.h-avx.h);
	_vxr.top+=(svx.v-avx.v);
	_vxr.bottom+=(svx.v-avx.v);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::fit_to_screen(){
	_gapp->mapIntf()->screenBounds(&_vxr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolPrintArea::fit_to_objs(bArray* arr){	
bGenericGeoElement*	o;
bGenericStyle*		style;
long				j;
CGRect				ro,rg=CGRectZero;
	
	for(long i=1;i<=arr->count();i++){
		arr->get(i,&o);
		for(j=1;j<=_gapp->layersAccessCtx()->count();j++){
			style=_gapp->layersAccessCtx()->get(j);
			if(!style->visible()){
				continue;
			}
			if(style->gettype()==NULL){
				continue;
			}
			if(style->gettype()->index()!=o->getType()){
				continue;
			}
			style->rect(o,&ro);
			if(CGRectIsEmpty(ro)){
				continue;
			}
			if(CGRectIsEmpty(rg)){
				rg=ro;
			}
			else{
				rg=CGRectUnion(rg,ro);
			}
		}
	}
	if(CGRectIsEmpty(rg)){
		return;
	}
	_vxr=cgr2ivr(_gapp,rg);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolPrintArea::load(){
bGenericXMLBaseElement* groot=bStdToolPres::load();
bGenericXMLBaseElement* elt=NULL;
	if(!groot){
		if(_gprm){
			delete _gprm;
			_gprm=NULL;
		}
		_gprm=new bToolPrintAreaParams(NULL,_gapp,getbundle());
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		if(_gprm){
			delete _gprm;
			_gprm=NULL;
		}
		_gprm=new bToolPrintAreaParams(NULL,_gapp,getbundle());
		return(NULL);
	}
char					val[_values_length_max_];
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_draw=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,1,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_pprnat=atoi(val);
		if((_pprnat<1)||(_pprnat>2)){
			_pprnat=1;
			if(_pprkind==kPrintAreaPopKindMenuStandard){
				_pprkind=kPrintAreaPopKindMenuCustom;
			}
		}
		
		elt=_gapp->classMgr()->NthElement(root,2,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_pprkind=atoi(val);
		
		
		elt=_gapp->classMgr()->NthElement(root,3,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_sizekind=atoi(val);
		break;
	}
	
	if(	(_sizekind<kPrintAreaPopKindMenuGeo)		||
		(_sizekind>kPrintAreaPopKindMenuStandard)	){
		_sizekind=1;
	}

	if(	(_pprkind<1)	||
		(_pprkind>_nppr)){
		_pprkind=1;
		if(_sizekind==kPrintAreaPopKindMenuStandard){
			_sizekind=kPrintAreaPopKindMenuCustom;
		}
	}
	
	if((_pprnat<1)||(_pprnat>2)){
		_pprnat=1;
		if(_sizekind==kPrintAreaPopKindMenuStandard){
			_sizekind=kPrintAreaPopKindMenuCustom;
		}
	}
	
	
	root=get_param(groot,"graphic");
	if(_gprm){
		delete _gprm;
		_gprm=NULL;
	}
	_gprm=new bToolPrintAreaParams(root,_gapp,getbundle());

	_gapp->classMgr()->ReleaseXMLInstance(groot);

	
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolPrintArea::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	add_idesc(arr,2,"bool",_draw);
	add_idesc(arr,2,"int",_pprnat);
	add_idesc(arr,2,"int",_pprkind);
	add_idesc(arr,2,"int",_sizekind);
	return(bStdToolPres::make(arr));
}

#pragma mark =>Graphic params
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolPrintAreaParams::bToolPrintAreaParams(bGenericXMLBaseElement* root,
										   bGenericMacMapApp* gapp,
										   CFBundleRef bndl)
					:bGraphicParams(root,gapp){
	if(_status==-1){
		_width=1;
		_stroke[0]=0;
		_stroke[1]=0;
		_stroke[2]=0;
		_stroke[3]=1;
		_fill[0]=0;
		_fill[1]=0;
		_fill[2]=0;
		_fill[3]=0;
	}
	_status=-1;
	_ndash=0;
	_dash=NULL;
	if(root!=NULL){
		parse(root,gapp);
	}
	else{
		root=GetXMLParamFromBundle(gapp,bndl,kStdGraphicParamsToolTraceName);
		if(root!=NULL){
			parse(root,gapp);
			gapp->classMgr()->ReleaseXMLInstance(root);
		}
		else{
			_dash=new CGFloat[2];
			_dash[0]=4;
			_dash[1]=4;
			_ndash=2;
		}
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bToolPrintAreaParams::~bToolPrintAreaParams(){
	if(_dash){
		delete _dash;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bToolPrintAreaParams::update(bArray& arr, 
								  long indent){
	bGraphicParams::update(arr,indent);
	for(long i=0;i<_ndash;i++){
		add_ddesc(arr,indent+1,"float",_dash[i],2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolPrintAreaParams::apply(CGContextRef ctx){
	if(_dash){
		CGContextSetLineDash(ctx,0,_dash,_ndash);
	}
	bGraphicParams::apply(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolPrintAreaParams::setDash(const float* dash, long ndash){
	if(_dash){
		delete _dash;
	}
	_dash=new CGFloat[ndash];
	_ndash=ndash;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolPrintAreaParams::parse(bGenericXMLBaseElement* root, bGenericMacMapApp* gapp){
bGenericXMLBaseElement* elt=NULL;
char					val[_values_length_max_];
long					i;
	
	for(i=0;;i++){
		elt=gapp->classMgr()->NthElement(root,i+10,"float");
		if(!elt){
			break;
		}
	}
	_ndash=i;
	_dash=new CGFloat[_ndash];
	for(i=0;i<_ndash;i++){
		elt=gapp->classMgr()->NthElement(root,i+10,"float");
		elt->getvalue(val);
		_dash[i]=matof(val);
	}
	_status=0;
}


