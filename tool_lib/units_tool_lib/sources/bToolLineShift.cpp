//----------------------------------------------------------------------------
// File : bToolLineShift.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (line shift by field) tool class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 19/06/2007 creation.
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolLineShift.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/bEventLog.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolLineShift	::bToolLineShift(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					: bStdToolPres(elt,gapp,bndl){
	setclassname("lineshifttool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolLineShift::~bToolLineShift(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolLineShift::create(bGenericXMLBaseElement* elt){
	return(new bToolLineShift(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::open(int* flags){
	bStdToolPres::open(flags);
	_fa=0;
	_ia=0,
	_cur=NULL;
	_styl=NULL;
	_xpts=NULL;
	_ypts=NULL;
	_offs=NULL;
	_npts=0;
	_noffs=0;
	
	set_use_angle(false);
	set_use_drag(true);
	set_use_track(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::clic(CGPoint pt, int count){
	bStdToolPres::clic(pt,count);
	if(count>1){
		return;
	}
	
	reset();
	
bArray*					ga;
bArray					runs(sizeof(bGenericXMLBaseElement*));
bGenericXMLBaseElement	*run,*gm,*elt;
bGenericGeoElement*		o=NULL;
int						i,n=_gapp->layersMgr()->count();
CGPoint					p;
	
	set_obj(NULL);	
	get_clic(&p);
	for(i=n;i>0;i--){
        _styl=_gapp->layersMgr()->get(i);
		if(!_styl){
			continue;
		}	
		ga=_styl->objsatpoint(&p,true);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(ga->count()>1){
			delete ga;
			break;
		}
		ga->get(1,&o);
		delete ga;
		break;
	}
	
	if(!o){
		reset();
		return;
	}

bGenericUnit*	scl=_gapp->scaleMgr()->get();

	_styl->get_styleruns(runs,o,scl->coef(),1);

char			val[_values_length_max_];
bGenericType	*tp=_gapp->typesMgr()->get(o->getType());

	for(int i=1;i<=runs.count();i++){
		runs.get(i,&run);
		_fa=0;
		gm=_gapp->classMgr()->NthElement(run,1,"decal");
		if(!gm){
			continue;
		}
		elt=_gapp->classMgr()->NthElement(gm,1,"field");
		if(!elt){
			continue;
		}
		elt->getvalue(val);
		_fa=tp->fields()->get_index(val);
		break;
	}
	
	if(!_fa){
		reset();
		return;
	}
	
bGenericGraphicContext*	ctx=_styl->get_context_for_object(o,0,false);
float					*xpts,*ypts;
int						*offs;

	ctx->getGeometry(&xpts,&ypts,&_npts,&offs,&_noffs);
	if(_npts<2){
		ctx->setGeometry(NULL,NULL,0,NULL,0);
		reset();
		return;
	}

	_xpts=new float[_npts];
	memmove(_xpts,xpts,_npts*sizeof(float));
	_ypts=new float[_npts];
	memmove(_ypts,ypts,_npts*sizeof(float));
	_offs=new int[_noffs];
	memmove(_offs,offs,_noffs*sizeof(int));

	ctx->setGeometry(NULL,NULL,0,NULL,0);

	o->getValue(_fa,&_ia);
	_cur=_ia;
	set_obj(o);
	
	set_use_track(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::end_clic(){
	bStdToolPres::end_clic();
	if(!get_active()){
		reset();
		return;
	}
	if(!get_use_drag()){
		reset();
		return;
	}
	
	if(!_fa||!_styl){
		reset();
		return;
	}

bGenericGeoElement*	o=get_obj();
	if(!o){
		reset();
		return;
	}
// Pour avoir la valeur précédente en annulation
	o->setValue(_fa,&_ia);
CGRect	bnds;
	_styl->rect(o,&bnds);
	_gapp->mapIntf()->inval(bnds);		
// Report de la nouvelle valeur
bEventLog	log(_gapp,
				getbundle(),
				kLineShiftMessageID,
				GetSignature(this));
	o->setValue(_fa,&_cur);
	log.close();
	_styl->rect(o,&bnds);
	_gapp->mapIntf()->inval(bnds);		

	reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::update(bool global){
	/*if(!global){
		clearTempPathContext(false);
	}*/
bGenericGeoElement*	o=get_obj();
	if(o){
		//clearTempPathContext(true);
		_gapp->layersMgr()->SwitchContext(kCtxGraphicContext,getTempPathContext());
		_styl->draw(o);
		_gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
		validTempPathContext();
	}
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::track_draw(){
	bStdToolPres::track_draw();
	if(!get_use_track()){
		return;
	}
	if(!get_active()){
		return;
	}

CGPoint		cgp;
i2dvertex	vx;
	get_cur(&vx);
	if(vx.h==INT_MIN){
		return;
	}
	Convert(&cgp,&vx);
double		val=get_dist_to_path(cgp);
	if(val!=_cur){
		_cur=val;
		put_value(_fa,val);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::put_value(int fidx, double val){
bGenericGeoElement*	o=get_obj();
	o->setValue(fidx,&val);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bToolLineShift::get_dist_to_path(CGPoint cgp){
int		idx;
float	r;
CGPoint	cgr;

float	d=XYProjOnPath(cgp.x,cgp.y,_xpts,_ypts,_npts,(float*)&cgr.x,(float*)&cgr.y,&idx,&r);
	if(idx==0){
		idx++;
	}
	r=d*XYSIAM(_xpts[idx-1],_ypts[idx-1],_xpts[idx],_ypts[idx],cgp.x,cgp.y)+_ia;
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLineShift::reset(){
	if(_xpts){
		delete _xpts;
	}
	if(_ypts){
		delete _ypts;
	}
	if(_offs){
		delete _offs;
	}
	_xpts=NULL;
	_ypts=NULL;
	_offs=NULL;
	_npts=0;
	_noffs=0;
	_fa=0;
	_ia=0;
	_cur=0;
	_styl=NULL;
	set_obj(NULL);
	set_use_track(false);
}
