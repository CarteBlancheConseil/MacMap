//----------------------------------------------------------------------------
// File : bLayersMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Layers management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2003 Carte Blanche Conseil.
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
// 30/05/2003 creation.
// 29/08/2014 remove QD, mapwd safe.
// 13/04/2017 merged with LayerAccessContext (removed).
//----------------------------------------------------------------------------

#include "bLayersMgr.h"
#include "bMacMapApp.h"
#include "bScaleMgr.h"
#include "bClassesMgr.h"
#include "bTypesMgr.h"
#include "bSelectionMgr.h"
#include "bContrastesMgr.h"
#include "bStyle.h"
#include "bVirtualStyle.h"
#include "bScreenObj.h"
#include "bMacMapGeoElement.h"

#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/endian.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/mm_messages.h>

#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define _ltrace_	1
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
//
// -----------
typedef struct containPrm{
    int						idx;
    int						n;
    bGenericXMLBaseElement* elt;
}containPrm;

// ---------------------------------------------------------------------------
//
// -----------
static bool _getIndElementContainingLayer(bGenericXMLBaseElement *elt, void *prm, int indent){
    containPrm	*p=(containPrm*)prm;
    char		clssname[_names_length_max_];
    
    elt->getclassname(clssname);
    if(!strcmp("layer",clssname)){
        p->n++;
        if(p->n==p->idx){
            p->elt=elt;
            return(false);
        }
    }
    else if(!strcmp("virtuallayer",clssname)){
        p->n++;
        if(p->n==p->idx){
            p->elt=elt;
            return(false);
        }
    }
    return(true);
}

// ---------------------------------------------------------------------------
//
// -----------
static bGenericXMLBaseElement* GetIndlayer(bGenericXMLBaseElement* root, int idx){
    containPrm	p;
    
    p.idx=idx;
    p.n=0;
    p.elt=NULL;
    (void)root->dotoall(&p,0,_getIndElementContainingLayer);
    return(p.elt);
}

