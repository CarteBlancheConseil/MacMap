//----------------------------------------------------------------------------
// File : bXMapCalcEditor.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, MapCalculator editor
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapCalcEditor.h"
#include <mox_intf/CGUtils.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bSelectSolver.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/endian.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCalcEditor	::bXMapCalcEditor(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("calceditor");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCalcEditor::~bXMapCalcEditor(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapCalcEditor::create(bGenericXMLBaseElement* elt){
	return(new bXMapCalcEditor(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapCalcEditor::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
	_kind=1;
	_type=1;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapCalcEditor::close(){
	bStdXMapWithModalIntf::close();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCalcEditor::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCalcEditor::test(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcEditor::edit(void* prm){
	_new=true;
	_root=NULL;
	_name[0]=0;
	_fknd=0;
	_fsgn=0;
	if(prm){
		if(!parse((bGenericXMLBaseElement*)prm)){
			NSBeep();
			return(true);
		}
	}
	return(bStdXMapWithModalIntf::edit(prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCalcEditor::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss=GetEventClass(evt);	
					
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				if(!save_action()){
					return(false);
				}
				break;
			case kHICommandCancel:
				break;
				
			case kXMapCalcEditorPopupKindCmd:
				kind_action();
				break;
			case kXMapCalcEditorPopupItemCmd:
				type_action();
				break;
			case kXMapCalcEditorClearCmd:
				clear_action();
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
void bXMapCalcEditor::wd_init(WindowRef wd){
ControlRef	c;

	_kind=kXMapCalcEditorKindOperator;
	_type=1;
	
	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupKindID);
	SetControl32BitValue(c,_kind);
	kind_action();
	
	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorListID);
	DataBrowserInit(c,item_handler,item_notifier,(long)this);
	populate_browser();
	
	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,kBaseNoKind,_type);
	
	if(!_new){
		c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorNameID);
		SetTextControlValue(c,_name);
		c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupReturnedKindID);
		switch(_fknd){
			case _char:
				SetControl32BitValue(c,1);
				break;
			case _bool:
				SetControl32BitValue(c,2);
				break;
			case _int:
				SetControl32BitValue(c,3);
				break;
			case _double:
				SetControl32BitValue(c,4);
				break;
			case _date:
				SetControl32BitValue(c,5);
				break;
			case _time:
				SetControl32BitValue(c,6);
				break;
		}

	}
	
// Preview
HIViewID		vid={kXMapCalcEditorSignature,kXMapCalcEditorViewID};
EventTypeSpec	spec[]={	{kEventClassControl,kEventControlDraw},
							{kEventClassControl,kEventControlClick},
							{kEventClassControl,kEventControlDragWithin}	};
HIViewRef		vw;
	HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
	InstallEventHandler(GetControlEventTarget(vw),
						view_event_proc,
						GetEventTypeCount(spec),
						spec,
						(void*)this,
						NULL);

CGPoint	cgp={-10,-10};
	bXMapCalcCell::set_gorigin(cgp);
}

// ---------------------------------------------------------------------------
// Handler
// -----------
pascal OSStatus bXMapCalcEditor::item_handler(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue){
CFStringRef				name;
char					cname[256]="";
bXMapCalcEditor*		editor=(bXMapCalcEditor*)GetControlReference(browser);
bArray					arr(sizeof(bGenericXMLBaseElement*));
bGenericMacMapApp*		gapp=(bGenericMacMapApp*)editor->getapp();
bGenericType*			tp;
bGenericXMLBaseElement*	elt;
	
	switch(property){
		case kXMapCalcEditorListProperty:
			if(!changeValue){
				switch(editor->_kind){
					case kXMapCalcEditorKindOperator:
						gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassOperator);
						if(arr.get(itemID,&elt)){
							elt->getclassname(cname);
						}
						break;
					case kXMapCalcEditorKindIterator:
						gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassIterator);
						if(arr.get(itemID,&elt)){
							elt->getclassname(cname);
						}
						break;
					case kXMapCalcEditorKindFunction:
						gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassFunction);
						if(arr.get(itemID,&elt)){
							elt->getclassname(cname);
						}
						break;
					case kXMapCalcEditorKindConstant:
						gapp->varMgr()->ext_name(itemID,cname);
						break;
					case kXMapCalcEditorKindField:
						tp=gapp->typesMgr()->get(editor->_type);
						if(tp){
							tp->fields()->get_name(itemID+kOBJ_Vertices_,cname);
						}
						break;
					case kXMapCalcEditorKindCalc:
						gapp->calcMgr()->ext_name(itemID,cname);
						break;
					case kXMapCalcEditorKindMacro:
						gapp->macroMgr()->ext_name(itemID,cname);
						break;

				}
				
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
void bXMapCalcEditor::item_notifier(ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapCalcEditor*	editor=(bXMapCalcEditor*)GetControlReference(browser);
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
			editor->list_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->list_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::populate_browser(){
ControlRef				c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorListID);
DataBrowserPropertyID	pid=kXMapCalcEditorListProperty;
bArray					arr(sizeof(bGenericXMLBaseElement*));
bGenericType*			tp;

	DataBrowserReset(c,1,&pid);
	switch(_kind){
		case kXMapCalcEditorKindOperator:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassOperator);
			AddDataBrowserItems(c,kDataBrowserNoItem,arr.count(),NULL,kXMapCalcEditorListProperty);
			break;
		case kXMapCalcEditorKindIterator:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassIterator);
			AddDataBrowserItems(c,kDataBrowserNoItem,arr.count(),NULL,kXMapCalcEditorListProperty);
			break;
		case kXMapCalcEditorKindFunction:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassFunction);
			AddDataBrowserItems(c,kDataBrowserNoItem,arr.count(),NULL,kXMapCalcEditorListProperty);
			break;
		case kXMapCalcEditorKindConstant:
			AddDataBrowserItems(c,kDataBrowserNoItem,_gapp->varMgr()->count(),NULL,kXMapCalcEditorListProperty);
			break;
		case kXMapCalcEditorKindField:
			tp=_gapp->typesMgr()->get(_type);
			if(tp){
				AddDataBrowserItems(c,kDataBrowserNoItem,tp->fields()->count()-kOBJ_Vertices_,NULL,kXMapCalcEditorListProperty);
			}
			break;
		case kXMapCalcEditorKindCalc:
			AddDataBrowserItems(c,kDataBrowserNoItem,_gapp->calcMgr()->count(),NULL,kXMapCalcEditorListProperty);
			break;
		case kXMapCalcEditorKindMacro:
			AddDataBrowserItems(c,kDataBrowserNoItem,_gapp->macroMgr()->count(),NULL,kXMapCalcEditorListProperty);
			break;
	}

}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::kind_action(){
ControlRef	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupKindID);

	_kind=GetControl32BitValue(c);
	switch(_kind){
		case kXMapCalcEditorKindOperator:
		case kXMapCalcEditorKindIterator:
		case kXMapCalcEditorKindFunction:
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorAddID);
			HideControl(c);
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
			HideControl(c);
			break;
		case kXMapCalcEditorKindConstant:
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorAddID);
			HideControl(c);
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
			HideControl(c);
			break;
		case kXMapCalcEditorKindField:
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorAddID);
			HideControl(c);
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
			ShowControl(c);
			break;
		case kXMapCalcEditorKindCalc:
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorAddID);
			HideControl(c);
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
			HideControl(c);
			break;
		case kXMapCalcEditorKindMacro:
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorAddID);
			HideControl(c);
			c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);
			HideControl(c);
			break;
	}
	populate_browser();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::type_action(){
ControlRef	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupItemID);

	_type=GetControl32BitValue(c);
	populate_browser();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::list_action(DataBrowserItemID item){
	if(item==0){
		return;
	}
bXMapCalcCell*			cell;
char					name[256];
bGenericXMLBaseElement*	elt;
bGenericType*			tp;
bArray					arr(sizeof(bGenericXMLBaseElement*));

	switch(_kind){
		case kXMapCalcEditorKindOperator:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassOperator);
			if(arr.get(item,&elt)){
				elt->getclassname(name);
			}
			cell=new bXMapCalcCellOperator(_gapp,name);
			break;
		case kXMapCalcEditorKindIterator:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassIterator);
			if(arr.get(item,&elt)){
				elt->getclassname(name);
			}
			cell=new bXMapCalcCellIterator(_gapp,name);
			break;
		case kXMapCalcEditorKindFunction:
			_gapp->classMgr()->FindXMLInstanceBySubClass(arr,kXMLSubClassFunction);
			if(arr.get(item,&elt)){
				elt->getclassname(name);
			}
			cell=new bXMapCalcCellFunction(_gapp,name);
			break;
		case kXMapCalcEditorKindConstant:
			_gapp->varMgr()->ext_name(item,name);
			cell=new bXMapCalcCellConstant(_gapp,name);
			break;
		case kXMapCalcEditorKindField:
			tp=_gapp->typesMgr()->get(_type);
			if(tp){
				tp->fields()->get_name(item+kOBJ_Vertices_,name);
			}
			cell=new bXMapCalcCellField(_gapp,name,tp);
			break;
		case kXMapCalcEditorKindCalc:
			_gapp->calcMgr()->ext_name(item,name);
			cell=new bXMapCalcCellCalc(_gapp,name);
			break;
		case kXMapCalcEditorKindMacro:
			_gapp->macroMgr()->ext_name(item,name);
			cell=new bXMapCalcCellMacro(_gapp,name);
			break;
	}
	if(_root){
bXMapCalcCell*	slct=bXMapCalcCell::get_select();
		if(!slct){
			delete cell;
			NSBeep();
		}
		else{
			if(slct->parent()){
				slct=slct->parent();
				if(!slct->add(cell)){
					delete cell;
					NSBeep();
				}
				else{
					cell->select(true);
				}
			}
			else if(slct->kind()==_kind){
				cell->set_left(slct->left());
				cell->set_right(slct->right());
				slct->set_left(NULL);
				slct->set_right(NULL);
				delete slct;
				_root=cell;
				_root->select(true);
			}
		}
	}
	else{
		_root=cell;
		_root->select(true);
	}
	update_preview();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::clear_action(){
	if(_root){
bXMapCalcCell*	slct=bXMapCalcCell::get_select();
		if(!slct){
			NSBeep();
		}
		else{
			if(slct->parent()){
bXMapCalcCell*	par=slct->parent();
				if(!par->remove(slct)){
					NSBeep();
				}
			}
			else{
				_root->select(false);
				delete _root;
				_root=NULL;
			}
		}
	}
	else{
		NSBeep();
	}
	update_preview();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcEditor::save_action(){
	if(!_root){
		return(true);
	}
ControlRef	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorNameID);
	GetTextControlValue(c,_name,FILENAME_MAX);
	if(strlen(_name)==0){
		NSBeep();
		return(false);
	}
	
	if(_gapp->calcMgr()->find(_name)){
		if(_new){
			NSBeep();
			return(false);
		}
	}
	
	if(_new){
		_fsgn=UniqueSignature(_gapp);
	}

	c=get_control(kXMapCalcEditorSignature,kXMapCalcEditorPopupReturnedKindID);
	switch(GetControl32BitValue(c)){
		case 1:
			_fknd=_char;
			break;
		case 2:
			_fknd=_bool;
			break;
		case 3:
			_fknd=_int;
			break;
		case 4:
			_fknd=_double;
			break;
		case 5:
			_fknd=_date;
			break;
		case 6:
			_fknd=_time;
			break;
	}


	
bArray	arr(sizeof(xmlelt));
char	csgn[10];
	add_cdesc(arr,0,"ucalc","");
	add_cdesc(arr,1,"version","3.0.0");
	add_cdesc(arr,1,"name",_name);
	sprintf(csgn,"%.4s",&_fsgn);
	add_cdesc(arr,1,"sign",csgn);
	sprintf(csgn,"%.4s",&_fknd);
	add_cdesc(arr,1,"sign",csgn);
	add_idesc(arr,1,"bool",0);
	add_cdesc(arr,1,"formula","");
	
	_root->write(arr,2);
	
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
		NSBeep();
		return(true);
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapCalcEditorCreateCalcMsgID,
				GetSignature(this));
	if(_new){
		if(!_gapp->calcMgr()->add(root)){
			NSBeep();
			return(true);
		}
	}
	else{
		if(!_gapp->calcMgr()->modify(_idx,root)){
			NSBeep();
			return(true);
		}
	}
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapCalcEditor::parse(bGenericXMLBaseElement* root){
_bTrace_("bXMapCalcEditor::parse",true);
bGenericXMLBaseElement*	elt;
char					sgn[10];


	elt=_gapp->classMgr()->NthElement(root,1,"name");
	if(elt){
		elt->getvalue(_name);
	}
	else{
_te_("no name");
		return(false);
	}

	elt=_gapp->classMgr()->NthElement(root,1,"sign");
	if(elt){
		elt->getvalue(sgn);
		_fsgn=*((UInt32*)sgn);
	}
	else{
_te_("no sign 1");
		return(false);
	}
	_idx=RecoverCalc(_gapp,sgn);

	elt=_gapp->classMgr()->NthElement(root,2,"sign");
	if(elt){
		elt->getvalue(sgn);
		_fknd=*((UInt32*)sgn);
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&_fknd,sizeof(UInt32));
#endif
	}
	else{
_te_("no sign 2");
		return(false);
	}
	
	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(elt){
		elt->getvalue(sgn);
		if(atoi(sgn)==1){
_te_("protected calc");	
			return(false);
		}
	}

