//----------------------------------------------------------------------------
// File : bStdXMLLowRenderElement.cpp
// Project : MacMap
// Purpose : C++ source file : XML rendering base class (low level)
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
// 13/02/2004 creation.
//----------------------------------------------------------------------------

#include "bStdXMLLowRenderElement.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMLLowRenderElement	::bStdXMLLowRenderElement(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl)
						: bStdXMLRenderingElement(elt,gapp,bndl){
	_subclss=kXMLSubClassLowRendering;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMLLowRenderElement::~bStdXMLLowRenderElement(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdXMLLowRenderElement::init(void* ctx){	
	if(_elts.count()>0){
		setobjectcompliant(true);
	}
bGenericType*			tp=((bGenericStyle*)ctx)->gettype();
	if(!tp){
		return;
	}
bStdXMLValueElement*	elt;
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			elt->init(tp);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::action(bGenericGraphicContext *ctx){
//_bTrace_("bStdXMLLowRenderElement::action",true);
bGenericGeoElement*			geo=ctx->getCurElement();
	if(geo){
bStdXMLValueElement*	elt;			
		for(int i=1;i<=_elts.count();i++){
			if(_elts.get(i,&elt)){
				if(actionval(ctx,elt,geo)){
					return(true);
				}
			}
		}
	}
	return(actionstd(ctx));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::actionstd(bGenericGraphicContext *ctx){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::actionval(bGenericGraphicContext* ctx,
										bStdXMLValueElement* elt,
										bGenericGeoElement* geo){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::applyforscale(bGenericGraphicContext *ctx){
//_bTrace_("bStdXMLLowRenderElement::applyforscale",true);
	if(!scalecompliant()){
		return(true);
	}
	return(action(ctx));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::applyforclass(bGenericGraphicContext *ctx){
//_bTrace_("bStdXMLLowRenderElement::applyforclass",true);
	if(!classcompliant()){
		return(true);
	}
	return(action(ctx));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLLowRenderElement::applyforobject(bGenericGraphicContext *ctx){
//_bTrace_("bStdXMLLowRenderElement::applyforobject",true);
	if(!objectcompliant()){
		return(true);
	}
	return(action(ctx));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdXMLValueElement* bStdXMLLowRenderElement::find_value(){
//_bTrace_("bStdXMLLowRenderElement::find_value",true);
char					value[_values_length_max_]="";
bStdXMLValueElement*	elt=NULL;
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			elt->getvalue(NULL,value);
//_tm_("value="+value);
			if(strlen(value)>0){
				return(elt);
			}
		}
	}
	return(NULL);
}
