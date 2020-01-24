//----------------------------------------------------------------------------
// File : bToolOffset.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (offset by field) tool class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2006 Carte Blanche Conseil.
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
// 29/11/2006 creation.
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolOffset.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolOffset	::bToolOffset(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolPres(elt,gapp,bndl){
	setclassname("offsettool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolOffset::~bToolOffset(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolOffset::create(bGenericXMLBaseElement* elt){
	return(new bToolOffset(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolOffset::clic(CGPoint pt, int count){
	bStdToolPres::clic(pt,count);
	if(count>1){
		return;
	}
	
bArray*					ga;
bArray					runs(sizeof(bGenericXMLBaseElement*));
bGenericXMLBaseElement	*run,*gm,*elt;
bGenericGeoElement*		o=NULL;
long					i,n=_gapp->layersMgr()->count();
CGPoint					p;
	
	set_obj(NULL);
	get_clic(&p);
	
	_fx=0;
	_fy=0;
	_styl=NULL;
	
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
		_styl=NULL;
		return;
	}

bGenericUnit*	scl=_gapp->scaleMgr()->get();

	_styl->get_styleruns(runs,o,scl->coef(),1);

char			val[_values_length_max_];
bGenericType	*tp=_gapp->typesMgr()->get(o->getType());

	for(int i=1;i<=runs.count();i++){
		runs.get(i,&run);
		_fx=0;
		_fy=0;
		gm=_gapp->classMgr()->NthElement(run,1,"offsetcoord");
		if(!gm){
			continue;
		}
		elt=_gapp->classMgr()->NthElement(gm,1,"field");
		if(!elt){
			continue;
		}
		elt->getvalue(val);
		_fx=tp->fields()->get_index(val);
		if(!_fx){
			continue;
		}
		elt=_gapp->classMgr()->NthElement(gm,2,"field");
		if(!elt){
			continue;
		}
		elt->getvalue(val);
		_fy=tp->fields()->get_index(val);
		if(!_fy){
			continue;
		}
		break;
	}
	if(_fx&&_fy){
		set_obj(o);
		set_use_drag(true);
		o->getValue(_fx,&_orig.x);
		o->getValue(_fy,&_orig.y);		
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolOffset::end_clic(){
	bStdToolPres::end_clic();
	if(!get_use_drag()){
		return;
	}
	if(!_fx||!_fy||!_styl){
		return;
	}
bGenericGeoElement*	o=get_obj();
	if(!o){
		return;
	}
	
d2dvertex	off;
	
	o->getValue(_fx,&off.x);
	o->getValue(_fy,&off.y);

// Pour avoir la valeur précédente en annulation
	o->setValue(_fx,_orig.x);
	o->setValue(_fy,_orig.y);
CGRect	bnds;
	_styl->rect(o,&bnds);
	_gapp->mapIntf()->inval(bnds);		
	
bEventLog	log(_gapp,
				getbundle(),
				kOffsetMessageID,
				GetSignature(this));
	o->setValue(_fx,off.x);
	o->setValue(_fy,off.y);
	log.close();
	_styl->rect(o,&bnds);
	_gapp->mapIntf()->inval(bnds);		
	
	set_obj(NULL);
	set_use_drag(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolOffset::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	bGenericGeoElement*	o=get_obj();
	if(o){
		clearTempPathContext(true);
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
void bToolOffset::drag(CGPoint pt){
	bStdToolPres::drag(pt);
	if(!_fx||!_fy||!_styl){
		return;
	}
bGenericGeoElement*	o=get_obj();
	if(!o){
		return;
	}

i2dvertex	ivx;
d2dvertex	a,b,off;

	get_clic(&ivx);
	Convert(&a,&ivx);
	get_cur(&ivx);
	Convert(&b,&ivx);
	off=_orig;
	if((off.x==0)&&(off.y==0)){
		ivertices*	vxs;
		o->getVertices(&vxs);
		Convert(&off,&vxs->vx.vx2[0]);
	}
	off.x+=(b.x-a.x);
	off.y+=(b.y-a.y);

	o->setValue(_fx,off.x);
	o->setValue(_fy,off.y);
}

