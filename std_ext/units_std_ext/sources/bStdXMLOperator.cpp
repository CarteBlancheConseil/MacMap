//----------------------------------------------------------------------------
// File : bStdXMLOperator.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for operator formula
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
// 17/03/2004 creation.
//----------------------------------------------------------------------------

#include "bStdXMLOperator.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMLOperatorElement	::bStdXMLOperatorElement(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						: bStdXMLFormulaElement(elt,gapp,bndl){
	_subclss=kXMLSubClassOperator;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMLOperatorElement::~bStdXMLOperatorElement(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLOperatorElement::solve(bStdSolver* ctx){
	if(!bStdXMLFormulaElement::solve(ctx)){
		return(false);
	}
long	n=ctx->count();
	if(n<2){
		return(false);
	}
	_a=ctx->get(n-1);
	_b=ctx->get(n);
	if((!_a)||(!_b)){
		return(false);
	}
	if(!ctx->rmv(n)){	
		return(false);
	}
	if(!ctx->rmv(n-1)){	
		return(false);
	}
	return(true);
}
