//----------------------------------------------------------------------------
// File : bXMapsGo.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classes, scrolls screen to top, bottom, left, right, or to the cursor location.
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
// 21/02/2007 creation.
//----------------------------------------------------------------------------

#include "bXMapsGo.h"
#include <mox_intf/bGenericTool.h>
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapGoRight::bXMapGoRight(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("goright");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGoRight::~bXMapGoRight(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGoRight::create(bGenericXMLBaseElement* elt){
	return(new bXMapGoRight(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoRight::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
ivx_rect	vr;
i2dvertex	vx;

	_gapp->mapIntf()->screenBounds(&vr);
	_gapp->mapIntf()->screenCenter(&vx);
	vx.h+=round((vr.right-vr.left)/2.0);
	_gapp->mapIntf()->setScreenCenter(vx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoRight::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapGoLeft::bXMapGoLeft(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("goleft");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGoLeft::~bXMapGoLeft(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGoLeft::create(bGenericXMLBaseElement* elt){
	return(new bXMapGoLeft(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoLeft::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
ivx_rect	vr;
i2dvertex	vx;

	_gapp->mapIntf()->screenBounds(&vr);
	_gapp->mapIntf()->screenCenter(&vx);
	vx.h-=round((vr.right-vr.left)/2.0);
	_gapp->mapIntf()->setScreenCenter(vx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoLeft::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapGoUp::bXMapGoUp(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("goup");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGoUp::~bXMapGoUp(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGoUp::create(bGenericXMLBaseElement* elt){
	return(new bXMapGoUp(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoUp::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
ivx_rect	vr;
i2dvertex	vx;

	_gapp->mapIntf()->screenBounds(&vr);
	_gapp->mapIntf()->screenCenter(&vx);
	vx.v-=round((vr.bottom-vr.top)/2.0);
	_gapp->mapIntf()->setScreenCenter(vx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoUp::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapGoDown::bXMapGoDown(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("godown");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGoDown::~bXMapGoDown(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGoDown::create(bGenericXMLBaseElement* elt){
	return(new bXMapGoDown(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoDown::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
ivx_rect	vr;
i2dvertex	vx;

	_gapp->mapIntf()->screenBounds(&vr);
	_gapp->mapIntf()->screenCenter(&vx);
	vx.v+=round((vr.bottom-vr.top)/2.0);
	_gapp->mapIntf()->setScreenCenter(vx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoDown::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapGoCurs::bXMapGoCurs(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("gocurs");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGoCurs::~bXMapGoCurs(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGoCurs::create(bGenericXMLBaseElement* elt){
	return(new bXMapGoCurs(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoCurs::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}

i2dvertex		vx;
bGenericTool*	tool=(bGenericTool*)CurTool(_gapp);
	tool->get_cur(&vx);
	if(vx.h==INT_MIN){
		return(true);
	}
	_gapp->mapIntf()->setScreenCenter(vx);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGoCurs::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapZoomInCurs::bXMapZoomInCurs(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("zoomincurs");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapZoomInCurs::~bXMapZoomInCurs(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapZoomInCurs::create(bGenericXMLBaseElement* elt){
	return(new bXMapZoomInCurs(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomInCurs::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
i2dvertex		vx;
bGenericTool*	tool=(bGenericTool*)CurTool(_gapp);
	tool->get_cur(&vx);
	if(vx.h==INT_MIN){
		return(true);
	}
	
	if(_gapp->scaleMgr()->set_current(_gapp->scaleMgr()->get_current()-1)){
		_gapp->mapIntf()->setScreenCenter(vx);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomInCurs::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapZoomOutCurs::bXMapZoomOutCurs(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("zoomoutcurs");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapZoomOutCurs::~bXMapZoomOutCurs(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapZoomOutCurs::create(bGenericXMLBaseElement* elt){
	return(new bXMapZoomOutCurs(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomOutCurs::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
i2dvertex		vx;
bGenericTool*	tool=(bGenericTool*)CurTool(_gapp);
	tool->get_cur(&vx);
	if(vx.h==INT_MIN){
		return(true);
	}
	
	if(_gapp->scaleMgr()->set_current(_gapp->scaleMgr()->get_current()+1)){
		_gapp->mapIntf()->setScreenCenter(vx);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapZoomOutCurs::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