bArray		arr(sizeof(bXMapCalcCell*));
parse_prm	prm;
	prm.stack=&arr;
	prm.cur=0;
	prm.root=NULL;
	
	elt=_gapp->classMgr()->NthElement(root,1,"formula");
	if(elt){
		elt->dotoall(&prm,0,bXMapCalcCell::parse);
	}
	if(prm.stack->count()>0){
		prm.stack->get(1,&_root);
	}
	
	_new=false;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::update_preview(){
HIViewID	vid={kXMapCalcEditorSignature,kXMapCalcEditorViewID};
HIViewRef	vw;
	HIViewFindByID(HIViewGetRoot(_wd),vid,&vw);
Rect		r;
	GetControlBounds(vw,&r);
	GetWindowPortBounds(_wd,&r);
	InvalWindowRect(_wd,&r);
	Draw1Control(vw);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapCalcEditor::draw(CGContextRef ctx, HIViewRef vw){
	TransformHIViewToCG(ctx,vw);
HIRect			hir;
	HIViewGetBounds(vw,&hir);
	CGContextSetRGBFillColor(ctx,1,1,1,1);
	CGContextFillRect(ctx,hir);

CGPoint		o;
	o.y=hir.origin.y+hir.size.height/2.0;
	o.x=5;
	if(_root){
		_root->draw(ctx,o);
	}
	CGContextSetRGBStrokeColor(ctx,0.05,0.05,0.05,1);
	CGContextStrokeRect(ctx,hir);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapCalcEditor::view_event_proc(	EventHandlerCallRef hdlr,
											EventRef evt,
											void* udt){
_bTrace_("bXMapCalcEditor::view_event_proc",false);

bXMapCalcEditor*	editor=(bXMapCalcEditor*)udt;
CGContextRef		ctx=NULL;
OSStatus			status=eventNotHandledErr;
HIViewRef			vw;
UInt32				cnt,knd=GetEventKind(evt);
HIPoint				cgp;

	if(knd==kEventControlDraw){
		status=GetEventParameter(	evt,
									kEventParamCGContextRef,
									typeCGContextRef,
									NULL,
									sizeof(CGContextRef),
									NULL,
									&ctx);
		if(status){
			return(status);
		}
		status=GetEventParameter(	evt,
									kEventParamDirectObject,
									typeControlRef,
									NULL,
									sizeof(HIViewRef),
									NULL,
									&vw);
		editor->draw(ctx,vw);
	}
	else if(knd==kEventControlClick){
		status=GetEventParameter(	evt,
									kEventParamDirectObject,
									typeControlRef,
									NULL,
									sizeof(HIViewRef),
									NULL,
									&vw);
		if(status){
_te_("GetEventParameter returns "+status);
			return(status);
		}
		
		TransformHIViewToCG(ctx,vw);

		status=GetEventParameter(	evt,
									kEventParamWindowMouseLocation,
									typeHIPoint,
									NULL,
									sizeof(HIPoint),
									NULL,
									&cgp);
		if(status){
_te_("GetEventParameter returns "+status);
			return(status);
		}
		status=GetEventParameter(	evt,
									kEventParamClickCount,
									typeUInt32,
									NULL,
									sizeof(UInt32),
									NULL,
									&cnt);
		
_tm_(cgp.x+":"+cgp.y);

//		CGPointFlipWithPort(GetWindowPort(editor->_wd),&cgp);
//		cgp.y+=46;
		
HIRect			hir;
		HIViewGetFrame(vw,&hir);
//_tm_("hir=%.0f,%.0f",hir.origin.x,hir.origin.y);
		
_tm_(hir.origin.x+":"+hir.origin.y+":"+hir.size.width+":"+hir.size.height);
	
		cgp.y=422-cgp.y;
		
		cgp.x-=hir.origin.x;
		cgp.y-=hir.origin.y;
		
#warning !!! HIView Point conversion !!!		
		cgp.x-=20;
//		cgp.y=cgp.y-60;
		cgp.y=cgp.y-14;
		
_tm_("tranform : "+cgp.x+":"+cgp.y);
		
//_tm_("new cgp=%.0f,%.0f",cgp.x,cgp.y);
		if(editor->_root){
			if(cnt>1){
				HIViewGetBounds(vw,&hir);
CGPoint			o;
				o.y=hir.origin.y+hir.size.height/2.0;
				o.x=hir.origin.x+hir.size.width/2.0;
CGPoint			go=bXMapCalcCell::gorigin();
				go.x=go.x+(cgp.x-o.x);
				go.y=go.y+(cgp.y-o.y);
				bXMapCalcCell::set_gorigin(go);
				editor->update_preview();
			}
			else{
				editor->_root->click(cgp);
				editor->update_preview();
			}
		}
	}
	else if(knd==kEventControlDragWithin){
_te_("kEventControlDragWithin");
	}
	return(status);
}
