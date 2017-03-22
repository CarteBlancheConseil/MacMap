//----------------------------------------------------------------------------
// File : bStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Style class
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
// 02/02/2004 creation.
// 27/03/2007 fonctionne avec les tris inverses
// 17/04/2007 prise en compte des false retournés dans les procédures applyfor…
// 04/09/2014 elimination des appels avec geometrie QD.
//----------------------------------------------------------------------------

#include "bStyle.h"
#include "bScreenObj.h"
#include "bStyleRun.h"
#include "bClassesMgr.h"
#include "bMacMapApp.h"
#include "bMacMapGeoElement.h"
#include "bLayersMgr.h"

#include <mox_intf/bGenericXMLRenderingElement.h>
#include <mox_intf/bGenericvDef.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Carb_Utils.h>

#include <MacMapSuite/bStdDirectory.h>	
#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>	

// ---------------------------------------------------------------------------
// 
// ------------

#define _ltrace_	0
#if(_ltrace_==1)
#define	_lbTrace_(a,b)	_bTrace_(a,b)
#define	_lm_(a)			_tm_(a)
#define	_le_(a)			_te_(a)
#define	_lw_(a)			_tw_(a)
#else
#define	_lbTrace_(a,b)	//_bTrace_(a,b)
#define	_lm_(a)			//_tm_(a)
#define	_le_(a)			//_te_(a)
#define	_lw_(a)			//_tw_(a)
#endif

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyle::bStyle(){
	_ctx=NULL;
	
	_typename=NULL;
	_typeindex=0;
	_sign=0;
	_name=NULL;
	_vis=true;
	_sel=true;
	_lock=true;

	_fieldname=NULL;
	_fieldindex=0;
	_nbpass=1;
	_scaleref=0;
	_unitcoef=1;
	_sort=1;
	
	_drawmethod=kNoStyleKind_;
	_drawp=drawNothing;
	_distp=distToNothing;
	_boundp=boundsForNothing;
			
	_screenobjs=new bArray(sizeof(bScreenObj*));
	_valbounds=new bArray(sizeof(double));
	_valnames=new bArray(sizeof(char*));
	_offsets=new bArray(sizeof(int));
	_counts=new bArray(sizeof(int));
	
	_root=NULL;
	_identifier=NULL;
	_globals=NULL;
	_condition=NULL;
	
	_styleruns=new bArray(sizeof(bStyleRun*));
	_goodstyleruns=new bArray(sizeof(bStyleRun*));
	
	_curstylerun=NULL;
	_curclass=1;
	_margin=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bStyle::~bStyle(){
int	k=typesmgr->index(_typename);
	if(k>0){
bGenericType* tp=typesmgr->get(k);
		k=tp->styles()->index(_name);
		if(k>0){
			tp->styles()->set_on_screen(k,false);
		}	
	}
	
	if(_typename){
		free(_typename);
	}
	if(_name){
		free(_name);
	}
	if(_fieldname){
		free(_fieldname);
	}
	/*if(_vers){
		free(_vers);
	}*/
	if(_screenobjs){
		flushscreen();
		delete _screenobjs;
	}
	if(_valbounds){
		delete _valbounds;
	}
	if(_valnames){
char*	nm;
		for(int i=1;i<=_valnames->count();i++){
			_valnames->get(i,&nm);
			if(nm){
				free(nm);
			}
		}
		delete _valnames;
	}
	if(_offsets){
		delete _offsets;
	}
	if(_counts){
		delete _counts;
	}
	/*if(_root){
		bGenericXMLBaseElement*	k=_root->instance();
		k->kill(_root);
	}*/
	if(_identifier){
	}
	if(_globals){
	}
	if(_condition){
	}
	if(_styleruns){
		flushstyleruns();
		delete _styleruns;
	}
	if(_goodstyleruns){
		delete _goodstyleruns;
		_goodstyleruns=NULL;
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setglobals(void *elt){
	_globals=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setidentification(void *elt){
	_identifier=(bGenericXMLBaseElement*)elt;
bGenericXMLBaseElement*	chld=clssmgr->NthElement(_identifier,1,"sign");
char					val[_values_length_max_]="";
	if(chld){
		chld->getvalue(val);
		_sign=(*((int*)val));
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setcondition(void *elt){
	_condition=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setdrawingmethod(int k){
	switch(k){
		case kIconStyleKind_:
			_drawp=drawIcon;
			_distp=distToIcon;
			_boundp=boundsForIcon;
			break;
		case kCircleStyleKind_:
			_drawp=drawCircle;
			_distp=distToCircle;
			_boundp=boundsForCircle;
			break;
		case kRectStyleKind_:
			_drawp=drawRect;
			_distp=distToRect;
			_boundp=boundsForRect;
			break;
		case kTextStyleKind_:
			_drawp=drawText;
			_distp=distToText;
			_boundp=boundsForText;
			break;
		case kLineStyleKind_:
			_drawp=drawLine;
			_distp=distToLine;
			_boundp=boundsForLine;
			break;
		case kSurfStyleKind_:
			_drawp=drawSurf;
			_distp=distToSurf;
			_boundp=boundsForSurf;
			break;
		case kRasterStyleKind_:
			_drawp=drawRaster;
			_distp=distToRaster;
			_boundp=boundsForRaster;
			break;
		default:
			_drawp=drawNothing;
			_distp=distToNothing;
			_boundp=boundsForNothing;
			break;
	}
	_drawmethod=k;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setclassfield(const char *name){
	if(_fieldname){
		free(_fieldname);
		_fieldname=NULL;
	}
	_fieldname=strdup(name);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setpasscount(int n){
	if(n>0){
		_nbpass=n;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setscaleref(double d){
	if(d>0){
		_scaleref=d;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setunitcoef(double d){
	if(d>0){
		_unitcoef=d;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setsortkind(int k){
	if((k>-3)&&(k<3)){
		_sort=k;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::addclassbound(double d){
	if(!_valbounds->add(&d)){
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::addclassname(const char* name){
char*	nm=strdup(name);
	if(!_valnames->add(&nm)){
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::addstylerun(void *elt){
	_curstylerun=new bStyleRun();	
	if(!_curstylerun){
		return;
	}
	_curstylerun->setroot(elt);
	if(!_styleruns->add(&_curstylerun)){
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunvalidity(void *elt){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setvalidity(elt);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunrender(void *elt){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setrender(elt);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrungeometry(void *elt){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setgeometry(elt);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunscalemin(double d){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setscalemin(d);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunscalemax(double d){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setscalemax(d);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunclassmin(int idx){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setclassmin(idx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunclassmax(int idx){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setclassmax(idx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setrunpass(int idx){
	if(!_curstylerun){
		return;
	}
	_curstylerun->setpass(idx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::settype(const char *name){
	if(_typename){
		free(_typename);
		_typename=NULL;
	}
	_typename=strdup(name);
	_typeindex=typesmgr->index(_typename);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setname(const char *name){
	if(_name){
		free(_name);
		_name=NULL;
	}
	_name=strdup(name);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setvisible(bool vis){
	_vis=vis;
    if(bMacMapGeoElement::get_inval()){
        _MMAPP_->mapIntf()->inval();
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setselectable(bool sel){
	_sel=sel;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setroot(void* elt){
	_root=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setlayer(void* elt){
//_bTrace_("bStyle::setlayer",true);
//_tm_((void*)elt);
	_layer=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setmargin(int margin){
	if(margin>=0){
		_margin=margin;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStyle::getlayer(){
	return(_layer);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setbounds(ivx_rect* bounds){
_lbTrace_("bStyle::setbounds",false);
	if(!_vis){
		return;
	}
	
/* CONDITION 20/01/09*/	
_lm_("init");
	_scale=scalemgr->get()->coef();
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

_lm_("applyglobals");
	if(!applyglobals()){
		return;
	}
/**/
	
	_nobjects=0;
	_area=(*bounds);
	
bGenericType* tp=typesmgr->get(_typeindex);
	if(!tp){
		return;
	}
	_fieldindex=tp->fields()->get_index(_fieldname);
	
	_lock=true;

/**/
	_area.left-=_margin;
	_area.right+=_margin;
	_area.top-=_margin;
	_area.bottom+=_margin;
/**/
	
//_tm_("find");
	tp->iterator()->iterate(&_area,this,find);
	if(_nobjects==0){
		_lock=false;
/**/
		_area=(*bounds);
/**/
		return;
	}
	_screenobjs=new bArray(sizeof(bScreenObj*),_nobjects);
	_nobjects=0;
//_tm_("fill");
	tp->iterator()->iterate(&_area,this,fill);
	if(_nobjects==0){
		_lock=false;
/**/
		_area=(*bounds);
/**/
		return;
	}
/**/
	_area=(*bounds);
/**/
	
/* CONDITION 20/01/09*/
int	i;
	if(_nobjects!=_screenobjs->count()){
		for(i=_screenobjs->count();i>_nobjects;i--){
			_screenobjs->rmv(i);
		}
	}	
/**/

	switch(_sort){
		case -2:
			_screenobjs->sort(classCompInv);
			break;
		case -1:
			_screenobjs->sort(fullCompInv);
			break;
		case 1:
			_screenobjs->sort(fullComp);
			break;
		case 2:
			_screenobjs->sort(classComp);
			break;
	}
	
int			n=_valbounds->count();
int			c=0;
int			k/*,k1*/;
bScreenObj*	o;
	
	k=0;
	for(i=1;i<=n;i++){
		_offsets->add(&k);
		_counts->add(&k);
	}
	
	k=0;
//	k1=0;
	if(_sort>0){
		for(i=1;i<=_screenobjs->count();i++){
			_screenobjs->get(i,&o);
			if(k!=o->getclass()){
				_offsets->put(o->getclass(),&i);
				_counts->put(k,&c);
				c=0;
				k=o->getclass();
			}
			c++;
		}
		_counts->put(k,&c);
	}
	else if(_sort<0){
		for(i=_screenobjs->count();i>0;i--){
			_screenobjs->get(i,&o);
			if(k!=o->getclass()){
				_offsets->put(o->getclass(),&i);
				_counts->put(k,&c);
				c=0;
				k=o->getclass();
			}
			c++;
		}
		_counts->put(k,&c);
	}
	else{
		i=1;
		k=_screenobjs->count();;
		_offsets->put(1,&i);
		_offsets->put(2,&k);
		_counts->put(1,&k);
	}
		
	_lock=false;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::setcontext(bGenericGraphicContext* ctx){
	_ctx=(bGenericGraphicContext*)ctx;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::load(){
_bTrace_("bStyle::load",true);
_tm_("loading "+_name);
int	i=typesmgr->index(_typename);
	if(i<=0){
_te_("bad type name "+_typename+", idx="+i);
		return(false);
	}
	
bGenericType* tp=typesmgr->get(i);

	i=tp->styles()->index(_name);
	if(i<=0){
_te_("bad style name "+_name+" (idx="+i+"), trying with first");
		i=1;
	}
	_root=tp->styles()->get_root(i);
	
	tp->styles()->set_on_screen(i,true);
	
	if(!_root){
_te_("no root");
		return(false);
	}
	
	_root->init(this);
	
	_valbounds->sort(boundsComp);
	if(!_valbounds->get(1,&_minbound)){
		_minbound=0;
	}
	if(!_valbounds->get(_valbounds->count(),&_maxbound)){
		_maxbound=0;
	}
	if((_valbounds->count()<2)||(_minbound==_maxbound)){
		_sort=0;
	}

double	d;
	for(i=1;i<=_valbounds->count();i++){
		_valbounds->get(i,&d);
_tm_("bound number "+i+" = "+d);		
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::save(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::draw(bStdWait& wt){
_lbTrace_("bStyle::draw",true);
_lm_("drawing "+_name);

	if(!_vis){
		return;
	}
	
long			i,j,ri=1,k,a,b,nbounds=_valbounds->count();
unsigned int	t1,t0=EventTimeToTicks(GetCurrentEventTime());
bScreenObj*		scr;

float			xa,xb,ya,yb;
float			xxa,xxb,yya,yyb;

	_scale=scalemgr->get()->coef();
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

_lm_("applyglobals");
	if(!applyglobals()){
		return;
	}

bLayersMgr*	mgr=(bLayersMgr*)layersmgr;
	
	for(i=1;i<=_nbpass;i++){
		if(mgr->breaked()){
_lw_("mgr->breaked() 1");
			break;
		}
		if(_sort>=0){
// ********************
// TRI CROISSANT
// ********************
			for(j=1;j<=nbounds-1;j++){
				if(mgr->breaked()){
					break;
				}
				_ctx->reset();
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				a=first(j);
				b=last(j);
				if((a==0)||(b==0)){
					continue;
				}
				if(b<a){
					b=first(j);
					a=last(j);
				}
				for(k=a;k<=b;k++){
					if(mgr->breaked()){
						break;
					}
					t1=EventTimeToTicks(GetCurrentEventTime());
					if(t1-t0>60){
						_ctx->flush();
						t0=t1;
					}
					
// 15/05/2009 -> PLUS DE FLUIDITE EN METTANT LE PROGRESS EN DEHORS DU TEST FLUSH
					wt.set_progress(0);
					if(!wt.get_progress()){
						break;
					}
					
					if(!_screenobjs->get(k,&scr)){
						break;
					}

					_ctx->setElement(scr);
/* CONDITION 20/01/09
					if(!applyconditions()){
						continue;
					}
*/
					for(ri=1;ri<=_goodstyleruns->count();ri++){
//_tm_("ri="+ri);
						if(!applystylesruns(ri)){
							continue;
						}
						if(!applystylesruns(scr,ri)){
							continue;
						}
						scr->getbounds(&xxa,&yya,&xxb,&yyb);
						(*_drawp)(_ctx);
						scr->getbounds(&xa,&ya,&xb,&yb);
						if((xa==0)&&(xb==0)&&(ya==0)&&(yb==0)){
						}
						else if((xxa==0)&&(xxb==0)&&(yya==0)&&(yyb==0)){
							xxa=xa;
							xxb=xb;
							yya=ya;
							yyb=yb;
						}
						else{
							if(xa<xxa){
								xxa=xa;
							}
							if(xb>xxb){
								xxb=xb;
							}
							if(ya<yya){
								yya=ya;
							}
							if(yb>yyb){
								yyb=yb;
							}
							scr->setbounds(xxa,yya,xxb,yyb);
						}
					}
				}
			}
		}
		else{
// ********************
// TRI DECROISSANT
// ********************
			for(j=nbounds-1;j>=1;j--){
				if(mgr->breaked()){
					break;
				}
				_ctx->reset();
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				a=first(j);
				b=last(j);
				if((a==0)||(b==0)){
					continue;
				}
				if(b<a){
					b=first(j);
					a=last(j);
				}
				for(k=a;k<=b;k++){
					if(mgr->breaked()){
						break;
					}
                    t1=EventTimeToTicks(GetCurrentEventTime());
					if(t1-t0>60){
						_ctx->flush();
						t0=t1;
					}

// 15/05/2009 -> PLUS DE FLUIDITE EN METTANT LE PROGRESS EN DEHORS DU TEST FLUSH
					wt.set_progress(0);
					if(!wt.get_progress()){
						break;
					}

					if(!_screenobjs->get(k,&scr)){
						break;
					}
					_ctx->setElement(scr);
/* CONDITION 20/01/09
					if(!applyconditions()){
						continue;
					}
*/
					for(ri=_goodstyleruns->count();ri>0;ri--){
						if(!applystylesruns(ri)){
							continue;
						}
						if(!applystylesruns(scr,ri)){
							continue;
						}
						
						scr->getbounds(&xxa,&yya,&xxb,&yyb);
						(*_drawp)(_ctx);
						scr->getbounds(&xa,&ya,&xb,&yb);
						if((xa==0)&&(xb==0)&&(ya==0)&&(yb==0)){
						}
						else if((xxa==0)&&(xxb==0)&&(yya==0)&&(yyb==0)){
							xxa=xa;
							xxb=xb;
							yya=ya;
							yyb=yb;
						}
						else{
							if(xa<xxa){
								xxa=xa;
							}
							if(xb>xxb){
								xxb=xb;
							}
							if(ya<yya){
								yya=ya;
							}
							if(yb>yyb){
								yyb=yb;
							}
							scr->setbounds(xxa,yya,xxb,yyb);
						}						
					}
				}
			}
		}
	}
	if(mgr->breaked()){
		return;
	}
	_ctx->reset();	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::draw(bGenericGeoElement* o){
_lbTrace_("bStyle:::draw(bGenericGeoElement*)",false);
	if(_lock){
		return;
	}
	if(o->masked()){
		return;
	}
double	val;
		
	o->getValue(_fieldindex,&val);
	if((val<_minbound)||(val>=_maxbound)){
		return;
	}
	
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
/**/
	_ctx->reset();
/**/
	_ctx->setElement(NULL);
	
	if(!applyglobals()){
		return;
	}
	
bScreenObj scr;
	scr.setreference(o);
	scr.setvalue(val);
	scr.setclass(findclass(val));
	_ctx->setElement(&scr);
		
	for(int i=1;i<=_nbpass;i++){
		findgoodstylesruns(i,scr.getclass());
		for(int ri=_goodstyleruns->count();ri>0;ri--){
			if(!applygeometry(ri)){
				continue;
			}
			// applyconditions à tester
			if(!applyconditions()){
				continue;
			}
//			if(!applygeometry(&scr,ri)){
//				continue;
//			}
			if(!applystylesruns(ri)){
				continue;
			}
			if(!applystylesruns(&scr,ri)){
				continue;
			}
			(*_drawp)(_ctx);
		}
	}
	_ctx->setElement(NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::rect(bGenericGeoElement* o, CGRect* bounds){
_lbTrace_("bStyle::rect",false);
	if(_lock){
//_tm_("locked");
		return;
	}
	if(o->masked()){
//_tm_("masked");
		return;
	}
double	val;
	
	*bounds=CGRectZero;
	
	o->getValue(_fieldindex,&val);
	if((val<_minbound)||(val>=_maxbound)){
//_tm_("bad val");
		return;
	}
	
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);
	
	if(!applyglobals()){
//_tm_("applyglobals failed");
		return;
	}
	
bScreenObj scr;
	scr.setreference(o);
	scr.setvalue(val);
	scr.setclass(findclass(val));
	_ctx->setElement(&scr);
	
float	xmin,ymin,xmax,ymax;
CGRect	tmpa=CGRectZero,tmpb;
	
	for(int i=1;i<=_nbpass;i++){
		findgoodstylesruns(i,scr.getclass());
//_tm_(i+"->"+scr.getclass());
		for(int ri=_goodstyleruns->count();ri>0;ri--){
//_tm_(ri);
			if(!applygeometry(ri)){
//_tm_("!applygeometry(ri)");
				continue;
			}
/* CONDITION 20/01/09
			 if(!applyconditions()){
			 continue;
			 }
*/
			if(!applygeometry(&scr,ri)){
//_tm_("!applygeometry(&scr,ri)");
				continue;
			}
			(*_boundp)(_ctx);
			scr.getbounds(&xmin,&ymin,&xmax,&ymax);
			if(CGRectIsEmpty(tmpa)){
				tmpa.origin.x=xmin;
				tmpa.origin.y=ymin;
				tmpa.size.width=xmax-xmin;
				tmpa.size.height=ymax-ymin;
				tmpb=tmpa;
			}
			else{
				tmpb.origin.x=xmin;
				tmpb.origin.y=ymin;
				tmpb.size.width=xmax-xmin;
				tmpb.size.height=ymax-ymin;
				tmpa=CGRectUnion(tmpa,tmpb);
			}
		}
	}
		
	*bounds=tmpa;
	
	_ctx->setElement(NULL);
}

// ---------------------------------------------------------------------------
// La, dificile de faire passer le coup du styleruns multiples... A VOIR !
// ------------
bGenericGraphicContext* bStyle::get_context_for_object(bGenericGeoElement* o, int pass, bool render){
_lbTrace_("bStyle::get_context_for_object",false);
    if(((bLayersMgr*)layersmgr)->drawing()){
        return NULL;
    }
	if(_lock){
//_tm_("locked");
		return(_ctx);
	}
	if(o->masked()){
//_tm_("masked");
		return(_ctx);
	}
double	val;
	
	o->getValue(_fieldindex,&val);
	if((val<_minbound)||(val>=_maxbound)){
_lm_("((val<_minbound)||(val>=_maxbound))");
		return(_ctx);
	}
	
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

//_tm_("applyglobals");
	if(!applyglobals()){
_lm_("globals not applied");
		return(_ctx);
	}
	
bScreenObj scr;
	scr.setreference(o);
	scr.setvalue(val);
	scr.setclass(findclass(val));
	_ctx->setElement(&scr);
	
int	first=(pass>0)?pass:1;
int	last=(pass>0)?pass:_nbpass;
int ri=(_sort>0)?_goodstyleruns->count():1;

	for(int i=first;i<=last;i++){
// ON CHERCHE LES STYLERUNS POUR LA CLASSE
		findgoodstylesruns(i,scr.getclass());
//_tm_("i="+i+"; class="+scr.getclass()+"; ri="+ri);

// ON APPLIQUE POUR LA CLASSE
		if(render){
// SI C'EST DU RENDER, ON APPLIQUE LE RENDER DU PREMIER RUN POUR LA CLASSE
//_tm_("applystylesruns(ri)");
			if(!applystylesruns(ri)){
_lm_("!applystylesruns(ri) for "+i);
				continue;
			}
		}
		else{
// ---------------------------------------------------------------------
// LE applygeometry(ri) EST NECESSAIRE POUR LES CALCS TEXTE A L'ECRAN...
//	---------------------------------------------------------------------
// SINON C'EST DU GEOMETRIE, ON APPLIQUE LE RENDER DU PREMIER RUN POUR LA CLASSE
//_tm_("applygeometry(ri)");
			if(!applygeometry(ri)){
_lm_("!applygeometry(ri) for "+i);
				continue;
			}
		}
		
/* CONDITION 20/01/2009 : VIRE EN AMONT LORS DE L'INIT DU DRAW
		if(!applyconditions()){
//_tm_("!applyconditions() for %d",i);
			continue;
		}
*/

// ON APPLIQUE POUR L'OBJET
		if(render){
// SI C'EST DU RENDER, ON APPLIQUE LE RENDER DU PREMIER RUN POUR L'OBJET
//_tm_("applystylesruns(&scr,ri)");
			if(!applystylesruns(&scr,ri)){
_lm_("!applystylesruns(&scr,ri) for "+ri);
				continue;
			}
		}
		else{
// SINON C'EST DU GEOMETRIE, ON APPLIQUE LE RENDER DU PREMIER RUN POUR L'OBJET
//_tm_("applygeometry(&scr,ri)");
			if(!applygeometry(&scr,ri)){
_lm_("!applygeometry(&scr,ri) for "+ri);
				continue;
			}
		}
	}
	
	_ctx->setElement(NULL);
	return(_ctx);
}

// ---------------------------------------------------------------------------
// La, dificile de faire passer le coup du styleruns multiples... A VOIR !
// ------------
bGenericGraphicContext* bStyle::get_context_for_class(int clss, int pass, bool render){
    if(((bLayersMgr*)layersmgr)->drawing()){
        return NULL;
    }
    if(_lock){
		return(_ctx);
	}
	
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

	if(!applyglobals()){
		return(_ctx);
	}
	
int	first=(pass>0)?pass:1;
int	last=(pass>0)?pass:_nbpass;
int ri;

	for(int i=first;i<=last;i++){
		findgoodstylesruns(i,clss);
		ri=(_sort>0)?_goodstyleruns->count():1;
		if(render){
			if(!applystylesruns(ri)){
				continue;
			}
		}
		else{
			if(!applygeometry(ri)){
				continue;
			}
		}
	}	
	_ctx->setElement(NULL);
	return(_ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::dist(bScreenObj* scr, CGPoint *cgp){
	if(_lock){
		return(-1);
	}
	_ctx->setElement(scr);	
	return((*_distp)(_ctx,cgp));
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericType* bStyle::gettype(){
	return(typesmgr->get(_typeindex));
}

// ---------------------------------------------------------------------------
// 
// ------------
char* bStyle::getname(){
	return(_name);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::visible(){
	return(_vis);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::selectable(){
	return(_sel);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::getdrawingmethod(){
	return(_drawmethod);
}

// ---------------------------------------------------------------------------
// 
// ------------
char* bStyle::getclassfield(){
	return(_fieldname);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::getpasscount(){
	return(_nbpass);
}

// ---------------------------------------------------------------------------
// 
// ------------
double bStyle::getscaleref(){
	return(_scaleref);
}

// ---------------------------------------------------------------------------
// 
// ------------
double bStyle::getunitcoef(){
	return(_unitcoef);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::getsortkind(){
	return(_sort);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::getclasscount(){
	return(_valbounds->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
double bStyle::getclassbound(int idx){
double	v=__nan();
	_valbounds->get(idx,&v);
	return(v);
}

// ---------------------------------------------------------------------------
// 
// ------------
char* bStyle::getclassname(int idx){
char*	v=NULL;
	_valnames->get(idx,&v);
	return(v);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::set_curclass(int idx){
	if((idx>0)&&(idx<_valbounds->count())){
		_curclass=idx;
	}
}
		
// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::get_curclass(){
	return(_curclass);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::getmargin(){
	return(_margin);
}


// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::report(){
bGenericXMLBaseElement* elt;
char					value[_values_length_max_];

	elt=clssmgr->NthElement(_layer,1,"selectable");
	if(!elt){
		return;
	}
	sprintf(value,"%d",_sel);
	elt->setvalue(value);
	
	elt=clssmgr->NthElement(_layer,1,"visible");
	if(!elt){
		return;
	}
	sprintf(value,"%d",_vis);
	elt->setvalue(value);	
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bStyle::objsinrect(CGRect* cgr, bool strict){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	if(_lock){
		return(arr);
	}
	if((!visible())||(!selectable())){
		return(arr);
	}
float				xmin,ymin,xmax,ymax;
CGRect				bounds;
bGenericGeoElement*	o;
bScreenObj*			scr;
	for(int j=_screenobjs->count();j>=1;j--){
		if(!_screenobjs->get(j,&scr)){
			continue;
		}
		if(!scr){
			continue;
		}
		scr->getbounds(&xmin,&ymin,&xmax,&ymax);
		bounds=CGRectMake(xmin,ymin,xmax-xmin,ymax-ymin);
		if(strict){
			if(CGRectContainsRect((*cgr),bounds)){
				o=scr->getreference();
				arr->add(&o);
			}
		}
		else{
			if(CGRectIntersectsRect((*cgr),bounds)){
				o=scr->getreference();
				arr->add(&o);
			}	
		}
	}
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bStyle::objsincircle(CGRect* cgr, bool strict){
_lbTrace_("bStyle::objsincircle",true);
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	if(_lock){
		return(arr);
	}
	if((!visible())||(!selectable())){
		return(arr);
	}
int						i,j,k,a,b,ri;
float					dmax=cgr->size.width/2.0;
CGPoint					cgp;
bGenericGeoElement*		o;
bScreenObj*				scr;
distProc				distp;

	switch(_drawmethod){
		case kIconStyleKind_:
			distp=distToIcon;
			break;
		case kCircleStyleKind_:
			distp=distToCircle;
			break;
		case kRectStyleKind_:
			distp=distToRect;
			break;
		case kTextStyleKind_:
			distp=distToText;
			break;
		case kLineStyleKind_:
		case kSurfStyleKind_:
		case kRasterStyleKind_:
			if(strict){
				distp=distMaxToLine;
			}
			else{
				distp=distToLine;
			}
			break;
		default:
			distp=distToNothing;
			break;
	}
	
	cgp.x=CGRectGetMidX(*cgr);
	cgp.y=CGRectGetMidY(*cgr);
		
	if(strict){
		for(i=1;i<=_screenobjs->count();i++){
			_screenobjs->get(i,&scr);
			scr->setflag(true);
		}
	}
	for(i=1;i<=_nbpass;i++){
		if(_sort>=0){
// ********************
// TRI CROISSANT
// ********************	
			for(j=_valbounds->count()-1;j>0;j--){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				// Retrait du 2010/06/2004 -> test erroné
				/*if(_goodstyleruns->count()==0){
					a=first(j);
					b=last(j);
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						scr->setflag(false);
					}
				}
				else */
				for(ri=_goodstyleruns->count();ri>0;ri--){
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					if(!applygeometry(ri)){
						for(k=b;k>=a;k--){
							if(!_screenobjs->get(k,&scr)){
								break;
							}
							scr->setflag(false);
						}
						continue;
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						if((!strict)&&(scr->flag())){
							continue;
						}
						else if((strict)&&(!scr->flag())){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							scr->setflag(false);
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							scr->setflag(false);
							continue;
						}
						if(strict){
							if((*distp)(_ctx,&cgp)>=dmax){
								scr->setflag(false);
							}
						}
						else{
							if((*distp)(_ctx,&cgp)<=dmax){
								scr->setflag(true);
							}
						}
					}
				}
			}
		}
		else{
// ********************
// TRI DECROISSANT
// ********************
			for(j=1;j<=_valbounds->count()-1;j++){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				// Retrait du 2010/06/2004 -> test erroné
				/*if(_goodstyleruns->count()==0){
					a=first(j);
					b=last(j);
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						scr->setflag(false);
					}					
				}
				else */
				for(ri=1;ri<=_goodstyleruns->count();ri++){
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					if(!applygeometry(ri)){
						for(k=b;k>=a;k--){
							if(!_screenobjs->get(k,&scr)){
								break;
							}
							scr->setflag(false);
						}
						continue;
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						if((!strict)&&(scr->flag())){
							continue;
						}
						else if((strict)&&(!scr->flag())){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							scr->setflag(false);
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							scr->setflag(false);
							continue;
						}
						if(strict){
							if((*distp)(_ctx,&cgp)>=dmax){
								scr->setflag(false);
							}
						}
						else{
							if((*distp)(_ctx,&cgp)<=dmax){
								scr->setflag(true);
							}
						}
					}
				}
			}		
		}
	}
	for(i=1;i<=_screenobjs->count();i++){
		_screenobjs->get(i,&scr);
		if(scr->flag()){
			scr->setflag(false);
			o=scr->getreference();
			arr->add(&o);
		}
	}
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bStyle::objsinpath(CGPoint* cgp, int nb, bool strict){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	if(_lock){
		return(arr);
	}
	if((!visible())||(!selectable())){
		return(arr);
	}
int						i,j,k,l,a,b,n,*of,nof,ri;
bGenericGeoElement*		o;
bScreenObj*				scr;
float					*x=new float[nb],*y=new float[nb];
float					*gx,*gy;
	
	for(i=0;i<nb;i++){
		x[i]=cgp[i].x;
		y[i]=cgp[i].y;
	}
	if(strict){
		for(i=1;i<=_screenobjs->count();i++){
			_screenobjs->get(i,&scr);
			scr->setflag(true);
		}
	}
	for(i=1;i<=_nbpass;i++){
		if(_sort>=0){
// ********************
// TRI CROISSANT
// ********************
			for(j=_valbounds->count()-1;j>0;j--){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				// Retrait du 2010/06/2004 -> test erroné
				/*if(_goodstyleruns->count()==0){
					a=first(j);
					b=last(j);
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						scr->setflag(false);
					}					
				}
				else */
				for(ri=_goodstyleruns->count();ri>0;ri--){
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					if(!applygeometry(ri)){
						for(k=b;k>=a;k--){
							if(!_screenobjs->get(k,&scr)){
								break;
							}
							scr->setflag(false);
						}
						continue;
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						if((!strict)&&(scr->flag())){
							continue;
						}
						else if((strict)&&(!scr->flag())){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							scr->setflag(false);
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							scr->setflag(false);
							continue;
						}
						_ctx->getGeometry(&gx,&gy,&n,&of,&nof);
						if(strict){
							for(l=0;l<n;l++){
								if(!XYInClosedPath(gx[l],gy[l],x,y,nb,0)){
									scr->setflag(false);
									break;
								}
							}
						}
						else{
							for(l=0;l<n;l++){
								if(XYInClosedPath(gx[l],gy[l],x,y,nb,1)){
									scr->setflag(true);
									break;
								}
							}
						}
					}
				}
			}
		}
		else{
// ********************
// TRI DECROISSANT
// ********************
			for(j=1;j<=_valbounds->count()-1;j++){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				// Retrait du 2010/06/2004 -> test erroné
				/*if(_goodstyleruns->count()==0){
					a=first(j);
					b=last(j);
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						scr->setflag(false);
					}					
				}
				else */
				for(ri=1;ri<=_goodstyleruns->count();ri++){
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					if(!applygeometry(ri)){
						for(k=b;k>=a;k--){
							if(!_screenobjs->get(k,&scr)){
								break;
							}
							scr->setflag(false);
						}
						continue;
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							break;
						}
						if((!strict)&&(scr->flag())){
							continue;
						}
						else if((strict)&&(!scr->flag())){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							scr->setflag(false);
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							scr->setflag(false);
							continue;
						}
						_ctx->getGeometry(&gx,&gy,&n,&of,&nof);
						if(strict){
							for(l=0;l<n;l++){
								if(!XYInClosedPath(gx[l],gy[l],x,y,nb,0)){
									scr->setflag(false);
									break;
								}
							}
						}
						else{
							for(l=0;l<n;l++){
								if(XYInClosedPath(gx[l],gy[l],x,y,nb,1)){
									scr->setflag(true);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	for(i=1;i<=_screenobjs->count();i++){
		_screenobjs->get(i,&scr);
		if(scr->flag()){
			scr->setflag(false);
			o=scr->getreference();
			arr->add(&o);
		}
	}
    delete[] x;
	delete[] y;
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bStyle::objsatpoint(CGPoint* cgp, bool all){
//_bTrace_("bStyle::objsatpoint",true);
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	if(_lock){
		return(arr);
	}
	if((!visible())||(!selectable())){
		return(arr);
	}
bool					flg=false;
int						i,j,k,a,b,ri;
float					xmin,ymin,xmax,ymax;
bGenericGeoElement*		o;
bScreenObj*				scr;
		
	for(i=1;i<=_nbpass;i++){
		if(_sort>=0){
// ********************
// TRI CROISSANT
// ********************	
			for(j=_valbounds->count()-1;j>0;j--){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				for(ri=_goodstyleruns->count();ri>0;ri--){
					if(!applygeometry(ri)){
						continue;
					}
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							flg=true;
							break;
						}
						scr->getbounds(&xmin,&ymin,&xmax,&ymax);
						if(!XYInBounds(cgp->x,cgp->y,xmin,ymin,xmax,ymax)){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							continue;
						}
						if((*_distp)(_ctx,cgp)<=3){
							o=scr->getreference();
							if(!all){
								arr->add(&scr);
								flg=true;
								break;
							}
							if(!scr->flag()){
								arr->add(&scr);
								scr->setflag(true);
							}
						//break;
						}
					}
					if(flg){
						break;
					}
				}
				if(flg){
					break;
				}
			}
		}
		else{
// ********************
// TRI DECROISSANT
// ********************
			for(j=1;j<=_valbounds->count()-1;j++){
				_ctx->setElement(NULL);
				findgoodstylesruns(i,j);
				for(ri=1;ri<=_goodstyleruns->count();ri++){
					if(!applygeometry(ri)){
						continue;
					}
					a=first(j);
					b=last(j);
					if((a==0)||(b==0)){
						continue;
					}
					if(b<a){
						b=first(j);
						a=last(j);
					}
					for(k=b;k>=a;k--){
						if(!_screenobjs->get(k,&scr)){
							flg=true;
							break;
						}
						scr->getbounds(&xmin,&ymin,&xmax,&ymax);
						if(!XYInBounds(cgp->x,cgp->y,xmin,ymin,xmax,ymax)){
							continue;
						}
						_ctx->setElement(scr);
/* CONDITION 20/01/09
						if(!applyconditions()){
							continue;
						}
*/
						if(!applygeometry(scr,ri)){
							continue;
						}
						if((*_distp)(_ctx,cgp)<=3){
							o=scr->getreference();
							if(!all){
								arr->add(&scr);
								flg=true;
								break;
							}
							if(!scr->flag()){
								arr->add(&scr);
								scr->setflag(true);
							}
						//break;
						}
					}
					if(flg){
						break;
					}
				}
				if(flg){
					break;
				}
			}
		}
		if(flg){
			break;
		}
	}
	for(i=1;i<=arr->count();i++){
		arr->get(i,&scr);
		scr->setflag(false);
		o=scr->getreference();
		arr->put(i,&o);
	}
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStyle::root(){
	return(_root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::get_styleruns(	bArray& arr,
							bGenericGeoElement* o,
							double scale,
							int pass){
	if(_lock){
		return;
	}
	if(o->masked()){
		return;
	}
	_ctx->setScaleRef(scale);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

	if(!applyglobals()){
		return;
	}
	
double	val;	
	o->getValue(_fieldindex,&val);
	if((val<_minbound)||(val>=_maxbound)){
		return;
	}
	
bScreenObj scr;
	scr.setreference(o);
	scr.setvalue(val);
	scr.setclass(findclass(val));
	_ctx->setElement(&scr);
/* CONDITION 20/01/09
	if(!applyconditions()){
		_ctx->setElement(NULL);
		return;
	}
*/
	findgoodstylesruns(pass,scr.getclass());

bStyleRun*				run;
bGenericXMLBaseElement*	root;
	for(int i=1;i<=_goodstyleruns->count();i++){
		_goodstyleruns->get(i,&run);
		root=run->root();
		arr.add(&root);
	}
	
	_ctx->setElement(NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::find(void *o, void *prm){
bGenericGeoElement*		geo=(bGenericGeoElement*)o;
bStyle*					style=(bStyle*)prm;
double					val;
ivx_rect				vr2,vr1;

	geo->getValue(style->_fieldindex,&val);
	if((val>=style->_minbound)&&(val<style->_maxbound)){
		if(!geo->masked()){
			geo->getBounds(&vr2);
			if(ivr_sect(&vr2,&style->_area,&vr1)){
				style->_nobjects++;
				geo->setneedupdate(true);
			}
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::fill(void *o, void *prm){
bGenericGeoElement*		geo=(bGenericGeoElement*)o;
bStyle*					style=(bStyle*)prm;
bScreenObj*				scr;
double					val;
ivx_rect				vr2,vr1;

	geo->getValue(style->_fieldindex,&val);
	if((val>=style->_minbound)&&(val<style->_maxbound)){
		if(!geo->masked()){
			geo->getBounds(&vr2);
			if(ivr_sect(&vr2,&style->_area,&vr1)){
				scr=new bScreenObj();
				scr->setreference(geo);
				scr->setvalue(val);
				scr->setclass(style->findclass(val));
/* CONDITION 20/01/09*/				
				style->_ctx->setElement(scr);
				if(!style->applyconditions()){
					/**/
					style->_ctx->setElement(NULL);
					delete scr;
					/**/
					return(0);
				}
/**/
				style->_nobjects++;
				style->_screenobjs->put(style->_nobjects,&scr);
				geo->setatscreen(true);
			}
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::boundsComp(const void *a, const void *b){	
double* da=(double*)a;
double*	db=(double*)b;
		
	if((*da)<(*db)){
		return(-1);
	}
	if((*da)>(*db)){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::boundsCompInv(const void *a, const void *b){	
	return(-boundsComp(a,b));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::classComp(const void *a, const void *b){	
bScreenObj	**sa=(bScreenObj**)a;
bScreenObj	**sb=(bScreenObj**)b;

	if((*sa)->getclass()<(*sb)->getclass()){
		return(-1);
	}
	if((*sa)->getclass()>(*sb)->getclass()){
		return(1);
	}
// Pour conserver l'ordre fichier
	if((*sa)->getreference()->getOffset()<(*sb)->getreference()->getOffset()){
		return(-1);
	}
	if((*sa)->getreference()->getOffset()>(*sb)->getreference()->getOffset()){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::classCompInv(const void *a, const void *b){	
	return(-classComp(a,b));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::fullComp(const void *a, const void *b){
bScreenObj	**sa=(bScreenObj**)a;
bScreenObj	**sb=(bScreenObj**)b;

	if((*sa)->getvalue()<(*sb)->getvalue()){
		return(-1);
	}
	if((*sa)->getvalue()>(*sb)->getvalue()){
		return(1);
	}
	return(	(*sb)->getreference()->getOffset()	-
			(*sa)->getreference()->getOffset()	);// Pour conserver l'ordre fichier
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::fullCompInv(const void *a, const void *b){	
	return(-fullComp(a,b));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::objcount(){
	return(_screenobjs->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
bScreenObj* bStyle::objget(int idx){
	bScreenObj*	scr;
	
	if(!_screenobjs->get(idx,&scr)){
		return(NULL);
	}
	return(scr);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::findclass(double val){
double	a,b;
	
	/*if(val==_maxbound){
		return(_valbounds->count());
	}*/
	_valbounds->get(1,&a);
	for(int i=1;i<_valbounds->count();i++){
		_valbounds->get(i+1,&b);
		if((val>=a)&&(val<b)){
			return(i);
		}
		a=b;
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::findclass(bGenericGeoElement* o){
double	d;
	o->getValue(_fieldindex,&d);
	return(findclass(d));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::flushscreen(){
bScreenObj*			scr;
bGenericGeoElement*	o;
	
	for(int i=1;i<=_screenobjs->count();i++){
		if((_screenobjs->get(i,&scr))&&(scr)){
			o=scr->getreference();
			if(o){
				o->setatscreen(false);
				o->setneedupdate(false);
			}
			delete scr;
		}		
	}
	_screenobjs->reset();
	_offsets->reset();
	_counts->reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::is_virtual(){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::flushstyleruns(){
bStyleRun*			run;
	
	for(int i=1;i<=_styleruns->count();i++){
		if((_styleruns->get(i,&run))&&(run)){
			delete run;
		}
	}
	_styleruns->reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::first(int clss){
int	k=0;
	_offsets->get(clss,&k);
	return(k);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bStyle::last(int clss){

int	k=0,c=0;
	_offsets->get(clss,&k);
	_counts->get(clss,&c);
	
	if(_sort>=0){
		if(k!=0){
			return((k+c)-1);
		}
	}
	else{
		if(k!=0){
			return((k-c)+1);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applyglobals(){
	if(!_globals){
fprintf(stderr,"bad globals\n");
		return(false);
	}
	return(_globals->applyforscale(_ctx));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applyconditions(){
	if(!_condition){
fprintf(stderr,"bad _condition\n");
		return(false);
	}
	return(_condition->applyforobject(_ctx));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applystylesruns(int ir){
	if(_goodstyleruns->count()<=0){
		return(false);
	}
bStyleRun*	run;
	if(_goodstyleruns->get(ir,&run)){
		if(!run->applyforclass(_ctx)){
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applystylesruns(bScreenObj* scr, int ir){
	if(_goodstyleruns->count()<=0){
		return(false);
	}
bStyleRun*	run;
	if(_goodstyleruns->get(ir,&run)){
		if(!run->applyforobject(_ctx)){
			return(false);// MOD 17/04/07
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applygeometry(int ir){
	if(_goodstyleruns->count()<=0){
		return(false);
	}
bStyleRun*	run;
	if(_goodstyleruns->get(ir,&run)){
		if(!run->applygeometryforclass(_ctx)){
			return(false);// MOD 17/04/07
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyle::applygeometry(bScreenObj* scr, int ir){
//_bTrace_("bStyle::applygeometry(bScreenObj* scr, int ir)",true);
	if(_goodstyleruns->count()<=0){
		return(false);
	}
//_tm_(ir+"/"+_goodstyleruns->count());

bStyleRun*	run;
		if(_goodstyleruns->get(ir,&run)){
			if(!run->applygeometryforobject(_ctx)){
				return(false);// MOD 17/04/07
			}
		}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::findgoodstylesruns(int pass, int clss){
//_bTrace_("bStyle::findgoodstylesruns",false);
int			i;
bStyleRun*	run;
	
	_goodstyleruns->reset();
	for(i=1;i<=_styleruns->count();i++){
		if(_styleruns->get(i,&run)){
			if(run->good(_scale,clss,pass)){
				_goodstyleruns->add(&run);
			}
		}
	}
//_tm_(_goodstyleruns->count()+" styleruns for scale="+_scale+" class="+clss+" pass="+pass);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawIcon(bGenericGraphicContext* ctx){
	ctx->drawIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawCircle(bGenericGraphicContext* ctx){
	ctx->drawCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawRect(bGenericGraphicContext* ctx){	
	ctx->drawRect();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawText(bGenericGraphicContext* ctx){
	ctx->drawText();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawLine(bGenericGraphicContext* ctx){
	ctx->drawLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawSurf(bGenericGraphicContext* ctx){
	ctx->drawSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawRaster(bGenericGraphicContext* ctx){
	ctx->drawRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::drawNothing(bGenericGraphicContext* ctx){
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToIcon(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToIcon((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToCircle(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToCircle((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToRect(bGenericGraphicContext* ctx, CGPoint* cgp){	
	return(ctx->distToRect((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToText(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToText((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToLine(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToLine((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distMaxToLine(bGenericGraphicContext* ctx, CGPoint* cgp){
float	*x,*y;
int		n,no,*o;	
//#warning manque le width/2
	ctx->getGeometry(&x,&y,&n,&o,&no);
float	d=XYDistMax2Vecs(cgp->x,cgp->y,x,y,n)-(ctx->getWidth()/2.0);
	if(d<0){
		d=0;
	}
	return(d);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToSurf(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToSurf((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToRaster(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(ctx->distToRaster((*cgp)));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bStyle::distToNothing(bGenericGraphicContext* ctx, CGPoint* cgp){
	return(SHRT_MAX);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForIcon(bGenericGraphicContext* ctx){
	ctx->boundsForIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForCircle(bGenericGraphicContext* ctx){
	ctx->boundsForCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForRect(bGenericGraphicContext* ctx){	
	ctx->boundsForRect();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForText(bGenericGraphicContext* ctx){
//_bTrace_("bStyle::boundsForText",true);
	ctx->boundsForText();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForLine(bGenericGraphicContext* ctx){
	ctx->boundsForLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForSurf(bGenericGraphicContext* ctx){
	ctx->boundsForSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForRaster(bGenericGraphicContext* ctx){
	ctx->boundsForRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyle::boundsForNothing(bGenericGraphicContext* ctx){
}

