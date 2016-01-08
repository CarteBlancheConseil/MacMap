//----------------------------------------------------------------------------
// File : bXMapTopoCheck.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, network topology check
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
// 16/11/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapTopoCheck.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/Type_Utils.h>
#include <Cocoa/Cocoa.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct get_prm{
	bArray*		arr;
}get_prm;

typedef struct mod_prm{
	ivertices*			vxs;
	bGenericGeoElement*	o;
}mod_prm;

// ---------------------------------------------------------------------------
// 
// ------------
static int get_objects(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
get_prm*			p=(get_prm*)prm;
/*ivertices*			vxs;
	geo->getVertices(&vxs);
	for(int i=0;i<p->vxs->nv;i++){	
		if(ivx2_exists(vxs,&p->vxs->vx.vx2[i])){*/
			p->arr->add(&geo);
			/*break;
		}
	}*/
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapTopoCheck	::bXMapTopoCheck(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_types(sizeof(bGenericType*)){
	setclassname("topocheck");
	set_flags(kXMapNeedEvents);
	_prm.types=&_types;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapTopoCheck::~bXMapTopoCheck(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapTopoCheck::create(bGenericXMLBaseElement* elt){
	return(new bXMapTopoCheck(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTopoCheck::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
bGenericType*	tp;
	for(int	i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		_types.add(&tp);
	}
	_prm.activated=false;
	read_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTopoCheck::close(){
	bStdXMapWithModalIntf::close();
	write_p();
	_types.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapTopoCheck::event(EventRef evt){
UInt32	clss=GetEventClass(evt);

	if(clss==kEventClassMacMap){
UInt32	kind=GetEventKind(evt);
		switch(kind){
			case kEventMacMapDataBase:
				check_events();
				break;
			default:
				break;
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTopoCheck::process(int msg, void* prm){
_bTrace_("bXMapTopoCheck::process",true);
	switch(msg){
		case kExtProcessCallFromIntf:
			break;
		case kExtProcessCallWithParams:
			break;
		case kExtProcessCallWithXMLTree:
			break;
		case kExtProcessCallGetData:
_tm_("kExtProcessCallGetData "+_prm.activated);
			*((topocheck_prm*)prm)=_prm;
			break;
		case kExtProcessCallSetData:
			_prm=*((topocheck_prm*)prm);
_tm_("kExtProcessCallSetData "+_prm.activated);
			break;
		default:
			return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTopoCheck::test(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapTopoCheck::wd_event(EventRef evt, WindowRef wd){
bool			b=true;
HICommand		cmd;
UInt32			clss=GetEventClass(evt);
ControlRef		c;
bGenericType*	tp;

	if(clss==kEventClassCommand){
		GetEventParameter(	evt,
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				c=get_control(kXMapTopoCheckSignature,kXMapTopoCheckPopTypeID);
				for(int i=1;i<=CountPopupControlMenuItems(c);i++){
					if(IsPopupControlMenuItemChecked(c,i)){
						tp=_gapp->typesMgr()->get(i);
					}
					else{
						tp=NULL;
					}
					_types.put(i,&tp);
				}
				break;
			case kHICommandCancel:
				break;			
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// Intf init
// ------------
void bXMapTopoCheck::wd_init(WindowRef wd){
ControlRef		c;
bGenericType*	tp;

	c=get_control(kXMapTopoCheckSignature,kXMapTopoCheckActivateID);
	SetControl32BitValue(c,_prm.activated);
    DisableControl(c);
	
	c=get_control(kXMapTopoCheckSignature,kXMapTopoCheckPopTypeID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,0,0);
	for(int i=1;i<=CountPopupControlMenuItems(c);i++){
		_types.get(i,&tp);
		if(tp){
			CheckPopupControlMenuItem(c,i,true);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapTopoCheck::check_events(){
//_bTrace_("bXMapTopoCheck::check_events",true);
bArray*				arr=_gapp->eventMgr()->events();
bGenericEvent*		evt;
bGenericType*		tp;
bGenericGeoElement	*mo,*o;
ivertices			*mvxs,*vxs;
int					i,j;
int					sign=GetSignature(this)/*,esign*/,cln='NtCl';
bArray				objs(sizeof(bGenericGeoElement*));


	for(i=1;i<=arr->count();i++){
		arr->get(i,&evt);
//_tm_("Event "+evt->eid());
//		esign=evt->creator();
		if(	(evt->creator()==sign)	||
			(evt->creator()==cln)	||
			(evt->is_undo())		||
			(evt->is_redo())		){
//_tm_("pass");
			continue;
		}
		if(evt->kind()==kEventKindGeoElement){
			if(!_prm.activated){
				continue;
			}
			if(evt->action()==kEventActionModify){
				for(j=1;j<=evt->elements()->count();j++){
					evt->elements()->get(j,&mo);
					tp=_gapp->typesMgr()->get(mo->getType());
					if(tp->kind()==kBaseKindPoint){
						_types.get(mo->getType(),&tp);
						if(tp==NULL){
							continue;
						}
						o=mo->get_ref();
						o->getVertices(&vxs);
						mo->getVertices(&mvxs);
						if(!mvxs){
							continue;
						}
						if(vxs->nv==mvxs->nv){
							objs.add(&mo);
						}
					}
				}
			}
		}
		else if(evt->kind()==kEventKindTypeElement){
			switch(evt->action()){
				case kEventActionCreate:
					tp=NULL;
					for(j=1;j<=evt->elements()->count();j++){
						_types.add(&tp);
					}
					break;
				case kEventActionDestroy:
					NSBeep();
					/*tp=NULL;
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&tp);
						k=_nodes.search(&tp,ObjRefComp);
						_nodes.add(&tp);
						_edges.add(&tp);
					}*/					
					break;
			}
		}
	}
	if(objs.count()>0){
		process_objects(objs);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapTopoCheck::process_objects(bArray& objects){
//_bTrace_("bXMapTopoCheck::process_objects",true);
int					i,j,k,l,m;
bGenericGeoElement	*a,*b,*t;
ivertices			*vsa,*vsb,*vst;
ivx_rect			vr;
bGenericType*		tp;
bArray				tgt(sizeof(bGenericGeoElement*));
get_prm				prm;
bArray				modifs(sizeof(mod_prm));
mod_prm				mod;

	prm.arr=&tgt;

	for(i=1;i<=objects.count();i++){
		objects.get(i,&a);
		b=a->get_ref();
		a->getVertices(&vsa);
		b->getVertices(&vsb);

		tgt.reset();
		for(j=1;j<=_types.count();j++){
			_types.get(j,&tp);
			if(!tp){
				continue;
			}
			a->getBounds(&vr);
			tgt.reset();
			tp->iterator()->iterate(&vr,&prm,get_objects);
//_tm_("count="+tgt.count());	
			for(k=1;k<=tgt.count();k++){
				tgt.get(k,&t);
				t->getVertices(&vst);
				mod.vxs=NULL;
				for(l=0;l<vsa->nv;l++){
					for(m=0;m<vst->nv;m++){
						if(eq_ivx2(&vsa->vx.vx2[l],&vst->vx.vx2[m])){
							if(mod.vxs==NULL){
								ivs2ivs(vst,&mod.vxs);
							}
							mod.vxs->vx.vx2[m]=vsb->vx.vx2[l];
						}
					}
				}
				if(mod.vxs!=NULL){
					mod.o=t;
					modifs.add(&mod);
				}
			}
		}
	}
	
	if(modifs.count()>0){
		for(i=1;i<=objects.count();i++){
			objects.get(i,&a);
			b=a->get_ref();
			a->getVertices(&vsa);
			b->getVertices(&vsb);
			mod.o=b;
			ivs2ivs(vsb,&mod.vxs);
			modifs.add(&mod);
			b->setVertices(vsa);
		}
		
bEventLog	log(_gapp,
				getbundle(),
				kXMapTopoCheckMessageID,
				GetSignature(this));
		for(int i=1;i<=modifs.count();i++){
			modifs.get(i,&mod);
			mod.o->setVertices(mod.vxs);
			ivs_free(mod.vxs);
		}
		log.close();
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTopoCheck::read_p(){
_bTrace_("bXMapTopoCheck::read_p",true);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,GetSignature(this),kXMapTopoCheckConfig);
char					val[_values_length_max_];
bGenericType*			tp;
int						i,idx;

	if((!b)||(!root)){
_tw_("((!b)||(!root)) for "+kXMapTopoCheckConfig);
		return;
	}

	tp=NULL;
	for(i=1;i<=_gapp->typesMgr()->count();i++){
		_types.put(i,&tp);
	}
	for(i=1;;i++){
		elt=_gapp->classMgr()->NthElement(root,i,"name");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		idx=_gapp->typesMgr()->index(val);
		if(idx){
			tp=_gapp->typesMgr()->get(idx);
			_types.put(idx,&tp);
		}
	}
	elt=_gapp->classMgr()->NthElement(root,1,"bool");
	if(elt){
		elt->getvalue(val);
		_prm.activated=atoi(val);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapTopoCheck::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
	if(!_gapp->document()->writeTree(root,GetSignature(this),kXMapTopoCheckConfig)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapTopoCheck::make_p(){
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

bGenericType*	tp;
char			nm[256];
int				i;

	for(i=1;i<=_types.count();i++){
		tp=NULL;
		_types.get(i,&tp);
		if(tp){
			tp->name(nm);
			add_cdesc(arr,1,"name",nm);
		}
	}
	add_idesc(arr,1,"bool",_prm.activated);

bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

