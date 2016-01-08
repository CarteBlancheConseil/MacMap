//----------------------------------------------------------------------------
// File : bXMapAssociationWithField.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, concatenate polylines geometries of a network comparing content of a field and class
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
// 22/12/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapAssociationWithField.h"
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <xmap_lib/bXMapAssociation.h>
#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct check_dist_prm{
	bArray&				res;
	i2dvertex			vx;
	
	int					aclss;
	char				aname[256];
	
	bGenericGeoElement*	oa;
	bGenericGeoElement*	ob;

	int					hfld;
	int					nfld;
	
	double				lmin;
	double				lmax;
	double				lcur;
	
}check_dist_prm;

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociationWithField::_sfhier=0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapAssociationWithField	::bXMapAssociationWithField(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
							:bStdXMapWithModalIntf(elt,gapp,bndl){
	setclassname("associationwithfield");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapAssociationWithField::~bXMapAssociationWithField(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapAssociationWithField::create(bGenericXMLBaseElement* elt){
	return(new bXMapAssociationWithField(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAssociationWithField::open(int* flags){	
	bStdXMapWithModalIntf::open(flags);
	_prm.lmin=10;
	_prm.lmax=10000;
	_prm.needhier=true;
	_prm.field=1;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociationWithField::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(associate());
			break;
		case kExtProcessCallWithParams:{
associationwithfield_prm*	p=(associationwithfield_prm*)prm;
				return(	associate(	p->needhier?kOBJ_SubType_:0,
									p->field,
									Measure_d2i(_gapp,p->lmin),
									Measure_d2i(_gapp,p->lmax),
									true));
			}
			break;
			case kExtProcessCallWithXMLTree:{
				if(!test(prm)){
					return(false);
				}
char						val[_values_length_max_];
bGenericXMLBaseElement*		elt;
associationwithfield_prm	p;
bGenericGeoElement*			geo;
bGenericType*				tp;
				
				_gapp->selMgr()->elements()->get(1,&geo);
				tp=_gapp->typesMgr()->get(geo->getType());
				
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.field=tp->fields()->get_index(val);
				if(!p.field){
					return(false);
				}

				elt=getelement(2);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.needhier=atoi(val);

				elt=getelement(3);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.lmin=atof(val);
				if(p.lmin<0){
					return(false);
				}
				
				elt=getelement(4);
				if(!elt){
					return(false);
				}
				elt->getvalue(val);
				p.lmax=atof(val);
				if(p.lmax<=p.lmin){
					return(false);
				}
				return(	associate(	p.needhier?kOBJ_SubType_:0,
									p.field,
									Measure_d2i(_gapp,p.lmin),
									Measure_d2i(_gapp,p.lmax),
									true));
			}	
			break;

	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociationWithField::test(void* prm){
int	k=SelectionIsMonoType(_gapp);
	if(!k){
		return(false);
	}
	k=SelectionIsMonoGender(_gapp);
	return((k==kBaseKindPolyline)&&(_gapp->selMgr()->count()>1));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociationWithField::wd_event(EventRef evt, WindowRef wd){
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
void bXMapAssociationWithField::wd_init(WindowRef wd){
ControlRef			c;
char				val[256];
bGenericGeoElement* o;
	_gapp->selMgr()->elements()->get(1,&o);
bGenericType*		tp=_gapp->typesMgr()->get(o->getType());

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldFieldID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithFields(tp,c,kOBJ_Name_,1);
	for(int i=2;i<=4;i++){
		DisablePopupControlMenuItem(c,i);
	}
	_prm.field=1;
	SetControlValue(c,_prm.field);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldHierarID);
	SetControlValue(c,_prm.needhier);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMinLengthID);	
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.lmin);
	SetTextControlValue(c,val);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMaxLengthID);	
	SetTextControlFilter(c,ufloat_filter);
	sprintf(val,"%.*f",_gapp->document()->dist_pref_digits(),_prm.lmax);
	SetTextControlValue(c,val);

bGenericUnit*		u=_gapp->distMgr()->get();
	u->short_name(val);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMinUnitID);
	SetTextControlValue(c,val);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMaxUnitID);
	SetTextControlValue(c,val);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociationWithField::associate(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapAssociationWithFieldMessageID,
				GetSignature(this));
	associate(_prm.needhier?kOBJ_SubType_:0,_prm.field-1+kOBJ_Name_,Measure_d2i(_gapp,_prm.lmin),Measure_d2i(_gapp,_prm.lmax),false);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapAssociationWithField::associate(	int fhier,
											int fass,
											double lmin,
											double lmax,
											bool silent){
_bTrace_("bXMapAssociationWithField::associate",true);
bArray					sel(*(bArray*)_gapp->selMgr()->elements());
bArray					remains(sizeof(bGenericGeoElement*));
bGenericGeoElement		*oa,*ob;
ivx_rect				bounds,vxr;
int						status=noErr;
ivertices				*vxsa,*vxsb,*vxsr;
bGenericGeoIterator*	iter=NULL;
char					msg[__MESSAGE_STRING_LENGTH_MAX__];
int						i,count=0,clss;
char					name[256];
bGenericType*			tp;
bool					asso;
bool					b;
#pragma unused(b)
    
	_sfhier=fhier;
	
	message_string(kMsgProgress,msg,1);
bProgressWait wt("",msg,true,true,sel.count()*3);
	
	_gapp->layersMgr()->SetObjInvalidation(false);
	
	FlushSelection(_gapp);
	FlushContrasts(_gapp);

	sel.get(1,&oa);
	oa->getBounds(&bounds);

//_tm_("determining bounds");
	tp=_gapp->typesMgr()->get(oa->getType());
	sel.sort(obj_comp);
	
	
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&ob);
		ob->getBounds(&vxr);
		ivr_union(&bounds,&vxr,&bounds);
	}
	
	for(;;){
gi_init_p	gp={&bounds,&status,SetFlagWrapper,GetFlagWrapper,GetBoundsWrapper};

//_tm_("indexing");
		iter=(bGenericGeoIterator*)(_gapp->extMgr()->get_component(kComponentGeoIterator)->i_allocate(_kNULLSign_,_gapp,&gp));
		if(!iter){
			status=1;
_te_("gIterator not allocated");
			break;
		}
		for(i=1;i<=sel.count();i++){
			if(!wt.set_progress(i)){
//_tm_("user break");
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

// Première boucle sans condition de longueur mini
		for(i=1;i<=sel.count();){
			if(!wt.set_progress(count++)){
				status=-1;
				break;
			}

			asso=false;
			sel.get(i,&oa);
			iter->rmv(oa);

			oa->getVertices(&vxsa);
			oa->getValue(fass,name);
			if(fhier){
				oa->getValue(fhier,&clss);
			}
			else{
				clss=1;
			}

//_tm_("1 processing "+sel.count()+"->"+name+"->"+clss);

// Dans le sens direct
			status=get_lines(oa,vxsa,vxsa->nv-1,clss,name,iter,sel,fhier,fass,0,lmax,&ob);
			if(status){
_te_("1 get_lines="+status);
				break;
			}
			if(ob){
				ob->getVertices(&vxsb);
				vxsr=ivs_associate_as_line(vxsa,vxsb,0);
				if(!vxsr){
_te_("1 ivs_associate_as_line failed");
					status=5;
					break;
				}
				asso=true;
				tp->kill_object(ob);
				oa->setVertices(vxsr);
				ivs_free(vxsr);
				oa->getVertices(&vxsa);// Pour le coup suivant
//_tm_("1 associated dir");
			}
			
// Dans le sens inverse
			status=get_lines(oa,vxsa,0,clss,name,iter,sel,fhier,fass,0,lmax,&ob);
			if(status){
_te_("1 get_lines="+status);
				break;
			}
			if(ob){
				ob->getVertices(&vxsb);
				vxsr=ivs_associate_as_line(vxsa,vxsb,0);
				if(!vxsr){
_te_("1 ivs_associate_as_line failed");									
					status=5;
					break;
				}
				asso=true;
				tp->kill_object(ob);
				oa->setVertices(vxsr);
				ivs_free(vxsr);
//_tm_("1 associated inv");
			}
			
			if(iter->add(oa)==false){
_te_("1 iter->add(oa) failed");				
			}
			if(!asso){
//_tm_("1 removing "+name); 
				sel.rmv(i);
			}
			
			oa->getVertices(&vxsa);
			if(ivs_length(vxsa)<lmin){
				remains.add(&oa);
			}
		}
		
		if(status){
			break;
		}

		sel.reset();
		b=sel+remains;
		remains.reset();
		sel.sort(obj_comp);
				
// Seconde boucle pour les tronçons dont la longueur < mini
		for(i=1;i<=sel.count();){
			if(!wt.set_progress(count++)){
				status=-1;
				break;
			}

			asso=false;
			sel.get(i,&oa);
			iter->rmv(oa);

			oa->getVertices(&vxsa);
			oa->getValue(fass,name);
			if(fhier){
				oa->getValue(fhier,&clss);
			}
			else{
				clss=1;
			}

//_tm_("2 processing "+(int)sel.count()+"->"+name+"->"+clss);

// Dans le sens direct
			status=get_lines(oa,vxsa,vxsa->nv-1,clss,name,iter,sel,fhier,fass,lmin,lmax,&ob);
			if(status){
_te_("2 get_lines="+status);
				break;
			}
			if(ob){
				ob->getVertices(&vxsb);
				vxsr=ivs_associate_as_line(vxsa,vxsb,0);
				if(!vxsr){
_te_("2 ivs_associate_as_line failed");
					status=5;
					break;
				}
				asso=true;
				tp->kill_object(ob);
				oa->setVertices(vxsr);
				ivs_free(vxsr);
				oa->getVertices(&vxsa);// Pour le coup suivant
//_tm_("2 associated dir");
			}
			
// Dans le sens inverse
			if(ivs_length(vxsa)<lmin){

				status=get_lines(oa,vxsa,0,clss,name,iter,sel,fhier,fass,lmin,lmax,&ob);
				if(status){
_te_("2 get_lines="+status);
					break;
				}
				if(ob){
					ob->getVertices(&vxsb);
					vxsr=ivs_associate_as_line(vxsa,vxsb,0);
					if(!vxsr){
_te_("2 ivs_associate_as_line failed");									
						status=5;
						break;
					}
					asso=true;
					tp->kill_object(ob);
					oa->setVertices(vxsr);
					ivs_free(vxsr);
					oa->getVertices(&vxsa);// Pour le coup suivant
//_tm_("2 associated inv");
				}
			}

			if(iter->add(oa)==false){
_te_("2 iter->add(oa) failed");				
			}
			if((!asso)||(ivs_length(vxsa)>=lmin)){
//_tm_("2 removing "+name); 
				sel.rmv(i);
			}
		}
		
		break;
		
	}
			
	if(iter){
		_gapp->extMgr()->get_component(kComponentGeoIterator)->i_free(iter);
	}
	if(sel.count()>0){
		_gapp->cntMgr()->add(&sel);
	}
	
	_gapp->layersMgr()->SetObjInvalidation(true);

	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapAssociationWithField::puts(){
char				val[256];
ControlRef			c;

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldFieldID);
	_prm.field=GetControl32BitValue(c);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldHierarID);
	_prm.needhier=GetControl32BitValue(c);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMinLengthID);	
	GetTextControlValue(c,val,sizeof(val)-1);
	_prm.lmin=atof(val);

	c=get_control(kXMapAssociationWithFieldSignature,kXMapAssociationWithFieldMaxLengthID);	
	GetTextControlValue(c,val,sizeof(val)-1);
	_prm.lmax=atof(val);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociationWithField::get_lines(	bGenericGeoElement* oa,		// Ref object
											ivertices* vxsa,			// Ref vertices
											int loc,					// Point to check (0 or vxsa->nv-1)
											int	aclss,					// Ref class
											char* aname,				// Ref name
											
											bGenericGeoIterator* iter,	// Iterator
											bArray& sel,				// Obj List (from selection)
											
											int hfld,					// Class Field
											int nfld,					// Name Field
											
											double	lmin,				// Min length
											double	lmax,				// Max length
											
											bGenericGeoElement** ob){	// Next object
_bTrace_("bXMapAssociationWithField::get_lines",false);
ivx_rect			vxr;
int					status=0;
int					idx;
bArray				res(sizeof(bGenericGeoElement*));
check_dist_prm		prm={res,vxsa->vx.vx2[loc],aclss,"",oa,NULL,hfld,nfld,lmin,lmax,ivs_length(vxsa)};
ivertices*			vxsb;

	strcpy(prm.aname,aname);
	
	*ob=NULL;
	for(;;){
		vxr.left=vxr.right=vxsa->vx.vx2[loc].h;
		vxr.top=vxr.bottom=vxsa->vx.vx2[loc].v;
		iter->iterate(&vxr,&prm,check_line_dist);
// Test de cohérence
		if(res.count()>1){
// Trop d'objets
//			status=3;
//_tm_("too much objects");
			break;
		}			
		if(res.count()==0){
// Fin de traitement
//_tm_("no object");
			break;
		}
// Retrait du candidat des structures			
		res.get(1,ob);
		(*ob)->getVertices(&vxsb);
// Test de dépassement de la longueur max
		if((ivs_length(vxsb)+ivs_length(vxsa))>lmax){
//_tm_("out of length");
			(*ob)=NULL;
			break;
		}
		idx=sel.search(ob,obj_comp);
		sel.rmv(idx);
		if(!iter->rmv(*ob)){
_te_("iter->rmv failed");
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
int bXMapAssociationWithField::check_line_dist(void *o, void *prm){
//_bTrace_("bXMapAssociation::check_dist",false);
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
check_dist_prm*		p=(check_dist_prm*)prm;
ivertices*			vxsb;
char				name[256];
int					clss=1;
	geo->getVertices(&vxsb);
int					p0=eq_ivx2(&p->vx,&(vxsb->vx.vx2[0]));
int					pn=eq_ivx2(&p->vx,&(vxsb->vx.vx2[vxsb->nv-1]));
	
	if((p0)&&(pn)){
		return(0);
	}
	if((!p0)&&(!pn)){
		return(0);
	}

	if(p->hfld){
		geo->getValue(p->hfld,&clss);
	}
	
	geo->getValue(p->nfld,name);

//_tm_("length ="+p->lcur+" for "+p->lmin);

	if(p->lcur<p->lmin){
//_tm_("length ("+p->lcur+" < "+p->lmin);
		if(strcmp(p->aname,name)){
//_tm_("bad name ("+name+" for "+p->aname+")");
			return(0);
		}	
	}
	else if(p->aclss>clss){// Si on rencontre un axe plus important, pas d'association possible
//_tm_("bad class ("+clss+" for "+p->aclss+")");
		p->res.reset();// Pas d'association possible
		return(1);// Pour arrêter l'itération
	}
	else if(p->aclss==clss){// Si on rencontre un axe de même importance et de nom différent, pas d'association possible
		if(strcmp(p->aname,name)){
//_tm_("bad name ("+name+" for "+p->aname+")");
			//return(0);
			p->res.reset();// Pas d'association possible
			return(1);// Pour arrêter l'itération
		}
	}
	else{// Si le nom est différent, on passe au suivant
		if(strcmp(p->aname,name)){
//_tm_("bad name ("+name+" for "+p->aname+")");
			return(0);
		}
	}
	
	p->res.add(&geo);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapAssociationWithField::obj_comp(const void* a,
										const void* b){
bGenericGeoElement*	oa=*((bGenericGeoElement**)a);
bGenericGeoElement*	ob=*((bGenericGeoElement**)b);
int					ia,ib;

	if(_sfhier>0){
		oa->getValue(_sfhier,&ia);
		ob->getValue(_sfhier,&ib);
		if(ia==ib){
			return(oa-ob);		
		}
		return(ib-ia);
	}
	return(oa-ob);
}

