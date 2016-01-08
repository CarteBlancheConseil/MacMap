//----------------------------------------------------------------------------
// File : bLayersMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Layers management class
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
// 30/05/2003 creation.
// 29/08/2014 suppression QD, mapwd safe.
//----------------------------------------------------------------------------

#include "bLayersMgr.h"
#include "bMacMapApp.h"
#include "bScaleMgr.h"
#include "bClassesMgr.h"
#include "bTypesMgr.h"
#include "bSelectionMgr.h"
#include "bContrastesMgr.h"
#include "bStyle.h"
#include "bScreenObj.h"
#include "bMacMapGeoElement.h"
#include "bMacMapLayerAccessContext.h"
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/bStdWait.h>
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
// Constructeur
// ------------
bLayersMgr	::bLayersMgr()
			:_ctxs(sizeof(bGenericGraphicContext*)){
	_ctx=NULL;
	_selg=NULL;
	_cntg=NULL;
	_drawing=false;
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
}

// ---------------------------------------------------------------------------
// Chargement classes XML externes
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
	
bMacMapLayerAccessContext	*ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
	if(ctxl->load()!=noErr){
_le_("ctxl->load()!=noErr");
		return(-1);
	}
	
	_selg=new bGraphicParams(kStdGraphicParamsSelectionName,_MMAPP_);
	_cntg=new bGraphicParams(kStdGraphicParamsContrastesName,_MMAPP_);
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
bMacMapLayerAccessContext*	ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
	if(save){
		ctxl->unload();
	}
	else{
		ctxl->flush();
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
_lm_("_ctx set to "+&sgn);
	if(data){
		switch(sgn){
			case kPDFGraphicContext:
			case kKMLGraphicContext:{
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
void bLayersMgr::DrawLayers(void* wd/*, CGrafPtr port*/, ivx_rect* bounds){
_lbTrace_("bLayersMgr::drawLayers",true);
long						i;
bStyle*						style;
bMacMapLayerAccessContext*	ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
		
	_breaked=false;
	
//	_ctx->set_window((WindowRef)wd);// Doit pouvoir être viré, y compris le set_window du ctx

double	d=scalemgr->get()->coef();
	_ctx->setScale(d);

// Reset des éléments à l'écran
_lm_("styles reset");
	for(i=1;i<=ctxl->count();i++){
		style=(bStyle*)ctxl->get(i);
		if(!style){
_le_("NULL style "+i);
			continue;
		}
//_lm_("style reset "+i+"/"+style->getname());
		style->flushscreen();
	}
	
	
// Init des styles	
_lm_("styles init");
	for(i=1;i<=ctxl->count();i++){
		if(_breaked){
_lm_("_breaked");
			break;
		}
		style=(bStyle*)ctxl->get(i);
		if(!style){
_le_("NULL style");
			continue;
		}
//_lm_("style init "+i+"/"+style->getname());
		style->setcontext(_ctx);
		style->setbounds(bounds);
	}

	_drawing=true;

bCursWait	wt(true);
	
// Dessin des styles	
_lm_("styles draw :");
	_ctx->beginDraw();
	for(i=1;i<=ctxl->count();i++){
		if(_breaked){
_lm_("_breaked");
			break;
		}
		style=(bStyle*)ctxl->get(i);
		if(!style){
_le_("NULL style");
			continue;
		}
_lm_("draw "+i+"/"+style->getname());
		style->draw(wt);
		_ctx->flush();
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
//		_ctx->flush();
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
	
int							i,j,k,l,n;
float						xmin,ymin,xmax,ymax;
bStyle*						style;
bMacMapLayerAccessContext	*ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
bScreenObj*					scr;
	
	CGContextSaveGState(ctx);
	_selg->apply(ctx);
	if(o){
		for(i=1;i<=ctxl->count();i++){
			style=(bStyle*)ctxl->get(i);
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
		for(i=1;i<=ctxl->count();i++){
			style=(bStyle*)ctxl->get(i);
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
	
int							i,j,k,l,n;
float						xmin,ymin,xmax,ymax;
bStyle*						style;
bMacMapLayerAccessContext	*ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
bScreenObj*					scr;
	
	CGContextSaveGState(ctx);
	_cntg->apply(ctx);
	if(o){
		for(i=1;i<=ctxl->count();i++){
			style=(bStyle*)ctxl->get(i);
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
		for(i=1;i<=ctxl->count();i++){
			style=(bStyle*)ctxl->get(i);
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
int							l;
CGRect						r;
bStyle*						style;
bMacMapLayerAccessContext*	ctxl=(bMacMapLayerAccessContext*)_MMAPP_->layersAccessCtx();
		
	for(int i=1;i<=ctxl->count();i++){
		style=(bStyle*)ctxl->get(i);
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

#warning PROBLEME SUR LES RECT		
		
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
