//----------------------------------------------------------------------------
// File : bStdUserMacro.cpp
// Project : MacMap
// Purpose : C++ source file : User macro class
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

#include "bStdUserMacro.h"
#include "bSelectSolver.h"
#include "endian.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserMacro	::bStdUserMacro(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdUserExt(elt,gapp,bndl),
				_roots(sizeof(bStdXMLFormulaElement*)){
	setclassname("umacro");
	_root=NULL;
	_subclss=kXMLSubClassExtMacro;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserMacro::~bStdUserMacro(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdUserMacro::create(bGenericXMLBaseElement* elt){
	return(new bStdUserMacro(elt,_gapp,getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserMacro::open(int* flags){
//_bTrace_("bStdUserMacro::open",true);
	_roots.reset();
	if(countelements()>0){
		bStdUserExt::open(flags);
//_tm_(_name);

int	i,n=_gapp->classMgr()->CountElements(this,"formula");
		for(i=1;i<=n;i++){
			_root=(bStdXMLFormulaElement*)(void*)_gapp->classMgr()->NthElement(this,i,"formula");
			_roots.add(&_root);
		}
		_roots.get(1,&_root);
//		_root=(bStdXMLFormulaElement*)(void*)_gapp->classMgr()->NthElement(this,1,"formula");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserMacro::process(int msg, void* prm){
//_bTrace_("bStdUserMacro::process",true);
//_tm_(_name);
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
//_tm_("kExtProcessCallFromIntf");
			break;
		case kExtProcessCallGetData:
//_tm_("kExtProcessCallGetData");
			bStdUserExt::process(msg,prm);
			p->content=_root;
			return(true);
			break;
		default:
//_tm_("default");
			return(bStdUserExt::process(msg,prm));
			break;
	}

//	if(!_root){
//		return(true);
//	}

	if(_roots.count()<=0){
//_te_("_roots.count()<=0");
		return(true);
	}

bStdSolver*		prv=(bStdSolver*)prm;
bSelectSolver	solver(_gapp,false);
	if(prv){
		solver.set_object(prv->get_object());
	}
	for(int i=1;i<=_roots.count();i++){
		_roots.get(i,&_root);
		solver.exec(_root);
UInt32	sgn=solver.get_next();
		if(sgn!=_kNULLSign_){
#ifdef __LITTLE_ENDIAN__
			LBSwapWord(&sgn,sizeof(UInt32));
#endif
bStdUserMacro*	nxt=(bStdUserMacro*)(void*)_gapp->macroMgr()->find(sgn);
			if(nxt){
				nxt->process(kExtProcessCallFromIntf,NULL);
			}
		}
	}
//	solver.exec(_root);
		
	return(true);
}
