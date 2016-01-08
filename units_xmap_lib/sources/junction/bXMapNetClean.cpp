//----------------------------------------------------------------------------
// File : bXMapNetClean.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, network cleaning
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
// 08/11/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapNetClean.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/vx_measure.h>

#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/bStdWait.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct nc_prm{
	bArray*		arr;
	double		d;
	i2dvertex	vx;
}nc_prm;

typedef struct it_prm{
	bGenericType*			tp;
	bGenericGeoIterator*	it;
	int						field;
}it_prm;

typedef struct init_prm{
	int						field;					
	bProgressWait*			wt;
}init_prm;

//bool				to=false;

// ---------------------------------------------------------------------------
// 
// ------------
int		all_objects		(	void *o, 
							void *prm);
int		dst_point		(	void *o, 
							void *prm);
int		dst_edge		(	void *o, 
							void *prm);
int		dst_edge_end	(	void *o, 
							void *prm);
int		init_ctl		(	void *o, 
							void *prm);
int		check_one_node	(	void *o, 
							void *prm);
int		check_one_edge	(	void *o, 
							void *prm);
void	set_net_flag	(	bGenericGeoElement*	o,
							int fld,
							int flg);

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapNetClean	::bXMapNetClean(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_nodes(sizeof(bGenericType*))
				,_edges(sizeof(bGenericType*))
				,_nitr(sizeof(it_prm))
				,_eitr(sizeof(it_prm)){
	setclassname("netclean");
	set_flags(kXMapNeedEvents);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapNetClean::~bXMapNetClean(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapNetClean::create(bGenericXMLBaseElement* elt){
	return(new bXMapNetClean(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
	_cfg_prm.nodes=&_nodes;
	_cfg_prm.edges=&_edges;
	_cfg_prm.tnod=NULL;
	_cfg_prm.stnod=1;
	_cfg_prm.name[0]=0;
	_cfg_prm.tbdg=NULL;
	_cfg_prm.stbdg=2;
	_cfg_prm.dnod=0;
	_cfg_prm.dbdg=0;
	_cfg_prm.autoclean=false;
	
	_act_prm.create_nodes=true;
	_act_prm.cut_edges=true;
	_act_prm.join_on_nodes=true;
	_act_prm.join_on_edges=false;
	_act_prm.check_nodes=true;
	_act_prm.check_edges=true;

bGenericType*	tp;
	for(int	i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->kind()!=kBaseKindPoint){
			tp=NULL;
		}
		_nodes.add(&tp);
		if(_cfg_prm.tnod==NULL){
			_cfg_prm.tnod=tp;
			_cfg_prm.tbdg=tp;
		}
	}
	for(int	i=1;i<=_gapp->typesMgr()->count();i++){
		tp=_gapp->typesMgr()->get(i);
		if(tp->kind()!=kBaseKindPolyline){
			tp=NULL;
		}
		_edges.add(&tp);
	}
	read_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::close(){
	bStdXMapWithModalIntf::close();
	write_p();
	_nodes.reset();
	_edges.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapNetClean::event(EventRef evt){
UInt32			clss=GetEventClass(evt);
UInt32			kind=GetEventKind(evt);

	if(clss==kEventClassMacMap){
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
bool bXMapNetClean::process(int msg, void* prm){
_bTrace_("bXMapNetClean::process",true);
netcleancfg_prm*	cp;
netcleanact_prm*	ap;
bool				b;
	switch(msg){
		case kExtProcessCallFromIntf:
			_silent=false;
			process_network(true);
			break;
		case kExtProcessCallWithParams:
			ap=(netcleanact_prm*)prm;
			_silent=false;
			process_network(false);
			break;
		case kExtProcessCallWithXMLTree:
			_silent=true;
			break;
		case kExtProcessCallGetData:
_tm_("kExtProcessCallGetData");
			cp=(netcleancfg_prm*)prm;
			if(cp->nodes){
				cp->nodes->reset();
				b=(*cp->nodes)+_nodes;
			}
			if(cp->edges){
				cp->edges->reset();
				b=(*cp->edges)+_edges;
			}
			cp->tnod=_cfg_prm.tnod;
			cp->stnod=_cfg_prm.stnod;
			strcpy(cp->name,_cfg_prm.name);
			cp->tbdg=_cfg_prm.tbdg;
			cp->stbdg=_cfg_prm.stbdg;
			cp->dnod=_cfg_prm.dnod;
			cp->dbdg=_cfg_prm.dbdg;
			cp->autoclean=_cfg_prm.autoclean;
			break;
		case kExtProcessCallSetData:
_tm_("kExtProcessCallSetData");
			cp=(netcleancfg_prm*)prm;
			if(cp->nodes){
				_nodes.reset();
				b=_nodes+(*cp->nodes);
			}
			if(cp->edges){
				_edges.reset();
				b=_edges+(*cp->edges);
			}
			_cfg_prm.tnod=cp->tnod;
			_cfg_prm.stnod=cp->stnod;
			strcpy(_cfg_prm.name,cp->name);
			_cfg_prm.tbdg=cp->tbdg;
			_cfg_prm.stbdg=cp->stbdg;
			_cfg_prm.dnod=cp->dnod;
			_cfg_prm.dbdg=cp->dbdg;
			_cfg_prm.autoclean=cp->autoclean;
			write_p();
			break;
		default:
			return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapNetClean::test(void* prm){
	return(true);
}

// ---------------------------------------------------------------------------
// Intf
// ------------
bool bXMapNetClean::wd_event(EventRef evt, WindowRef wd){
bool		b=true;
HICommand	cmd;
UInt32		clss=GetEventClass(evt);
int			val;
ControlRef	c;

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
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsNodeCreateID);
				_act_prm.create_nodes=GetControl32BitValue(c);
				if(!_act_prm.create_nodes){
					_act_prm.cut_edges=false;
				}
				else{
					c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsEdgeCutID);
					_act_prm.cut_edges=GetControl32BitValue(c);
				}
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinID);
				if(GetControl32BitValue(c)){
					c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinPopupID);
					if(GetControl32BitValue(c)==1){
						_act_prm.join_on_nodes=true;
						_act_prm.join_on_edges=false;
					}
					else{
						_act_prm.join_on_nodes=false;
						_act_prm.join_on_edges=true;
					}
				}
				else{
					_act_prm.join_on_nodes=false;
					_act_prm.join_on_edges=false;
				}
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsNodeCheckID);
				_act_prm.check_nodes=GetControl32BitValue(c);
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsEdgeCheckID);
				_act_prm.check_edges=GetControl32BitValue(c);
				break;
			case kHICommandCancel:
				break;
			case kXMapNetCleanOptionsNodeCreateCmd:
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsNodeCreateID);
				val=GetControl32BitValue(c);
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsEdgeCutID);
				SetControlAbility(c,val!=0);
				break;
			case kXMapNetCleanOptionsJoinCmd:
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinID);
				val=GetControl32BitValue(c);
				c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinPopupID);
				SetControlAbility(c,val!=0);
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
void bXMapNetClean::wd_init(WindowRef wd){
ControlRef	c;
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsNodeCreateID);
	SetControl32BitValue(c,_act_prm.create_nodes);
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsEdgeCutID);
	if(!_act_prm.create_nodes){
		_act_prm.cut_edges=false;
		SetControl32BitValue(c,_act_prm.cut_edges);
		DisableControl(c);
	}
	else{
		SetControl32BitValue(c,_act_prm.cut_edges);	
	}
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinPopupID);
	if(_act_prm.join_on_nodes){
		SetControl32BitValue(c,1);
	}
	else{
		SetControl32BitValue(c,2);
	}
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinID);
	if((!_act_prm.join_on_nodes)&&(!_act_prm.join_on_edges)){
		SetControl32BitValue(c,0);
		c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsJoinPopupID);
		DisableControl(c);
	}
	else{
		SetControl32BitValue(c,1);
	}
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsNodeCheckID);
	SetControl32BitValue(c,_act_prm.check_nodes);
	c=get_control(kXMapNetCleanSignature,kXMapNetCleanOptionsEdgeCheckID);
	SetControl32BitValue(c,_act_prm.check_edges);
}

