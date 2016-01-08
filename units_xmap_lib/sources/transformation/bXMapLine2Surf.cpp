//----------------------------------------------------------------------------
// File : bXMapLine2Surf.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, create polygon from polylines
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

#include "bXMapLine2Surf.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>


//----------------------------------------------------------------------------

class l2s_object{
	public:		
		l2s_object				(	ivertices*	vxs);
		virtual ~l2s_object		(	);
		
		virtual void set_flag	(	bool b);
		virtual bool get_flag	(	);
		virtual void get_bounds	(	ivx_rect* vr);
		virtual ivertices* geom	(	);

	protected:		
		ivertices*	_vxs;
		bool		_flag;
		
	private:

};

//----------------------------------------------------------------------------

class l2s_node : public l2s_object{
	public:		
		l2s_node					(	i2dvertex* vx,
										bGenericGeoIterator* it);
		virtual ~l2s_node			(	);
		virtual void add			(	void* lnk);
		virtual void rmv			(	void* lnk);
		virtual void* get_best		(	ivertices* vxs,
										int idx,
										int* dir);
		virtual bool check_ok		(	);
		
	protected:		
		
	private:
		bArray	_lnk;
};

//----------------------------------------------------------------------------

class l2s_surf : public l2s_object{
	public:		
		l2s_surf				(	ivertices* vxs);
		virtual ~l2s_surf		(	);
		
		virtual bool is_best	(	);
		virtual void set_best	(	bool b);

		static bArray _elts;
		
	protected:		
		bool	_best;
		
	private:

};

//----------------------------------------------------------------------------

class l2s_link : public l2s_object{
	public:		
		l2s_link				(	ivertices* vxs,
									bGenericGeoIterator* it,
									int lid);
		virtual ~l2s_link		(	);
		virtual int travel		(	ivertices** iti,
									int dir,
									l2s_surf** res);
		virtual bool check_ok	(	);
		virtual bool check_surf	(	l2s_surf* srf);

		int			_lid;
		l2s_surf*	_right;
		l2s_surf*	_left;
		
	protected:		
		
	private:
		l2s_node*	_te;
		l2s_node*	_ab;
		bool		_vis;
};

//----------------------------------------------------------------------------

typedef struct bg_prm{
	bGenericGeoIterator*	it;
	bArray*					arr;
}bg_prm;

typedef struct gn_prm{
	bArray*		arr;
	i2dvertex	vx;
}gn_prm;

