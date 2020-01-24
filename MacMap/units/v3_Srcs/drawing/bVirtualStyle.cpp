//----------------------------------------------------------------------------
// File : bVirtualStyle.cpp
// Project : MacMap
// Purpose : C++ source file : Virtual style class (i.e. styles for non objects : WMS, tiling, legends, titles, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 28/01/2009 creation.
// 04/09/2014 elimination des appels avec geometrie QD.
//----------------------------------------------------------------------------

#include "bVirtualStyle.h"
#include "bMacMapApp.h"
#include "bMacMapDocument.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bVirtualStyle	::bVirtualStyle(bGenericLayersMgr* mgr)
				:bStyle(mgr){
	_sel=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bVirtualStyle::~bVirtualStyle(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bVirtualStyle::settype(const char *name){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bVirtualStyle::setselectable(bool sel){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bVirtualStyle::setbounds(ivx_rect* bounds){
	if(!_vis){
		return;
	}
	
	_scale=scalemgr->get()->coef();
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

	if(!applyglobals()){
		return;
	}

	_nobjects=1;
	_area=(*bounds);
	
	_lock=true;

	_screenobjs=new bArray(sizeof(bScreenObj*));
bScreenObj*	scr=new bScreenObj();
	scr->setreference(NULL);
	scr->setvalue(0);
	scr->setclass(1);
	_screenobjs->add(&scr);
	
	_lock=false;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bVirtualStyle::load(){
_bTrace_("bVirtualStyle::load",true);
_tm_("loading "+_name);
bMacMapDocument*	doc=(bMacMapDocument*)map_doc;
int					i=doc->styles()->index(_name);
	if(i<=0){
_te_("bad style name "+_name+" (idx="+i+"), trying with first");
		i=1;
	}
	_root=doc->styles()->get_root(i);
	
	doc->styles()->set_on_screen(i,true);
	
	if(!_root){
_te_("no root");
		return(false);
	}
	
	_root->init(this);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bVirtualStyle::draw(bStdWait& wt){
_bTrace_("bVirtualStyle::draw",false);
_tm_("drawing "+_name);

	if(!_vis){
		return;
	}
int				i,ri=1;
//unsigned int	t1,t0=EventTimeToTicks(GetCurrentEventTime());
bScreenObj*		scr;
	
	_scale=scalemgr->get()->coef();
	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

_tm_("applyglobals");
	if(!applyglobals()){
		return;
	}

	for(i=1;i<=_nbpass;i++){
		if(_sort>=0){
// ********************
// TRI CROISSANT
// ********************
            _ctx->reset();
            _ctx->setElement(NULL);
            findgoodstylesruns(i,1);
/*            t1=EventTimeToTicks(GetCurrentEventTime());
            if(t1-t0>60){
                _ctx->flush();
                t0=t1;
            }*/
            wt.set_progress(0);
            if(!wt.get_progress()){
                break;
            }
            if(!_screenobjs->get(1,&scr)){
                break;
            }
            _ctx->setElement(scr);
            for(ri=1;ri<=_goodstyleruns->count();ri++){
                if(!applystylesruns(ri)){
                    continue;
                }
                if(!applystylesruns(scr,ri)){
                    continue;
                }
                (*_drawp)(_ctx);
            }
		}
		else{
// ********************
// TRI DECROISSANT
// ********************
            _ctx->reset();
            _ctx->setElement(NULL);
            findgoodstylesruns(i,1);
/*            t1=EventTimeToTicks(GetCurrentEventTime());
            if(t1-t0>60){
                _ctx->flush();
                t0=t1;
            }*/
            wt.set_progress(0);
            if(!wt.get_progress()){
                break;
            }
            if(!_screenobjs->get(1,&scr)){
                break;
            }
            _ctx->setElement(scr);
            for(ri=_goodstyleruns->count();ri>0;ri--){
                if(!applystylesruns(ri)){
                    continue;
                }
                if(!applystylesruns(scr,ri)){
                    continue;
                }
                (*_drawp)(_ctx);
            }
		}
	}
	_ctx->reset();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bVirtualStyle::rect(bGenericGeoElement* o, CGRect* bounds){
	*bounds=CGRectZero;
}

// ---------------------------------------------------------------------------
// La, dificile de faire passer le coup du styleruns multiples... A VOIR !
// ------------
bGenericGraphicContext* bVirtualStyle::get_context_for_object(bGenericGeoElement* o, int pass, bool render){
	if(_lock){
		return(_ctx);
	}

	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

	if(!applyglobals()){
		return(_ctx);
	}
    
	_ctx->setElement(NULL);
	return(_ctx);
}

// ---------------------------------------------------------------------------
// La, dificile de faire passer le coup du stylerus multiples... A VOIR !
// ------------
bGenericGraphicContext* bVirtualStyle::get_context_for_class(int clss, int pass, bool render){
	if(_lock){
		return(_ctx);
	}

	_ctx->setScaleRef(_scaleref);
	_ctx->setUnitCoef(_unitcoef);
	_ctx->setElement(NULL);

	if(!applyglobals()){
		return(_ctx);
	}
	_ctx->setElement(NULL);
	return(_ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bVirtualStyle::dist(bScreenObj* scr, CGPoint *cgp){
	return(LONG_MAX);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bVirtualStyle::objsinrect(CGRect* cgr, bool strict){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bVirtualStyle::objsincircle(CGRect* cgr, bool strict){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bVirtualStyle::objsinpath(CGPoint* cgp, int nb, bool strict){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bArray* bVirtualStyle::objsatpoint(CGPoint* cgp, bool all){
bArray*	arr=new bArray(sizeof(bGenericGeoElement*));
	return(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bVirtualStyle::is_virtual(){
	return(true);
}
