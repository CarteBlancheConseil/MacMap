//----------------------------------------------------------------------------
// File : bXMapFExp.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, export format and projection
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
// 08/06/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapFExp.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>
#include "mm_config.h"

// ---------------------------------------------------------------------------
// 
// ------------

// ---------------------------------------------------------------------------
// 
// ------------
class type_p{
	public:
		type_p					(	bGenericType*	tp);
		virtual ~type_p			(	);
		
		virtual bool load		(	bGenericMacMapApp* gapp);
		virtual bool dump		(	bGenericMacMapApp* gapp);

		static bool read_proc	(	bGenericXMLBaseElement* elt,
									void* prm,
									int indent);
											
		bGenericType* 		_tp;
		bArray				_flds;
};

// ---------------------------------------------------------------------------
// Constructeur
// ------------
type_p	::type_p(bGenericType* tp) 
		:_flds(sizeof(fexp_field)){
	_tp=tp;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
type_p	::~type_p(){
	
}

// ---------------------------------------------------------------------------
// 
// ------------
bool type_p::load(bGenericMacMapApp* gapp){
_bTrace_("type_p::load",true);
bGenericXMLBaseElement*	root=GetXMLParamFromType(gapp,_tp,kXMapFExpMessageID,kXMapFExpStdParamID);
fexp_field				fld;
	if(!root){
_te_("root==NULL");
//int		fid;
		
		for(int i=kOBJ_Name_;i<=_tp->fields()->count();i++){
			_tm_(i);
			fld.fid=_tp->fields()->get_id(i);
			//_tp->fields()->get_name(i,fld.fname);
			fld.fname[0]=0;
			_flds.add(&fld);
		}
		return(true);
	}

	root->dotoall(this,0,read_proc);
	
	for(long i=_flds.count();i>0;i--){
		_flds.get(i,&fld);
		if(fld.fid==0){
			_flds.rmv(i);
		}
	}
	
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool type_p::dump(bGenericMacMapApp* gapp){
_bTrace_("type_p::dump",false);
bArray		arr(sizeof(xmlelt));
fexp_field	fld;

	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	for(int i=1;i<=_flds.count();i++){
		_flds.get(i,&fld);
		add_idesc(arr,1,"int",fld.fid);
		add_cdesc(arr,1,"name",fld.fname);
	}
bGenericXMLBaseElement*			root=gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return(false);
	}
	if(!SetXMLParamToType(_tp,kXMapFExpMessageID,kXMapFExpStdParamID,root)){
_te_("SetXMLParamToType failed");
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool type_p::read_proc(	bGenericXMLBaseElement* elt,
						void* prm,
						int indent){
type_p*			p=(type_p*)prm;
char			name[_names_length_max_];
char			value[_values_length_max_];
//int				fidx;
//int				fid;
fexp_field		fld;
	
	elt->getclassname(name);
	if(!strcmp(name,"int")){
		elt->getvalue(value);
		fld.fid=atoi(value);
//		fidx=p->_tp->fields()->get_index(fld.fid);
//		if(fidx){
			p->_flds.add(&fld);
//		}
	}
	else if(!strcmp(name,"name")){
		p->_flds.get(p->_flds.count(),&fld);
		elt->getvalue(fld.fname);
		p->_flds.put(p->_flds.count(),&fld);

		//		elt->getvalue(value);
//		fid=p->_tp->fields()->get_id(value);
//		if(fid){
//			p->_flds.add(&fid);
//		}
	}
	return(true);
}
											
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapFExp	::bXMapFExp(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMapWithModalIntf(elt,gapp,bndl)
			,_arr(sizeof(type_p*)){
	setclassname("fexp");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapFExp::~bXMapFExp(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapFExp::create(bGenericXMLBaseElement* elt){
	return(new bXMapFExp(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapFExp::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tp=0;
	_srid=-1;
	read_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFExp::process(int msg, void* prm){
fexp_prm* p=(fexp_prm*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			break;
		case kExtProcessCallWithParams:
			break;
		case kExtProcessCallGetData:
			get(p->tidx,p->flds,&p->srid);
			break;
		case kExtProcessCallSetData:
			set(p->tidx,p->flds,p->srid);
			break;
		case kExtProcessCallWithXMLTree:
			return(parse());
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFExp::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFExp::wd_event(EventRef evt, WindowRef wd){
	_bTrace_("bXMapFExp::wd_event",true);
bool		b=true;
HICommand	cmd;
UInt32		clss;	
			
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				reset(true);
				DataBrowserClose(get_control(kXMapFExpSignature,kXMapFExpLeftListID));
				DataBrowserClose(get_control(kXMapFExpSignature,kXMapFExpRightListID));
				write_p();
				break;
			case kHICommandCancel:
				reset(false);
				DataBrowserClose(get_control(kXMapFExpSignature,kXMapFExpLeftListID));
				DataBrowserClose(get_control(kXMapFExpSignature,kXMapFExpRightListID));
				break;
			
			case kXMapFExpPopTypeCmd:
				type_action();
				break;
			case kXMapFExpAddCmd:
				add_action();
				break;
			case kXMapFExpInsertCmd:
				insert_action();
				break;
			case kXMapFExpAllCmd:
				addall_action();
				break;
			case kXMapFExpRemoveCmd:
				rmv_action();
				break;

			case kXMapFExpProjCmd:
				proj_action();
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
// ------------
void bXMapFExp::wd_init(WindowRef wd){
_bTrace_("bXMapFExp::wd_init",true);

ControlRef	c;
type_p*		p;

	_arr.reset();
	for(_tp=1;_tp<=_gapp->typesMgr()->count();_tp++){
_tm_("type n "+_tp);

		p=new type_p(_gapp->typesMgr()->get(_tp));
		_arr.add(&p);
		p->load(_gapp);
	}
_tm_("LOOP PASSEE");
	
	_tp=1;
		
	c=get_control(kXMapFExpSignature,kXMapFExpPopTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,_tp);

	c=get_control(kXMapFExpSignature,kXMapFExpLeftListID);
	DataBrowserInit(c,left_handler,left_notifier,(long)this);
	populate_left();
	
	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
	DataBrowserInit(c,right_handler,right_notifier,(long)this);
	populate_right();

char	name[512];
    GetProjName(_srid,name);
    c=get_control(kXMapFExpSignature,kXMapFExpProjNameID);
    SetTextControlValue(c,name);
    if(_gapp->document()->srid()==-1){
        DisableControl(c);
        c=get_control(kXMapFExpSignature,kXMapFExpProjID);
        DisableControl(c);
    }
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapFExp::left_handler(	ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){
CFStringRef			name;
char				cname[256];
bXMapFExp*			editor=(bXMapFExp*)GetControlReference(browser);
bGenericType*		tp=_gapp->typesMgr()->get(editor->_tp);
bStdBaseAccessor*	fields=tp->fields();
	
	switch(property){
		case kXMapFExpDBProperty:
			if(!changeValue){
				fields->get_name(fields->get_index(itemID),cname);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		case kDataBrowserItemIsContainerProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// Notifier
// -----------
void bXMapFExp::left_notifier(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
    switch(msg){  
        case kDataBrowserItemAdded:
			break;
        case kDataBrowserItemRemoved:
            break;
        case kDataBrowserContainerOpened:
            break;
        case kDataBrowserContainerClosing:
            break;
        case kDataBrowserContainerClosed:
			break;
        case kDataBrowserItemSelected:
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// Handler
// -----------
OSStatus bXMapFExp::right_handler(	ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){

CFStringRef			name;
char				cname[256];
bXMapFExp*			editor=(bXMapFExp*)GetControlReference(browser);
bGenericType*		tp=_gapp->typesMgr()->get(editor->_tp);
bStdBaseAccessor*	fields=tp->fields();
	
	switch(property){
		case kXMapFExpDBProperty:
			if(!changeValue){
				fields->get_name(fields->get_index(itemID),cname);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		case kDataBrowserItemIsContainerProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// Notifier
// -----------
void bXMapFExp::right_notifier(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
    switch(msg){  
        case kDataBrowserItemAdded:
			break;
        case kDataBrowserItemRemoved:
            break;
        case kDataBrowserContainerOpened:
            break;
        case kDataBrowserContainerClosing:
            break;
        case kDataBrowserContainerClosed:
			break;
        case kDataBrowserItemSelected:
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::populate_left(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapFExpSignature,kXMapFExpLeftListID);
bStdBaseAccessor*	fields=(_gapp->typesMgr()->get(_tp))->fields();
	
	for(int i=kOBJ_Name_;i<=fields->count();i++){
		itemId=fields->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapFExpDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::populate_right(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
type_p*				p;
fexp_field			fld;
	
	_arr.get(_tp,&p);
	for(int i=1;i<=p->_flds.count();i++){
		p->_flds.get(i,&fld);
		itemId=fld.fid;
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapFExpDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::db_resets(){
ControlRef			c;

	c=get_control(kXMapFExpSignature,kXMapFExpLeftListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapFExpDBProperty);
	SetDataBrowserScrollPosition(c,0,0);
	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapFExpDBProperty);
	SetDataBrowserScrollPosition(c,0,0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::type_action(){
ControlRef	c=get_control(kXMapFExpSignature,kXMapFExpPopTypeID);

	report();
	db_resets();
//	c=get_control(kXMapFExpSignature,kXMapFExpPopTypeID);
	_tp=GetControl32BitValue(c);
	populate_left();
	populate_right();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::add_action(){
ControlRef			c=get_control(kXMapFExpSignature,kXMapFExpLeftListID);
bArray				arr(sizeof(DataBrowserItemID));
DataBrowserItemID	item;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_field,&arr);
	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
	for(int i=arr.count();i>0;i--){
		arr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapFExpDBProperty);
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::insert_action(){
ControlRef			c=get_control(kXMapFExpSignature,kXMapFExpLeftListID);
bArray				arrl(sizeof(DataBrowserItemID)),arrr(sizeof(DataBrowserItemID)),rev(sizeof(DataBrowserItemID));
DataBrowserItemID	item;
int					i,j,idx;

	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_add_field,&arrl);
	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_field,&arrr);
	for(i=arrr.count();i>0;i--){
		arrr.get(i,&item);
		rev.add(&item);
	}
	arrr.reset();
	arrr+rev;
	
	for(i=1;i<=arrr.count();i++){
		arrr.get(i,&item);
		if(IsDataBrowserItemSelected(c,item)){
			break;
		}
	}
	idx=i;

	for(j=1;j<=arrl.count();j++){
		arrl.get(j,&item);
		arrr.insert(idx,&item);
	}

	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,0,NULL,kXMapFExpDBProperty);
	for(int i=1;i<=arrr.count();i++){
		arrr.get(i,&item);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapFExpDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::addall_action(){
DataBrowserItemID 	itemId;
ControlRef			c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
bStdBaseAccessor*	fields=(_gapp->typesMgr()->get(_tp))->fields();
	
	for(int i=kOBJ_Name_;i<=fields->count();i++){
		itemId=fields->get_id(i);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapFExpDBProperty);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::rmv_action(){
ControlRef	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemIsSelected,_rmv_field,this);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::proj_action(){
	if(GetAProj(&_srid,true)){
char		name[512];
		GetProjName(_srid,name);
ControlRef	c=get_control(kXMapFExpSignature,kXMapFExpProjNameID);
		SetTextControlValue(c,name);
		write_p();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::_add_field(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void* clientData){
bArray*		arr=(bArray*)clientData;
fexp_field	fld;
	fld.fid=item;
	fld.fname[0]=0;
	arr->add(&fld);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::_rmv_field(	DataBrowserItemID item, 
							DataBrowserItemState state, 
							void* clientData){
bXMapFExp*	editor=(bXMapFExp*)clientData;
ControlRef	c=editor->get_control(kXMapFExpSignature,kXMapFExpRightListID);
	(void)RemoveDataBrowserItems(c,kDataBrowserNoItem,1,&item,kXMapFExpDBProperty);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::reset(bool dump){
type_p*	p;

	if(dump){
		report();
	}
	for(_tp=1;_tp<=_arr.count();_tp++){
		_arr.get(_tp,&p);
		if(dump){
			p->dump(_gapp);
		}
		delete p;
	}
	_arr.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::report(){
ControlRef	c=get_control(kXMapFExpSignature,kXMapFExpRightListID);
type_p*		p;
bArray		rev(sizeof(fexp_field));
fexp_field	fld;
	
	_arr.get(_tp,&p);
	p->_flds.reset();
	(void)ForEachDataBrowserItem(c,kDataBrowserNoItem,false,kDataBrowserItemAnyState,_add_field,&p->_flds);	
	for(int i=p->_flds.count();i>0;i--){
		p->_flds.get(i,&fld);
		rev.add(&fld);
	}
	p->_flds.reset();
	p->_flds+rev;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::get(int tidx, bArray* arr, int* srid){
bGenericType*	tp=_gapp->typesMgr()->get(tidx);
	if(tp){
type_p			p(tp);
		p.load(_gapp);
		arr->reset();
		*arr+p._flds;
	}
	*srid=_srid;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapFExp::set(int tidx, bArray* arr, int srid){
bGenericType*	tp=_gapp->typesMgr()->get(tidx);
	if(tp){	
type_p			p(tp);
		p._flds+(*arr);
		p.dump(_gapp);
	}
	if(_gapp->document()->srid()!=-1){
		_srid=srid;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapFExp::read_p(){
_bTrace_("bXMapFExp::read_p",false);
char	name[_names_length_max_];
	getclassname(name);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,GetSignature(this),name);
char					val[_values_length_max_];
	
	_srid=-1;
	
	if((!b)||(!root)){
		write_p();
		/*b=*/(void)_gapp->document()->readTree(&root,GetSignature(this),name);
		return;
	}
	if((!b)||(!root)){
_te_("((!b)||(!root)) for "+name);
		return;
	}
	elt=_gapp->classMgr()->NthElement(root,1,"int");
	if(!elt){
		return;
	}
	elt->getvalue(val);
	_srid=atoi(val);
			
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapFExp::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	getclassname(name);

	if(!_gapp->document()->writeTree(root,GetSignature(this),name)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapFExp::make_p(){
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

	add_idesc(arr,1,"int",_srid);

bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapFExp::parse(){
bGenericXMLBaseElement*	elt;
bGenericXMLBaseElement*	chld;
char					val[_values_length_max_];	
bGenericType*			tp;
bArray					arr(sizeof(int));
int						nb;
fexp_field				fld;
	
	elt=_gapp->classMgr()->NthElement(this,1,"int");
	if(elt){
		elt->getvalue(val);
		_srid=atoi(val);
		write_p();
	}
	
	for(int i=1;;i++){
		elt=_gapp->classMgr()->NthElement(this,i,"container");
		if(!elt){
			break;
		}
		
		chld=_gapp->classMgr()->NthElement(elt,1,"int");
		if(!chld){
			return(false);
		}
		chld->getvalue(val);
		tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
		if(!tp){
			return(false);
		}
		
		nb=_gapp->classMgr()->CountElements(elt,"name");
		arr.reset();
		for(int j=1;j<=nb;j++){
			chld=_gapp->classMgr()->NthElement(elt,j,"name");
			chld->getvalue(val);
			fld.fid=tp->fields()->get_id(tp->fields()->get_index(val));
			if(fld.fid==0){
				return(false);
			}
			fld.fname[0]=0;
			chld=_gapp->classMgr()->NthElement(elt,j,"string");
			if(chld){
				chld->getvalue(fld.fname);
			}
			arr.add(&fld);
		}
		
		
		set(tp->index(),&arr,_srid);
	}
	return(true);
}

