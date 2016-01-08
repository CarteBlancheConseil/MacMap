//----------------------------------------------------------------------------
// File : bStdXMLRenderingElement.cpp
// Project : MacMap
// Purpose : C++ source file : XML rendering base class
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
// Classe de base MacMap
//----------------------------------------------------------------------------
// 22/10/2003 creation.
//----------------------------------------------------------------------------

#include "bStdXMLRenderingElement.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMLRenderingElement	::bStdXMLRenderingElement(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl)
						: bStdXMLBaseElement(elt,gapp,bndl){
	_scalecmp=false;
	_classcmp=false;
	_objectcmp=false;
	_clss=kXMLClassRendering;
	_subclss=kXMLSubClassRendering;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMLRenderingElement::~bStdXMLRenderingElement(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLRenderingElement::init(void* ctx){
int							i;
bStdXMLRenderingElement*	elt;
	
	for(i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			elt->init(ctx);
		}
	}
	
	for(i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			if(elt->scalecompliant()){
				setscalecompliant(true);
			}
			if(elt->classcompliant()){
				setclasscompliant(true);
			}
			if(elt->objectcompliant()){
				setobjectcompliant(true);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::action(bGenericGraphicContext *ctx){
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::applyforscale(bGenericGraphicContext *ctx){
	if(!scalecompliant()){
		return(true);
	}
	if(!action(ctx)){
		return(false);
	}
bStdXMLRenderingElement*	elt;	
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			if(!elt->applyforscale(ctx)){
				return(false);
			}
		}
		else{
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::applyforclass(bGenericGraphicContext *ctx){
	if(!classcompliant()){
		return(true);
	}
	if(!action(ctx)){
		return(false);
	}
bStdXMLRenderingElement*	elt;	
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			if(!elt->applyforclass(ctx)){
				return(false);
			}
		}
		else{
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::applyforobject(bGenericGraphicContext *ctx){
	if(!objectcompliant()){
		return(true);
	}
	if(!action(ctx)){
		return(false);
	}
bStdXMLRenderingElement*	elt;	
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			if(!elt->applyforobject(ctx)){
				return(false);
			}
		}
		else{
			return(false);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::scalecompliant(){
	return(_scalecmp);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::classcompliant(){
	return(_classcmp);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLRenderingElement::objectcompliant(){
	return(_objectcmp);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLRenderingElement::setscalecompliant(bool b){
	_scalecmp=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLRenderingElement::setclasscompliant(bool b){
	_classcmp=b;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLRenderingElement::setobjectcompliant(bool b){
	_objectcmp=b;
}
