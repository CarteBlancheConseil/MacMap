//----------------------------------------------------------------------------
// File : bXMapAssociation.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, concatenate geometries into the first object of selection
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
// 16/12/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapAssociation.h"
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct check_dist_prm{
	bArray&		res;
	double		dmax;
	i2dvertex	vx;
	ivertices*	vxs;
}check_dist_prm;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapAssociation	::bXMapAssociation(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("association");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapAssociation::~bXMapAssociation(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapAssociation::create(bGenericXMLBaseElement* elt){
	return(new bXMapAssociation(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAssociation::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm.dmax=0;
	_prm.kill=true;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(associate());
			break;
		case kExtProcessCallWithParams:{
association_prm* p=(association_prm*)prm;
				return(associate(Measure_d2i(_gapp,p->dmax),p->kill));
			}
			break;
		case kExtProcessCallWithXMLTree:{
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
association_prm			p;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.dmax=matof(val);
				
				elt=getelement(2);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.kill=atoi(val);
				
				return(associate(Measure_d2i(_gapp,p.dmax),p.kill));
			}break;

	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::test(void* prm){
int	k=SelectionIsMonoGender(_gapp);
	return(	(	(k==kBaseKindPolygon)		||
				(k==kBaseKindPolyline)		)	&&
			(_gapp->selMgr()->count()>1)		);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				puts();
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
// 
// ------------
void bXMapAssociation::wd_init(WindowRef wd){
ControlRef		c;
char			val[256];

	c=get_control(kXMapAssociationSignature,kXMapAssociationDistID);	
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.dmax);
	SetTextControlValue(c,val);
	SetKeyboardFocus(wd,c,kControlFocusNextPart);

bGenericUnit*	u=_gapp->distMgr()->get();
	c=get_control(kXMapAssociationSignature,kXMapAssociationUnitID);
	u->short_name(val);
	SetTextControlValue(c,val);
	
	c=get_control(kXMapAssociationSignature,kXMapAssociationKillID);
	SetControlValue(c,_prm.kill);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate(){
_bTrace_("bXMapAssociation::associate()",true);
bEventLog	log(_gapp,
				getbundle(),
				kXMapAssociationMessageID,
				GetSignature(this));
	associate(Measure_d2i(_gapp,_prm.dmax),_prm.kill);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate(double dmax, bool kill){
_bTrace_("bXMapAssociation::associate(double,bool)",true);
	_gapp->layersMgr()->SetObjInvalidation(false);
bool	res=true;
	switch(SelectionIsMonoGender(_gapp)){
		case kBaseKindPoint:
			res=associate_points(dmax,kill);
			break;
		case kBaseKindPolyline:
			res=associate_polylines(dmax,kill);
			break;
		case kBaseKindText:
			res=associate_texts(dmax,kill);
			break;
		case kBaseKindPolygon:
			res=associate_polygons(dmax,kill);
			break;
		case kBaseKindRaster:
			res=associate_rasters(dmax,kill);
			break;
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(res);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate_points(double dmax, bool kill){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate_polylines(double dmax, bool kill){
_bTrace_("bXMapAssociation::associate_polylines",true);
bArray					sel(*(bArray*)_gapp->selMgr()->elements());
bArray					killed(sizeof(bGenericGeoElement*));
bGenericGeoElement		*oa,*ob;
ivx_rect				bounds,vxr;
int						status=noErr;
ivertices				*vxsa,*vxsb,*vxsr,*vxsback;
bGenericGeoIterator*	iter=NULL;
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
int						count=0;

	message_string(kMsgProgress,msg,1);
bProgressWait wt("",msg,true,true,sel.count()*2);
	
	FlushSelection(_gapp);
	FlushContrasts(_gapp);

	sel.get(1,&oa);
	oa->getBounds(&bounds);
	oa->getVertices(&vxsa);
	ivs2ivs(vxsa,&vxsback);
	ivs_strip_offs(vxsa);
	vxsa=vxsback;
	
	sel.rmv(1);
	sel.sort(ObjRefComp);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&ob);
		ob->getBounds(&vxr);
		ivr_union(&bounds,&vxr,&bounds);
	}
	
	for(;;){
gi_init_p	gp={&bounds,&status,SetFlagWrapper,GetFlagWrapper,GetBoundsWrapper};
		iter=(bGenericGeoIterator*)(_gapp->extMgr()->get_component(kComponentGeoIterator)->i_allocate(_kNULLSign_,_gapp,&gp));
		if(!iter){
			status=1;
_te_("gIterator not allocated");
			break;
		}
		for(int i=1;i<=sel.count();i++){
			if(!wt.set_progress(i)){
_te_("user break");
				status=-1;
				break;
			}
			sel.get(i,&ob);
			if(!iter->add(ob)){
_te_("iter->add");
				status=2;
				break;
			}
		}
		if(status){
			break;
		}
		count=sel.count()+1;

// Dans le sens direct
		while(true){
			if(!wt.set_progress(count++)){
				status=-1;
				break;
			}
// Sélection des plus proches
			status=get_lines(vxsa,vxsa->nv-1,dmax,iter,sel,&ob);
			if(status){
_te_("get_lines="+status);
				break;
			}
			if(!ob){
// Fin de traitement
				break;
			}
			if(kill){
				killed.add(&ob);
			}
// Association			
			ob->getVertices(&vxsb);
			vxsr=ivs_associate_as_line(vxsa,vxsb,dmax);
			if(!vxsr){
				break;
			}
			ivs_free(vxsa);
			vxsa=vxsr;
			ivs_strip_offs(vxsa);
		}
		if((status)&&(status<3)){
			break;
		}

// Dans le sens inverse
		while(true){
			if(!wt.set_progress(count++)){
				status=-1;
				break;
			}
// Sélection des plus proches
			status=get_lines(vxsa,0,dmax,iter,sel,&ob);
			if(status){
_te_("get_lines="+status);
				break;
			}
			if(!ob){
// Fin de traitement
				break;
			}
			if(kill){
				killed.add(&ob);
			}
// Association			
			ob->getVertices(&vxsb);
			vxsr=ivs_associate_as_line(vxsb,vxsa,dmax);
			if(!vxsr){
				break;
			}
			ivs_free(vxsa);
			vxsa=vxsr;
			ivs_strip_offs(vxsa);
		}
		
		if((status)&&(status<3)){
			break;
		}
		if(vxsback==vxsa){
_te_("vxsback==vxsa");
			status=4;
			break;
		}
		if(kill){
			killed.add(&oa);
		}
		if(!(_gapp->typesMgr()->get(oa->getType()))->clone_object(oa,&ob)){
_te_("clone_object");
			status=5;
			break;
		}
		oa=ob;
		if(!oa->setVertices(vxsa)){
_te_("setVertices");
			status=6;
			break;
		}
		_gapp->selMgr()->add(oa);
		if(kill){		
			clean(killed);
		}
		break;
	}
			
	if(iter){
		_gapp->extMgr()->get_component(kComponentGeoIterator)->i_free(iter);
	}
	if(vxsa&&(vxsback!=vxsa)){
		ivs_free(vxsa);
	}
	if(sel.count()>0){
		_gapp->cntMgr()->add(&sel);
		if(!status){
			status=3;
		}
	}
	switch(status){
		case -1:
			status=0;
			break;
		case 1:
		case 2:
			b_message_string(kXMapAssociationIndexErr,msg,getbundle(),1);
			break;
		case 3:
			b_message_string(kXMapAssociationObjNotAssociateWarning,msg,getbundle(),1);
			break;
		case 4:
			b_message_string(kXMapAssociationVerticesErr,msg,getbundle(),1);
			break;
		case 5:
			b_message_string(kXMapAssociationCloneObjectErr,msg,getbundle(),1);
			break;
		case 6:
			b_message_string(kXMapAssociationSetVerticesErr,msg,getbundle(),1);
			break;
	}
	
	if(status){
bAlertStop	alrt(msg,"");
	}

	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate_texts(double dmax, bool kill){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate_polygons(double dmax, bool kill){
_bTrace_("bXMapAssociation::associate_polygons",false);
bArray					sel(*(bArray*)_gapp->selMgr()->elements());
bArray					killed(sizeof(bGenericGeoElement*));
bGenericGeoElement		*oa,*ob;
ivx_rect				bounds,vxr;
int						status=noErr;
ivertices				*vxsa,*vxsb,*vxsr,*vxsback;
bGenericGeoIterator*	iter=NULL;
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
int						count=0;

	message_string(kMsgProgress,msg,1);
bProgressWait wt("",msg,true,true,sel.count()*2);
	
	FlushSelection(_gapp);
	FlushContrasts(_gapp);

	sel.get(1,&oa);
	oa->getBounds(&bounds);
	oa->getVertices(&vxsa);
	ivs2ivs(vxsa,&vxsback);
	ivs_strip_offs(vxsa);
	vxsa=vxsback;
	
	sel.rmv(1);
	sel.sort(ObjRefComp);
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&ob);
		ob->getBounds(&vxr);
		ivr_union(&bounds,&vxr,&bounds);
	}
	
	for(;;){
gi_init_p	gp={&bounds,&status,SetFlagWrapper,GetFlagWrapper,GetBoundsWrapper};
		iter=(bGenericGeoIterator*)(_gapp->extMgr()->get_component(kComponentGeoIterator)->i_allocate(_kNULLSign_,_gapp,&gp));
		if(!iter){
			status=1;
_te_("gIterator not allocated");
			break;
		}
		for(int i=1;i<=sel.count();i++){
			if(!wt.set_progress(i)){
_te_("user break");
				status=-1;
				break;
			}
			sel.get(i,&ob);
			if(!iter->add(ob)){
_te_("iter->add");
				status=2;
				break;
			}
		}
		if(status){
			break;
		}
		count=sel.count()+1;

// Dans le sens direct
		while(true){
			if(!wt.set_progress(count++)){
				status=-1;
				break;
			}
// Sélection des plus proches
			status=get_surfs(vxsa,dmax,iter,sel,&ob);
			if(status){
_te_("get_surfs="+status);
				break;
			}
			if(!ob){
// Fin de traitement
				break;
			}
			if(kill){
				killed.add(&ob);
			}
// Association			
			ob->getVertices(&vxsb);
			vxsr=ivs_associate_as_surf(vxsa,vxsb,dmax);
			if(!vxsr){
				break;
			}
			ivs_free(vxsa);
			vxsa=vxsr;
		}
		
		if((status)&&(status<3)){
			break;
		}
		if(vxsback==vxsa){
_te_("vxsback==vxsa");
			status=4;
			break;
		}
		if(kill){
			killed.add(&oa);
		}
		if(!(_gapp->typesMgr()->get(oa->getType()))->clone_object(oa,&ob)){
_te_("clone_object");
			status=5;
			break;
		}
		oa=ob;
		if(!oa->setVertices(vxsa)){
_te_("setVertices");
			status=6;
			break;
		}
		_gapp->selMgr()->add(oa);
		if(kill){		
			clean(killed);
		}
		break;
	}
			
	if(iter){
		_gapp->extMgr()->get_component(kComponentGeoIterator)->i_free(iter);
	}
	if(vxsa&&(vxsback!=vxsa)){
		ivs_free(vxsa);
	}
	if(sel.count()>0){
		_gapp->cntMgr()->add(&sel);
		if(!status){
			status=3;
		}
	}
	switch(status){
		case -1:
			status=0;
			break;
		case 1:
		case 2:
			b_message_string(kXMapAssociationIndexErr,msg,getbundle(),1);
			break;
		case 3:
			b_message_string(kXMapAssociationObjNotAssociateWarning,msg,getbundle(),1);
			break;
		case 4:
			b_message_string(kXMapAssociationVerticesErr,msg,getbundle(),1);
			break;
		case 5:
			b_message_string(kXMapAssociationCloneObjectErr,msg,getbundle(),1);
			break;
		case 6:
			b_message_string(kXMapAssociationSetVerticesErr,msg,getbundle(),1);
			break;
	}
	
	if(status){
bAlertStop	alrt(msg,"");
	}

	return(true);
}
// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociation::associate_rasters(double dmax, bool kill){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAssociation::clean(bArray& arr){
bGenericGeoElement*	o;
bGenericType*		t;
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&o);
		t=_gapp->typesMgr()->get(o->getType());
		t->kill_object(o);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAssociation::puts(){
char	val[256];

	GetTextControlValue(get_control(kXMapAssociationSignature,kXMapAssociationDistID),
						val,
						sizeof(val)-1);
	_prm.dmax=matof(val);
	_prm.kill=GetControlValue(get_control(kXMapAssociationSignature,kXMapAssociationKillID));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociation::get_lines(ivertices* vxsa,
								int loc,
								double dmax,
								bGenericGeoIterator* iter,
								bArray& sel,
								bGenericGeoElement** ob){
ivx_rect			vxr;
int					status=0;
int					idx;
bArray				res(sizeof(bGenericGeoElement*));
check_dist_prm		prm={res,dmax,vxsa->vx.vx2[loc],NULL};

	*ob=NULL;
	for(;;){
		vxr.left=vxr.right=vxsa->vx.vx2[loc].h;
		vxr.top=vxr.bottom=vxsa->vx.vx2[loc].v;
		ivr_inset(&vxr,-round(dmax),-round(dmax));
		iter->iterate(&vxr,&prm,check_line_dist);
// Test de cohérence
		if(res.count()>1){
// Trop d'objets
			status=3;
			break;
		}			
		if(res.count()==0){
// Fin de traitement
			break;
		}
// Retrait du candidat des structures			
		res.get(1,ob);
		idx=sel.search(ob,ObjRefComp);
		sel.rmv(idx);
		if(!iter->rmv(*ob/*,false*/)){
			status=2;
			break;
		}
		break;
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociation::get_surfs(ivertices* vxsa,
								double dmax,
								bGenericGeoIterator* iter,
								bArray& sel,
								bGenericGeoElement** ob){
ivx_rect			vxr;
int					status=0;
int					idx;
bArray				res(sizeof(bGenericGeoElement*));
check_dist_prm		prm={res,dmax,{0,0},vxsa};

	*ob=NULL;
	for(;;){
		ivs_bounds(vxsa,&vxr);
		ivr_inset(&vxr,-round(dmax),-round(dmax));
		iter->iterate(&vxr,&prm,check_surf_dist);
// Test de cohérence
		if(res.count()==0){
// Fin de traitement
			break;
		}
// Retrait du candidat des structures			
		res.get(1,ob);
		idx=sel.search(ob,ObjRefComp);
		sel.rmv(idx);
		if(!iter->rmv(*ob/*,false*/)){
			status=2;
			break;
		}
		break;
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociation::check_line_dist(void *o, void *prm){
//_bTrace_("bXMapAssociation::check_dist",false);
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
check_dist_prm*		p=(check_dist_prm*)prm;
ivertices*			vxsb;
	geo->getVertices(&vxsb);
double				d1=ivx2_dist(&vxsb->vx.vx2[0],&p->vx);
double				d2=ivx2_dist(&vxsb->vx.vx2[vxsb->nv-1],&p->vx);
	if((d1<=p->dmax)&&(d2<=p->dmax)){
//_te_("(d1<=p->dmax)&&(d2<=p->dmax)");
		return(1);
	}
	if((d1<=p->dmax)||(d2<=p->dmax)){
		p->res.add(&geo);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociation::check_surf_dist(void *o, void *prm){
//_bTrace_("bXMapAssociation::check_dist",false);
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
check_dist_prm*		p=(check_dist_prm*)prm;
ivertices*			vxsb;
	geo->getVertices(&vxsb);
double				d=vxs_dist(vxsb,p->vxs);
	if(d<=p->dmax){
		p->res.add(&geo);
		return(1);// arrêter l'itération
	}
	return(0);
}