// ---------------------------------------------------------------------------
// Std call
// ------------
void bXMapNetClean::process_network(bool log){
_bTrace_("bXMapNetClean::process_network(bool)",true);
bGenericType*	tp;
bool			badf=false;
char			name[__MESSAGE_STRING_LENGTH_MAX__];
it_prm			iter;
int				fields[_gapp->typesMgr()->count()];
nc_prm			prm;
bArray			nodes(sizeof(bGenericGeoElement*));
bArray			edges(sizeof(bGenericGeoElement*));

	b_message_string(kXMapNetCleanControlField,name,getbundle(),0);
// Liste des itérateurs	
	iter.it=0;
	for(int i=1;i<=_nodes.count();i++){
		_nodes.get(i,&tp);
		if(tp){
			iter.tp=tp;
			iter.it=tp->iterator();
			iter.field=tp->fields()->get_index(name);
			_nitr.add(&iter);
			if(!iter.field&&_act_prm.check_nodes){
				badf=true;
			}
		}
	}
	for(int i=1;i<=_edges.count();i++){
		_edges.get(i,&tp);
		if(tp){
			iter.tp=tp;
			iter.it=tp->iterator();
			iter.field=tp->fields()->get_index(name);
			_eitr.add(&iter);
			if(!iter.field&&_act_prm.check_edges){
				badf=true;
			}
		}
	}

// Champs controle
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		fields[i-1]=0;
	}

// Listes d'objets	
	prm.arr=&nodes;
	for(int i=1;i<=_nitr.count();i++){
		_nitr.get(i,&iter);
		iter.it->iterate(&prm,all_objects);
	}
	prm.arr=&edges;
	for(int i=1;i<=_eitr.count();i++){
		_eitr.get(i,&iter);
		iter.it->iterate(&prm,all_objects);
	}
	
	if(log){
bEventLog	log(_gapp,
				getbundle(),
				kXMapNetCleanMessageID,
				GetSignature(this));
		if(badf){
			b_message_string(kXMapNetCleanAskAddControlField,name,getbundle(),0);
bAlertWarningYes	alrt(name,"",_silent);
			if(!alrt.result()){
				_act_prm.check_edges=false;
				_act_prm.check_nodes=false;		
			}
		}
		init_fields(&_act_prm);
		for(int i=1;i<=_nitr.count();i++){
			_nitr.get(i,&iter);
			fields[iter.tp->index()-1]=iter.field;
		}
		for(int i=1;i<=_eitr.count();i++){
			_eitr.get(i,&iter);
			fields[iter.tp->index()-1]=iter.field;
		}
		process_network(&_act_prm,fields,nodes,edges);
		log.close();
	}
	else{
		if(badf){
			b_message_string(kXMapNetCleanAskAddControlField,name,getbundle(),0);
bAlertWarningYes	alrt(name,"",_silent);
			if(!alrt.result()){
				_act_prm.check_edges=false;
				_act_prm.check_nodes=false;		
			}
		}
		init_fields(&_act_prm);
		for(int i=1;i<=_nitr.count();i++){
			_nitr.get(i,&iter);
			fields[iter.tp->index()-1]=iter.field;
		}
		for(int i=1;i<=_eitr.count();i++){
			_eitr.get(i,&iter);
			fields[iter.tp->index()-1]=iter.field;
		}
		process_network(&_act_prm,fields,nodes,edges);
	}
	_nitr.reset();
	_eitr.reset();
}

