//----------------------------------------------------------------------------
// File : bStdXMLScriptElement.cpp
// Project : MacMap
// Purpose : C++ source file : XML script base class
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
// 09/03/2004 creation.
//----------------------------------------------------------------------------

#include "bStdXMLScriptElement.h"

#include <mox_intf/XMLSignatures.h>

#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdXMLScriptElement	::bStdXMLScriptElement(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						: bStdXMLBaseElement(elt,gapp,bndl){
	_subclss=kXMLSubClassExtScript;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdXMLScriptElement::~bStdXMLScriptElement(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdXMLScriptElement::exec(){
_bTrace_("bStdXMLScriptElement::exec",true);
bGenericExt*	elt;
char			nm[_names_length_max_];
	for(int i=1;i<=_elts.count();i++){
		if(_elts.get(i,&elt)){
			elt->getclassname(nm);
_tm_(nm);
			if(!elt->process(kExtProcessCallWithXMLTree,NULL)){
_te_(nm+" : process failed");
				return(false);
			}
		}
	}
	return(true);
}
