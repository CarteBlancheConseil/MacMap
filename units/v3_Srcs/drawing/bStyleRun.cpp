//----------------------------------------------------------------------------
// File : bStyleRun.cpp
// Project : MacMap
// Purpose : C++ source file : Style param set for a class of object
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
// 06/02/2004 creation.
//----------------------------------------------------------------------------

#include "bStyleRun.h"

#include "bMacMapApp.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStyleRun::bStyleRun(){
	_scalemin=0;
	_scalemax=LONG_MAX;
	_classmin=0;
	_classmax=LONG_MAX;
	_pass=1;
	
	_root=NULL;
	_validity=NULL;
	_render=NULL;
	_geometry=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bStyleRun::~bStyleRun(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setroot(void *elt){
	_root=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setvalidity(void *elt){
	_validity=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setrender(void *elt){
	_render=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setgeometry(void *elt){
	_geometry=(bGenericXMLRenderingElement*)elt;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setscalemin(double d){
	_scalemin=d;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setscalemax(double d){
	_scalemax=d;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setclassmin(int idx){
	_classmin=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setclassmax(int idx){
	_classmax=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setpass(int idx){
	_pass=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::good(double scale, int clss, int pass){
	if(clss<_classmin){
		return(false);
	}
	if(clss>=_classmax){
		return(false);
	}
	if(pass!=_pass){
		return(false);
	}
	if(scale<_scalemin){
		return(false);
	}
	if(scale>=_scalemax){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::applyforclass(bGenericGraphicContext* ctx){
//_bTrace_("bStyleRun::applyforclass",true);
//_tm_("_geometry->applyforclass");
	if(!_geometry->applyforclass(ctx)){
		return(false);
	}
//_tm_("_render->applyforclass");
	return(_render->applyforclass(ctx));
}


// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::applyforobject(bGenericGraphicContext* ctx){
	if(!_geometry->applyforobject(ctx)){
//_MMAPP_->message("bStyleRun::applyforobject : bad ");
		return(false);
	}
	return(_render->applyforobject(ctx));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::applygeometryforclass(bGenericGraphicContext* ctx){
	return(_geometry->applyforclass(ctx));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::applygeometryforobject(bGenericGraphicContext* ctx){
	return(_geometry->applyforobject(ctx));
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bStyleRun::root(){
	return(_root);
}

