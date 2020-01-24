//----------------------------------------------------------------------------
// File : bStdUserVar.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for user Variables/Constants
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#include "bStdUserVar.h"
#include "bStdUserExt.h"

#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/bStdSolver.h>

#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserVar	::bStdUserVar(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdVar(elt,gapp,bndl){
	setclassname("uvar");
	_name=NULL;
	_kind=_kNULLSign_;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserVar::~bStdUserVar(){
	if(_name){
		free(_name);
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdUserVar::create(bGenericXMLBaseElement* elt){
	return(new bStdUserVar(elt,_gapp,getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserVar::open(int* flags){
	*flags=0;
	read();
	if(countelements()>0){
		_root=(bStdXMLFormulaElement*)(void*)_gapp->classMgr()->NthElement(this,1,"formula");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserVar::process(int msg, void* prm){
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			return(bStdVar::process(msg,prm));
			break;
		case kExtProcessCallSetData:
		
			break;
		case kExtProcessCallGetData:
			p->sign=_sign;
			p->name=_name;
			p->content=_root;
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdUserVar::kind(){
	return(_kind);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserVar::get_params(char* name, char* value){
	sprintf(name,kUserVarSolverName);
	sprintf(value,"%.4s",(char*)&_sign);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserVar::write(bStdDBValue* val){
	if(!val){
		WriteMessage(_gapp,"????");
		return;
	}
char			cv[256];
	val->get(cv);
	WriteMessage(_gapp,cv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserVar::read(){
_bTrace_("bStdUserVar::read",false);
char				buff[_values_length_max_];
bStdXMLBaseElement* elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,1,"sign");
	if(elt){
		elt->getvalue(buff);
		_sign=*((UInt32*)buff);
//_tm_("_sign=%.4s",&_sign);
	}
	else{
//_te_("no sign");
	}
	elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,2,"sign");
	if(elt){
		elt->getvalue(buff);
		_kind=*((UInt32*)buff);
#ifdef __LITTLE_ENDIAN__
		LBSwapWord(&_kind,sizeof(UInt32));
#endif
//_tm_("_kind="+(UInt32*)&_kind);
	}
	else{
//_te_("no kind");
	}
	elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,1,"name");
	if(elt){
		elt->getvalue(buff);
		_name=strdup(buff);
//_tm_("_name=%.4s",_name);
	}
	else{
//_te_("no name");
	}
}
