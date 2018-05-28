//----------------------------------------------------------------------------
// File : bToolNSGeomWithJoin.cpp
// Project : MacMap
// Purpose : C++ source file : Subclass for geometry tools using join functionnalities
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 31/03/2005 creation.
// 18/03/2014 Ready for CG geometry	& drawing
// 28/03/2017 cocoa intf, and renamed from bToolGeomWithJoin to bToolNSGeomWithJoin
//----------------------------------------------------------------------------

#include "bToolNSGeomWithJoin.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>
#include <std_ext/bXMapStdIntf.h>

// ---------------------------------------------------------------------------
//
// ------------
bArray bToolNSGeomWithJoin::_gwjs(sizeof(bToolNSGeomWithJoin*));

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolNSGeomWithJoin	::bToolNSGeomWithJoin(bGenericXMLBaseElement* elt,
                                        bGenericMacMapApp* gapp,
                                        CFBundleRef bndl)
					: bStdToolGeom(elt,gapp,bndl)
					,_tp_use(sizeof(bGenericType*)){
bToolNSGeomWithJoin*   inst=this;
    _gwjs.add(&inst);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolNSGeomWithJoin::~bToolNSGeomWithJoin(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolNSGeomWithJoin::open(int* flags){
	_use_nea=false;
	_use_dir=false;
	_use_ang=false;
	_use_end=false;
	
	_k_nea=0;
	_k_dir=0;
	_k_ang=0;
	_k_end=0;
	
	bStdToolGeom::open(flags);
	init_join();
    
    _lsi=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolNSGeomWithJoin::close(){
	bStdToolGeom::close();
	_tp_use.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolNSGeomWithJoin::update(bool global){
CGPoint	a;
	get_cur(&a);
	if(a.x!=SHRT_MIN){
		hilite_cross(getTempPathContext(),a);
	}
}

// ---------------------------------------------------------------------------
//
// -----------
void bToolNSGeomWithJoin::track_obj(){
// Méthode sans raccourci clavier supplémentaire pour verrouiller sur l'objet sélectionné
bArray*				ga;
bGenericStyle*		style;
bGenericGeoElement	*o=NULL,*prev;
long				i,n=_gapp->layersAccessCtx()->count();
CGPoint				pt;
    
    get_cur(&pt);
    prev=get_obj();
    if(prev!=NULL){
        for(i=n;i>0;i--){
            style=_gapp->layersAccessCtx()->get(i);
            if(!style){
                continue;
            }
            if(!style->gettype()){
                continue;
            }
            if((style->gettype()->kind()==kBaseKindPoint)   ||
               (style->gettype()->kind()==kBaseKindText)    ){
                continue;
            }
            ga=style->objsatpoint(&pt,true);
            if(!ga){
                continue;
            }
            if(_lsi<i){// Pour permettre de sélectionner un objet dans une couche au dessus
                delete ga;
                break;
            }
            for(long j=1;j<=ga->count();j++){
                ga->get(j,&o);
                if(o==prev){// Pour rester vérouillé sur l'objet en cours
                    delete ga;
                    return;
                }
            }
            delete ga;
        }
    }
    
// Pas de précédent retrouvé, on prend la méthode normale
    for(i=n;i>0;i--){
        style=_gapp->layersAccessCtx()->get(i);
        if(!style){
            continue;
        }
        if(!style->gettype()){
            continue;
        }
        if((style->gettype()->kind()==kBaseKindPoint)   ||
           (style->gettype()->kind()==kBaseKindText)    ){
            continue;
        }
        ga=style->objsatpoint(&pt,false);
        if(!ga){
            continue;
        }
        if(ga->count()>0){
            ga->get(1,&o);
            delete ga;
            if(o!=prev){
                set_obj(o);
                _lsi=i;
            }
            break;
        }
        delete ga;
    }
    if(o==NULL){
        _lsi=0;
        set_obj(NULL);
    }
    
// Verrouillage par touche sur la sélection
// pour éviter les changements d'objets intempestifs
//    if((get_obj()!=NULL)&&is_modifiers(controlKey)){
//        return;
//    }
//    bStdTool::track_obj();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolNSGeomWithJoin::track_join(){
i2dvertex	br;
    if(get_join(&br)){
		set_cur(&br);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolNSGeomWithJoin::init_join(){
    _jnea=_gapp->xmapMgr()->find('JNea');//nearest
    _jdir=_gapp->xmapMgr()->find('JDir');//dir
    _jang=_gapp->xmapMgr()->find('JAng');//vertex
    _jend=_gapp->xmapMgr()->find('JEnd');//ends

    if(_jnea==NULL){
        _use_nea=false;
    }
    if(_jdir==NULL){
        _use_dir=false;
    }
    if(_jang==NULL){
        _use_ang=false;
    }
    if(_jend==NULL){
        _use_end=false;
    }
	
	set_use_join((_jnea!=NULL)||(_jdir!=NULL)||(_jang!=NULL)||(_jend!=NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolNSGeomWithJoin::get_join(i2dvertex* res){
i2dvertex		br,r;
double			bd=LONG_MAX,d=LONG_MAX;
bGenericType*	tp;

	for(long i=1;i<=_gapp->typesMgr()->count();i++){
        if(_tp_use.get(i,&tp)==false){
			continue;
		}
		if(tp==NULL){
			continue;
		}
		if((get_join(i,&r,&d))&&(d<bd)){
			bd=d;
			br=r;
		}
	}
	if(bd!=LONG_MAX){
		*res=br;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolNSGeomWithJoin::get_join(int tidx, i2dvertex* res, double* d){
bGenericGeoIterator*	it=_gapp->typesMgr()->get(tidx)->iterator();
	if(!it){
		return(false);
	}

proc_prm	p;
i2dvertex	vx;
double		dm=get_dmax();
double		di=Measure_d2i(_gapp,dm);
	if(dm<=0){
		return(false);
	}
	get_cur(&vx);
	p.t=this;
	p.d=LONG_MAX;
	p.b.top=vx.v-round(di);
	p.b.bottom=vx.v+round(di);
	p.b.left=vx.h-round(di);
	p.b.right=vx.h+round(di);
	p.r=vx;

	it->iterate(&p.b,&p,proc);
	if(p.d<LONG_MAX){
		*res=p.r;
		*d=p.d;
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolNSGeomWithJoin::get_join(bGenericGeoElement* o, i2dvertex* res, double* d){
join_prm	p;
bool		found=false;
	
	*d=LONG_MAX;
	o->getVertices(&p.ref);
	get_cur(&p.vx);
	get_clic(&p.prev);

	if(_jnea&&_use_nea){
		if((!_k_nea)||((_k_nea)&&(is_key(_k_nea)))){
			p.dmax=-1;
			if(_jnea->process(kExtProcessCallWithParams,&p)){
				*res=p.res;
				*d=p.d;
				found=true;
			}
		}
	}
	
	if(_jdir&&_use_dir){
		if((!_k_dir)||((_k_dir)&&(is_key(_k_dir)))){
			p.dmax=-1;
			if((_jdir->process(kExtProcessCallWithParams,&p))&&(p.d<*d)){
				*res=p.res;
				*d=p.d;
				found=true;
			}
		}
	}
	
	if(_jang&&_use_ang){
		if((!_k_ang)||((_k_ang)&&(is_key(_k_ang)))){
			p.dmax=-1;
			if((_jang->process(kExtProcessCallWithParams,&p))&&(p.d<*d)){
bGenericType*   tp=_gapp->typesMgr()->get(o->getType());
// Controle pour les linéaires, pour éviter les rabbatements sur les extrémités : utile ?
                if(tp->kind()==kBaseKindPolyline){
i2dvertex*      vx;
int             nv;
bool            bad=false;
                    for(long i=0;i<ivs_n_parts(p.ref);i++){
                        vx=ivs2_part(p.ref,i,&nv);
                        if(eq_ivx2(&p.res,&vx[0])    ||
                           eq_ivx2(&p.res,&vx[nv-1]) ){
                            bad=true;
                            break;
                        }
                    }
                    if(!bad){
                        *res=p.res;
                        *d=p.d;
                        found=true;
                    }
                }
                else{
                    *res=p.res;
                    *d=p.d;
                    found=true;
                }
			}
		}
	}
	
	if(_jend&&_use_end){
		if((!_k_end)||((_k_end)&&(is_key(_k_end)))){
			p.dmax=-1;
			if((_jend->process(kExtProcessCallWithParams,&p))&&(p.d<*d)){
				*res=p.res;
				*d=p.d;
				found=true;
			}
		}
	}
	
	return(found);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolNSGeomWithJoin::get_dmax(){
join_prm	p;
double		d=0;
	
    if(_jnea){
		(void)_jnea->process(kExtProcessCallGetData,&p);
		if(p.dmax>d){
			d=p.dmax;
		}
	}
	if(_jdir){
		(void)_jdir->process(kExtProcessCallGetData,&p);
		if(p.dmax>d){
			d=p.dmax;
		}
	}
	if(_jang){
		(void)_jang->process(kExtProcessCallGetData,&p);
		if(p.dmax>d){
			d=p.dmax;
		}
	}
	if(_jend){
		(void)_jend->process(kExtProcessCallGetData,&p);
		if(p.dmax>d){
			d=p.dmax;
		}
	}
	
	return(d);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolNSGeomWithJoin::load(){
bGenericXMLBaseElement* groot=bStdToolGeom::load();
bGenericXMLBaseElement* elt=NULL;
bGenericType*			tp=NULL;
long					k=1;

	_use_nea=false;
	_k_nea=0;
	_use_dir=false;
	_k_dir=0;
	_use_ang=false;
	_k_ang=0;
	_use_end=false;
	_k_end=0;
	
	if(!groot){
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"join");
	if(!root){
		return(groot);
	}
char					val[_values_length_max_];
	for(k=1;k<=_gapp->typesMgr()->count();k++){
		_tp_use.add(&tp);
	}
	
	k=1;
	for(;;){
		elt=_gapp->classMgr()->NthElement(root,1,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_use_nea=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,2,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_use_dir=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,3,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_use_ang=atoi(val);
		
		elt=_gapp->classMgr()->NthElement(root,4,"bool");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_use_end=atoi(val);

		elt=_gapp->classMgr()->NthElement(root,1,"string");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_k_nea=val[0];
		
		elt=_gapp->classMgr()->NthElement(root,2,"string");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_k_dir=val[0];
		
		elt=_gapp->classMgr()->NthElement(root,3,"string");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_k_ang=val[0];
		
		elt=_gapp->classMgr()->NthElement(root,4,"string");
		if(!elt){
			break;
		}
		elt->getvalue(val);
		_k_end=val[0];
		
		while((elt=_gapp->classMgr()->NthElement(root,k,"name"))){
			elt->getvalue(val);
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(val));
			_tp_use.put(_gapp->typesMgr()->index(val),&tp);
			k++;
		}
		
		break;
	}

    if(!_gapp->xmapMgr()->find('JNea')){
        _use_nea=false;
        _k_nea=0;
    }
    if(!_gapp->xmapMgr()->find('JDir')){
        _use_dir=false;
        _k_dir=0;
    }
    if(!_gapp->xmapMgr()->find('JAng')){
        _use_ang=false;
        _k_ang=0;
    }
    if(!_gapp->xmapMgr()->find('JEnd')){
        _use_end=false;
        _k_end=0;
    }

	return(groot);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolNSGeomWithJoin::make(bArray &arr){
char			val[256];
bGenericType*	tp;
	
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","join");
	add_idesc(arr,2,"bool",_use_nea);
	add_idesc(arr,2,"bool",_use_dir);
	add_idesc(arr,2,"bool",_use_ang);
	add_idesc(arr,2,"bool",_use_end);
	val[1]=0;
	val[0]=_k_nea;
	add_cdesc(arr,2,"string",val);
	val[0]=_k_dir;
	add_cdesc(arr,2,"string",val);
	val[0]=_k_ang;
	add_cdesc(arr,2,"string",val);
	val[0]=_k_end;
	add_cdesc(arr,2,"string",val);
	for(int i=1;i<=_tp_use.count();i++){
		_tp_use.get(i,&tp);
		if(tp){
			tp->name(val);
			add_cdesc(arr,2,"name",val);
		}
	}
	return(bStdToolGeom::make(arr));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bToolNSGeomWithJoin::proc(void* o, void* prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
proc_prm*			p=(proc_prm*)prm;
double				d=LONG_MAX;
i2dvertex			r;
ivx_rect			b,dummy;
	
	if(geo->masked()){
		return(0);
	}
	geo->getBounds(&b);
	if(!ivr_sect(&b,&p->b,&dummy)){
		return(0);
	}
	if(	(p->t->get_join(geo,&r,&d))	&&
		(d<p->d)					){
		p->d=d;
		p->r=r;
	}
	return(0);
}

// ---------------------------------------------------------------------------
//
// -----------
void bToolNSGeomWithJoin::putKeys(){
bToolNSGeomWithJoin*	tool;
    for(long i=1;i<=_gwjs.count();i++){
        _gwjs.get(i,&tool);
        tool->set_use_nea_key(_k_nea);
        tool->set_use_dir_key(_k_dir);
        tool->set_use_ang_key(_k_ang);
        tool->set_use_end_key(_k_end);
        tool->save();
    }
}

