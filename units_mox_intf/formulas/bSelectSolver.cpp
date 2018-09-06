//----------------------------------------------------------------------------
// File : bSelectSolver.cpp
// Project : MacMap
// Purpose : C++ source file : Class for selection solver (used database and geographic requests)
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
// 08/03/2004 creation.
// 14/02/2008 ajout du flag _wait pour ne pas mettre de curswait dans certains
//			par exemple, quand on utilise le solver dans des macro incluses
//			dans un remplissage.
//----------------------------------------------------------------------------

#include <mox_intf/bSelectSolver.h>
#include <MacMapSuite/bTrace.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/mm_messages.h>

#define _DEBUG_SOLVER_ 0

// ---------------------------------------------------------------------------
// 
// ------------
#define _sapp ((bGenericMacMapApp*)_gapp)

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSelectSolver	::bSelectSolver(void* gapp, bool wait)
				:bStdSolver(gapp){
#if _DEBUG_SOLVER_
_bTrace_("bSelectSolver::bSelectSolver",false);
_tm_((void*)this);
#endif
	_root=NULL;
	_objs=NULL;
	_source=NULL;
	_ocount=0;
	_selkind=kSelReplace;
	_srckind=kSrcNone;
	_use_masked=false;
	_pre_process=false;
	_wait=wait;
	_next=0;
	_add2cnt=false;
	_add2hdn=kAdd2HiddenNone;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bSelectSolver::~bSelectSolver(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_select_kind(int	k){
	_selkind=k;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::get_select_kind(){
	return(_selkind);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_use_masked(bool b){
	_use_masked=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSelectSolver::get_use_masked(){
	return(_use_masked);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_next(UInt32 nxt){
#if _DEBUG_SOLVER_
_bTrace_("bSelectSolver::set_next",true);
_tm_(&nxt);
#endif
	_next=nxt;
}

// ---------------------------------------------------------------------------
// 
// -----------
UInt32 bSelectSolver::get_next(){
	return(_next);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_add2contrast(bool b){
	_add2cnt=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSelectSolver::get_add2contrast(){
	return(_add2cnt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_add2hidden(int mode){
#if _DEBUG_SOLVER_
_bTrace_("bSelectSolver::set_add2hidden",true);
_tm_(mode);
#endif
	_add2hdn=mode;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::get_add2hidden(){
	return(_add2hdn);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::set_preprocess(){
	_pre_process=true;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::get_source_kind(){
	return(_srckind);
}

// ---------------------------------------------------------------------------
// 
// -----------
bArray* bSelectSolver::get_source(){
	return(_source);
}

// ---------------------------------------------------------------------------
// 
// -----------
bArray* bSelectSolver::get_processed(){
	return(_objs);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::valid_object(bGenericGeoElement* o){
	if(!o->flag1()){
		o->set_flag1(true);
		_objs->add(&o);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bSelectSolver::inval_object(int idx){
bGenericGeoElement* o;
	if(!_objs->get(idx,&o)){
		return;
	}
	o->set_flag1(false);
	if(!_objs->rmv(idx)){
		return;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bSelectSolver::exec(bGenericXMLBaseElement* elt){
#if _DEBUG_SOLVER_
_bTrace_("bSelectSolver::exec",true);
#endif
bool				r=false,good;	
int					i,n;
bGenericGeoElement*	o;
OSStatus			status=noErr;
bool				root_reset=false;

#if _DEBUG_SOLVER_
double			t1,t2;
_tm_("solver init begin");
t1=clock();
#endif

	_root=NULL;
	_objs=NULL;
	_source=NULL;
	_ocount=0;
	_selkind=kSelReplace;
	_use_masked=false;
	_pre_process=false;
	set_type(NULL);
	_next=_kNULLSign_;
	_add2cnt=false;
	_add2hdn=kAdd2HiddenNone;
		
	if(get_object()!=NULL){
// Si on a déjà positionné un objet, c'est pas pour des prunes !
		_source=new bArray(sizeof(bGenericGeoElement*),1);
		o=get_object();
		_source->put(1,&o);
		_srckind=kSrcObj;		
	}
	else if(_sapp->cntMgr()->count()>0){
		_source=new bArray(*((bArray*)_sapp->cntMgr()->elements()));
		_srckind=kSrcCnt;
	}
	else if(_sapp->selMgr()->count()>0){
		_source=new bArray((*(bArray*)_sapp->selMgr()->elements()));
		_srckind=kSrcSel;
	}
	else{
		_source=new bArray(sizeof(bGenericGeoElement*));
		_srckind=kSrcNone;
	}
	
#if _DEBUG_SOLVER_
t2=clock();
_tm_("init : "+(int)(t2-t1));
_tm_("solver init end");
#endif

	for(;;){

#if _DEBUG_SOLVER_
_tm_("begin selection process");
t1=clock();
#endif
		_root=(bGenericXMLFormulaElement*)(void*)elt;
		if(!_root){
#if _DEBUG_SOLVER_
_te_("no root");	
#endif
			break;
		}
		
		_objs=new bArray(sizeof(bGenericGeoElement*));
		if(!_objs){
#if _DEBUG_SOLVER_
_te_("no _objs");	
#endif
			break;
		}
		
// Init de la formule
// Les geogs peuvent constituer la liste initiale
// en pré-traitement à l'init

#if _DEBUG_SOLVER_
_tm_("_root->init begin");
t1=clock();
#endif
		_root->init(this);
#if _DEBUG_SOLVER_
t2=clock();
_tm_("init : "+(int)(t2-t1));
_tm_("_root->init end");
#endif

		if(!get_type()){
#if _DEBUG_SOLVER_
_te_("no type");
#endif
			break;
		}
		
		_ocount=0;
		
		if(!_pre_process){
// Comptage pour allocation du tableau à n objets
#if _DEBUG_SOLVER_
_tm_("iterator()->iterate objcount begin");
#endif
			get_type()->iterator()->iterate(this,obj_count);
#if _DEBUG_SOLVER_
_tm_("iterator()->iterate objcount end");
#endif
			if(_ocount<=0){
#if _DEBUG_SOLVER_
_te_("no objects in current type");
#endif
				break;
			}
			n=_ocount;

// Effacement et réallocation du tableau			
			delete _objs;
			_objs=new bArray(sizeof(bGenericGeoElement*),_ocount);
			
			if(!_objs){
#if _DEBUG_SOLVER_
_te_("no _objs");	
#endif
				break;
			}
			_ocount=0;

// Remplissage du tableau
#if _DEBUG_SOLVER_
_tm_("iterator()->iterate obj_add begin");
#endif
			get_type()->iterator()->iterate(this,obj_add);
#if _DEBUG_SOLVER_
_tm_("iterator()->iterate obj_add end");
#endif

// Retrait des cases non utilisées du tableau
//normalement, ça ne doit pas se produire
/*			for(i=n;i>_ocount;i--){
				if(!_objs->rmv(i)){
					status=-1;
					break;
				}
			}*/
		}
		else{
			_ocount=_objs->count();
		}


// Application du filtrage principal et retrait
// des objets refusés
bool			ub=false;
int				ik=0;
bStdWait*		wt=NULL;

	if(_wait){
#if _DEBUG_SOLVER_
_tm_("init wait for "+_ocount+" objects");
t1=clock();
#endif
		//wt=new bCursWait(true);
		
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
		message_string(_CSTR_(kMsgProgress),msg,1);
		wt=new bProgressWait("",(const char*)msg,true,(long)_ocount);
	}
	
#if _DEBUG_SOLVER_
_tm_("filter loop begin");
t1=clock();
#endif

		n=_ocount;
		for(i=1;i<=_ocount;i++){
			_objs->get(i,&o);
			ik++;
			if((wt)&&(ik%500==0)){
#if _DEBUG_SOLVER_
_tm_("filter loop "+ik);
#endif			
				if(!wt->set_progress(ik)){
#if _DEBUG_SOLVER_
_tm_("user break");
#endif
					ub=true;
					break;
				}
			}
			status=obj_test(o,&good);
			if(!good||status){
				o->set_flag1(false);
/* 12/05/2009 : Le RMV prend beaucoup de temps
				if(!_objs->rmv(i)){
					status=-1;
				}
				i--;
				_ocount--;
*/
				o=NULL;
				_objs->put(i,&o);
				n--;
			}
			if(status){
#if _DEBUG_SOLVER_
_te_(status);
#endif
				break;
			}
		}
		if(wt){
			delete wt;
		}
#if _DEBUG_SOLVER_
t2=clock();
_tm_("loop : "+(int)(t2-t1));
_tm_("filter loop end");
#endif
		
		if(status){
			break;
		}

/* 12/05/2009 : beaucoup plus rapide comme ça !!!*/
		if((n>0)&&(n!=_ocount)){
bArray		tmp(sizeof(bGenericGeoElement*),n);
int			itmp=0;
			for(i=1;i<=_ocount;i++){
				_objs->get(i,&o);
				if(o!=NULL){
					itmp++;
					tmp.put(itmp,&o);
				}
			}
			_objs->reset();
bool		btmp=(*_objs)+tmp;
#pragma unused (btmp)
			_ocount=_objs->count();
		}
		else if(n<=0){
			_ocount=0;
			_objs->reset();
		}
/**/		
		
// Les geogs peuvent modifier la liste initiale
// en post-traitement au reset
#if _DEBUG_SOLVER_
_tm_("_root->reset begin");
t1=clock();
#endif
		_root->reset(this);
#if _DEBUG_SOLVER_
t2=clock();
_tm_("reset : "+(int)(t2-t1));
_tm_("_root->reset end");
#endif
		root_reset=true;

// Sélection des objets en fonction du type de sélection
		if(!ub){
#if _DEBUG_SOLVER_
t1=clock();
#endif
/* A FAIRE ICI POUR QUE LES COMPTEURS DU TYPE SOIENT OK*/
		if(_objs){
			for(i=1;i<=_objs->count();i++){
				_objs->get(i,&o);
				if((_use_masked)&&(o->masked())){
					o->setmasked(false,false);
				}
			}
		}
/**/
bool	done=false;
			switch(_selkind){
				case kSelReplace:
#if _DEBUG_SOLVER_
_tm_("kSelReplace : obj count = "+_objs->count());
#endif
					_sapp->selMgr()->flush();
					_sapp->selMgr()->add(_objs);
					done=_objs->count()>0;
// <25/05/09>
// Ajouter aux contrastes
					if(_add2cnt){
						_sapp->cntMgr()->flush();
						_sapp->cntMgr()->add(_objs);
					}
// <25/05/09>	
					break;
				case kSelAdd:
#if _DEBUG_SOLVER_
_tm_("kSelAdd : obj count = "+_objs->count());
#endif
					_sapp->selMgr()->add(_objs);
					done=_objs->count()>0;
// <25/05/09>
// Ajouter aux contrastes
					if(_add2cnt){
//						_sapp->cntMgr()->flush();
						_sapp->cntMgr()->add(_objs);
					}
// <25/05/09>					
					break;
				case kSelRemove:{
#if _DEBUG_SOLVER_
_tm_("kSelRemove : obj count = "+_objs->count());
#endif
bArray				buf(*((bArray*)_sapp->selMgr()->elements()));
int					idx,n=_objs->count();
						
						buf.sort(obj_comp);
						for(i=n;i>=1;i--){
							(void)_objs->get(i,&o);
							if(o->selected()){
								idx=buf.search(&o,obj_comp);
								if(idx>0){
									buf.rmv(idx);
								}
							}
						}
						_sapp->selMgr()->flush();
						_sapp->selMgr()->add(&buf);
						done=buf.count()>0;
// <25/05/09>
// Ajouter aux contrastes
						if(_add2cnt){
							_sapp->cntMgr()->flush();
							_sapp->cntMgr()->add(&buf);
						}
// <25/05/09>
					}
					break;
			}
			r=true;
			if(_use_masked&&done){
#if _DEBUG_SOLVER_
_tm_("use_masked");
#endif
//				_sapp->mapwd()->inval();
				_sapp->mapIntf()->inval();
			}
#if _DEBUG_SOLVER_
t2=clock();
_tm_("select : "+(int)(t2-t1));
#endif

		}
				
		break;// loop
	}

// <22/12/06>
	if(!r){
		if(_selkind==kSelReplace){
			_sapp->selMgr()->flush();			
		}
	}
// <22/12/06>

// Libération
	if(_root){
		if(!root_reset){
			_root->reset(this);
		}
		_root=NULL;
	}
	if(_objs){
		for(i=1;i<=_objs->count();i++){
			_objs->get(i,&o);
			o->set_flag1(false);
			/**/
			//if((_use_masked)&&(o->masked())){
			//	o->setmasked(false,false);
			//}
			/**/
		}
		delete _objs;
		_objs=NULL;
	}
	if(_source){
		delete _source;
		_source=NULL;
	}
	set_object(NULL);// Pour le coup suivant

// <25/05/09>
// Masquer
	if(_add2hdn){
		_sapp->layersMgr()->SetObjInvalidation(false);
		get_type()->iterator()->iterate(this,obj_mask);			
		_sapp->layersMgr()->SetObjInvalidation(true);
	}
// <25/05/09>

	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bSelectSolver::obj_test(bGenericGeoElement* o, bool* good){
bStdDBValue*	val;
OSStatus		status=noErr;
   
    *good=false;
	
	if((!_use_masked)&&(o->masked())){
		return(status);
	}
	set_object(o);
	_root->solve(this);
	if(result(&val)){
		val->get(good);
	}
	else{
		if(count()==0){
			*good=true;
		}
		else{
			status=-1;
		}
	}
	reset();
//	if((_use_masked)&&(*good)&&(o->masked())){
//		o->setmasked(false,false);
//	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::obj_count(void* o, void* prm){
bSelectSolver*		solver=(bSelectSolver*)prm;
bGenericGeoElement* geo=(bGenericGeoElement*)o;
	if((!solver->_use_masked)&&(geo->masked())){
		return(0);
	}
	solver->_ocount++;
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::obj_add(void* o, void* prm){
// filtrer sur le mask
bSelectSolver*		solver=(bSelectSolver*)prm;
bGenericGeoElement* geo=(bGenericGeoElement*)o;

	if((!solver->_use_masked)&&(geo->masked())){
		return(0);
	}	
	solver->_ocount++;
	if(!solver->_objs->put(solver->_ocount,&geo)){
		solver->_ocount--;
		solver->reset();
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
int bSelectSolver::obj_comp(const void* a, const void* b){
bGenericGeoElement* ga=(*((bGenericGeoElement**)a));
bGenericGeoElement* gb=(*((bGenericGeoElement**)b));

	if(ga>gb){
		return(1);
	}
	if(ga<gb){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bSelectSolver::obj_mask(void* o, void* prm){
bSelectSolver*		solver=(bSelectSolver*)prm;
bGenericGeoElement* geo=(bGenericGeoElement*)o;

	if(solver->_add2hdn==kAdd2Hidden){
		if(geo->selected()){
			geo->setselect(false,false);
			geo->setmasked(true,false);
		}
	}
	else if(solver->_add2hdn==kAdd2HiddenInv){
		if(!geo->selected()){
			geo->setmasked(true,false);
		}
	}
	return(0);
}
