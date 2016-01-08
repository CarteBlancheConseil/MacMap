//----------------------------------------------------------------------------
// File : bToolSelect.cpp
// Project : MacMap
// Purpose : C++ source file : Select tool class
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
// 27/12/2004 creation.
//----------------------------------------------------------------------------
// 17/06/2011 CGGeometry OK.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolSelect.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct TypeRec{
	bool	swp;
	bool	sbd;
}TypeRec;

// ---------------------------------------------------------------------------
// 
// ------------
static bool TYP_SWP(bGenericMacMapApp* gapp, bGenericType* tp){
	if(tp==NULL){
		return(false);
	}
bGenericXMLBaseElement	*elt,*root=GetXMLParamFromType(gapp,tp,"selecttool","config.xml");
char					val[_values_length_max_];
	if(!root){
		return(true);
	}
	elt=gapp->classMgr()->NthElement(root,1,"bool");
	if(!elt){
		gapp->classMgr()->ReleaseXMLInstance(root);
		return(true);
	}
	elt->getvalue(val);
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(atoi(val));
}

// ---------------------------------------------------------------------------
// 
// ------------
static bool TYP_SBD(bGenericMacMapApp* gapp, bGenericType* tp){
	if(tp==NULL){
		return(false);
	}
bGenericXMLBaseElement	*elt,*root=GetXMLParamFromType(gapp,tp,"selecttool","config.xml");
char					val[_values_length_max_];
	if(!root){
		return(true);
	}
	elt=gapp->classMgr()->NthElement(root,2,"bool");
	if(!elt){
		gapp->classMgr()->ReleaseXMLInstance(root);
		return(true);
	}
	elt->getvalue(val);
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(atoi(val));
}