// ---------------------------------------------------------------------------
// Event call
// ------------
void bXMapNetClean::process_network(bArray& nodes, bArray& edges){
_bTrace_("bXMapNetClean::process_network(bArray&,bArray&)",true);
bGenericType*	tp;
it_prm			iter;
int				fields[_gapp->typesMgr()->count()];
netcleanact_prm	bkprm;

	bkprm=_act_prm;

	_act_prm.create_nodes=true;
	_act_prm.cut_edges=true;
	_act_prm.join_on_nodes=true;
	_act_prm.join_on_edges=false;
	_act_prm.check_nodes=false;
	_act_prm.check_edges=false;

// Liste des itérateurs	
	for(int i=1;i<=_nodes.count();i++){
		_nodes.get(i,&tp);
		if(tp){
			iter.tp=tp;
			iter.it=tp->iterator();
			iter.field=0;
			_nitr.add(&iter);
		}
	}
	for(int i=1;i<=_edges.count();i++){
		_edges.get(i,&tp);
		if(tp){
			iter.tp=tp;
			iter.it=tp->iterator();
			iter.field=0;
			_eitr.add(&iter);
		}
	}

// Champs controle
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
		fields[i-1]=0;
	}

bEventLog	log(_gapp,
				getbundle(),
				kXMapNetCleanMessageID,
				GetSignature(this));
	process_network(&_act_prm,fields,nodes,edges);
	log.close();
	_nitr.reset();
	_eitr.reset();
	
	_act_prm=bkprm;
}

