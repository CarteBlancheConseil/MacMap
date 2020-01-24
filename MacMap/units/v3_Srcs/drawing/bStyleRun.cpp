//----------------------------------------------------------------------------
// File : bStyleRun.cpp
// Project : MacMap
// Purpose : C++ source file : Style param set for a class of object
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
void bStyleRun::setclassmin(long idx){
	_classmin=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setclassmax(long idx){
//_bTrace_("bStyleRun::setclassmax",false);_tm_(idx);
	_classmax=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStyleRun::setpass(long idx){
	_pass=idx;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::good(double scale, long clss, long pass){
//_bTrace_("bStyleRun::good",true);
//    _tm_(_classmin+" < "+clss+" < "+_classmax);
	if(clss<_classmin){
		return(false);
	}
	if(clss>=_classmax){
		return(false);
	}
//    _tm_(pass+" = "+_pass);
	if(pass!=_pass){
		return(false);
	}
//    _tm_(_scalemin+" < "+scale+" < "+_scalemax);
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
	if(!_geometry->applyforclass(ctx)){
		return(false);
	}
	return(_render->applyforclass(ctx));
}


// ---------------------------------------------------------------------------
// 
// ------------
bool bStyleRun::applyforobject(bGenericGraphicContext* ctx){
	if(!_geometry->applyforobject(ctx)){
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

