//----------------------------------------------------------------------------
// File : bCGLocConverter.cpp
// Project : MacMap
// Purpose : C++ source file : Coordinates conversion class
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
// 25/11/2004 creation.
//----------------------------------------------------------------------------

#include "bCGLocConverter.h"
#include "bMacMapApp.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCGLocConverter::bCGLocConverter(){
	_method=kTransformationNone;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bCGLocConverter::~bCGLocConverter(){
}

// ---------------------------------------------------------------------------
// A+
// -----------
bool bCGLocConverter::convert(i2dvertex* lt, i2dvertex* lf){
	(*lt)=(*lf);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCGLocConverter::convert(i2dvertex* lt, d2dvertex* lf){
bGenericUnit*	ur=_MMAPP_->document()->dist_resolution_unit();
bGenericUnit*	up=_MMAPP_->distMgr()->get();
double			r=((ur->coef()*_MMAPP_->document()->dist_resolution())/up->coef());
d2dvertex		origin=_MMAPP_->document()->get_origin();

	lt->h=round((lf->x-origin.x)/r);
	lt->v=round(-(lf->y-origin.y)/r);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCGLocConverter::convert(i2dvertex* lt, CGPoint* lf){
	lt->h=round(((double)lf->x)/_conv)+_port_bnds.left;
	lt->v=round(((double)(_port_rect.origin.y+_port_rect.size.height)-lf->y)/_conv)+_port_bnds.top;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCGLocConverter::convert(d2dvertex* lt, i2dvertex* lf){
bGenericUnit*	ur=_MMAPP_->document()->dist_resolution_unit();
bGenericUnit*	up=_MMAPP_->distMgr()->get();
double			r=((ur->coef()*_MMAPP_->document()->dist_resolution())/up->coef());
double			x=lf->h;
double			y=lf->v;
d2dvertex		origin=_MMAPP_->document()->get_origin();

	lt->x=x*r;
	lt->y=(-y)*r;
	lt->x+=origin.x;
	lt->y+=origin.y;
	return(true);
}

// ---------------------------------------------------------------------------
// A+
// -----------
bool bCGLocConverter::convert(d2dvertex* lt, d2dvertex* lf){
	(*lt)=(*lf);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCGLocConverter::convert(d2dvertex* lt, CGPoint* lf){
bGenericUnit*	ur=_MMAPP_->document()->dist_resolution_unit();
bGenericUnit*	up=_MMAPP_->distMgr()->get();
double			r=((ur->coef()*_MMAPP_->document()->dist_resolution())/up->coef());
d2dvertex		origin=_MMAPP_->document()->get_origin();

	lt->x=(((double)lf->x)/_conv)+(double)_port_bnds.left;
	lt->y=(((double)lf->y)/_conv)+(double)(-_port_bnds.bottom);
	lt->x*=r;
	lt->y*=r;
	lt->x+=origin.x;
	lt->y+=origin.y;
	return(true);
}

// ---------------------------------------------------------------------------
// OK
// -----------
bool bCGLocConverter::convert(CGPoint* lt, i2dvertex* lf){
	lt->x=((double)(lf->h-_port_bnds.left))*_conv;
	lt->y=((double)((-lf->v)-(-_port_bnds.bottom)))*_conv;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCGLocConverter::convert(CGPoint* lt, d2dvertex* lf){
i2dvertex	vx;
	convert(&vx,lf);
	convert(lt,&vx);
	return(true);
}

// ---------------------------------------------------------------------------
// A+
// -----------
bool bCGLocConverter::convert(CGPoint* lt, CGPoint* lf){
	(*lt)=(*lf);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::set_lockup(dvertices* fich,	
								 dvertices* click,	
								 int method){
	switch(method){
		case kTransformationSimil:
			SIMIL_Init(&_CalP,fich,click);
			_method=kTransformationSimil;
dump_calp(&_CalP);
			break;
		default:
			_method=kTransformationNone;
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::transform(i2dvertex* fich, 
								i2dvertex* click){	
	if(_method==kTransformationNone){
		*fich=*click;
		return;
	}
d2dvertex	dfich,dclick;
	convert(&dclick,click);
	transform(&dfich,&dclick);
	convert(fich,&dfich);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::transform(d2dvertex* fich, 
								d2dvertex* click){
	switch(_method){
		case kTransformationSimil:
			SIMIL_Transform(fich,click,&_CalP);
			break;
		default:
			*fich=*click;
			break;
	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::transform(CGPoint* fich, 
								CGPoint* click){
	if(_method==kTransformationNone){
		*fich=*click;
		return;
	}
d2dvertex	dfich,dclick;
	convert(&dclick,click);
	transform(&dfich,&dclick);
	convert(fich,&dfich);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::init(ivx_rect* vxr){	
	_port_bnds=(*vxr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::reset(ivx_rect* vxr){
_bTrace_("bCGLocConverter::reset",true);
	_conv=_MMAPP_->scaleMgr()->get()->conv();
	
	_port_bnds=(*vxr);
_tm_(_trrect_(_port_bnds));

CGPoint		cgr;
i2dvertex	vx;
	
	vx.h=vxr->left;
	vx.v=vxr->bottom;
	convert(&cgr,&vx);
	_port_rect.origin.x=cgr.x;
	_port_rect.origin.y=cgr.y;
	
	vx.h=vxr->right;
	vx.v=vxr->top;
	convert(&cgr,&vx);
	_port_rect.size.width=cgr.x-_port_rect.origin.x;
	_port_rect.size.height=cgr.y-_port_rect.origin.y;
	
_tm_(_trxysz_(_port_rect));
}

#pragma mark -> DEBUG
// ---------------------------------------------------------------------------
// 
// -----------
void bCGLocConverter::dump_calp(SIMILPrm* CalP){
fprintf(stderr,"----------------------\n");
fprintf(stderr,"Tx=%f\n",CalP->Tx);
fprintf(stderr,"Ty=%f\n",CalP->Ty);
fprintf(stderr,"R[1]=%f\n",CalP->R[1]);
fprintf(stderr,"R[2]=%f\n",CalP->R[2]);
fprintf(stderr,"R[3]=%f\n",CalP->R[3]);
fprintf(stderr,"R[4]=%f\n",CalP->R[4]);
fprintf(stderr,"R[5]=%f\n",CalP->R[5]);
fprintf(stderr,"R[6]=%f\n",CalP->R[6]);
fprintf(stderr,"EchelleX=%f\n",CalP->EchelleX);
fprintf(stderr,"EchelleY=%f\n",CalP->EchelleY);
fprintf(stderr,"ResiduX=%f\n",CalP->ResiduX);
fprintf(stderr,"ResiduY=%f\n",CalP->ResiduY);
}
