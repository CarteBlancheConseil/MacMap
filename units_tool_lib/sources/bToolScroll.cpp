//----------------------------------------------------------------------------
// File : bToolScroll.cpp
// Project : MacMap
// Purpose : C++ source file : Scroll tool class
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
// 05/01/2005 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolScroll.h"
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolScroll	::bToolScroll(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolNav(elt,gapp,bndl){
	setclassname("scrolltool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolScroll::~bToolScroll(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolScroll::create(bGenericXMLBaseElement* elt){
	return(new bToolScroll(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolScroll::open(int* flags){	
	set_use_trace(true);
	bStdToolNav::open(flags);
	_cclose=load_curs("ClosedCursor.png");
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolScroll::close(){
	bStdToolNav::close();
	if(_cclose){
		ReleaseCocoaCursor(_cclose);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolScroll::clic(CGPoint pt, int count){
	bStdToolNav::clic(pt,count);
	_gapp->layersMgr()->StopDraw();
	if(count==1){
		set_use_drag(true);
		set_curs(_cclose);
		_cgrcur=getTempPathContextRect();
		_cgrlast=CGRectZero;
		_cgrlast.size=_cgrcur.size;
		clearTempPathContext(true);
		hilite_rect(getTempPathContext(),_cgrcur,true,true);
		validTempPathContext();
	}
	else{
i2dvertex	vx;
		get_clic(&vx);
i2dvertex	o,wo;
CGRect		cgr=getTempPathContextRect();
CGPoint		cgp=CGPointMake(CGRectGetMidX(cgr),CGRectGetMidY(cgr));
		Convert(&wo,&cgp);
		_gapp->mapIntf()->screenCenter(&o);
		vx.h+=(o.h-wo.h);
		vx.v+=(o.v-wo.v);
		_gapp->mapIntf()->setScreenCenter(vx);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolScroll::end_clic(){
	bStdToolNav::end_clic();
	if(get_use_drag()){
i2dvertex	a,b,c;
		get_clic(&a);
		get_cur(&b);
		if(b.h==LONG_MIN){
			get_last(&b);
		}
		if((b.h!=LONG_MIN)&&(!eq_ivx2(&a,&b))){
			_gapp->mapIntf()->screenCenter(&c);
			c.h-=(b.h-a.h);
			c.v-=(b.v-a.v);
			_gapp->mapIntf()->setScreenCenter(c);		
		}
		_cgrcur=CGRectZero;
		_cgrlast=CGRectZero;
		set_use_drag(false);
		set_curs();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolScroll::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	if(get_on_drag()){
CGPoint	a,b;
		get_clic(&a);
		get_cur(&b);
		if(a.x!=SHRT_MIN){
			_cgrcur=CGRectOffset(_cgrlast,b.x-a.x,b.y-a.y);
			hilite_rect(getTempPathContext(),_cgrcur,true,true);
		}
	}
	if(!global){
		validTempPathContext();
	}
}

