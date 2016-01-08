//----------------------------------------------------------------------------
// File : bXMapZoomOut.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, zoom out
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapZoomOut.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapZoomOut::bXMapZoomOut(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("zoomout");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapZoomOut::~bXMapZoomOut(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapZoomOut::create(bGenericXMLBaseElement* elt){
	return(new bXMapZoomOut(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomOut::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->scaleMgr()->set_current(_gapp->scaleMgr()->get_current()+1)){
		if(_gapp->selMgr()->count()>0){
bGenericGeoElement* o;
			_gapp->selMgr()->elements()->get(1,&o);
			if(o){
ivx_rect	vr;
i2dvertex	vx;
				o->getBounds(&vr);
				ivr_mid(&vr,&vx);
				_gapp->mapIntf()->setScreenCenter(vx);
			}		
		}
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomOut::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

