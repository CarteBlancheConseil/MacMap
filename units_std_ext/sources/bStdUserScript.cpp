//----------------------------------------------------------------------------
// File : bStdUserScript.cpp
// Project : MacMap
// Purpose : C++ source file : User script class
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
// 05/12/2005 creation.
//----------------------------------------------------------------------------

#include "bStdUserScript.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserScript	::bStdUserScript(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				: bStdUserExt(elt,gapp,bndl){
	setclassname("uscript");
	_root=NULL;
	_subclss=kXMLSubClassExtScript;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserScript::~bStdUserScript(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdUserScript::create(bGenericXMLBaseElement* elt){
	return(new bStdUserScript(elt,_gapp,getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserScript::open(int* flags){
	if(countelements()>0){
		bStdUserExt::open(flags);
		_root=(bStdXMLScriptElement*)(void*)_gapp->classMgr()->NthElement(this,1,"script");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserScript::process(int msg, void* prm){
_bTrace_("bStdUserScript::process",false);
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			break;
		case kExtProcessCallGetData:
			bStdUserExt::process(msg,prm);
			p->content=_root;
			return(true);
			break;
		default:
			return(bStdUserExt::process(msg,prm));
			break;
	}
	if(!_root){
_te_("_root == NULL");
		return(true);
	}
	_root->exec();	
	return(true);
}