// ---------------------------------------------------------------------------
// 
// ------------
static void l2s_set_flag(void* o, bool b){
l2s_object*	lo=(l2s_object*)o;
	lo->set_flag(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
static bool l2s_get_flag(void* o){
l2s_object*	lo=(l2s_object*)o;
	return(lo->get_flag());
}

// ---------------------------------------------------------------------------
// 
// ------------
static void l2s_set_bounds(void* o, ivx_rect* vr){
l2s_object*	lo=(l2s_object*)o;
	lo->get_bounds(vr);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int get_node(void *o, void *prm){
l2s_node*	nd=(l2s_node*)o;
gn_prm*		p=(gn_prm*)prm;
	if(eq_ivx2(&p->vx,&nd->geom()->vx.vx2[0])){
		p->arr->add(&nd);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int all_node(void *o, void *prm){
l2s_node*	nd=(l2s_node*)o;
bArray*		arr=(bArray*)prm;
	arr->add(&nd);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int build_graphe(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
	if(geo->masked()){
		return(0);
	}
bg_prm*		p=(bg_prm*)prm;
ivertices*	vxs;
	geo->getVertices(&vxs);
l2s_link*	lnk=new l2s_link(vxs,p->it,geo->getID());
	p->arr->add(&lnk);
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapLine2Surf	::bXMapLine2Surf(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("line2surf");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapLine2Surf::~bXMapLine2Surf(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapLine2Surf::create(bGenericXMLBaseElement* elt){
	return(new bXMapLine2Surf(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapLine2Surf::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_tl=NthTypeOfKind(_gapp,1,kBaseKindPolyline);
	_ts=NthTypeOfKind(_gapp,1,kBaseKindPolygon);
	_stp=1;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLine2Surf::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			make();
			break;
		case kExtProcessCallWithParams:
			break;
		default:
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLine2Surf::test(void* prm){
	return(	(NthTypeOfKind(_gapp,1,kBaseKindPolyline)!=NULL)	&&
			(NthTypeOfKind(_gapp,1,kBaseKindPolygon)!=NULL)	);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapLine2Surf::wd_event(EventRef evt, WindowRef wd){
bool			b=true;
HICommand		cmd;
UInt32			clss=GetEventClass(evt);
ControlRef		c;
bGenericType*	tp;

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopLineID);
				_tl=_gapp->typesMgr()->get(GetControl32BitValue(c));
				c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSurfID);
				_ts=_gapp->typesMgr()->get(GetControl32BitValue(c));
				c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSubtypeID);
				_stp=GetControl32BitValue(c);
				break;
			case kHICommandCancel:
				break;
			case kXMapLine2SurfPopSurfCmd:
				c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSurfID);
				tp=_gapp->typesMgr()->get(GetControl32BitValue(c));
				c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSubtypeID);
				ClearPopupControl(c,1);
				_stp=1;
				PopulatePopupControlWithConstraints(tp,kOBJ_SubType_,c,_stp);
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
void bXMapLine2Surf::wd_init(WindowRef wd){
ControlRef	c;
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopLineID);
	ClearPopupControl(c,1);
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSurfID);
	ClearPopupControl(c,1);
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSubtypeID);
	ClearPopupControl(c,1);
	
	if(_tl==NULL){
		_tl=NthTypeOfKind(_gapp,1,kBaseKindPolyline);
	}
	if(_tl==NULL){
		return;
	}
	if(_ts==NULL){
		_ts=NthTypeOfKind(_gapp,1,kBaseKindPolygon);
	}
	if(_ts==NULL){
		return;
	}			
	
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopLineID);
	PopulatePopupControlWithType(_gapp,c,kBaseKindPolyline,_tl->index());
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSurfID);
	PopulatePopupControlWithType(_gapp,c,kBaseKindPolygon,_ts->index());
	c=get_control(kXMapLine2SurfSignature,kXMapLine2SurfPopSubtypeID);
	PopulatePopupControlWithConstraints(_ts,kOBJ_SubType_,c,_stp);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapLine2Surf::make(){
_bTrace_("bXMapLine2Surf::make",true);
ivx_rect	bounds;
int			status;
	_tl->iterator()->bounds(&bounds);
gi_init_p	gp={&bounds,&status,l2s_set_flag,l2s_get_flag,l2s_set_bounds};
bGenericGeoIterator*	
			iter=	(bGenericGeoIterator*)
					(_gapp->extMgr()->get_component(kComponentGeoIterator)->i_allocate(
					_kNULLSign_,_gapp,&gp));
bArray		arr(sizeof(l2s_link*));
bg_prm		prm;
	prm.it=iter;
	prm.arr=&arr;
_tm_("build graphe");
	_tl->iterator()->iterate(&prm,build_graphe);
	
l2s_node*			nd;
l2s_link*			lnk;
l2s_surf*			srf;
ivertices			*vxs;
bGenericGeoElement*	o;

_tm_("remove deadends");
	for(int i=arr.count();i>0;i--){
		arr.get(i,&lnk);
		if(!lnk->check_ok()){
			arr.rmv(i);
			delete lnk;
			i=arr.count()+1;
		}
	}

_tm_("travel");
char			msg[__MESSAGE_STRING_LENGTH_MAX__];
char			ttl[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapLine2SurfMessageID,ttl,getbundle(),0);
	message_string(kMsgProgress,msg,1);
bProgressWait	wt(ttl,msg,true,true,arr.count());	
	for(int i=1;i<=arr.count();i++){
		if(!wt.set_progress(i)){
_tm_("user break");
			break;
		}
		arr.get(i,&lnk);
//_tm_(i);
		for(int j=-1;j<=1;j+=2){
			vxs=ivs_new(_2D_VX,0,0);
			if(lnk->travel(&vxs,j,&srf)==-1){
				ivs_free(vxs);
			}
		}
	}
	
_tm_("delete links");
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&lnk);
// Correction
		if((lnk->_right)&&(!lnk->_right->is_best())){
			lnk->_right->set_best(true);
		}
		if((lnk->_left)&&(!lnk->_left->is_best())){
			lnk->_left->set_best(true);
		}
		delete lnk;
	}
	
_tm_("delete nodes");
	arr.reset();
	iter->iterate(&arr,all_node);
	_gapp->extMgr()->get_component(kComponentGeoIterator)->i_free(iter);
	for(int i=1;i<=arr.count();i++){
		arr.get(i,&nd);
		delete nd;
	}

_tm_("create surf");
bEventLog	log(_gapp,
				getbundle(),
				kXMapLine2SurfMessageID,
				GetSignature(this));
	for(int i=1;i<=l2s_surf::_elts.count();i++){
		l2s_surf::_elts.get(i,&srf);
		if(srf->is_best()){
			if(_ts->new_object(&o)){
				o->setVertices(srf->geom());
			}
		}
		delete srf;
	}
	log.close();

	l2s_surf::_elts.reset();	
}


// ---------------------------------------------------------------------------
// Constructeur
// ------------
l2s_object::l2s_object(ivertices* vxs){
	_vxs=vxs;
	_flag=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
l2s_object::~l2s_object(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void l2s_object::set_flag(bool b){
	_flag=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool l2s_object::get_flag(){
	return(_flag);
}

// ---------------------------------------------------------------------------
// 
// -----------
void l2s_object::get_bounds(ivx_rect* vr){
	ivs_bounds(_vxs,vr);
}

// ---------------------------------------------------------------------------
// 
// -----------
ivertices* l2s_object::geom(){
	return(_vxs);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
l2s_node::l2s_node(i2dvertex* vx, bGenericGeoIterator* it)
		:l2s_object(NULL)
		,_lnk(sizeof(l2s_link*)){
	_vxs=ivs_new(_2D_VX,1,0);
	_vxs->vx.vx2[0]=*vx;
	it->add(this);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
l2s_node::~l2s_node(){
	ivs_free(_vxs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void l2s_node::add(void* lnk){
	_lnk.add(&lnk);
}

// ---------------------------------------------------------------------------
// 
// -----------
void l2s_node::rmv(void* lnk){
int		k=_lnk.search(&lnk,ObjRefComp);
	if(k){
		_lnk.rmv(k);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void* l2s_node::get_best(ivertices* vxs, int idx, int* dir){
_bTrace_("l2s_node::get_best",false);
l2s_link*	lnk;
l2s_link*	bst=NULL;
	if(_lnk.count()==0){
_te_("no links");
		return(NULL);
	}
	if(_lnk.count()==1){
		_lnk.get(1,&lnk);
		*dir=-(*dir);
		return((void*)lnk);
	}
i2dvertex	a,b;
int			i,j,ndir=0,bdir=0;
double		a1,a2,ang,amax=0;

	amax=360;
	if(idx==0){
		for(i=1;i<vxs->nv;i++){
			if(!eq_ivx2(&_vxs->vx.vx2[0],&vxs->vx.vx2[i])){
				a=vxs->vx.vx2[i];
				break;
			}
		}
		if(i==vxs->nv){
_te_("1 bad at a(1) ");
//_te_("bad at a(1) "+_vxs->vx.vx2[0].h+":"+_vxs->vx.vx2[0].v);
			return(NULL);
		}
	}
	else{
		for(i=vxs->nv-2;i>-1;i--){
			if(!eq_ivx2(&_vxs->vx.vx2[0],&vxs->vx.vx2[i])){
				a=vxs->vx.vx2[i];
				break;
			}
		}
		if(i==-1){
_te_("1 bad at a(-1) ");
//_te_("bad at a(-1) "+_vxs->vx.vx2[0].h+":"+_vxs->vx.vx2[0].v);
			return(NULL);
		}	
	}
	
	for(i=1;i<=_lnk.count();i++){
		_lnk.get(i,&lnk);
		if(lnk->geom()==vxs){
			continue;
		}
		if(eq_ivx2(&_vxs->vx.vx2[0],&lnk->geom()->vx.vx2[0])){
			for(j=1;j<lnk->geom()->nv;j++){
				if(!eq_ivx2(&_vxs->vx.vx2[0],&lnk->geom()->vx.vx2[j])){
					b=lnk->geom()->vx.vx2[j];
					break;
				}
			}
			if(j==lnk->geom()->nv){
_te_("2 bad at b(1)");
//_te_("bad at b(1) "+_vxs->vx.vx2[0].h+":"+_vxs->vx.vx2[0].v);
				continue;
			}
			ndir=1;
		}
		else if(eq_ivx2(&_vxs->vx.vx2[0],&lnk->geom()->vx.vx2[lnk->geom()->nv-1])){
			for(j=lnk->geom()->nv-2;j>-1;j--){
				if(!eq_ivx2(&_vxs->vx.vx2[0],&lnk->geom()->vx.vx2[j])){
					b=lnk->geom()->vx.vx2[j];
					break;
				}
			}
			if(j==-1){
_te_("2 bad at b(-1) ");
//_te_("bad at b(-1) "+_vxs->vx.vx2[0].h+":"+_vxs->vx.vx2[0].v);
				continue;
			}
			ndir=-1;
		}
		else{
_te_("bad lnk->geom()");
//_te_("bad lnk->geom() for "+lnk->_lid+" at "+_vxs->vx.vx2[0].h+":"+_vxs->vx.vx2[0].v);
			continue;// Normalement impossible
		}
		a1=rad2deg(xy_vx_angle(&_vxs->vx.vx2[0],&a));
		a2=rad2deg(xy_vx_angle(&_vxs->vx.vx2[0],&b));
		ang=a1-a2;
		if(ang<0){
			ang+=360.0;
		}
//_tm_(ang);
		if(ang<amax){
			amax=ang;
			bdir=ndir;
			bst=lnk;
		}
	}
	*dir=bdir;
	return((void*)bst);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool l2s_node::check_ok(){
	return(_lnk.count()>1);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
l2s_link::l2s_link(ivertices* vxs, bGenericGeoIterator* it, int lid)
		:l2s_object(vxs){
_bTrace_("l2s_link::l2s_link",false);
	_lid=lid;
	_left=NULL;
	_right=NULL;
	_vis=false;
	
ivx_rect	vr;
	vr.left=vr.right=_vxs->vx.vx2[0].h;
	vr.top=vr.bottom=_vxs->vx.vx2[0].v;
bArray		arr(sizeof(l2s_node*));
gn_prm		prm;
	prm.arr=&arr;
	
	prm.vx=_vxs->vx.vx2[0];
	it->iterate(&vr,&prm,get_node);
	if(arr.count()==0){
		_te=new l2s_node(&_vxs->vx.vx2[0],it);
	}
	else if(arr.count()==1){
		arr.get(1,&_te);
	}
	else{
_te_(_lid+" : bad node count on te");
		arr.get(1,&_te);
	}
	_te->add(this);
	
	vr.left=vr.right=_vxs->vx.vx2[_vxs->nv-1].h;
	vr.top=vr.bottom=_vxs->vx.vx2[_vxs->nv-1].v;
	arr.reset();
	prm.vx=_vxs->vx.vx2[_vxs->nv-1];
	it->iterate(&vr,&prm,get_node);
	if(arr.count()==0){
		_ab=new l2s_node(&_vxs->vx.vx2[_vxs->nv-1],it);
	}
	else if(arr.count()==1){
		arr.get(1,&_ab);
	}
	else{
_te_(_lid+" : bad node count on ab");
		arr.get(1,&_ab);
	}
	_ab->add(this);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
l2s_link::~l2s_link(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int l2s_link::travel(ivertices** iti, int dir, l2s_surf** res){
_bTrace_("l2s_link::travel",false);
l2s_link*	lnk;
ivertices*	buf;
l2s_surf*	srf;
//_tm_(_lid);
	
	_vis=true;
	if(dir==1){
		*iti=ivs_concat(*iti,_vxs);
	}
	else if(dir==-1){
		ivs2ivs(_vxs,&buf);
		ivs_reverse(buf);
		*iti=ivs_concat(*iti,buf);
		ivs_free(buf);
	}
	else{
_te_("bad dir for "+_lid);
		_vis=false;
		return(-1);
	}

	if(ivs_closed(*iti)){
		if(((*iti)->nv<=3)||(ivs_surf(*iti)<=0)){
_te_("bad surf for "+_lid);
			_vis=false;
			return(-1);
		}
		srf=new l2s_surf(*iti);
		*res=srf;
		_vis=false;
		return(check_surf(srf));
	}
	
	lnk=(dir==1)?
		(l2s_link*)_ab->get_best(_vxs,_vxs->nv-1,&dir):
		(l2s_link*)_te->get_best(_vxs,0,&dir);
	if(!lnk){
_tm_("no best");
		_vis=false;
		return(-1);
	}
	if(lnk->_vis==true){
//_tm_("deja utilisé");
		_vis=false;
		return(-1);
	}
	if(lnk->travel(iti,dir,res)>-1){
		_vis=false;
		return(check_surf(*res));
	}
//_tm_("je sais pas quoi...");
	_vis=false;
	return(-1);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool l2s_link::check_surf(l2s_surf* srf){
i2dvertex	mid;
	mid.h=round(0.5*(_vxs->vx.vx2[0].h+_vxs->vx.vx2[1].h));
	mid.v=round(0.5*(_vxs->vx.vx2[0].v+_vxs->vx.vx2[1].v));
double		r,a=vx_angle(&_vxs->vx.vx2[0],&_vxs->vx.vx2[1]);
int			rc,ac;
i2dvertex	p;
	(void)ivx_proj_at_dist(&mid,&p,1,a-(M_PI/2.0));	
	if(ivx_in_ivs(&p,srf->geom(),0)){
		if(!_right){
			_right=srf;
			return(true);
		}
		r=ivs_surf(_right->geom());
		a=ivs_surf(srf->geom());
		if(r>a){
			_right->set_best(false);
			_right=srf;
			return(true);
		}
		else if(r<a){
			srf->set_best(false);
			return(false);
		}
		else{
			rc=ivx2_clockwise(_right->geom()->vx.vx2,_right->geom()->nv);
			ac=ivx2_clockwise(srf->geom()->vx.vx2,srf->geom()->nv);
			if((rc==-1)&&(ac==1)){
				_right->set_best(false);
				_right=srf;
				return(true);
			}
			else if((rc==1)&&(ac==-1)){
				srf->set_best(false);
				return(false);
			}
			else{
				srf->set_best(false);
				return(false);
			}
		}
	}
	else{
		if(!_left){
			_left=srf;
			return(true);
		}
		r=ivs_surf(_left->geom());
		a=ivs_surf(srf->geom());
		if(r>a){
			_left->set_best(false);
			_left=srf;
			return(true);
		}
		else if(r<a){
			srf->set_best(false);
			return(false);
		}
		else{
			rc=ivx2_clockwise(_left->geom()->vx.vx2,_left->geom()->nv);
			ac=ivx2_clockwise(srf->geom()->vx.vx2,srf->geom()->nv);
			if((rc==-1)&&(ac==1)){
				_left->set_best(false);
				_left=srf;
				return(true);
			}
			else if((rc==1)&&(ac==-1)){
				srf->set_best(false);
				return(false);
			}
			else{
				srf->set_best(false);
				return(false);
			}
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool l2s_link::check_ok(){
bool	tc=_te->check_ok();
bool	ac=_ab->check_ok();

	if(!tc||!ac){
		_te->rmv(this);
		_ab->rmv(this);
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray l2s_surf::_elts(sizeof(l2s_surf*));

// ---------------------------------------------------------------------------
// Constructeur
// ------------
l2s_surf::l2s_surf(ivertices* vxs)
		:l2s_object(vxs){
l2s_surf*	srf=this;
	_elts.add(&srf);
	_best=true;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
l2s_surf::~l2s_surf(){
	ivs_free(_vxs);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool l2s_surf::is_best(){
	return(_best);
}

// ---------------------------------------------------------------------------
// 
// -----------
void l2s_surf::set_best(bool b){
	_best=b;
}