// ---------------------------------------------------------------------------
//
// -----------
static bool debugDump(bGenericXMLBaseElement *elt, void *prm, int indent){
    char    clssname[_names_length_max_];
    elt->getclassname(clssname);
    fprintf(stderr,"%s\n",clssname);
    return(true);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bLayersMgr	::bLayersMgr()
			:_ctxs(sizeof(bGenericGraphicContext*)){
	_ctx=NULL;
	_selg=NULL;
	_cntg=NULL;
	_drawing=false;
                
    _cursor=0;
    _elts=new bArray(sizeof(bStyle*));

}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bLayersMgr::~bLayersMgr(){
bGenericGraphicContext*	ctx;
	for(int i=1;i<=_ctxs.count();i++){
		_ctxs.get(i,&ctx);
		extmgr->get_component(kComponentGraphics)->i_free(ctx);
	}
	_ctx=NULL;
    
    if(_elts){
        flush();
        delete _elts;
    }
}

// ---------------------------------------------------------------------------
// Chargement
// -----------
int bLayersMgr::load(){
_lbTrace_("bLayersMgr::load",true);
	if(_ctx==NULL){
        _ctx=(bGenericGraphicContext*)(extmgr->get_component(kComponentGraphics)->i_allocate(kCGGraphicContext,_MMAPP_,NULL));
	}
	
bGenericGraphicContext*	ctx;
float					clr[5];
int						spc;
	
	GetBackground(clr,&spc);
	for(int i=1;i<=_ctxs.count();i++){
		_ctxs.get(i,&ctx);
		ctx->set_background(clr,spc);
	}
	
    if(curview_load()!=noErr){
        return -1;
    }
    
    _selg=new bGraphicParams(kStdGraphicParamsSelectionName,_MMAPP_);
    _cntg=new bGraphicParams(kStdGraphicParamsContrastesName,_MMAPP_);
  
	return(noErr);
}

// ---------------------------------------------------------------------------
// Chargement
// -----------
int bLayersMgr::curview_load(){
_lbTrace_("bLayersMgr::curview_load",true);
long                    margin=0;
bGenericXMLBaseElement	*root,*elt;
    
    if(map_doc->readTree(&root,0,"drawsearch")){
_lm_("found drawsearch.xml");
char	val[_values_length_max_];
        elt=clssmgr->NthElement(root,1,"int");
        if(elt){
            elt->getvalue(val);
            margin=atoi(val);
_lm_("margin="+(int)margin);
        }
        else{
_lw_("no margin");
        }
        clssmgr->ReleaseXMLInstance(root);
        if(margin<0){
            margin=0;
        }
    }
    
    _curview=viewmgr->get_root();
    if(!_curview){
_le_("no _curview == NULL");
        return(-1);
    }
    _curview->init(this);
bStyle*	style;
    for(int i=1;i<=_elts->count();i++){
        if(!_elts->get(i,&style)){
_le_("_elts->get("+i+",&style)");
            return(-1);
        }
        if(	(style->gettype()==NULL)	&&
            (!style->is_virtual())		){
_le_("type not found for style "+i);
            remove(i);
            i--;
            continue;
        }
_lm_("* parsing "+i);
        if(!parse(i)){
        }
        style->setmargin(margin);
    }
    
    if(count()>0){
        set_current(1);
    }

    return(noErr);

}

// ---------------------------------------------------------------------------
// 
// -----------
int bLayersMgr::unload(){
	return(unload(true));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bLayersMgr::unload(bool save){
_lbTrace_("bLayersMgr::unload(bool)",true);
	if(save){
bStyle*	style;
        
        for(int i=1;i<=_elts->count();i++){
            if(!_elts->get(i,&style)){
                continue;
            }
            style->report();
        }
_tm_("* report ok");
        flush();
_tm_("* flush ok");

	}
	else{
        flush();
	}
	
	delete _selg;
	delete _cntg;
	_selg=NULL;
	_cntg=NULL;

    return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::AddContext(bGenericGraphicContext* ctx){
	_ctxs.add(&ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::SwitchContext(	int symbolicName,
								void* data){
_lbTrace_("bLayersMgr::switchContext",true);
bGenericGraphicContext*	ctx;
	for(int i=1;i<=_ctxs.count();i++){
		_ctxs.get(i,&ctx);
		if(ctx->signature()==symbolicName){
			_ctx=ctx;
			break;
		}
	}
UInt32	sgn=_ctx->signature();

char    csgn[8];
#ifdef __LITTLE_ENDIAN__
    LBSwapWord(&sgn,sizeof(UInt32));
#endif
    strncpy(csgn,(char*)&sgn,4);
    csgn[4]=0;
#ifdef __LITTLE_ENDIAN__
    LBSwapWord(&sgn,sizeof(UInt32));
#endif

_lm_("_ctx set to "+csgn);
    
	if(data){
		switch(sgn){
			case kPDFGraphicContext:
			case kKMLGraphicContext:
            case kSVGGraphicContext:{
VectorData*	dt=(VectorData*)data;
				_ctx->set_path(dt->path);
				_ctx->set_box(dt->bounds);
				}break;
			case kBitMapGraphicContext:{
BitmapData*	dt=(BitmapData*)data;
				_ctx->set_bitmap_info(dt->data,
									  dt->width,
									  dt->height,
									  dt->bitsPerComponent,
									  dt->bytesPerRow,
									  dt->colorspace,
									  dt->bitmapInfo,
									  &dt->outbm);
				}
				break;
			case kCtxGraphicContext:{
CGContextRef	cgctx=(CGContextRef)data;
				_ctx->set_context(cgctx);
				}
				break;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::DrawLayers(void* wd, ivx_rect* bounds){
_lbTrace_("bLayersMgr::DrawLayers",true);
long    i;
bStyle* style;
_lm_((void*)this);
	_breaked=false;
	
double	d=scalemgr->get()->coef();
	_ctx->setScale(d);

clock_t  t;
// Reset des éléments à l'écran
_lm_("styles reset");
	for(i=1;i<=count();i++){
t=clock();
		style=(bStyle*)get(i);
		if(!style){
_le_("NULL style "+i);
			continue;
		}
//_lm_("style reset "+i+"/"+style->getname());
		style->flushscreen();
_lm_("Reset "+i+" : "+(int)(clock()-t));
	}
	
// Init des styles	
_lm_("styles init");
	for(i=1;i<=count();i++){
t=clock();
		if(_breaked){
_lm_("_breaked");
			break;
		}
		style=(bStyle*)get(i);
		if(!style){
_le_("NULL style");
			continue;
		}
//_lm_("style init "+i+"/"+style->getname());
		style->setcontext(_ctx);
		style->setbounds(bounds);
_lm_("Init "+i+" : "+(int)(clock()-t));
	}

	_drawing=true;

bCursWait	wt(true);
	
// Dessin des styles	
_lm_("styles draw :");
	_ctx->beginDraw();
	for(i=1;i<=count();i++){
t=clock();
		if(_breaked){
_lm_("_breaked");
			break;
		}
		style=(bStyle*)get(i);
		if(!style){
_le_("NULL style");
			continue;
		}
_lm_("draw "+i+"/"+style->getname());
		style->draw(wt);
		_ctx->flush();
_lm_("Draw "+i+" : "+(int)(clock()-t));
	}
	_ctx->endDraw();
	
	_drawing=false;	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::StopDraw(){
_lbTrace_("bLayersMgr::stopDraw",true);
	_breaked=true;
	if(_drawing){
_lm_("Drawing process stopped");
		_drawing=false;
		_ctx->endDraw();
	}
}

// ---------------------------------------------------------------------------
// A revoir
// -----------
void bLayersMgr::UpdateLayers(){

}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::DrawSelection(CGContextRef ctx, bGenericGeoElement* o){	
	if(selmgr->count()<=0){
		return;
	}
	if(!bMacMapGeoElement::get_inval()){
		return;
	}
	
int         i,j,k,l,n;
float       xmin,ymin,xmax,ymax;
bStyle*     style;
bScreenObj* scr;
	
	CGContextSaveGState(ctx);
	_selg->apply(ctx);
	if(o){
		for(i=1;i<=count();i++){
			style=(bStyle*)get(i);
			if(!style){
				continue;
			}
			if((!style->visible())||(!style->selectable())){
				continue;
			}
			l=style->findclass(o);
			if(!l){
				continue;
			}
			k=style->first(l);
			n=style->last(l);
			if((k==0)||(n==0)){
				continue;
			}
			if(n<k){
				n=style->first(l);
				k=style->last(l);
			}
			for(j=k;j<=n;j++){
				scr=style->objget(j);
				if(!scr){
					continue;
				}
				if(scr->getreference()!=o){
					continue;
				}
				if(!scr->getreference()->selected()){
					continue;
				}
				if(!scr->visible()){
					continue;
				}
				scr->getbounds(&xmin,&ymin,&xmax,&ymax);
				CGContextAddRect(ctx,CGRectInset(CGRectMake(xmin,ymin,xmax-xmin,ymax-ymin),-2,-2));
				break;
			}
		}
	}
	else{
		for(i=1;i<=count();i++){
			style=(bStyle*)get(i);
			if(!style){
				continue;
			}
			if((!style->visible())||(!style->selectable())){
				continue;
			}
			for(j=1;j<=style->objcount();j++){
				scr=style->objget(j);
				if(!scr){
					continue;
				}
				if(!scr->getreference()->selected()){
					continue;
				}
				if(!scr->visible()){
					continue;
				}
				scr->getbounds(&xmin,&ymin,&xmax,&ymax);
				CGContextAddRect(ctx,CGRectInset(CGRectMake(xmin,ymin,xmax-xmin,ymax-ymin),-2,-2));
			}
		}
	}
	CGContextDrawPath(ctx,_selg->pathDrawingMode());
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::DrawContrastes(CGContextRef ctx, bGenericGeoElement* o){
	if(cntmgr->count()<=0){
		return;
	}
	if(!bMacMapGeoElement::get_inval()){
		return;
	}
	
int         i,j,k,l,n;
float       xmin,ymin,xmax,ymax;
bStyle*     style;
bScreenObj* scr;
	
	CGContextSaveGState(ctx);
	_cntg->apply(ctx);
	if(o){
		for(i=1;i<=count();i++){
			style=(bStyle*)get(i);
			if(!style){
				continue;
			}
			if((!style->visible())||(!style->selectable())){
				continue;
			}
			l=style->findclass(o);
			if(!l){
				continue;
			}
			k=style->first(l);
			n=style->last(l);
			if((k==0)||(n==0)){
				continue;
			}
			if(n<k){
				n=style->first(l);
				k=style->last(l);
			}
			for(j=k;j<=n;j++){
				scr=style->objget(j);
				if(!scr){
					continue;
				}
				if(scr->getreference()!=o){
					continue;
				}
				if(!scr->getreference()->contrasted()){
					continue;
				}
				if(!scr->visible()){
					continue;
				}
				scr->getbounds(&xmin,&ymin,&xmax,&ymax);
				CGContextAddRect(ctx,CGRectInset(CGRectMake(xmin,ymin,xmax-xmin,ymax-ymin),-4,-4));
				break;
			}
		}
	}
	else{		
		for(i=1;i<=count();i++){
			style=(bStyle*)get(i);
			if(!style){
				continue;
			}
			if((!style->visible())||(!style->selectable())){
				continue;
			}
			for(j=1;j<=style->objcount();j++){
				scr=style->objget(j);
				if(!scr){
					continue;
				}
				if(!scr->getreference()->contrasted()){
					continue;
				}
				if(!scr->visible()){
					continue;
				}
				scr->getbounds(&xmin,&ymin,&xmax,&ymax);
				CGContextAddRect(ctx,CGRectInset(CGRectMake(xmin,ymin,xmax-xmin,ymax-ymin),-4,-4));
			}
		}
	}
	CGContextDrawPath(ctx,_cntg->pathDrawingMode());
	CGContextRestoreGState(ctx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::InvalElement(bGenericGeoElement* o){
	if(!o->atscreen()){
		return;
	}
int     l;
CGRect  r;
bStyle* style;
		
	for(int i=1;i<=count();i++){
		style=(bStyle*)get(i);
		if(!style){
			continue;
		}
		if(!style->visible()){
			continue;
		}
		if(style->gettype()==NULL){
			continue;
		}
		if(style->gettype()->index()!=o->getType()){
			continue;
		}
		l=style->findclass(o);
		if(!l){
			continue;
		}			
		style->rect(o,&r);
		
		_MMAPP_->mapIntf()->inval(r);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::ViewChanged(){
	unload(false);
	load();
	_MMAPP_->mapIntf()->inval();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::SetObjInvalidation(bool b){
_bTrace_("bLayersMgr::SetObjInvalidation",true);
_tm_("set to "+b);
    bMacMapGeoElement::set_inval(b);
	if(b){
		_MMAPP_->mapIntf()->inval();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::BackgroundChanged(){
float					clr[5];
int						spc;
bGenericGraphicContext*	ctx;

	GetBackground(clr,&spc);
	for(int i=1;i<=_ctxs.count();i++){
		_ctxs.get(i,&ctx);
		ctx->set_background(clr,spc);
	}
	_MMAPP_->mapIntf()->inval();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bLayersMgr::GetBackground(float* color, int* spc){
bGenericXMLBaseElement	*root,*elt;
int						i;
	for(i=1;i<=5;i++){
		color[i-1]=1;
	}
	*spc=_rgb;
	if(!map_doc->readTree(&root,kBackgroundSignature,kBackgroundConfigName)){
		return;
	}
char	val[_values_length_max_];
	for(i=1;i<=5;i++){
		elt=clssmgr->NthElement(root,i,"float");
		if(elt){
			elt->getvalue(val);
			color[i-1]=matof(val);
		}
		else{
			break;
		}
	}
	if(i>5){
		*spc=_cmyk;
	}
	clssmgr->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
//
// -----------
bGenericLayersMgr*  bLayersMgr::clone(){
bLayersMgr* mgr=new bLayersMgr();

bGenericGraphicContext*	ctx;
    for(int i=1;i<=_ctxs.count();i++){
        _ctxs.get(i,&ctx);
        mgr->_ctxs.add(&ctx);
    }
    
    mgr->_ctx=_ctx;
    
    if(mgr->curview_load()!=noErr){
    }

    mgr->_selg=new bGraphicParams(kStdGraphicParamsSelectionName,_MMAPP_);
    mgr->_cntg=new bGraphicParams(kStdGraphicParamsContrastesName,_MMAPP_);

    return mgr;
}

// ---------------------------------------------------------------------------
//
// -----------
void bLayersMgr::cloneDelete(bGenericLayersMgr* mgr){
bLayersMgr* lmgr=(bLayersMgr*)mgr;
    
    lmgr->_ctxs.reset();
    lmgr->_ctx=NULL;
    
    delete lmgr;
}


// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::addlayer(void* elt){
_bTrace_("bLayersMgr::addlayer",true);
_tm_("addlayer");
bStyle*	style=new bStyle(this);
    if(!style){
        return(false);
    }
    if(!_elts->add(&style)){
        return(false);
    }
    style->setlayer(elt);
    _cursor=count();
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::addvirtuallayer(void* elt){
_bTrace_("bLayersMgr::addvirtuallayer",true);
_tm_("addlayer");
bStyle*	style=new bVirtualStyle(this);
    if(!style){
        return(false);
    }
    if(!_elts->add(&style)){
        return(false);
    }
    style->setlayer(elt);
    _cursor=count();
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::settype(const char *name){
_bTrace_("bLayersMgr::settype",true);
bGenericStyle*	style;
    if(!_elts->get(_cursor,&style)){
        return;
    }
    style->settype(name);
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setstyle(const char *name){
bStyle*	style;
    if(!_elts->get(_cursor,&style)){
        return;
    }
    style->setname(name);
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setvisible(bool vis){
bStyle*	style;
    if(!_elts->get(_cursor,&style)){
        return;
    }
    style->setvisible(vis);
bGenericXMLBaseElement	*elt=clssmgr->NthElement(_array,_cursor,"visible");
    if(elt){
        elt->setvalue(vis?(char*)"1":(char*)"0");
    }
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setselectable(bool sel){
bStyle*	style;
    if(!_elts->get(_cursor,&style)){
        return;
    }
    style->setselectable(sel);
bGenericXMLBaseElement	*elt=clssmgr->NthElement(_array,_cursor,"selectable");
    if(elt){
        elt->setvalue(sel?(char*)"1":(char*)"0");
    }
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setroot(void* elt){
bStyle*	style;
    if(!_elts->get(_cursor,&style)){
        return;
    }
    style->setroot(elt);
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setarray(void* elt){
_bTrace_("bLayersMgr::setarray",true);
_array=(bGenericXMLBaseElement*)elt;
_tm_("-> done");
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::setidentification(void* elt){
_bTrace_("bLayersMgr::setidentification",true);
    _identifier=(bGenericXMLBaseElement*)elt;
_tm_("-> done");
}

// ---------------------------------------------------------------------------
//
// ------------
void bLayersMgr::flush(){
bStyle*	style;
    
    for(int i=1;i<=_elts->count();i++){
        if(_elts->get(i,&style)){
            delete style;
        }
    }
    _elts->reset();
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::parse(int idx){
bStyle*	style;
    if(_elts->get(idx,&style)){
        return(style->load());
    }
    return(false);
}

// ---------------------------------------------------------------------------
//
// ------------
int bLayersMgr::count(){
    return(_elts->count());
}

// ---------------------------------------------------------------------------
//
// ------------
bGenericStyle* bLayersMgr::get(int idx){
bGenericStyle*	style;
    if(!_elts->get(idx,&style)){
        return(NULL);
    }
    return(style);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::set_current(int idx){
    if((idx<1)||(idx>count())){
        return(false);
    }
    _cursor=idx;
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
int bLayersMgr::get_current(){
    return(_cursor);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::move(int idx, int offset){
    if(!_elts->push(idx,offset)){
        return(false);
    }
    _array->pushelement(idx,offset);
    if(idx==_cursor){
        _cursor+=offset;
    }
    _MMAPP_->mapIntf()->inval();
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::remove(int idx){
bStyle*	style;
    if(!_elts->get(idx,&style)){
        return(false);
    }
    delete style;
    if(!_elts->rmv(idx)){
        return(false);
    }
bGenericXMLBaseElement*	elt=_array->getelement(idx);
    _array->rmvelement(idx);
    if(elt){
        bGenericXMLBaseElement*	k=elt->instance();
        k->kill(elt);
    }
    if((idx==_cursor)&&(idx==count()+1)){
        _cursor=count();
    }
    _MMAPP_->mapIntf()->inval();
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::add(int tidx, int sidx){
bGenericType*	tp=typesmgr->get(tidx);
bStyle*			style;
    
    if(tp){
        style=new bStyle(this);
    }
    else{
        style=new bVirtualStyle(this);
    }
    
    if(!style){
        return(false);
    }
    if(!_elts->add(&style)){
        delete style;
        return(false);
    }
    
bArray  arr(sizeof(xmlelt));
    if(tp){
        add_cdesc(arr,0,"layer","");
    }
    else{
        add_cdesc(arr,0,"virtuallayer","");
    }
    
char name[256];
    if(tp){
        tp->name(name);
    }
    else{
        message_string(kMsgVirtual,name,1);
    }
    style->settype(name);
    if(tp){
        add_cdesc(arr,1,"type",name);
    }
    
    if(tp){
        tp->styles()->get_name(sidx,name);
    }
    else{
        map_doc->styles()->get_name(sidx,name);
    }
    style->setname(name);
    add_cdesc(arr,1,"style",name);
    style->setvisible(true);
    add_idesc(arr,1,"visible",1);
    style->setselectable(true);
    add_idesc(arr,1,"selectable",(tp!=0));
    
bGenericXMLBaseElement	*root=clssmgr->ParseXMLDescriptors(&arr);
    free_descs(arr);
    if(!root){
        delete style;
        _elts->rmv(_elts->count());
        return(false);
    }
    style->setlayer(root);
    _array->addelement(root);
    parse(_elts->count());
    _MMAPP_->mapIntf()->inval();
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bLayersMgr::change(int idx, int sidx){
_bTrace_("bLayersMgr::change",true);
bStyle*	style;
    if(!_elts->get(idx,&style)){
_te_("_elts->get failed for "+idx);
        return(false);
    }
bool					v=style->visible();
bool					s=style->selectable();
bGenericXMLBaseElement*	root;
char					name[256];
    
    if(style->is_virtual()){
        delete style;
        style=new bVirtualStyle(this);
        if(!_elts->put(idx,&style)){
_te_("_elts->put failed for virtualstyle "+idx);
            return(false);
        }
        map_doc->styles()->get_name(sidx,name);
_tm_("changing "+name+" (virtual)");
    }
    else{
bGenericType*			tp=style->gettype();
        if(tp==NULL){
_te_("NULL type");
            return(false);
        }
        delete style;
        style=new bStyle(this);
        if(!_elts->put(idx,&style)){
_te_("_elts->put failed for style "+idx);
            return(false);
        }
        tp->name(name);
        style->settype(name);
        tp->styles()->get_name(sidx,name);
_tm_("changing "+name);
    }
    
    style->setname(name);
    style->setvisible(v);
    style->setselectable(s);
    root=GetIndlayer(_array,idx);
    if(root==NULL){
_te_("NULL root");
        return(false);
    }
    
bGenericXMLBaseElement*	elt=clssmgr->NthElement(root,1,"style");
    if(elt==NULL){
_te_("NULL style element");
        _array->dotoall(NULL,0,debugDump);		
        return(false);
    }
    elt->setvalue(name);
    style->setlayer(root);	
    parse(idx);
    _MMAPP_->mapIntf()->inval();
    check_on_screen();
    return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bLayersMgr::check_on_screen(){
_bTrace_("bLayersMgr::check_on_screen",true);
bStyle*					style;
int						sidx;
bGenericXMLBaseElement*	root;
bGenericType*			tp;
    
    for(int i=1;i<=_elts->count();i++){
        if(!_elts->get(i,&style)){
        }
        root=style->root();
        if(!root){
            continue;
        }
        tp=style->gettype();
        if(tp){
            sidx=tp->styles()->index(root);
            if(sidx==0){
                continue;
            }
            tp->styles()->set_on_screen(sidx,true);
        }
        else{
            sidx=map_doc->styles()->index(root);
            if(sidx==0){
                continue;
            }
            map_doc->styles()->set_on_screen(sidx,true);
        }
    }
}


