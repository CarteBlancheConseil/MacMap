//----------------------------------------------------------------------------
// File : bXMapPrefs.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, preferences
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
// 18/06/2007 creation. Les Prefs du 18 juin !
//----------------------------------------------------------------------------

#include "bXMapPrefs.h"
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/xmldesc_utils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bStdProj.h>
#include <MacMapSuite/C_Utils.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPrefs::bXMapPrefs(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("prefs");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPrefs::~bXMapPrefs(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPrefs::create(bGenericXMLBaseElement* elt){
	return(new bXMapPrefs(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapPrefs::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tbl=NULL;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPrefs::process(int msg, void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPrefs::test(void* prm){
	return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPrefs::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
ControlRef			c;
char				str[256];
int					bf;
float				clr[4];
HIViewID			vid={kXMapPrefsSignature,kXMapPrefsBkgColorID};
HIViewRef			vw;
d2dvertex			origin={0,0};

	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				c=get_control(kXMapPrefsSignature,kXMapPrefsPopupUnitID);
				_gapp->distMgr()->set_current(GetControl32BitValue(c));

				c=get_control(kXMapPrefsSignature,kXMapPrefsDigitsID);
				GetTextControlValue(c,str,sizeof(str)-1);
				bf=atoi(str);
				if(bf<0||bf>32){	
					bf=6;
				}
				_gapp->document()->set_dist_pref_digits(bf);
				
				if(_flg){
					c=get_control(kXMapPrefsSignature,kXMapPrefsSRIDID);
					GetTextControlValue(c,str,sizeof(str)-1);
					bf=atoi(str);
					if(bf>0){	
						_gapp->document()->set_srid(bf);
					}
				}
				
				c=get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
				DataBrowserClose(c);
				if(_tbl){
					wtbl_free(_tbl);
				}
				
				HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
				MMColorWellGetRGBColor(vw,clr);
				put_color(clr);
				_gapp->layersMgr()->BackgroundChanged();
				
				c=get_control(kXMapPrefsSignature,kXMapPrefsSaveObjStateID);
				_gapp->document()->set_save_objstate(GetControl32BitValue(c));
				
				str[0]=0;
				c=get_control(kXMapPrefsSignature,kXMapPrefsOriginXStateID);
				GetTextControlValue(c,str,sizeof(str)-1);
				origin.x=matof(str);
				str[0]=0;
				c=get_control(kXMapPrefsSignature,kXMapPrefsOriginYStateID);
				GetTextControlValue(c,str,sizeof(str)-1);
				origin.y=matof(str);
				_gapp->document()->set_origin(origin);
				break;
			case kHICommandCancel:
				c=get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
				DataBrowserClose(c);				
				if(_tbl){
					wtbl_free(_tbl);
				}
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
void bXMapPrefs::wd_init(WindowRef wd){
ControlRef	c;
char		str[256];

	_tbl=NULL;
	_rpt=true;
	_lsrid=_gapp->document()->srid();
	
	init_projs();
	
//	c=get_control(kXMapPrefsSignature,kXMapPrefsPopupUnitID);
	populate_units();	

	c=get_control(kXMapPrefsSignature,kXMapPrefsDigitsID);
	SetTextControlFilter(c,uint_filter);
	sprintf(str,"%d",_gapp->document()->dist_pref_digits());
	SetTextControlValue(c,str);

	c=get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
	DataBrowserInit(c,proj_handler,proj_notifier,(long)this);
	populate_projs();	
		
	c=get_control(kXMapPrefsSignature,kXMapPrefsSRIDID);
	SetTextControlFilter(c,int_filter);
	sprintf(str,"%d",_lsrid);
	SetTextControlValue(c,str);
const EventTypeSpec	kControlEvents[]={{kEventClassControl,kEventControlDraw}};
	InstallControlEventHandler(c,ctrl_evt_hdlr,GetEventTypeCount(kControlEvents),kControlEvents,(void*)this,NULL);

	_flg=(_gapp->document()->srid()==-1);
	c=get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
	SetControlAbility(c,_flg);
	c=get_control(kXMapPrefsSignature,kXMapPrefsSRIDLegID);
	SetControlAbility(c,_flg);
	c=get_control(kXMapPrefsSignature,kXMapPrefsSRIDID);
	SetControlAbility(c,_flg);

HIViewID	vid={kXMapPrefsSignature,kXMapPrefsBkgColorID};
HIViewRef	vw;
float		clr[4];
	HIViewFindByID(HIViewGetRoot(wd),vid,&vw);
	CreateMMColorWellWithHIView(vw);
	get_color(clr);
	MMColorWellSetRGBColor(vw,clr);
	
	c=get_control(kXMapPrefsSignature,kXMapPrefsSaveObjStateID);
	SetControl32BitValue(c,_gapp->document()->get_save_objstate());

d2dvertex	origin=_gapp->document()->get_origin();

	c=get_control(kXMapPrefsSignature,kXMapPrefsOriginXStateID);
	SetTextControlFilter(c,float_filter);
	sprintf(str,"%.*f",_gapp->document()->dist_pref_digits(),origin.x);
	SetTextControlValue(c,str);
	c=get_control(kXMapPrefsSignature,kXMapPrefsOriginYStateID);
	SetTextControlFilter(c,float_filter);
	sprintf(str,"%.*f",_gapp->document()->dist_pref_digits(),origin.y);
	SetTextControlValue(c,str);
	
	c=get_control(kXMapPrefsSignature,kXMapPrefsResoInfoID);
bGenericUnit*	u=_gapp->document()->dist_resolution_unit();
char			un[256];
	u->short_name(un);
	sprintf(str,"%.6f %s",_gapp->document()->dist_resolution(),un);
	SetTextControlValue(c,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::populate_units(){
ControlHandle		c=get_control(kXMapPrefsSignature,kXMapPrefsPopupUnitID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithDistUnits(_gapp,c,_gapp->distMgr()->get_current());
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::populate_projs(){
ControlRef			c=get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
int					srid;
DataBrowserItemID	cur=0;
char				prm[1024];

	for(DataBrowserItemID itemId=1;itemId<=(DataBrowserItemID)_tbl->CountRecords();itemId++){
		_tbl->ReadVal(itemId,3,prm);
bStdProj	prj(prm);
		if(prj.is_latlong()){
			continue;
		}
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapPrefsEPSGProperty);
		AddDataBrowserItems(c,kDataBrowserNoItem,1,&itemId,kXMapPrefsProjProperty);
		_tbl->ReadVal(itemId,1,&srid);
		if(srid==_lsrid){
			cur=itemId;
		}
	}
	if(cur!=0){
		SetDataBrowserSelectedItems(c,1,&cur,kDataBrowserItemsAssign);
		RevealDataBrowserItem(c,cur,kXMapPrefsEPSGProperty,kDataBrowserRevealOnly);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapPrefs::proj_handler(	ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){
CFStringRef			name;
int					srid;
char				cname[1024];
bXMapPrefs*			editor=(bXMapPrefs*)GetControlReference(browser);
	
	switch(property){
		case kXMapPrefsEPSGProperty:
			if(!changeValue){
				editor->_tbl->ReadVal(itemID,1,&srid);
				sprintf(cname,"%d",srid);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kXMapPrefsProjProperty:
			if(!changeValue){
				editor->_tbl->ReadVal(itemID,2,cname);
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
			if(SetDataBrowserItemDataBooleanValue(itemData,false)){
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
void bXMapPrefs::proj_notifier(	ControlRef browser, 
								DataBrowserItemID item, 
								DataBrowserItemNotification msg){
bXMapPrefs*	editor=(bXMapPrefs*)GetControlReference(browser);
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
			editor->proj_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				editor->proj_action(0);
			}
            break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::proj_action(DataBrowserItemID item){
	if(!_rpt){
		return;
	}
char		str[256]="";
ControlRef	c=get_control(kXMapPrefsSignature,kXMapPrefsSRIDID);

	if(item==0){
		SetTextControlValue(c,str);
		_flg=false;
		return;
	}
int			srid;
bool		b;
	_tbl->ReadVal(item,1,&srid);
bStdProj	p(srid,&b);
	if(!b){
		SetTextControlValue(c,str);
		NSBeep();
		_flg=false;
		return;
	}
	if(p.is_latlong()){
		SetTextControlValue(c,str);
		NSBeep();
		_flg=false;
		return;
	}
	_flg=true;
	sprintf(str,"%d",srid);
	SetTextControlValue(c,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapPrefs::ctrl_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
bXMapPrefs*	editor=(bXMapPrefs*)up;

char				str[64];
ControlRef			c=editor->get_control(kXMapPrefsSignature,kXMapPrefsSRIDID);
	GetTextControlValue(c,str,sizeof(str)-1);
int					srid=atoi(str);
	if(editor->_lsrid==srid){
		return(eventNotHandledErr);
	}
	editor->_lsrid=srid;
bool				b;
DataBrowserItemID	item;
bStdProj			p(srid,&b);

	c=editor->get_control(kXMapPrefsSignature,kXMapPrefsProjListID);
	editor->_flg=false;
	editor->_rpt=false;
	if(!b){
		item=DataBrowserSelectedItem(c);
		SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);		
		editor->_rpt=true;
		return(eventNotHandledErr);
	}
	if(p.is_latlong()){
		item=DataBrowserSelectedItem(c);
		SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);		
		editor->_rpt=true;
		NSBeep();
		return(eventNotHandledErr);
	}
	editor->_flg=true;
	for(item=1;item<=editor->_tbl->CountRecords();item++){
		editor->_tbl->ReadVal(item,1,&srid);
		if(editor->_lsrid==srid){
			SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsAssign);
			RevealDataBrowserItem(c,item,kXMapPrefsEPSGProperty,kDataBrowserRevealOnly);
			editor->_rpt=true;
			return(eventNotHandledErr);
		}
	}
	item=DataBrowserSelectedItem(c);
	SetDataBrowserSelectedItems(c,1,&item,kDataBrowserItemsRemove);
	editor->_rpt=true;
	editor->_flg=false;
	return(eventNotHandledErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::init_projs(){
_bTrace_("bXMapPrefs::init_projs()",false);
	_tbl=wprj_gettable();
	if(_tbl==NULL){
_te_("GetProjTable failed");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::get_color(float* color){
_bTrace_("bXMapPrefs::get_color",true);
bGenericXMLBaseElement	*root,*elt;
int						i;
	for(i=1;i<=4;i++){
		color[i-1]=1;
	}
	if(!_gapp->document()->readTree(&root,kBackgroundSignature,kBackgroundConfigName)){
_te_("param not found");
		return;
	}
char	val[_values_length_max_];
	for(i=1;i<=4;i++){
		elt=_gapp->classMgr()->NthElement(root,i,"float");
		if(elt){
			elt->getvalue(val);
			color[i-1]=matof(val);
		}
		else{
			break;
		}
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapPrefs::put_color(float* color){
bGenericXMLBaseElement	*root;
bArray					arr(sizeof(xmlelt));

	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	for(int i=0;i<4;i++){
		add_ddesc(arr,1,"float",color[i],2);
	}
	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!_gapp->document()->writeTree(root,kBackgroundSignature,kBackgroundConfigName)){
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

