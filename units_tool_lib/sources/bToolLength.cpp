//----------------------------------------------------------------------------
// File : bToolLength.cpp
// Project : MacMap
// Purpose : C++ source file : Length measurement tool class
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
// 09/05/2005 creation.
//----------------------------------------------------------------------------
// 17/06/2011 CGGeometry OK.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolLength.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolLength	::bToolLength(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdToolNav(elt,gapp,bndl){
	setclassname("lengthtool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolLength::~bToolLength(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolLength::create(bGenericXMLBaseElement* elt){
	return(new bToolLength(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLength::open(int* flags){	
	set_use_trace(true);
	bStdToolNav::open(flags);
	set_use_locs(true);
	set_use_track(true);
	set_use_angle(true);
	reset_clic();
	_len=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLength::clic(CGPoint pt, int count){
	if(count>1){
		clearTempPathContext(true);
		validTempPathContext();
		_len=0;
	}
	bStdToolNav::clic(pt,count);
	if(count==1){
		calc_len();
		report_len();
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLength::update(bool global){
	if(!get_active()){
		return;
	}
	draw_poly(false,true);
	report_len();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bToolLength::set_key(int k){
bool	b=false;
	switch(k){
		case kEnterCharCode:
		case kReturnCharCode:
			action(0);
			(void)bStdToolNav::set_key(k);
			clearTempPathContext(true);
			validTempPathContext();
			k=0;
			WriteMessage(_gapp,"");
			b=true;
			break;
		case kEscapeCharCode:
			(void)bStdToolNav::set_key(k);
			clearTempPathContext(true);
			validTempPathContext();
			k=0;			
			WriteMessage(_gapp,"");
			b=true;
			break;
		case kBackspaceCharCode:
			if(get_locs()->count()<2){
				break;
			}
			(void)bStdToolNav::set_key(k);
			clearTempPathContext(true);
			track_draw();
			validTempPathContext();
			k=0;
			calc_len();
			report_len();
			break;
	}
	(void)bStdToolNav::set_key(k);
	return(b);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLength::calc_len(){
bArray*		arr=get_locs();
	if(arr==NULL){
		_len=0;
		return;
	}
i2dvertex	vxa,vxb;

	_len=0;
	for(int i=1;i<arr->count();i++){
		arr->get(i,&vxa);
		arr->get(i+1,&vxb);
		_len+=ivx2_dist(&vxa,&vxb);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolLength::report_len(){
	if(get_locs()==NULL){
		return;
	}
	if(get_locs()->count()==0){
		return;
	}
	if(_gapp->document()==NULL){
		return;
	}
i2dvertex		vxa,vxb;

	get_clic(&vxa);
	get_cur(&vxb);

double			d=ivx2_dist(&vxa,&vxb)+_len;
bGenericUnit*	up=_gapp->distMgr()->get();
int				decs=_gapp->document()->dist_pref_digits();
char			lib[256],bf[256];
	
	up->short_name(lib);
	d=Measure_i2d(_gapp,d);
	sprintf(bf,"%.*f %s",decs,d,lib);
	WriteMessage(_gapp,bf);
}
