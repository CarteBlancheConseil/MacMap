//----------------------------------------------------------------------------
// File : bStdUserCalc.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for user Calcs
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

#include "bStdUserCalc.h"
#include "bStdUserExt.h"

#include <mox_intf/endian.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <mox_intf/bStdSolver.h>

#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserCalc	::bStdUserCalc(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				: bStdCalc(elt,gapp,bndl){
	setclassname("ucalc");
	_name=NULL;
	_kind=_kNULLSign_;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserCalc::~bStdUserCalc(){
	if(_name){
		free(_name);
	}
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bStdUserCalc::create(bGenericXMLBaseElement* elt){
	return(new bStdUserCalc(elt,_gapp,getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserCalc::open(int* flags){
	*flags=0;
	read();
	if(countelements()>0){
		_root=(bStdXMLFormulaElement*)(void*)_gapp->classMgr()->NthElement(this,1,"formula");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserCalc::process(int msg, void* prm){
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			return(bStdCalc::process(msg,prm));
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
int bStdUserCalc::kind(){
	return(_kind);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserCalc::get_params(char* name, char* value){
//_bTrace_("bStdUserCalc::get_params",false);
	sprintf(name,kUserCalcSolverName);
	sprintf(value,"%.4s",(char*)&_sign);
//_tm_((UInt32*)&_sign);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserCalc::write(bStdDBValue* val){
//_bTrace_("bStdUserCalc::write",true);
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
void bStdUserCalc::read(){
//_bTrace_("bStdUserCalc::read",false);
char				buff[_values_length_max_];
bStdXMLBaseElement* elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,1,"sign");
	if(elt){
		elt->getvalue(buff);
		_sign=*((UInt32*)buff);
//_tm_("_sign="+(UInt32*)&_sign);
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
//_tm_("_name="+_name);
	}
	else{
//_te_("no name");
	}
}

