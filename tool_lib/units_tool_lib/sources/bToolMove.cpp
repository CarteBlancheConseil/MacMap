//----------------------------------------------------------------------------
// File : bToolMove.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (geometry translation) tool class
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
// 16/08/2005 creation.
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolMove.h"
#include <std_ext/bXMapStdIntf.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolMove	::bToolMove(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolGeom(elt,gapp,bndl){
	setclassname("movetool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolMove::~bToolMove(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolMove::create(bGenericXMLBaseElement* elt){
	return(new bToolMove(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::open(int* flags){
	set_use_trace(true);
	bStdToolGeom::open(flags);

	_ccopy=load_curs("CopyCursor.png");
	_vxs=NULL;
	_copy=false;
	_srep=true;

	set_use_sel(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::close(){
	bStdToolGeom::close();
	if(_ccopy){
		ReleaseCocoaCursor(_ccopy);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::clic(CGPoint pt, int count){
	bStdToolGeom::clic(pt,count);
bGenericGeoElement* o=get_obj();
	if(!o){
		return;
	}
	set_use_sel(false);
	set_use_drag(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::end_clic(){
	bStdToolGeom::end_clic();
	set_use_drag(false);
	set_use_sel(true);
	
bEventLog	log(_gapp,
				getbundle(),
				kMoveMessageID,
				GetSignature(this));
	move();
	log.close();
	_vxs=NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::update(bool global){
	if(!global){
		clearTempPathContext(false);
	}
	if(get_on_drag()){
CGPoint	v1,v2;
		get_clic(&v1);
		get_cur(&v2);
		
CGFloat	dx=v2.x-v1.x;
CGFloat	dy=v2.y-v1.y;
		
bGenericGeoElement*	o;
bArray				sel(*_gapp->selMgr()->elements());
ivx_rect			vr;
		
		for(long i=1;i<=sel.count();i++){
			sel.get(i,&o);
			o->getVertices(&_vxs);
			ivs_bounds(_vxs,&vr);			
			hilite_rect(getTempPathContext(),CGRectOffset(CGRectInset(ivr2cgr(_gapp,vr),-2,-2),dx,dy),true,true);
		}
	}
	if(!global){
		validTempPathContext();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::set_modifiers(int k){	
	bStdToolGeom::set_modifiers(k);
	if(!get_active()){
		return;
	}
	if(is_modifiers(optionKey)){
		set_curs(_ccopy);
		_copy=true;
	}
	else{
		set_curs();
		_copy=false;
	}
	_srep=!is_modifiers(shiftKey);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::drag(CGPoint pt){
	bStdToolGeom::drag(pt);
	report();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::track_obj(){
	if(get_use_sel()){
bGenericGeoElement* o=get_obj();
		bStdToolGeom::track_obj();
		if(_srep){
			if(o!=get_obj()){
				FlushSelection(_gapp);
			}
			if((get_obj())&&(o!=get_obj())){
				get_obj()->setselect(true,true);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::move(){
i2dvertex	v1,v2;
	get_clic(&v1);
	get_cur(&v2);
long	dx=v2.h-v1.h;
long	dy=v2.v-v1.v;

bGenericGeoElement	*o,*no;
bArray				sel(*_gapp->selMgr()->elements());
ivertices*			vxs;

	_gapp->layersMgr()->SetObjInvalidation(false);	
	for(int i=1;i<=sel.count();i++){
		sel.get(i,&o);
		o->getVertices(&_vxs);
		ivs2ivs(_vxs,&vxs);
		if(_copy){
			if(_gapp->typesMgr()->get(o->getType())->clone_object(o,&no)){
				o=no;
			}
		}
		ivs_move(vxs,dx,dy);
		o->setVertices(vxs);
		ivs_free(vxs);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolMove::report(){
double			dx,dy;
bGenericUnit*	u=_gapp->distMgr()->get();
char			str[256],lib[64];
int				dg=_gapp->document()->dist_pref_digits();
i2dvertex		vxa,vxb;
	
	get_clic(&vxb);
	get_cur(&vxa);
	dx=Measure_i2d(_gapp,fabs(vxb.h-vxa.h));
	dy=Measure_i2d(_gapp,fabs(vxb.v-vxa.v));
	u->short_name(lib);
	
	sprintf(str,"dx=%.*f %s, dy=%.*f %s",dg,dx,lib,dg,dy,lib);
	WriteMessage(_gapp,str);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolMove::load(){
bGenericXMLBaseElement* groot=bStdToolGeom::load();
	if(!groot){
		return(NULL);
	}
bGenericXMLBaseElement* root=get_param(groot,"private");
	if(!root){
		_gapp->classMgr()->ReleaseXMLInstance(groot);
		return(NULL);
	}
	_gapp->classMgr()->ReleaseXMLInstance(groot);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLBaseElement* bToolMove::make(bArray &arr){
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version",kCurrentToolParamVers);
	add_cdesc(arr,1,"param","");
	add_cdesc(arr,2,"sign","private");
	return(bStdToolGeom::make(arr));
}