// ---------------------------------------------------------------------------
// Common call
// ------------
void bXMapNetClean::process_network(netcleanact_prm* prm, 
									int* fields,
									bArray& nodes, 
									bArray& edges){
_bTrace_("bXMapNetClean::process_network(netcleanact_prm,int*,bArray&,bArray&)",true);
	if(_cfg_prm.tnod==NULL){
		return;
	}
	if(	(_cfg_prm.tbdg==_cfg_prm.tnod)		&&
		(_cfg_prm.stbdg==_cfg_prm.stnod)	){
		_cfg_prm.tbdg=NULL;
	}
	if(	(_cfg_prm.stnod<1)															||
		(_cfg_prm.stnod>_cfg_prm.tnod->fields()->count_constraints(kOBJ_SubType_))	){
		if(	(_cfg_prm.tbdg)		&&
			(_cfg_prm.stbdg==1)	){
			return;
		}
		_cfg_prm.stnod=1;
	}

/* 
Inval et Draw pour provoquer un reset des styles
Puis StopDraw derrière pour empécher le dessin
Ceci pour régler le cas ou on supprime un objet 
dans la fonction précédente, qui va rester dans les
index des styles et qui sera effacé définitivement
par la déclaration du eventlog du nettoyage automatique
*/
//	_gapp->mapwd()->inval();
	_gapp->mapIntf()->inval();
//	_gapp->mapwd()->draw();
	_gapp->mapIntf()->draw();
	_gapp->layersMgr()->StopDraw();
/**/

	_gapp->layersMgr()->SetObjInvalidation(false);	
	if(prm->create_nodes){
		(void)create_nodes(edges,nodes,fields,prm->check_nodes);
		if(prm->cut_edges){
			(void)cut_edges(nodes,edges,fields,prm->check_edges);
		}
	}
	if(prm->join_on_nodes){
		(void)join_on_nodes(edges,fields,prm->check_edges);
	}
	else if(prm->join_on_edges){
		(void)join_on_edges(nodes,fields,prm->check_nodes);
	}
	if(prm->check_nodes){
		(void)check_nodes(nodes,fields);	
	}
	if(prm->check_edges){
		(void)check_edges(edges,fields);	
	}
	_gapp->layersMgr()->SetObjInvalidation(true);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapNetClean::check_events(){
//_bTrace_("bXMapNetClean::check_events",true);
bArray*				arr=_gapp->eventMgr()->events();
bGenericEvent*		evt;
bGenericType*		tp;
bGenericGeoElement*	o;
int					i,j;
int					sign=GetSignature(this);
bArray				nods(sizeof(bGenericGeoElement*));
bArray				edgs(sizeof(bGenericGeoElement*));

UInt32				cs;

	for(i=1;i<=arr->count();i++){
		arr->get(i,&evt);
//_tm_("Event "+evt->eid());

		cs=evt->creator();
//_tm_("creator:"+&cs);
		if(	(evt->creator()==sign)	||
			(evt->is_undo())		||
			(evt->is_redo())		){
//_tm_("creator=NetClean, Undo, or Redo");
			continue;
		}
		if(evt->kind()==kEventKindGeoElement){
			if(!_cfg_prm.autoclean){
				continue;
			}
			switch(evt->action()){
				case kEventActionModify:
				case kEventActionCreate:
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&o);
						if(evt->action()==kEventActionModify){
							o=o->get_ref();
						}
						tp=_gapp->typesMgr()->get(o->getType());
						if(tp->kind()==kBaseKindPoint){
							_nodes.get(o->getType(),&tp);
							if(tp==NULL){
								continue;
							}
							if(o->flag1()){
								continue;
							}
							nods.add(&o);
							o->set_flag1(true);
						}
						else if(tp->kind()==kBaseKindPolyline){
							_edges.get(o->getType(),&tp);
							if(tp==NULL){
								continue;
							}
							if(o->flag1()){
								continue;
							}
							edgs.add(&o);
							o->set_flag1(true);
						}	
					}
					break;
			}
		}
		else if(evt->kind()==kEventKindTypeElement){
			switch(evt->action()){
				case kEventActionCreate:
					tp=NULL;
					for(j=1;j<=evt->elements()->count();j++){
						_nodes.add(&tp);
						_edges.add(&tp);
					}
					break;
				case kEventActionDestroy:
					//SysBeep(5);
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
	if((nods.count()+edgs.count())>0){
		for(i=1;i<=nods.count();i++){
			nods.get(i,&o);
			o->set_flag1(false);
		}
		for(i=1;i<=edgs.count();i++){
			edgs.get(i,&o);
			o->set_flag1(false);
		}
		process_network(nods,edgs);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::create_nodes(bArray& loop, bArray& added, int* fields, bool report){
_bTrace_("bXMapNetClean::create_nodes",true);
	if(!_cfg_prm.tnod){
		return(0);
	}
bGenericGeoElement	*oa,*ob,*op;
ivertices			*vsa,*vsb,*vsp;
i2dvertex			vx;
ivx_rect			vxr;
int					i,j,k,l;
bArray				edg(sizeof(bGenericGeoElement*));
bArray				bdg(sizeof(bGenericGeoElement*));
bArray				nod(sizeof(bGenericGeoElement*));
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];


	vsp=ivs_new(_2D_VX,1,0);

	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanNodeCreationMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&oa);
		oa->getVertices(&vsa);

/*if(oa->getID()==1547){
_tm_("1547 reached for oa");
}
if(oa->getID()==1489){
_tm_("1489 reached for oa");
}*/	
		
		get_nodes(&vsa->vx.vx2[0],nod);
		if(nod.count()==0){
			_cfg_prm.tnod->new_object(&op);
			vsp->vx.vx2[0]=vsa->vx.vx2[0];
			op->setVertices(vsp);
			op->setSubType(_cfg_prm.stnod);
			op->setName(_cfg_prm.name);
			if(report){
				set_net_flag(op,fields[_cfg_prm.tnod->index()-1],_kNodeCreated);
			}
			added.add(&op);
		}
		
		get_nodes(&vsa->vx.vx2[vsa->nv-1],nod);
		if(nod.count()==0){
			_cfg_prm.tnod->new_object(&op);
			vsp->vx.vx2[0]=vsa->vx.vx2[vsa->nv-1];
			op->setVertices(vsp);
			op->setSubType(_cfg_prm.stnod);
			op->setName(_cfg_prm.name);
			if(report){
				set_net_flag(op,fields[_cfg_prm.tnod->index()-1],_kNodeCreated);
			}		
			added.add(&op);
		}

		oa->getBounds(&vxr);
		get_edges(&vxr,edg);
		for(j=1;j<=edg.count();j++){
			edg.get(j,&ob);
			if(oa==ob){
				continue;
			}
/*if(ob->getID()==1547){
_tm_("1547 reached for ob with %d",oa->getID());
}
if(ob->getID()==1489){
_tm_("1489 reached for ob with %d",oa->getID());
}*/	
//to=(((ob->getID()==1489)&&(oa->getID()==1547))||((oa->getID()==1489)&&(ob->getID()==1547)));

			ob->getVertices(&vsb);
			for(k=0;k<vsa->nv-1;k++){
				for(l=0;l<vsb->nv-1;l++){
					if(ivx2_sect(	&vsa->vx.vx2[k],
									&vsa->vx.vx2[k+1],
									&vsb->vx.vx2[l],
									&vsb->vx.vx2[l+1])){
						ivx2_get_sect(	&vsa->vx.vx2[k],
										&vsa->vx.vx2[k+1],
										&vsb->vx.vx2[l],
										&vsb->vx.vx2[l+1],
										&vx);
//if(to){
//_tm_("sect at %d;%d",vx.h,vx.v);
//}

						get_bridges(&vx,bdg);
						if(bdg.count()>0){
//if(to){
//_tm_("bridge");
//}
							continue;
						}
						get_nodes(&vx,nod);
						if(nod.count()>0){
//if(to){
//_tm_("node exists %d %d",oa->getID(),ob->getID());
//_tm_("node exists");
//for(int z=1;z<=nod.count();z++){
//	nod.get(z,&op);
//_tm_("%d",op->getID());
//}
//}

							/**/
							nod.get(1,&op);
							added.add(&op);
							/**/
							continue;
						}
						_cfg_prm.tnod->new_object(&op);
						vsp->vx.vx2[0]=vx;
						op->setVertices(vsp);
						op->setSubType(_cfg_prm.stnod);
						op->setName(_cfg_prm.name);
						if(report){
							set_net_flag(op,fields[_cfg_prm.tnod->index()-1],_kNodeCreated);
						}						
						added.add(&op);
					}
				}
			}
		}
	}
	
	ivs_free(vsp);
	
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::cut_edges(bArray& loop, bArray& added, int* fields, bool report){
_bTrace_("bXMapNetClean::cut_edges",true);
bGenericGeoElement	*oa,*ob,*op;
bGenericType		*tp;
ivertices			*vsa,*vsb,*vse,*vsp;
int					i,j,k;
bArray				edg(sizeof(bGenericGeoElement*));
bArray				bdg(sizeof(bGenericGeoElement*));
double				d,r;
i2dvertex			vr;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];

	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanCutEdgesMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());	
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&op);
		op->getVertices(&vsp);
		get_bridges(&vsp->vx.vx2[0],bdg);
		if(bdg.count()>0){
_tm_("bridge");
			continue;
		}
		
		get_edges(&vsp->vx.vx2[0],edg,false);
		for(j=1;j<=edg.count();j++){
			edg.get(j,&oa);
			oa->getVertices(&vsa);
			d=ivx_proj(	&vsp->vx.vx2[0],
						vsa,
						&vr,
						&k,
						&r);
			if(ivs2ivs(vsa,&vse)){
_te_("ivs2ivs failed");
				continue;
			}
			vsa=NULL;
			vsb=NULL;
			vse=ivx2_insert(vse,&vr,k);
			if((k==0)||(k==vse->nv-1)){
				ivs_free(vse);
				continue;
			}
			ivs_split(vse,k,&vsa,&vsb);
			if((!vsa)||(!vsb)){
				ivs_free(vse);
				if(vsa){
					ivs_free(vsa);
				}
				if(vsb){
					ivs_free(vsb);
				}
_tm_("ivs_split failed at "+k);
				continue;
			}
			tp=_gapp->typesMgr()->get(oa->getType());
			if(!tp->clone_object(oa,&ob)){
				ivs_free(vse);
				ivs_free(vsa);
				ivs_free(vsb);
_tm_("clone_object failed");
				break;
			}
			oa->setVertices(vsa);
			ob->setVertices(vsb);
			if(report){
				set_net_flag(oa,fields[tp->index()-1],_kEdgeCreated);
				set_net_flag(ob,fields[tp->index()-1],_kEdgeCreated);
			}
			added.add(&ob);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::join_on_nodes(bArray& loop, int* fields, bool report){
_bTrace_("bXMapNetClean::join_on_nodes",true);
bGenericGeoElement	*oa,*op;
ivertices			*vsa,*vsp;
int					i;
bArray				bdg(sizeof(bGenericGeoElement*));
bArray				nod(sizeof(bGenericGeoElement*));
bool				mod;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	
	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanJoinOnNodesMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&oa);
		oa->getVertices(&vsa);
		mod=false;
		get_bridges(&vsa->vx.vx2[0],bdg);
		if(bdg.count()==0){
			get_nodes(&vsa->vx.vx2[0],nod);
			if(nod.count()==1){
				nod.get(1,&op);
				op->getVertices(&vsp);			
				vsa->vx.vx2[0]=vsp->vx.vx2[0];
				mod=true;
			}
		}
		get_bridges(&vsa->vx.vx2[vsa->nv-1],bdg);
		if(bdg.count()==0){
			get_nodes(&vsa->vx.vx2[vsa->nv-1],nod);
			if(nod.count()==1){
				nod.get(1,&op);
				op->getVertices(&vsp);			
				vsa->vx.vx2[vsa->nv-1]=vsp->vx.vx2[0];
				mod=true;
			}
		}
		
		if(mod){
			oa->setVertices(vsa);
			if(report){
				set_net_flag(oa,fields[oa->getType()-1],_kEdgeJoined);
			}
		}
	}

	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::join_on_edges(bArray& loop, int* fields, bool report){
_bTrace_("bXMapNetClean::join_on_edges",true);
bGenericGeoElement	*oa,*ob;
ivertices			*vsa,*vsb;
i2dvertex			vx,vmin;
int					i,j;
bArray				edg(sizeof(bGenericGeoElement*));
bArray				bdg(sizeof(bGenericGeoElement*));
double				dte,dab;
bool				flg;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];

	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanJoinOnEdgesMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&oa);
		oa->getVertices(&vsa);
		get_bridges(&vsa->vx.vx2[0],bdg);
		if(bdg.count()!=0){
			continue;
		}
		get_edges(&vsa->vx.vx2[0],edg,true);
		flg=(edg.count()>0);
		for(j=1;j<=edg.count();j++){
			edg.get(j,&ob);
			ob->getVertices(&vsb);
			dte=ivx2_dist(&vsa->vx.vx2[0],&vsb->vx.vx2[0]);
			dab=ivx2_dist(&vsa->vx.vx2[0],&vsb->vx.vx2[vsb->nv-1]);
			if(dte<dab){
				vx=vsb->vx.vx2[0];
			}
			else{
				vx=vsb->vx.vx2[vsb->nv-1];
			}
			if(j==1){
				vmin=vx;
			}
			else{
				if(!eq_ivx2(&vmin,&vx)){
					flg=false;
					break;
				}
			}
		}
		if(!flg){
			continue;
		}
		vsa->vx.vx2[0]=vmin;
		oa->setVertices(vsa);
		if(report){
			set_net_flag(oa,fields[oa->getType()-1],_kNodeJoined);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::check_nodes(bArray& loop, int* fields){
_bTrace_("bXMapNetClean::check_nodes",true);
bGenericGeoElement	*oa;
ivertices			*vsa;
int					i;
bArray				edg(sizeof(bGenericGeoElement*));
bArray				nod(sizeof(bGenericGeoElement*));
double				dbk=_cfg_prm.dnod;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	
	_cfg_prm.dnod=0;

	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanCheckNodesMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&oa);
		oa->getVertices(&vsa);
		get_nodes(&vsa->vx.vx2[0],nod);
		if(nod.count()>1){
			set_net_flag(oa,fields[oa->getType()-1],_kNodeDoubled);
		}
		get_edges(&vsa->vx.vx2[0],nod,true);
		if(edg.count()==0){
			set_net_flag(oa,fields[oa->getType()-1],_kNodeAlone);
			set_net_flag(oa,fields[oa->getType()-1],_kNodeNot3);
		}
		else if(edg.count()==1){
			set_net_flag(oa,fields[oa->getType()-1],_kNodeDeadEnd);
			set_net_flag(oa,fields[oa->getType()-1],_kNodeNot3);
		}
		else if(edg.count()==2){
			set_net_flag(oa,fields[oa->getType()-1],_kNodeNot3);
		}
	}
	
	_cfg_prm.dnod=dbk;

	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapNetClean::check_edges(bArray& loop, int* fields){
_bTrace_("bXMapNetClean::check_edges",true);
bGenericGeoElement	*oa;
ivertices			*vsa;
int					i;
bArray				arra(sizeof(bGenericGeoElement*));
bArray				arrb(sizeof(bGenericGeoElement*));
double				dbk=_cfg_prm.dnod;
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	
	_cfg_prm.dnod=0;
	
	b_message_string(kXMapNetCleanMessageID,ttl,getbundle(),0);
	b_message_string(kXMapNetCleanCheckEdgesMessage,msg,getbundle(),0);
bProgressWait	wt(ttl,msg,true,true,loop.count());
	for(i=1;i<=loop.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		loop.get(i,&oa);
		oa->getVertices(&vsa);
		
		get_edges(&vsa->vx.vx2[0],arra,true);
		get_edges(&vsa->vx.vx2[vsa->nv-1],arrb,true);
		if((arra.count()==1)&&(arrb.count()==1)){
			set_net_flag(oa,fields[oa->getType()-1],_kEdgeAlone);
		}
		else if((arra.count()==1)||(arrb.count()==1)){
			set_net_flag(oa,fields[oa->getType()-1],_kEdgeDeadEnd);
		}

		get_nodes(&vsa->vx.vx2[0],arra);
		get_nodes(&vsa->vx.vx2[vsa->nv-1],arrb);
		if((arra.count()==0)||(arrb.count()==0)){
			set_net_flag(oa,fields[oa->getType()-1],_kEdgeNotJoined);
		}
	}
	
	_cfg_prm.dnod=dbk;

	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::get_nodes(i2dvertex* vx, bArray& arr){
//_bTrace_("bXMapNetClean::get_nodes",true);
	arr.reset();
it_prm					iter;
bGenericGeoElement*		o;
ivx_rect				vr={vx->v,vx->h,vx->v,vx->h};
nc_prm					prm;

	prm.arr=&arr;
	prm.d=Measure_d2i(_gapp,_cfg_prm.dnod);
//_tm_("dst=%.6f",prm.d);
	prm.vx=*vx;
	ivr_inset(&vr,-prm.d,-prm.d);
	for(int i=1;i<=_nitr.count();i++){
		_nitr.get(i,&iter);
		iter.it->iterate(&vr,&prm,dst_point);
	}
	if(_cfg_prm.tbdg){
		for(int i=arr.count();i>0;i--){
			arr.get(i,&o);
			if(	(o->getType()==_cfg_prm.tbdg->index())	&&
				(o->getSubType()==_cfg_prm.stbdg)		){
				arr.rmv(i);
				continue;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::get_bridges(i2dvertex* vx, bArray& arr){
//_bTrace_("bXMapNetClean::get_bridges",true);
	arr.reset();
//_tm_("Bridge type at address "+_cfg_prm.tbdg);
	if(_cfg_prm.tbdg==NULL){
		return;
	}
it_prm					iter;
bGenericGeoElement*		o;
ivx_rect				vr={vx->v,vx->h,vx->v,vx->h};
nc_prm					prm;

	prm.arr=&arr;
	prm.d=Measure_d2i(_gapp,_cfg_prm.dbdg);
	prm.vx=*vx;
	ivr_inset(&vr,-prm.d,-prm.d);
	for(int i=1;i<=_nitr.count();i++){
		_nitr.get(i,&iter);
		iter.it->iterate(&vr,&prm,dst_point);
	}
	for(int i=arr.count();i>0;i--){
		arr.get(i,&o);
		if(	(o->getType()!=_cfg_prm.tbdg->index())	||
			(o->getSubType()!=_cfg_prm.stbdg)		){
			arr.rmv(i);
			continue;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::get_edges(ivx_rect* vr, bArray& arr){
//_bTrace_("bXMapNetClean::get_edges(ivx_rect* vr, bArray& arr)",true);
	arr.reset();
it_prm					iter;
nc_prm					prm;

	prm.arr=&arr;
	for(int i=1;i<=_eitr.count();i++){
		_eitr.get(i,&iter);
		iter.it->iterate(vr,&prm,all_objects);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::get_edges(i2dvertex* vx, bArray& arr, bool by_end){
//_bTrace_("bXMapNetClean::get_edges(i2dvertex* vx, bArray& arr, bool by_end)",true);
	arr.reset();
it_prm					iter;
ivx_rect				vr={vx->v,vx->h,vx->v,vx->h};
nc_prm					prm;

	prm.arr=&arr;
	prm.d=Measure_d2i(_gapp,_cfg_prm.dnod);
	prm.vx=*vx;
	ivr_inset(&vr,-prm.d,-prm.d);
	for(int i=1;i<=_eitr.count();i++){
		_eitr.get(i,&iter);
		if(by_end){
			iter.it->iterate(&vr,&prm,dst_edge_end);
		}
		else{
			iter.it->iterate(&vr,&prm,dst_edge);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::init_fields(netcleanact_prm* prm){
_bTrace_("bXMapNetClean::init_fields",true);
it_prm			iter;
init_prm		ip;
char			name[__MESSAGE_STRING_LENGTH_MAX__];

	b_message_string(kXMapNetCleanControlField,name,getbundle(),0);

	if(prm->check_nodes){
		for(int i=1;i<=_nitr.count();i++){
			_nitr.get(i,&iter);
			iter.field=iter.tp->fields()->get_index(name);
			if(!iter.field){
				iter.tp->fields()->add(_char,8,0,name);
				iter.field=iter.tp->fields()->get_index(name);
				if(iter.field){
					iter.tp->fields()->set_default_value(iter.field,(void*)kXMapNetCleanControlFieldDefault);
				}
			}
			if(iter.field){
				ip.field=iter.field;
				iter.it->iterate(&ip,init_ctl);
			}
			_nitr.put(i,&iter);
		}
	}
	
	if(prm->check_edges){
		for(int i=1;i<=_eitr.count();i++){
			_eitr.get(i,&iter);
			iter.field=iter.tp->fields()->get_index(name);
			if(!iter.field){
				iter.tp->fields()->add(_char,8,0,name);
				iter.field=iter.tp->fields()->get_index(name);
				if(iter.field){
					iter.tp->fields()->set_default_value(iter.field,(void*)kXMapNetCleanControlFieldDefault);
				}
			}
			if(iter.field){
				ip.field=iter.field;
				iter.it->iterate(&ip,init_ctl);
			}
			_eitr.put(i,&iter);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::read_p(){
_bTrace_("bXMapNetClean::read_p",true);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* prm1=NULL;
bGenericXMLBaseElement* prm2=NULL;
bGenericXMLBaseElement* prm3=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,GetSignature(this),kXMapNetCleanConfig);
char					val[_values_length_max_];
bGenericType*			tp;
int						i,idx;

	if((!b)||(!root)){
_tw_("((!b)||(!root)) for "+kXMapNetCleanConfig);
		return;
	}

	prm1=_gapp->classMgr()->NthElement(root,2,"param");
	if(!prm1){
_te_("prm1 not found");
		return;
	}
	prm2=_gapp->classMgr()->NthElement(root,3,"param");
	if(!prm2){
_te_("prm2 not found");
		return;
	}
	prm3=_gapp->classMgr()->NthElement(root,4,"param");
	if(!prm3){
_te_("prm3 not found");
		return;
	}

	tp=NULL;
	for(i=1;i<=_gapp->typesMgr()->count();i++){
		_nodes.put(i,&tp);
		_edges.put(i,&tp);
	}
	for(i=1;;i++){
		elt=_gapp->classMgr()->NthElement(prm1,i,"name");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		idx=_gapp->typesMgr()->index(val);
		if(idx){
			tp=_gapp->typesMgr()->get(idx);
			_nodes.put(idx,&tp);
		}
	}
	for(i=1;;i++){
		elt=_gapp->classMgr()->NthElement(prm2,i,"name");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		idx=_gapp->typesMgr()->index(val);
		if(idx){
			tp=_gapp->typesMgr()->get(idx);
			_edges.put(idx,&tp);
		}
	}
	elt=_gapp->classMgr()->NthElement(prm3,1,"name");
	if(elt){
		elt->getvalue(val);
		idx=_gapp->typesMgr()->index(val);
		_cfg_prm.tnod=_gapp->typesMgr()->get(idx);
	}
	elt=_gapp->classMgr()->NthElement(prm3,1,"int");
	if(elt){
		elt->getvalue(val);
		_cfg_prm.stnod=atoi(val);
	}	
	elt=_gapp->classMgr()->NthElement(prm3,2,"name");
	if(elt){
		elt->getvalue(_cfg_prm.name);
	}
	
	elt=_gapp->classMgr()->NthElement(prm3,3,"name");
	if(elt){
		elt->getvalue(val);
		idx=_gapp->typesMgr()->index(val);
		_cfg_prm.tbdg=_gapp->typesMgr()->get(idx);
_tm_("Bridges="+val+" at index "+idx+" type "+_cfg_prm.tbdg);
	}
	elt=_gapp->classMgr()->NthElement(prm3,2,"int");
	if(elt){
		elt->getvalue(val);
		_cfg_prm.stbdg=atoi(val);
	}	

	elt=_gapp->classMgr()->NthElement(prm3,1,"float");
	if(elt){
		elt->getvalue(val);
		_cfg_prm.dnod=Measure_i2d(_gapp,atof(val));
	}	
	elt=_gapp->classMgr()->NthElement(prm3,2,"float");
	if(elt){
		elt->getvalue(val);
		_cfg_prm.dbdg=Measure_i2d(_gapp,atof(val));
	}	

	elt=_gapp->classMgr()->NthElement(prm3,1,"bool");
	if(elt){
		elt->getvalue(val);
		_cfg_prm.autoclean=atoi(val);
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapNetClean::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
	if(!_gapp->document()->writeTree(root,GetSignature(this),kXMapNetCleanConfig)){
		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapNetClean::make_p(){
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

bGenericType*	tp;
double			d;
char			nm[256];
int				i;

	add_cdesc(arr,1,"param","");
	for(i=1;i<=_nodes.count();i++){
		tp=NULL;
		_nodes.get(i,&tp);
		if(tp){
			tp->name(nm);
			add_cdesc(arr,2,"name",nm);
		}
	}
	
	add_cdesc(arr,1,"param","");
	for(i=1;i<=_edges.count();i++){
		tp=NULL;
		_edges.get(i,&tp);
		if(tp){
			tp->name(nm);
			add_cdesc(arr,2,"name",nm);
		}
	}

	add_cdesc(arr,1,"param","");
	if(_cfg_prm.tnod){
		_cfg_prm.tnod->name(nm);
		add_cdesc(arr,2,"name",nm);
		add_idesc(arr,2,"int",_cfg_prm.stnod);
	}
	else{
		add_cdesc(arr,2,"name","");
		add_idesc(arr,2,"int",0);
	}
	add_cdesc(arr,2,"name",_cfg_prm.name);
	if(_cfg_prm.tbdg){
		_cfg_prm.tbdg->name(nm);
		add_cdesc(arr,2,"name",nm);
		add_idesc(arr,2,"int",_cfg_prm.stbdg);
	}
	else{
		add_cdesc(arr,2,"name","");
		add_idesc(arr,2,"int",0);
	}
	d=Measure_d2i(_gapp,_cfg_prm.dnod);
	add_ddesc(arr,2,"float",_cfg_prm.dnod,_gapp->document()->dist_pref_digits());
	d=Measure_d2i(_gapp,_cfg_prm.dbdg);
	add_ddesc(arr,2,"float",_cfg_prm.dbdg,_gapp->document()->dist_pref_digits());
	add_idesc(arr,2,"bool",_cfg_prm.autoclean);

bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
int all_objects(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
nc_prm*				p=(nc_prm*)prm;
	p->arr->add(&geo);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int dst_point(void *o, void *prm){
//_bTrace_("dst_point",to);
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
nc_prm*				p=(nc_prm*)prm;
ivertices*			vxs;
	geo->getVertices(&vxs);
double				d=ivx2ivs_dist(&p->vx,vxs);

//if(to){
//_tm_("%d;%d\n",p->vx.h,p->vx.v);
//for(int i=0;i<vxs->nv;i++){
//_tm_("%d;%d",vxs->vx.vx2[i].h,vxs->vx.vx2[i].v);
//}
//}
//_tm_("%d : %.2f->%.2f",geo->getID(),p->d,d);
	if(d<=p->d){
//if(to){
//_tm_("node %d->%f for %f",geo->getID(),d,p->d);
//}
		p->arr->add(&geo);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int dst_edge(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
nc_prm*				p=(nc_prm*)prm;
ivertices*			vxs;
	geo->getVertices(&vxs);
	if(ivx2ivs_dist(&p->vx,vxs)<=p->d){
		p->arr->add(&geo);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int dst_edge_end(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
nc_prm*				p=(nc_prm*)prm;
ivertices*			vxs;
	geo->getVertices(&vxs);
double				d;
	d=ivx2_dist(&p->vx,&vxs->vx.vx2[0]);
	if(d<=p->d){
		p->arr->add(&geo);
	}
	d=ivx2_dist(&p->vx,&vxs->vx.vx2[vxs->nv-1]);
	if(d<=p->d){
		p->arr->add(&geo);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int init_ctl(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
init_prm*			ip=(init_prm*)prm;
	geo->setValue(ip->field,kXMapNetCleanControlFieldDefault);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void set_net_flag(	bGenericGeoElement*	o,
					int fld,
					int flg){
_bTrace_("set_net_flag",false);
char	val[32];
	o->getValue(fld,val);
	if(strlen(val)!=8){
		strcpy(val,kXMapNetCleanControlFieldDefault);
	}
	val[flg-1]='1';
	if(!o->setValue(fld,val)){
_te_("setValue failed for "+fld);
	}
}