// ---------------------------------------------------------------------------
// 
// ------------
static void TYP_PUT(bGenericMacMapApp* gapp, bGenericType* tp, TypeRec rec){
_bTrace_("TYP_PUT",false);
	if(tp==NULL){
_te_("tp==NULL");
		return;
	}
bGenericXMLBaseElement*	root;
bArray					arr(sizeof(xmlelt));
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	add_idesc(arr,1,"bool",rec.swp);
	add_idesc(arr,1,"bool",rec.sbd);
	root=gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	
	if(root){
		if(SetXMLParamToType(tp,"selecttool","config.xml",root)==false){
_te_("SetXMLParamToType failed");
		}
		gapp->classMgr()->ReleaseXMLInstance(root);
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolSelect	::bToolSelect(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolNav(elt,gapp,bndl)
			,_types(sizeof(TypeRec)){
	setclassname("selecttool");
	
	_pt_all_obj=false;
	_pt_all_typ=false;
	_full_include=true;
	_poly_angle=true;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolSelect::~bToolSelect(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolSelect::create(bGenericXMLBaseElement* elt){
	return(new bToolSelect(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::open(int* flags){
    _pt_all_obj=false;
    _pt_all_typ=false;
    _full_include=true;
    _poly_angle=true;

    set_use_trace(true);
	bStdToolNav::open(flags);
	_ctoggle=load_curs("ShiftCursor.png");
	set_use_drag(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::close(){
	bStdToolNav::close();
	if(_ctoggle){
		ReleaseCocoaCursor(_ctoggle);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::activate(){
	if(get_mnu_cmd()==kSelectDragPoly){
		draw_poly(false,false);
	}
	bStdToolNav::activate();
	if(!get_on_drag()){
		return;
	}
	set_use_track(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::deactivate(){
	if(get_mnu_cmd()==kSelectDragPoly){
		draw_poly(false,false);
	}
	bStdToolNav::deactivate();
	if(!get_on_drag()){
		if(get_mnu_cmd()!=kSelectDragPoly){
			set_use_track(false);
		}
		return;
	}
	set_use_track(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::pop(cocoaMenuPrm prm){
	bStdToolNav::pop(prm);
	if(get_mnu_cmd()==kSelectDragPoly){
		set_use_locs(true);
		set_use_track(true);
		set_use_angle(_poly_angle);
		reset_clic();
	}
	else{
		set_use_locs(false);
		set_use_track(false);
		set_use_angle(false);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::clic(CGPoint pt, int count){
	if((count>1)&&(get_mnu_cmd()==kSelectDragPoly)){
		draw_poly(false,false);
		validTempPathContext();
		polygon_select();
	}
	bStdToolNav::clic(pt,count);
	if((count==1)&&(get_mnu_cmd()==kSelectDragPoly)){
		draw_poly(false,true);
	}
	if((count>1)&&(get_mnu_cmd()!=kSelectDragPoly)){
		action(2);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::end_clic(){
	bStdToolNav::end_clic();
	if(get_mnu_cmd()!=kSelectDragPoly){
		set_use_track(false);
	}
	if(!get_use_drag()){
		return;
	}
	if(!get_active()){
		return;
	}
	switch(get_mnu_cmd()){
		case kSelectDragRect:
		case kSelectDragCircle:
			action(1);
			break;
		case kSelectDragPoly:
			break;
	}
	clearTempPathContext(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	switch(get_mnu_cmd()){
		case kSelectDragRect:
			if(get_on_drag()){
				draw_rect();
				report();
			}
			break;
		case kSelectDragCircle:
			if(get_on_drag()){
				draw_c_circle();
				report();
			}
			break;
		case kSelectDragPoly:
			draw_poly(false,true);
			break;
	}
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::set_modifiers(int k){	
	bStdToolNav::set_modifiers(k);
	if(!get_active()){
		return;
	}
	if(is_modifiers(shiftKey)){
		set_curs(_ctoggle);
	}
	else{
		set_curs();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolSelect::set_key(int k){
bool	b=false;

	if(get_mnu_cmd()==kSelectDragPoly){
		switch(k){
			case kEnterCharCode:
			case kReturnCharCode:
				draw_poly(false,false);
				validTempPathContext();
				polygon_select();
				b=true;
				break;
			case kEscapeCharCode:
				draw_poly(false,false);
				validTempPathContext();
				b=true;
				break;
			case kBackspaceCharCode:{
					if(get_locs()->count()<2){
						break;
					}
i2dvertex a,b,c;
					validTempPathContext();
					get_clic(&a);
					get_cur(&b);
					get_locs()->get(get_locs()->count()-1,&c);
					set_clic(&c);
					set_cur(&a);
					set_cur(&b);
					set_clic(&a);
				}
				break;
		}
	}
	(void)bStdToolNav::set_key(k);
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::action(int count){
	if(count==1){
CGPoint	clic;
CGPoint	cur;
		
		get_clic(&clic);
		get_cur(&cur);
		if(	(cur.x==SHRT_MIN)	||
			(clic.x==SHRT_MIN)	){
			return;
		}
		if(!CGPointEqualToPoint(clic,cur)){
			switch(get_mnu_cmd()){
				case kSelectDragRect:
					rect_select();
					break;
				case kSelectDragCircle:
					circle_select();
					break;
				case kSelectDragPoly:
					break;
			}
			return;
		}
		else{
			if(get_on_drag()){
				set_use_drag(false);
			}
			point_select();
		}
	}
	else{
		if(get_on_drag()){
				set_use_drag(false);
		}
		SendCommandToApplication('fich');
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolSelect::edit_event(EventRef evt, WindowRef wd){
bool			b=true;
char			val[256];
HICommand		cmd;
ControlRef		c;
ControlID		cid;
TypeRec			rec;
bGenericType*	tp;
	cid.signature=kSelectEditSign;
	
	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				cid.id=kSelectAngleValueID;
				GetControlByID(wd,&cid,&c);
				GetTextControlValue(c,val,sizeof(val));
				if((matof(val)>0)&&(matof(val)<=360)){
					set_angle(matof(val));
				}
				if(get_mnu_cmd()==kSelectDragPoly){
					set_use_angle(_poly_angle);
				}
				for(int i=1;i<=_types.count();i++){
					_types.get(i,&rec);
					tp=_gapp->typesMgr()->get(i);
					TYP_PUT(_gapp,tp,rec);
				}
				_types.reset();
				break;
			case kHICommandCancel:
				_types.reset();
				break;
			case kSelectAllObjectsCmd:
				cid.id=kSelectAllObjectsID;
				GetControlByID(wd,&cid,&c);
				_pt_all_obj=GetControl32BitValue(c);
				break;
			case kSelectAllTypesCmd:
				cid.id=kSelectAllTypesID;
				GetControlByID(wd,&cid,&c);
				_pt_all_typ=GetControl32BitValue(c);
				break;
			case kSelectIncludeCmd:
				cid.id=kSelectIncludeID;
				GetControlByID(wd,&cid,&c);
				_full_include=GetControl32BitValue(c);
				break;
			case kSelectAngleUseCmd:
				cid.id=kSelectAngleUseID;
				GetControlByID(wd,&cid,&c);
				_poly_angle=GetControl32BitValue(c);
				for(cid.id=kSelectAngleTextID;cid.id<=kSelectAngleUnitID;cid.id++){
					GetControlByID(wd,&cid,&c);
					SetControlAbility(c,_poly_angle);
				}
				break;
			case kSelectAngleValueCmd:
				break;
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::edit_init(WindowRef wd){
ControlRef	c;
ControlID	cid;
	
	cid.signature=kSelectEditSign;
	
	cid.id=kSelectAllObjectsID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_pt_all_obj);
	
	cid.id=kSelectAllTypesID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_pt_all_typ);
	
	cid.id=kSelectIncludeID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_full_include);
	
	cid.id=kSelectAngleUseID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_poly_angle);
	
char	val[256];
	cid.id=kSelectAngleValueID;
	GetControlByID(wd,&cid,&c);
	sprintf(val,"%.0f",get_angle());
	SetTextControlValue(c,val);
	SetTextControlFilter(c,ufloat_filter);
	for(cid.id=kSelectAngleTextID;cid.id<=kSelectAngleUnitID;cid.id++){
		GetControlByID(wd,&cid,&c);
		SetControlAbility(c,_poly_angle);
	}
	
TypeRec			rec;
bGenericType*	tp;
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		rec.swp=TYP_SWP(_gapp,tp);
		rec.sbd=TYP_SBD(_gapp,tp);
		_types.add(&rec);
	}
	
	cid.id=kSelectPTypesID;
	GetControlByID(wd,&cid,&c);
	DataBrowserInit(c,dtb_proc,NULL,(long)this);
	populate_types(wd);
	SetDataBrowserPropertyFlags(c,kSelectCheckSWPProperty,kDataBrowserPropertyIsEditable);

	cid.id=kSelectDTypesID;
	GetControlByID(wd,&cid,&c);
	DataBrowserInit(c,dtb_proc,NULL,(long)this);
	populate_types(wd);
	SetDataBrowserPropertyFlags(c,kSelectCheckSBDProperty,kDataBrowserPropertyIsEditable);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolSelect::load(){
_bTrace_("bToolSelect::load",true);
bGenericXMLBaseElement* groot=bStdToolNav::load();
bGenericXMLBaseElement* elt=NULL;
	if(!groot){
_tw_("no groot");
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
_tw_("no root");
		return(NULL);
	}
char					val[_values_length_max_];
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_pt_all_obj=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,2,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_pt_all_typ=atoi(val);

		elt=_gapp->classMgr()->NthElement(root,3,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_full_include=atoi(val);

		elt=_gapp->classMgr()->NthElement(root,4,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_poly_angle=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,1,"float");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		if((matof(val)>0)&&(matof(val)<=360)){
			set_angle(matof(val));
		}
		break;
	}
    
    _tm_(_pt_all_obj);
    _tm_(_pt_all_typ);
    _tm_(_full_include);
    _tm_(_poly_angle);
    
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolSelect::make(bArray& arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	add_idesc(arr,2,"bool",_pt_all_obj);
	add_idesc(arr,2,"bool",_pt_all_typ);
	add_idesc(arr,2,"bool",_full_include);
	add_idesc(arr,2,"bool",_poly_angle);
	add_ddesc(arr,2,"float",get_angle(),2);
	return(bStdToolNav::make(arr));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::rect_select(){
bArray*			ga;
bArray			gh(sizeof(bGenericGeoElement*));
bGenericStyle*	style;
long			i,n=_gapp->layersAccessCtx()->count();
CGPoint			a,b;
CGRect			r;
bool			toggles=is_modifiers(shiftKey);
	
	get_clic(&a);
	get_cur(&b);
	r=CGRectStandardize(CGRectMake(a.x,a.y,b.x-a.x,b.y-a.y));
	set_obj(NULL);
	if(!toggles){
		FlushSelection(_gapp);
	}
	for(i=n;i>0;i--){
		style=_gapp->layersAccessCtx()->get(i);
		if(!style){
			continue;
		}	
		if(!TYP_SBD(_gapp,style->gettype())){
			continue;
		}
		ga=style->objsinrect(&r,_full_include);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(!toggles){
			_gapp->selMgr()->add(ga);
		}
		else{
			toggle(ga,&gh);
		}
		delete ga;
	}
	reset_handled(&gh);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::circle_select(){
bArray*			ga;
bArray			gh(sizeof(bGenericGeoElement*));
bGenericStyle*	style;
long			i,n=_gapp->layersAccessCtx()->count();
CGPoint			a,b;
CGRect			r;
bool			toggles=is_modifiers(shiftKey);
float			d;
	
	get_clic(&a);
	get_cur(&b);

	d=round(sqrt((float)(b.x-a.x)*(float)(b.x-a.x)+(float)(b.y-a.y)*(float)(b.y-a.y)));
	r.origin.x=a.x-d;
	r.origin.y=a.y-d;
	r.size.width=d*2;
	r.size.height=d*2;
	
	set_obj(NULL);
	if(!toggles){
		FlushSelection(_gapp);
	}
	for(i=n;i>0;i--){
		style=_gapp->layersAccessCtx()->get(i);
		if(!style){
			continue;
		}	
		if(!TYP_SBD(_gapp,style->gettype())){
			continue;
		}
		ga=style->objsincircle(&r,_full_include);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(!toggles){
			_gapp->selMgr()->add(ga);
		}
		else{
			toggle(ga,&gh);
		}
		delete ga;
	}
	reset_handled(&gh);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::polygon_select(){
bArray*			ga;
bArray			gh(sizeof(bGenericGeoElement*));
bGenericStyle*	style;
long			i,n,nlocs;
CGPoint*		cgp;
i2dvertex		vx;
bool			toggles=is_modifiers(shiftKey);
	
	ga=get_locs();
	nlocs=ga->count();
	cgp=new CGPoint[nlocs+1];
	if(!cgp){
		return;
	}
	for(i=1;i<=nlocs;i++){
		ga->get(i,&vx);
		_gapp->locConverter()->convert(&cgp[i-1],&vx);
	}
	cgp[nlocs]=cgp[0];
	set_obj(NULL);
	if(!toggles){
		FlushSelection(_gapp);
	}
	n=_gapp->layersAccessCtx()->count();
	for(i=n;i>0;i--){
		style=_gapp->layersAccessCtx()->get(i);
		if(!style){
			continue;
		}	
		if(!TYP_SBD(_gapp,style->gettype())){
			continue;
		}
		ga=style->objsinpath(cgp,nlocs+1,_full_include);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(!toggles){
			_gapp->selMgr()->add(ga);
		}
		else{
			toggle(ga,&gh);
		}
		delete ga;
	}
	reset_handled(&gh);
	delete[] cgp;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::point_select(){
bArray*				ga;
bArray				gh(sizeof(bGenericGeoElement*));
bGenericStyle*		style;
bGenericGeoElement*	o=NULL;
long				i,n=_gapp->layersAccessCtx()->count();
CGPoint				pt;
bool				toggles=is_modifiers(shiftKey);

	get_clic(&pt);
	if(!toggles){
		FlushSelection(_gapp);
	}
	for(i=n;i>0;i--){
		style=_gapp->layersAccessCtx()->get(i);
		if(!style){
			continue;
		}
		if(!TYP_SWP(_gapp,style->gettype())){
			continue;
		}
		ga=style->objsatpoint(&pt,_pt_all_obj);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(_pt_all_obj){
			if(!toggles){
				_gapp->selMgr()->add(ga);
			}
			else{
				toggle(ga,&gh);
			}
		}
		else{
			ga->get(1,&o);
			if(!toggles){
				o->setselect(true,true);
			}
			else{
				if(o->selected()){
					o->setselect(false,true);
				}
				else{
					o->setselect(true,true);
				}
			}
		}
		if(!_pt_all_typ){
			delete ga;
			break;
		}
		delete ga;
	}
	reset_handled(&gh);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::report(){
double			dx,dy;
bGenericUnit*	u=_gapp->distMgr()->get();
char			str[256]="",lib[64];
int				dg=_gapp->document()->dist_pref_digits();
i2dvertex		vxa,vxb;

	u->short_name(lib);

	get_clic(&vxb);
	get_cur(&vxa);
	if(get_mnu_cmd()==kSelectDragRect){
		dx=Measure_i2d(_gapp,fabs(vxb.h-vxa.h));
		dy=Measure_i2d(_gapp,fabs(vxb.v-vxa.v));
		sprintf(str,"dx=%.*f %s, dy=%.*f %s",dg,dx,lib,dg,dy,lib);
	}
	else if(get_mnu_cmd()==kSelectDragCircle){
		dx=Measure_i2d(_gapp,ivx2_dist(&vxa,&vxb)*2.0);
		sprintf(str,"d=%.*f %s",dg,dx,lib);
	}
	WriteMessage(_gapp,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::toggle(bArray* input, bArray* handled){
bGenericGeoElement*	o;
	for(long i=1;i<=input->count();i++){
		input->get(i,&o);
		if(o->handled()){
			continue;
		}
		if(o->selected()){
			o->setselect(false,true);
		}
		else{
			o->setselect(true,true);
		}
		o->sethandled(true);
		handled->add(&o);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::reset_handled(bArray* handled){
long				n=handled->count();
bGenericGeoElement*	o;
	
	for(long i=1;i<=n;i++){
		handled->get(i,&o);
		o->sethandled(false);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bToolSelect::dtb_proc(ControlRef browser, 
									  DataBrowserItemID itemID, 
									  DataBrowserPropertyID property, 
									  DataBrowserItemDataRef itemData, 
									  Boolean changeValue){
bToolSelect*	t=(bToolSelect*)GetControlReference(browser);
CFStringRef		cfs;
bGenericType*	tp;
TypeRec			rec;
Boolean			b;
char			name[256];

	switch(property){
		case kSelectCheckSWPProperty:
			t->_types.get(itemID,&rec);
			if(!changeValue){
				b=rec.swp;
				SetDataBrowserItemDataBooleanValue(itemData,b);				
			}
			else{
				GetDataBrowserItemDataBooleanValue(itemData,&b);
				rec.swp=b;
				t->_types.put(itemID,&rec);
			}
			break;
		case kSelectCheckSBDProperty:
			t->_types.get(itemID,&rec);
			if(!changeValue){
				b=rec.sbd;
				SetDataBrowserItemDataBooleanValue(itemData,b);				
			}
			else{
				GetDataBrowserItemDataBooleanValue(itemData,&b);
				rec.sbd=b;
				t->_types.put(itemID,&rec);
			}
			break;			
		case kSelectNameProperty:
			if(!changeValue){
				tp=_gapp->typesMgr()->get(itemID);
				tp->name(name);
				cfs=CFStringCreateWithCString(kCFAllocatorDefault,name,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,cfs);
				CFRelease(cfs);
			}
			break;
			
		case kDataBrowserItemIsEditableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
			
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
			
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolSelect::populate_types(WindowRef wd){
ControlRef			c;
ControlID			cid={kSelectEditSign,kSelectPTypesID};
DataBrowserItemID	itemId;
	
	GetControlByID(wd,&cid,&c);
	for(long i=1;i<=_gapp->typesMgr()->count();i++){
		itemId=i;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kSelectCheckSWPProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kSelectNameProperty);
	}
	cid.id=kSelectDTypesID;
	GetControlByID(wd,&cid,&c);
	for(long i=1;i<=_gapp->typesMgr()->count();i++){
		itemId=i;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kSelectCheckSBDProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kSelectNameProperty);
	}
}

