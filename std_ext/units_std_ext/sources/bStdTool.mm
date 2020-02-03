//----------------------------------------------------------------------------
// File : bStdTool.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base class for Tools
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2020 Carte Blanche Conseil.
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
// 27/12/2004 creation.
// 07/01/2020 full Cocoa.
//----------------------------------------------------------------------------

#include "bStdTool.h"

#include <mox_intf/ext_utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/endian.h>
#include <mox_intf/NSContextualMenuWrappers.h>
#include <mox_intf/NSEventWrappers.h>

#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>

#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
//
// -----------
static void NSMakeMapKeyWindow(bGenericMacMapApp* gapp){
    if([[NSApplication sharedApplication] modalWindow]==nil){
NSWindow*   ref=(__bridge NSWindow*)gapp->mapIntf()->ref();
        [ref makeKeyWindow];
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void t_message_string(char* msg_id, char* string, CFBundleRef bndl){
	string[0]=0;
CFStringRef	cfid=CFStringCreateWithCString(kCFAllocatorDefault,msg_id,kCFStringEncodingMacRoman);
CFStringRef msg=CFCopyLocalizedStringFromTableInBundle(cfid,CFSTR("HotSpot"),bndl,NULL);	
	CFRelease(cfid);
	if(!msg){
		return;
	}
	CFStringGetCString(msg,string,255,kCFStringEncodingMacRoman);
	CFRelease(msg);
}

// ---------------------------------------------------------------------------
// 
// -----------
static int convertModifier(void* nsevt){
long	mod=NSEvent_modifierFlags(nsevt);
int		r=0;
	if(mod&NSEventModifierFlagCapsLock){
		r|=alphaLock;
	}
	if(mod&NSEventModifierFlagShift){
		r|=shiftKey;
	}
	if(mod&NSEventModifierFlagControl){
		r|=controlKey;
	}
	if(mod&NSEventModifierFlagOption){
		r|=optionKey;
	}
	if(mod&NSEventModifierFlagCommand){
		r|=cmdKey;
	}
	return(r);
}

bool bStdTool::_curs_lock=false;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdTool::bStdTool(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		: bStdExt(elt,gapp,bndl){
	_curs=NULL;
	_clic_count=0;
	_mnu_cmd=1;
	_key=0;
	_mod=0;
	_on=false;
	_on_drag=false;
	_on_edit=false;
	_use_drag=false;
	_use_track=false;
	_use_join=false;
	_use_angle=false;
	_use_sel=false;
	_use_locs=false;
	_reset_locs=false;
	_limit=0.01;
	_angle=45;
	_last_loc.h=INT_MIN;
	_last_loc.v=INT_MIN;
	_cur_loc=_last_loc;
	_clic_loc=_last_loc;
	_o=NULL;
	_locs=NULL;
	_subclss=kXMLSubClassExtTool;
	_btn=kEventMouseButtonPrimary;
	
	_use_trace=false;
	_gprm=NULL;
//	_curs_lock=false;
            
    _stilldown=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdTool::~bStdTool(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::open(int* flags){
	(*flags)=0;

	_curs=load_curs("StdCursor.png");
	if(!_curs){
	}

	_clic_count=0;
	_mnu_cmd=1;
	_key=0;
	_mod=0;
	_on=false;
	_on_drag=false;
	_on_edit=false;
	_use_drag=false;
	_use_track=false;
	_use_join=false;
	_use_angle=false;
	_use_sel=false;
	_use_locs=false;
	_reset_locs=false;
	_limit=0.01;
	_angle=45;
	_last_loc.h=INT_MIN;
	_last_loc.v=INT_MIN;
	_cur_loc=_last_loc;
	_clic_loc=_last_loc;
	_o=NULL;
	_locs=new bArray(sizeof(i2dvertex));
//	_subclss=kXMLSubClassExtTool;
	_btn=kEventMouseButtonPrimary;
	
//	_use_trace=false;// Non, c'est fixé par le tool
	_gprm=NULL;
//	_curs_lock=false;
	
    _stilldown=false;

	(void)load();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::close(){
	save();
	
	if(_curs){
		ReleaseCocoaCursor(_curs);
		_curs=NULL;
	}
	
	if(_locs){
		delete _locs;
		_locs=NULL;
	}
	if(_gprm){
		delete _gprm;
		_gprm=NULL;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::nsevent(void* nsevt){
//_bTrace_("bStdTool::nsevent(void*)",false);
//_tm_("");
long etype=NSEvent_type(nsevt);
    if(_curs_lock){
        return false;
    }
    
	switch(etype){
		case NSEvent_typeLeftMouseDown://_tm_("event type:NSEvent_typeLeftMouseDown");
            set_modifiers(convertModifier(nsevt));
            _stilldown=true;
            clic(NSEvent_locationInWindow(nsevt),NSEvent_clickCount(nsevt));
			break;
		case NSEvent_typeLeftMouseUp://_tm_("event type:NSEvent_typeLeftMouseUp");
            _stilldown=false;
            end_clic();
			break;
		case NSEvent_typeRightMouseDown://_tm_("event type:NSEvent_typeRightMouseDown");
            set_modifiers(convertModifier(nsevt));
            pop_tools(NSEvent_locationInWindow(nsevt),NSEvent_window(nsevt));
			break;
		case NSEvent_typeRightMouseUp://_tm_("event type:NSEvent_typeRightMouseUp");
			break;
		case NSEvent_typeMouseMoved://_tm_("event type:NSEvent_typeMouseMoved");
			{
            set_modifiers(convertModifier(nsevt));
CGPoint     loc=NSEvent_locationInWindow(nsevt);
            idle(&loc);
			}
			break;
		case NSEvent_typeLeftMouseDragged://_tm_("event type:NSEvent_typeLeftMouseDragged");
            set_modifiers(convertModifier(nsevt));
            drag(NSEvent_locationInWindow(nsevt));
            break;
		case NSEvent_typeRightMouseDragged://_tm_("event type:NSEvent_typeRightMouseDragged");
			break;
		case NSEvent_typeMouseEntered://_tm_("event type:NSEvent_typeMouseEntered");
            set_modifiers(convertModifier(nsevt));
            activate();
			break;
		case NSEvent_typeMouseExited://_tm_("event type:NSEvent_typeMouseExited");
            set_modifiers(convertModifier(nsevt));
            deactivate();
			break;
		case NSEvent_typeKeyDown://_tm_("event type:NSEvent_typeKeyDown");
			set_modifiers(convertModifier(nsevt));
//kVK_Tab kVK_Space kVK_LeftArrow kVK_RightArrow kVK_DownArrow kVK_UpArrow
            switch(NSEvent_keyCode(nsevt)){
                case kVK_Return:
                    (void)set_key(kEnterCharCode);
                    break;
                case kVK_Delete:
                    (void)set_key(kBackspaceCharCode);
                    break;
                case kVK_Escape:
                    (void)set_key(kEscapeCharCode);
                    break;
                case kVK_ANSI_KeypadEnter:
                    (void)set_key(kReturnCharCode);
                    break;
                default:
                    set_key(NSEvent_charactersIgnoringModifiers(nsevt));
                    break;
            }
//            _m_("NSEvent_charactersIgnoringModifiers<"+(int)NSEvent_charactersIgnoringModifiers(nsevt)+">");
//            _m_("NSEvent_characters<"+(int)NSEvent_characters(nsevt)+">");
//            _m_("NSEvent_keyCode<"+(int)NSEvent_keyCode(nsevt)+">");
			break;
		case NSEvent_typeKeyUp://_tm_("event type:NSEvent_typeKeyUp");
			set_key(0);
			break;
		case NSEvent_typeFlagsChanged://_tm_("event type:NSEvent_typeFlagsChanged");
			set_modifiers(convertModifier(nsevt));
			break;
			
		case NSEvent_typeCursorUpdate://_tm_("event type:NSEvent_typeCursorUpdate");
			break;
		case NSEvent_typeScrollWheel://_tm_("event type:NSEvent_typeScrollWheel");
			break;
		case NSEvent_typeOtherMouseDown://_tm_("event type:NSEvent_typeOtherMouseDown");
			break;
		case NSEvent_typeOtherMouseUp://_tm_("event type:NSEvent_typeOtherMouseUp");
			break;
		case NSEvent_typeOtherMouseDragged://_tm_("event type:NSEvent_typeOtherMouseDragged");
			break;
	}
	
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::idle(void* prm){
//_bTrace_("bStdTool::idle(void*)",true);
    if(_curs_lock){
        return;
    }

	if(_on_edit){
//_tm_("on edit");
		return;
	}
	if(!_on){
//_tm_("activate");
		activate();
	}
    
	push();
    
    if(prm)set_cur((CGPoint*)prm);
	
	clearTempPathContext(true);
	
	if(get_use_join()){
//_tm_("track_join");
		track_join();
	}
	if(get_use_angle()){
//_tm_("constraint_cur");
		constraint_cur();
	}
	if(get_use_sel()){
//_tm_("track_obj");
		track_obj();
	}
	if(get_use_track()){
//_tm_("track_draw");
		track_draw();
	}
	if(_use_trace){
//_tm_("update");
		update(true);
	}
	
	validTempPathContext();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::process(int msg, void* prm){
_bTrace_("bStdTool::process()",true);
bool b=false;
	
	switch(msg){
		case kExtProcessCallFromIntf:
			break;
		case kToolProcessCocoaMenu:
			pop(*(cocoaMenuPrm*)prm);
			b=true;
			break;
        case kExtProcessCallWithXMLTree:
            edit(NULL);
            break;
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdTool::tool_class(){
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// -----------
CGImageRef bStdTool::symb(){
	return(LoadPNGImageFromBundle(getbundle(),CFSTR("StdSymb.png")));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::activate(){
//_bTrace_("bStdTool::activate",true);
	if(_on_edit){
		//return;
	}
    else{
        if(!_curs_lock){
            NSMakeMapKeyWindow(_gapp);
        }
    }
	set_curs();
	_on=true;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::deactivate(){
//_bTrace_("bStdTool::deactivate",true);
	if(_on_edit){
		return;
	}
	push();

	clearTempPathContext(true);
	
	if(get_use_join()){
		track_join();
	}
	if(((get_use_drag())&&(_on_drag))||(get_use_track())){
		track_draw();
	}
	
	validTempPathContext();
	
	set_arrow();
	_on=false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::clic(i2dvertex loc, int count){
//_bTrace_("bStdTool::clic(i2dvertex, int)",true);
i2dvertex	vx;
    
	push();
	if((get_use_angle())&&(is_modifiers(shiftKey))&&(_clic_loc.h!=INT_MIN)){
		ivx_angleproj(&vx,&_clic_loc,&_last_loc,deg2rad(_angle));
		set_clic(&vx);
		set_cur(&vx);
	}
	else{
		set_clic(&loc);
		set_cur(&loc);
	}
	_clic_count=count;
	if(_use_locs){
		if(count==1){
			if(_locs->count()>0){
				_locs->get(_locs->count(),&vx);
				if(!eq_ivx2(&vx,&_clic_loc)){
					_locs->add(&_clic_loc);
				}
			}
			else{
				_locs->add(&_clic_loc);
			}
		}
		else{
			_locs->reset();
			reset_clic();
			push();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::clic(CGPoint loc, int count){
//_bTrace_("bStdTool::clic(CGPoint, int)",true);
i2dvertex	vx;
	Convert(&vx,&loc);
	clic(vx,count);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::edit(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// Contextual tool menu
// -----------
void bStdTool::pop(cocoaMenuPrm prm){
_bTrace_("bStdTool::pop(cocoaMenuPrm)",true);
bGenericXMLBaseElement*	root=GetXMLParamFromBundle(_gapp,getbundle(),"toolmenu");
bGenericXMLBaseElement* elt=NULL;
	if(!root){
_tw_("no root");
		return;
	}

long			cmd,nb=_gapp->classMgr()->CountElements(root,"string");
menuitem_desc*	desc=new menuitem_desc[nb];
	if(!desc){
		_gapp->classMgr()->ReleaseXMLInstance(root);
_te_("no desc");
		return;
	}
	for(long i=1;i<=nb;i++){
		elt=_gapp->classMgr()->NthElement(root,i,"string");
		elt->getvalue(desc[i-1].name);
		desc[i-1].checked=(_mnu_cmd==i);
		desc[i-1].disabled=false;
        desc[i-1].index=0;
	}
	//if(prm.wdIsCocoa){
		cmd=popUpContextMenuWithCGPointAndNSWindow(prm.location,
												   prm.inWindow,
												   desc,
												   nb,
                                                   1.0);
	//}
	/*else{
		cmd=popUpContextMenuWithCGPointAndWindowRef(prm.location,
													(WindowRef)prm.inWindow,
													desc,
													nb);		
	}*/
	if(cmd!=0){
		_mnu_cmd=cmd;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
	delete[] desc;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::update(bool global){
//_bTrace_("bStdTool::update",false);
//char	nm[256];
//	getclassname(nm);
//_tw_("DON'T CALL bStdTool::update >"+nm);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::drag(CGPoint cgp){
//_bTrace_("bStdTool::drag",true);
	_on_drag=true;
	push();
	set_cur(&cgp);	
	if(get_use_angle()){
		constraint_cur();
	}
	if(get_use_drag()){
//_tm_("get_use_drag");
		clearTempPathContext(true);
		track_join();
		track_draw();
		if(_use_trace){
//_tm_("_use_trace");
			update(true);
		}
		validTempPathContext();
	}

CGRect	cgr=getTempPathContextRect();
	if(_on){
		if(!CGRectContainsPoint(cgr,cgp)){
			deactivate();
		}
	}
	else{
		if(CGRectContainsPoint(cgr,cgp)){
			activate();
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::end_clic(){
//_bTrace_("bStdTool::end_clic()",true);
	_on_drag=false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_drag(bool b){
	_use_drag=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_drag(){
	return(_use_drag);
}


// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_on_drag(){
	return(_on_drag);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_active(){
	return(_on);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_mnu_cmd(int cmd){
	_mnu_cmd=cmd;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdTool::get_mnu_cmd(){
	return(_mnu_cmd);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::set_key(int k){
	if(_use_locs){
		switch(k){
			case kEnterCharCode:
			case kReturnCharCode:
			case kEscapeCharCode:
				_locs->reset();
				reset_clic();
				push();
				break;
			case kBackspaceCharCode:
				if(_locs->count()>1){
long				n=_locs->count();
i2dvertex			vx;
					
					_locs->get(n-1,&vx);
					set_clic(&vx);
					_locs->rmv(n);
				}
				break;
		}
	}
	_key=k;
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::is_key(int k){
	return(_key==k);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_modifiers(int k){
	_mod=k;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::is_modifiers(int k){
	return(_mod&k);
}

// ---------------------------------------------------------------------------
// 
// -----------
CursorRef bStdTool::load_curs(const char* name){
_bTrace_("bStdTool::load_curs",false);
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,kCFStringEncodingMacRoman);
CGImageRef	img=LoadPNGImageFromBundle(getbundle(),cfs);
	CFRelease(cfs);
	if(!img){
char	nm[256];
		getclassname(nm);
_te_("cursor \""+name+"\"not found for tool "+nm);
		return NULL;
	}
float		hsx=0,hsy=0;
char		fn[256],val[256];
	sprintf(fn,"X%s",name);
	t_message_string(fn,val,getbundle());
	hsx=atof(val);
	sprintf(fn,"Y%s",name);
	t_message_string(fn,val,getbundle());
	hsy=atof(val);
CursorRef	crs=CreateCocoaCursor(img,hsx,hsy);
	if(img){
		CGImageRelease(img);
	}
	return(crs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_curs(CursorRef curs){
	if(curs){
		_ccurs=curs;
		SetCocoaCursor(_ccurs);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_curs(){
	if(!_curs_lock){
		SetCocoaCursor(_curs);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_arrow(){
	if(!_curs_lock){
//		SetThemeCursor(kThemeArrowCursor);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_obj(bGenericGeoElement* o){
	_o=o;
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericGeoElement* bStdTool::get_obj(){
	return(_o);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_track(bool b){
	_use_track=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_track(){
	return(_use_track);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_join(bool b){
	_use_join=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_join(){
	return(_use_join);
}


// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_locs(bool b){
	_use_locs=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_locs(){
	return(_use_locs);
}

// ---------------------------------------------------------------------------
// 
// -----------
bArray* bStdTool::get_locs(){
	return(_locs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_angle(bool b){
	_use_angle=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_angle(){
	return(_use_angle);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_angle(double d){
	_angle=d;
}

// ---------------------------------------------------------------------------
// 
// -----------
double bStdTool::get_angle(){
	return(_angle);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_use_sel(bool b){
	_use_sel=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdTool::get_use_sel(){
	return(_use_sel);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_limit(double d){
	_limit=d;
}

// ---------------------------------------------------------------------------
// 
// -----------
double bStdTool::get_limit(){
	return(_limit);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bStdTool::get_scaled_limit(){
bGenericUnit*	su=_gapp->scaleMgr()->get();
double			scale=su->coef();
bGenericUnit*	mu=_gapp->document()->dist_resolution_unit();
double			coef=mu->coef();
	return(_limit*scale/coef);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_last(CGPoint *loc){
	if(_last_loc.h==INT_MIN){
		*loc=CGPointMake(SHRT_MIN,SHRT_MIN);
		return;
	}
	Convert(loc,&_last_loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_last(i2dvertex *loc){
	(*loc)=_last_loc;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_last(CGPoint *loc){
	if(loc->x==SHRT_MIN){
//_bTrace_("bStdTool::set_last(CGPoint)",true);
		_last_loc.h=INT_MIN;
		_last_loc.v=INT_MIN;
		return;
	}
	Convert(&_last_loc,loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_last(i2dvertex *loc){
	_last_loc=(*loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::reset_last(){
//_bTrace_("bStdTool::reset_last()",true);
	_last_loc.h=INT_MIN;
	_last_loc.v=INT_MIN;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_cur(CGPoint *loc){
	if(_cur_loc.h==INT_MIN){
		*loc=CGPointMake(SHRT_MIN,SHRT_MIN);
		return;
	}
	Convert(loc,&_cur_loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_cur(i2dvertex *loc){
	(*loc)=_cur_loc;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_cur(CGPoint *loc){
	if(loc->x==SHRT_MIN){
//_bTrace_("bStdTool::set_cur(CGPoint)",true);
		_cur_loc.h=INT_MIN;
		_cur_loc.v=INT_MIN;
		return;
	}
	Convert(&_cur_loc,loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_cur(i2dvertex *loc){
	_cur_loc=(*loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::reset_cur(){
//	_bTrace_("bStdTool::reset_cur()",true);
	_cur_loc.h=INT_MIN;
	_cur_loc.v=INT_MIN;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::constraint_cur(){
	if(is_modifiers(shiftKey)&&(_clic_loc.h!=INT_MIN)){
i2dvertex	vx;

		ivx_angleproj(&vx,&_clic_loc,&_cur_loc,deg2rad(_angle));
		set_cur(&vx);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_clic(CGPoint *loc){
	if(_clic_loc.h==INT_MIN){
		*loc=CGPointMake(SHRT_MIN,SHRT_MIN);
		return;
	}
	Convert(loc,&_clic_loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::get_clic(i2dvertex *loc){
	(*loc)=_clic_loc;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_clic(CGPoint *loc){
	if(loc->x==SHRT_MIN){
//_bTrace_("bStdTool::set_clic(CGPoint)",true);
		_clic_loc.h=INT_MIN;
		_clic_loc.v=INT_MIN;
		return;
	}
	Convert(&_clic_loc,loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::set_clic(i2dvertex *loc){
	_clic_loc=(*loc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::reset_clic(){
//_bTrace_("bStdTool::reset_clic()",true);
	_clic_loc.h=INT_MIN;
	_clic_loc.v=INT_MIN;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::push(){
	_last_loc=_cur_loc;
	reset_cur();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_rect(){
//_bTrace_("bStdTool::draw_rect()",true);
CGPoint	a,b;
	get_clic(&a);
	if(a.x==SHRT_MIN){
		return;
	}
	get_cur(&b);
	if(b.x!=SHRT_MIN){
CGContextRef	ctx=getTempPathContext();
CGRect			r=CGRectMake(a.x,a.y, b.x-a.x,b.y-a.y);			
		CGContextSaveGState(ctx);
		if(_gprm){
			_gprm->apply(ctx);
		}			
		CGContextAddRect(ctx,r);
		CGContextDrawPath(ctx,kCGPathFillStroke);
		CGContextRestoreGState(ctx);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_c_rect(){
CGPoint	a,b;
	get_clic(&a);
	if(a.x==SHRT_MIN){
		return;
	}
	get_cur(&b);
	if(b.x!=SHRT_MIN){	
CGContextRef	ctx=getTempPathContext();
CGFloat			dx=b.x-a.x;
CGFloat			dy=b.y-a.y;
CGRect			r=CGRectMake(a.x-dx,a.y-dy,dx*2,dy*2);			
		CGContextSaveGState(ctx);
		if(_gprm){
			_gprm->apply(ctx);
		}			
		CGContextAddRect(ctx,r);
		CGContextDrawPath(ctx,kCGPathFillStroke);
		CGContextRestoreGState(ctx);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_circle(){
CGPoint	a,b;
	get_clic(&a);
	if(a.x==SHRT_MIN){
		return;
	}
	get_cur(&b);
	if(b.x!=SHRT_MIN){
CGContextRef	ctx=getTempPathContext();
CGFloat			d=CGPointsDist(&a,&b);
CGPoint			c=CGPointMake((a.x+b.x)/2.0,(a.y+b.y)/2.0);
CGRect			r=CGRectMake(c.x-(d/2.0),c.y-(d/2.0),d,d);			
		CGContextSaveGState(ctx);
		if(_gprm){
			_gprm->apply(ctx);
		}			
		CGContextAddEllipseInRect(ctx,r);
		CGContextDrawPath(ctx,kCGPathFillStroke);
		CGContextRestoreGState(ctx);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_c_circle(){
CGPoint	a,b;
	get_clic(&a);
	if(a.x==SHRT_MIN){
		return;
	}
	get_cur(&b);
	if(b.x!=SHRT_MIN){
CGContextRef	ctx=getTempPathContext();
CGFloat			d=CGPointsDist(&a,&b);
CGRect			r=CGRectMake(a.x-d,a.y-d,d*2,d*2);
		CGContextSaveGState(ctx);
		if(_gprm){
			_gprm->apply(ctx);
		}			
		CGContextAddEllipseInRect(ctx,r);
		CGContextDrawPath(ctx,kCGPathFillStroke);
		CGContextRestoreGState(ctx);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_poly(bool use_handles, bool addCur){
	if(!_locs){
		return;
	}

CGContextRef	ctx=getTempPathContext();
CGPoint			a;
i2dvertex		vx;
		
	if(_locs->count()>0){
		CGContextSaveGState(ctx);
		if(_gprm){
			_gprm->apply(ctx);
		}			
		_locs->get(1,&vx);
		Convert(&a,&vx);
		CGContextMoveToPoint(ctx,a.x,a.y);
		for(long i=2;i<=_locs->count();i++){
			_locs->get(i,&vx);
			Convert(&a,&vx);
			CGContextAddLineToPoint(ctx,a.x,a.y);

		}
		if(addCur){
			get_cur(&a);
			if(a.x!=SHRT_MIN){
				CGContextAddLineToPoint(ctx,a.x,a.y);
			}
		}
		if(use_handles){
			for(long i=1;i<=_locs->count();i++){
				_locs->get(i,&vx);
				Convert(&a,&vx);
				CGContextAddRect(ctx,CGRectMake(a.x-2,a.y-2,4,4));
			}
			if(addCur){
				get_cur(&a);
				if(a.x!=SHRT_MIN){
					CGContextAddRect(ctx,CGRectMake(a.x-2,a.y-2,4,4));
				}
			}
		}
		CGContextDrawPath(ctx,kCGPathStroke);
		CGContextRestoreGState(ctx);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::draw_poly(ivertices* vxs, bool use_handles){
//_bTrace_("bStdTool::draw_poly",true);
CGContextRef	ctx=getTempPathContext();
CGPoint			a;
				
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}
	Convert(&a,&vxs->vx.vx2[0]);
	CGContextMoveToPoint(ctx,a.x,a.y);
	for(long i=1;i<vxs->nv;i++){
		Convert(&a,&vxs->vx.vx2[i]);
		CGContextAddLineToPoint(ctx,a.x,a.y);
	}
	if(use_handles){
		for(long i=0;i<vxs->nv;i++){
			Convert(&a,&vxs->vx.vx2[i]);
			CGContextAddRect(ctx,CGRectMake(a.x-2,a.y-2,4,4));
		}
	}
	CGContextDrawPath(ctx,kCGPathStroke);
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::track_obj(){
_bTrace_("bStdTool::track_obj",true);
bArray*				ga;
bGenericStyle*		style;
bGenericGeoElement	*o=NULL,*prev;
long				n=_gapp->layersMgr()->count();
CGPoint				pt;
	
	get_cur(&pt);
	prev=get_obj();
	for(long i=n;i>0;i--){
        style=_gapp->layersMgr()->get(i);
		if(!style){
			continue;
		}	
		ga=style->objsatpoint(&pt,false);
		if(!ga){
			continue;
		}
		if(ga->count()>0){
			ga->get(1,&o);
			delete ga;
			if(o!=prev){
				set_obj(o);
			}
			break;
		}
		delete ga;
	}
	if(o==NULL){
		set_obj(NULL);
	}
//_tm_("obj="+(void*)o);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::track_draw(){
    //if(_on_drag/*&&!StillDown()*/){
    if(_on_drag&&!_stilldown){
		end_clic();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::track_join(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::action(int count){
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bStdTool::load(){
_bTrace_("bStdTool::load",true);
bGenericXMLBaseElement* groot=NULL;
char					nm[_names_length_max_+4];
	getclassname(nm);
	strcat(nm,"_prm");
bool					b=_gapp->document()->readTree(&groot,kStdToolConfigSign,nm);

	if(_use_trace){
_tm_("use trace");
		if(_gprm){
			delete _gprm;
			_gprm=NULL;
		}
		if(b&&groot){
bGenericXMLBaseElement* root=get_param(groot,"graphic");
			if(root){
				_gprm=new bGraphicParams(root,_gapp);
			}
			else{
				_gprm=new bGraphicParams(nm,_gapp);
			}
			if(_gprm->status()){
				delete _gprm;
				_gprm=NULL;
			}
			_use_trace=2;
		}
		if(_gprm==NULL){
			_gprm=new bGraphicParams(nm,_gapp);
		}
	}

	return(groot);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::save(){
bArray					arr(sizeof(xmlelt));
bGenericXMLBaseElement* root=make(arr);
	if(!root){
		return;
	}
char					nm[_names_length_max_+4];
	getclassname(nm);
	strcat(nm,"_prm");
	if(!_gapp->document()->writeTree(root,kStdToolConfigSign,nm)){
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bStdTool::make(bArray &arr){
	if((_gprm)&&(_use_trace!=2)){
		_gprm->update(arr,1);
	}
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bStdTool::get_param(bGenericXMLBaseElement* root, const char* cfg_name){
bGenericXMLBaseElement	*prm,*sgn;
char					val[_values_length_max_];
	for(long i=2;;i++){
		prm=_gapp->classMgr()->NthElement(root,i,"param");
		if(prm==NULL){
			break;
		}
		sgn=_gapp->classMgr()->NthElement(prm,1,"sign");
		if(sgn==NULL){
			break;
		}
		sgn->getvalue(val);
		if(strcmp(cfg_name,val)==0){
			return(prm);
		}
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::hilite_oval(CGContextRef ctx, CGRect r){
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}			
	CGContextAddEllipseInRect(ctx,r);
	CGContextDrawPath(ctx,kCGPathStroke);
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::hilite_rect(CGContextRef ctx, CGRect r, bool paint, bool frame){
	CGContextAddRect(ctx,r);
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}			
	if(paint&&frame){
		CGContextDrawPath(ctx,kCGPathFillStroke);
	}
	else if(paint){
		CGContextDrawPath(ctx,kCGPathFill);
	}
	else if(frame){
		CGContextDrawPath(ctx,kCGPathStroke);
	}
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::hilite_node(CGContextRef ctx, CGPoint pt){
CGRect	r=CGRectMake(pt.x-2,pt.y-2,pt.x+2,pt.y+2);
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}			
	CGContextAddRect(ctx,r);
	CGContextDrawPath(ctx,kCGPathStroke);
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdTool::hilite_cross(CGContextRef ctx, CGPoint pt){
	CGContextMoveToPoint(ctx,pt.x-4,pt.y-4);
	CGContextAddLineToPoint(ctx,pt.x+4,pt.y+4);
	CGContextMoveToPoint(ctx,pt.x-4,pt.y+4);
	CGContextAddLineToPoint(ctx,pt.x+4,pt.y-4);
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}			
	CGContextDrawPath(ctx,kCGPathStroke);
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// Contextual menu for tool choice
// -----------
void bStdTool::pop_tools(CGPoint location,
						 void* inWindow){
long                    i,j,nb,cmd;
char					name[256];
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
UInt32					sgn;
menuitem_desc*			desc;
	
	for(;;){		
		_gapp->document()->readTree(&root,kStdToolConfigSign,kStdToolMenuConfig);
		if(root){
			nb=_gapp->classMgr()->CountElements(root,"sign");
			desc=new menuitem_desc[nb];
			for(i=1;i<=nb;i++){
				elt=_gapp->classMgr()->NthElement(root,i,"sign");
				elt->getvalue(name);
				sgn=*((UInt32*)name);
#ifdef __LITTLE_ENDIAN__
				LBSwapWord(&sgn,sizeof(UInt32));
#endif
				j=_gapp->toolMgr()->ext_index(sgn);
				if(j!=0){
					_gapp->toolMgr()->ext_name(j,desc[i-1].name);
					desc[i-1].checked=(_gapp->toolMgr()->get_current()==j);
					desc[i-1].disabled=false;
                    desc[i-1].index=0;
				}
			}
			cmd=popUpContextMenuWithCGPointAndNSWindow(location,
                                                       inWindow,
                                                       desc,
                                                       nb,
                                                       1.0);
			if(cmd!=0){
				elt=_gapp->classMgr()->NthElement(root,cmd,"sign");
				elt->getvalue(name);
				sgn=*((UInt32*)name);
#ifdef __LITTLE_ENDIAN__
				LBSwapWord(&sgn,sizeof(UInt32));
#endif
				cmd=_gapp->toolMgr()->ext_index(sgn);
			}
			_gapp->classMgr()->ReleaseXMLInstance(root);
		}
		else{
			nb=_gapp->toolMgr()->count();
			desc=new menuitem_desc[nb];
			for(i=1;i<=_gapp->toolMgr()->count();i++){
				_gapp->toolMgr()->ext_name(i,desc[i-1].name);
				desc[i-1].checked=false;
                desc[i-1].disabled=false;
                desc[i-1].index=0;
			}
			desc[_gapp->toolMgr()->get_current()-1].checked=true;
			cmd=popUpContextMenuWithCGPointAndNSWindow(location,
                                                       inWindow,
                                                       desc,
                                                       nb,
                                                       1.0);
		}
		if((cmd!=0)&&(cmd!=_gapp->toolMgr()->get_current())){
			_gapp->toolMgr()->set_current(cmd);
		}
		break;
	}
	if(desc){
		delete[] desc;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
CGContextRef bStdTool::getTempPathContext(){
	return CGLayerGetContext(_gapp->mapIntf()->getPathLayer());
}

// ---------------------------------------------------------------------------
// 
// -----------
CGRect bStdTool::getTempPathContextRect(){
CGLayerRef	lyr=_gapp->mapIntf()->getPathLayer();
    if(lyr==NULL){
        return CGRectZero;
    }
CGSize		sz=CGLayerGetSize(lyr);
	return(CGRectMake(0,0,sz.width,sz.height));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::clearTempPathContext(bool utrc){
//_bTrace_("bStdTool::clearTempPathContext",true);
//_tm_(utrc);
CGLayerRef		lyr=_gapp->mapIntf()->getPathLayer();
    if(lyr==NULL){
//_te_("lyr==NULL");
        return;
    }
//_tm_("lyr:"+(void*)lyr);
CGContextRef	ctx=CGLayerGetContext(lyr);
    if(ctx==NULL){
//_te_("ctx==NULL");
        return;
    }
//_tm_("ctx:"+(void*)ctx);
CGSize			sz=CGLayerGetSize(lyr);
	CGContextClearRect(ctx,CGRectMake(0,0,sz.width,sz.height));
	
	if(utrc){
bGenericTool*	tool;
		for(long i=1;i<=_gapp->toolMgr()->count();i++){
			tool=(bGenericTool*)(void*)_gapp->toolMgr()->get(i);
			if(tool!=this){
				tool->update(true);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdTool::validTempPathContext(){
//_bTrace_("bStdTool::validTempPathContext",true);
	_gapp->mapIntf()->updatePath();
}

