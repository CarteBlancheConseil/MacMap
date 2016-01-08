//----------------------------------------------------------------------------
// File : bToolInfo.cpp
// Project : MacMap
// Purpose : C++ source file : Object info tooltip class (show a set of field when mouse is over objects)
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
// 07/01/2005 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolInfo.h"
#include <mox_intf/mm_messages.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Type_Utils.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/base_def.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------

#define kCursOffset		16.0

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolInfo	::bToolInfo(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolNav(elt,gapp,bndl)
			,_fields(sizeof(bArray*)){
	setclassname("infotool");

	_cleft=NULL;
	_cleft_on=false;
	
	_wd_width=0;
	_wd_height=0;
	_show_type=true;
	_fnt_size=9;
	strcpy(_fnt,"Geneva");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolInfo::~bToolInfo(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolInfo::create(bGenericXMLBaseElement* elt){
	return(new bToolInfo(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::open(int* flags){
	_cleft=NULL;
	_cleft_on=false;
	
	_wd_width=0;
	_wd_height=0;
	_show_type=true;
	_fnt_size=9;
	strcpy(_fnt,"Geneva");
	
	_cleft=load_curs("InvCursor.png");
	
bArray*	a;

	for(long i=1;i<=_gapp->typesMgr()->count();i++){
		a=new bArray(sizeof(fld));
		_fields.add(&a);
	}

	set_use_trace(true);
	bStdToolNav::open(flags);
	
	set_use_track(true);
	set_use_sel(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::close(){
	bStdToolNav::close();
	if(_cleft){
		ReleaseCocoaCursor(_cleft);
		_cleft=NULL;
	}

bArray*	a;
	for(int i=1;i<=_fields.count();i++){
		_fields.get(i,&a);
		delete a;
	}
	_fields.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::clic(CGPoint pt, int count){
	bStdToolNav::clic(pt,count);
bGenericGeoElement* o=get_obj();
	if(o){
		FlushSelection(_gapp);
		o->setselect(true,true);
		if(count>1){
			SendCommandToApplication('fich');
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::update(bool global){
	if(!get_active()){
		return;
	}
	if(get_obj()){
		init_draw();
		if(_wd_height>0){
			check_bounds();
			draw();
		}
		else{
		}
	}
	else{
		if(_wd_height>0){
			check_bounds();
			draw();
		}
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolInfo::edit_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
ControlRef	c;
ControlID	cid;
int			t;

	cid.signature=kInfoEditSign;
	
	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				cid.id=kInfoFieldsID;
				GetControlByID(wd,&cid,&c);
				DataBrowserClose(c);
				break;
			case kInfoShowTypeCmd:
				cid.id=kInfoShowTypeID;
				GetControlByID(wd,&cid,&c);
				_show_type=GetControl32BitValue(c);
			case kInfoTypesCmd:
				cid.id=kInfoTypesID;
				GetControlByID(wd,&cid,&c);
				t=GetControl32BitValue(c);
				if(t!=_edit_type){	
					flush_fields(wd);
					_edit_type=t;
					populate_fields(wd);
				}
				break;	
			case kInfoFieldsCmd:
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
void bToolInfo::edit_init(WindowRef wd){
ControlRef		c;
ControlID		cid;

/* A GERER PAR UN MAP EVENT*/
bArray*			a;

	for(int i=_fields.count()+1;i<=_gapp->typesMgr()->count();i++){
		a=new bArray(sizeof(fld));
		_fields.add(&a);
	}
	for(int i=_fields.count();i>_gapp->typesMgr()->count();i--){
		_fields.get(i,&a);
		delete a;
		_fields.rmv(i);
	}
/* A GERER PAR UN MAP EVENT*/

	_edit_type=1;
	
	cid.signature=kInfoEditSign;
	
	cid.id=kInfoShowTypeID;
	GetControlByID(wd,&cid,&c);
	SetControl32BitValue(c,_show_type);
	
	cid.id=kInfoTypesID;
	GetControlByID(wd,&cid,&c);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,1);

	cid.id=kInfoFieldsID;
	GetControlByID(wd,&cid,&c);
	DataBrowserInit(c,dtb_proc,NULL,(long)this);
	populate_fields(wd);
	SetDataBrowserPropertyFlags(c,kInfoFieldsCheckProperty,kDataBrowserPropertyIsEditable);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolInfo::load(){
bGenericXMLBaseElement* groot=bStdToolNav::load();
bGenericXMLBaseElement* elt=NULL;
	if(!groot){
		_gprm=new bToolInfoParams(groot,_gapp,getbundle());
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		return(NULL);
	}
char					val[_values_length_max_];
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_show_type=atoi(val);
		
		/*A VIRER*/
		elt=_gapp->classMgr()->NthElement(root,1,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_fnt_id=atoi(val);

		elt=_gapp->classMgr()->NthElement(root,1,"string");
		if(!elt){
			break;
		}
		elt->getvalue(_fnt);
		
		elt=_gapp->classMgr()->NthElement(root,2,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_fnt_size=matof(val);		
		if((_fnt_size<5)||(_fnt_size>44)){
			_fnt_size=9;
		}
		/**/

		break;
	}
	
	root=get_param(groot,"graphic");
	if(_gprm){
		delete _gprm;
		_gprm=NULL;
	}
	_gprm=new bToolInfoParams(root,_gapp,getbundle());
	
	_gapp->classMgr()->ReleaseXMLInstance(groot);	
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		load_type(i);
	}
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolInfo::make(bArray& arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);

	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	
	add_idesc(arr,2,"bool",_show_type);
	
	/*A VIRER*/
	add_idesc(arr,2,"int",_fnt_id);
	add_cdesc(arr,2,"string",_fnt);
	add_idesc(arr,2,"float",_fnt_size);
	/**/
	
	return(bStdToolNav::make(arr));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::save(){
	bStdToolNav::save();
bGenericXMLBaseElement* root=NULL;
char					nm[_names_length_max_];
	getclassname(nm);
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		root=make(i);
		if(!root){
			continue;
		}
		SetXMLParamToType(_gapp->typesMgr()->get(i),nm,"config.xml",root);
		_gapp->classMgr()->ReleaseXMLInstance(root);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::load_type(int tidx){
char					nm[_names_length_max_];
	getclassname(nm);
bGenericType*			tp=_gapp->typesMgr()->get(tidx);
bGenericXMLBaseElement* root=GetXMLParamFromType(_gapp,tp,nm,"config.xml");
char					val[_values_length_max_];
fld						f;
bArray*					a;
	
	f.value[0]=0;
	if(!root){
		return;
	}

bGenericXMLBaseElement* elt=NULL;
	
	_fields.get(tidx,&a);
	a->reset();
	for(int i=1;;i++){
		elt=_gapp->classMgr()->NthElement(root,i,"int");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		f.fid=atoi(val);
		f.idx=tp->fields()->get_index(f.fid);
		if(!f.idx){
			continue;
		}
		tp->fields()->get_name(f.idx,f.name);
		a->add(&f);
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolInfo::make(int tidx){
bGenericXMLBaseElement*	root;
bArray					arr(sizeof(xmlelt));
bArray*					a;
bGenericType*			tp;
fld						f;
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	if(_fields.get(tidx,&a)){
		tp=_gapp->typesMgr()->get(tidx);
		for(int i=1;i<=a->count();i++){
			a->get(i,&f);
			f.idx=tp->fields()->get_index(f.fid);
			if(f.idx){
				add_idesc(arr,1,"int",f.fid);
				tp->fields()->get_name(f.idx,f.name);
				a->put(i,&f);
			}
		}
	}
	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
pascal OSStatus bToolInfo::dtb_proc(	ControlRef browser, 
										DataBrowserItemID itemID, 
										DataBrowserPropertyID property, 
										DataBrowserItemDataRef itemData, 
										Boolean changeValue){
bToolInfo*		t=(bToolInfo*)GetControlReference(browser);
CFStringRef		cfs;
fld				f;
bArray*			a;
char			name[256];
bGenericType*	tp=_gapp->typesMgr()->get(t->_edit_type);
int				idx;

	switch(property){
		case kInfoFieldsCheckProperty:
			idx=t->get_index(t->_edit_type,itemID);
			if(!changeValue){
				SetDataBrowserItemDataBooleanValue(itemData,(idx!=0));				
			}
			else{
				t->_fields.get(t->_edit_type,&a);
				if(idx){
					a->rmv(idx);
				}
				else{
					f.fid=itemID;
					a->add(&f);
				}
			}
			break;
			
		case kInfoFieldsNameProperty:
			if(!changeValue){
				tp->fields()->get_name(tp->fields()->get_index(itemID),name);
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
void bToolInfo::init_draw(){
bGenericGeoElement* o=get_obj();
	if(!o){
		return;
	}
	_wd_width=0;
	_wd_height=0;
	
char	name[256],buff[256],fnt[256];
float	fntsz=9;
	if(_gprm){
		fntsz=((bToolInfoParams*)_gprm)->fontSize();
		strcpy(fnt,((bToolInfoParams*)_gprm)->font());
	}
	if(_show_type){
bGenericType* tp=_gapp->typesMgr()->get(o->getType());
		message_string(kMsgType,name,1);
		sprintf(buff,"%s : ",name);
		tp->name(name);
		strcat(buff,name);
bCoreTextInfo	txt(fnt,fntsz,0,0,0,buff);
		_wd_width=txt.textWidth();
		_wd_height=txt.textHeight();
	}
bArray* a;
	if(!_fields.get(o->getType(),&a)){
		a=new bArray(sizeof(fld));
		_fields.add(&a);
	}
CGFloat	w;
fld		f;
	for(int i=1;i<=a->count();i++){
		a->get(i,&f);
		o->getValue(f.idx,f.value);
		a->put(i,&f);
		sprintf(buff,"%s : %s",f.name,f.value);
bCoreTextInfo	txt(fnt,fntsz,0,0,0,buff);
		w=txt.textWidth();
		if(w>_wd_width){
			_wd_width=w;
		}
		_wd_height+=txt.textHeight();
	}		
	if((_wd_width>0)&&(_wd_height>0)){
		_wd_width+=4;
		_wd_height+=2;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::draw(){
bGenericGeoElement* o=get_obj();
	if(!o){
		return;
	}
	
char	name[256],buff[256],fnt[256];
float	fntsz=9;
float	rad=3;
	if(_gprm){
		fntsz=((bToolInfoParams*)_gprm)->fontSize();
		rad=((bToolInfoParams*)_gprm)->radius();
		strcpy(fnt,((bToolInfoParams*)_gprm)->font());
	}
	
CGContextRef	ctx=getTempPathContext();
CGPoint			pt;
	
	get_cur(&pt);
CGFloat			v=pt.y-kCursOffset-(CGFloat)fntsz;		
CGRect			r=CGRectMake(pt.x+kCursOffset,pt.y-kCursOffset-_wd_height-2,_wd_width,_wd_height+2);
	
	CGContextSaveGState(ctx);
	if(_gprm){
		_gprm->apply(ctx);
	}
	CGContextSetShadow(ctx,CGSizeMake(5,-5),10);
	if(_gprm){
		CGContextAddRoundedRect(ctx,r,rad);
	}
	else{
		CGContextAddRect(ctx,r);
	}
	CGContextDrawPath(ctx,kCGPathFillStroke);
	if(_gprm){
		((bToolInfoParams*)_gprm)->applyFont(ctx);
	}
	
	pt.x+=2;
	if(_show_type){
bGenericType* tp=_gapp->typesMgr()->get(o->getType());
		message_string(kMsgType,name,1);
		sprintf(buff,"%s : ",name);
		tp->name(name);
		strcat(buff,name);
bCoreTextInfo	txt(fnt,fntsz,0,0,0,buff);
		CGContextShowGlyphsAtPoint(ctx,pt.x+kCursOffset,v,txt.glyphs(),txt.nGlyphs());
		v-=txt.textHeight();	
	}

bArray* a;
	if(!_fields.get(o->getType(),&a)){
		a=new bArray(sizeof(fld));
		_fields.add(&a);
	}
fld	f;
	for(int i=1;i<=a->count();i++){
		a->get(i,&f);
		sprintf(buff,"%s : %s",f.name,f.value);
bCoreTextInfo	txt(fnt,fntsz,0,0,0,buff);
		CGContextShowGlyphsAtPoint(ctx,pt.x+kCursOffset,v,txt.glyphs(),txt.nGlyphs());
		v-=txt.textHeight();	
	}
	
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::check_bounds(){
CGPoint	cgp;
CGRect	r=getTempPathContextRect();
bool	h=false;
		
	get_cur(&cgp);
	if(cgp.x+_wd_width+kCursOffset+2>r.size.width){
		cgp.x=r.size.width-(_wd_width+kCursOffset+2);
		if(!_cleft_on){
			_cleft_on=true;
			set_curs(_cleft);
		}
		h=true;
	}
	else{
		if(_cleft_on){
			_cleft_on=false;
			set_curs();
		}
	}
	if(cgp.y-(_wd_height+kCursOffset+2)<0){
		if(!h){
			cgp.y=_wd_height+kCursOffset+4;
		}
		else{
			cgp.y+=(_wd_height+kCursOffset+3);
		}
	}
	set_cur(&cgp);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::flush_fields(WindowRef wd){
ControlRef			c;
ControlID			id={kInfoEditSign,kInfoFieldsID};
bGenericType*		tp=_gapp->typesMgr()->get(_edit_type);
DataBrowserItemID	itemId;

	GetControlByID(wd,&id,&c);
	for(int i=tp->fields()->count();i>=kOBJ_Name_;i--){
		itemId=tp->fields()->get_id(i);
		RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kInfoFieldsCheckProperty);
		RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kInfoFieldsNameProperty);
	}
	SetDataBrowserScrollPosition(c,0,0);

}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolInfo::populate_fields(WindowRef wd){
ControlRef			c;
ControlID			cid={kInfoEditSign,kInfoFieldsID};
bGenericType*		tp=_gapp->typesMgr()->get(_edit_type);
DataBrowserItemID	itemId;

	GetControlByID(wd,&cid,&c);
	for(int i=kOBJ_Name_;i<=tp->fields()->count();i++){
		itemId=tp->fields()->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kInfoFieldsCheckProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kInfoFieldsNameProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bToolInfo::get_index(int tpidx, int fid){
bArray*	a;
	if(!_fields.get(tpidx,&a)){
		return(0);
	}
fld		f;
	for(int i=1;i<=a->count();i++){
		a->get(i,&f);
		if(f.fid==fid){
			return(i);
		}
	}
	return(0);
}

#pragma mark =>Graphic params
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolInfoParams::bToolInfoParams(bGenericXMLBaseElement* root,
								 bGenericMacMapApp* gapp,
								 CFBundleRef bndl)
				:bGraphicParams(root,gapp){
	if(_status==-1){
		_width=0;
		_stroke[0]=0;
		_stroke[1]=0;
		_stroke[2]=0;
		_stroke[3]=0;
		_fill[0]=0;
		_fill[1]=0;
		_fill[2]=0;
		_fill[3]=0.8;
	}
	_status=-1;
	strcpy(_fnt,"Geneva");
	_fnt_sz=9;
	_fnt_fill[0]=1;
	_fnt_fill[1]=1;
	_fnt_fill[2]=1;
	_fnt_fill[3]=1;
	_radius=3;
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
			_fill[0]=0;
			_fill[1]=0;
			_fill[2]=0;
			_fill[3]=0.8;
		}
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bToolInfoParams::~bToolInfoParams(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bToolInfoParams::update(bArray& arr, 
							long indent){
	bGraphicParams::update(arr,indent);
	add_cdesc(arr,indent+1,"name",_fnt);
	add_ddesc(arr,indent+1,"float",_fnt_sz,2);
	for(long i=0;i<4;i++){
		add_ddesc(arr,indent+1,"float",_fnt_fill[i],2);
	}
	add_ddesc(arr,indent+1,"float",_radius,2);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::applyFont(CGContextRef ctx){
	CGContextSetRGBFillColor(ctx,_fnt_fill[0],_fnt_fill[1],_fnt_fill[2],_fnt_fill[3]);
	CGContextSelectFont(ctx,_fnt,_fnt_sz,kCGEncodingMacRoman);
	CGContextSetTextDrawingMode(ctx,kCGTextFill);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::setFont(const char* fnt){
	strcpy(_fnt,fnt);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::setFontSize(float sz){
	_fnt_sz=sz;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::setFontFill(const float* clr){
	for(long i=0;i<4;i++){
		_fnt_fill[i]=clr[i];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::setRadius(float radius){
	_radius=radius;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bToolInfoParams::parse(bGenericXMLBaseElement* root, bGenericMacMapApp* gapp){
bGenericXMLBaseElement* elt=NULL;
char					val[_values_length_max_];

	elt=gapp->classMgr()->NthElement(root,1,"name");
	if(!elt){
		return;
	}
	elt->getvalue(_fnt);
	
	elt=gapp->classMgr()->NthElement(root,10,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_fnt_sz=matof(val);
	
	for(long i=0;i<4;i++){
		elt=gapp->classMgr()->NthElement(root,i+11,"float");
		if(!elt){
			return;
		}
		elt->getvalue(val);
		_fnt_fill[i]=matof(val);
	}
	
	elt=gapp->classMgr()->NthElement(root,15,"float");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_radius=matof(val);	
	
	_status=0;
}

