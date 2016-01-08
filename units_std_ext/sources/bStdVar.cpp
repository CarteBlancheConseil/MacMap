//----------------------------------------------------------------------------
// File : bStdVar.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Variables/Constants
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
// 04/05/2007 creation.
//----------------------------------------------------------------------------

#include "bStdVar.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <std_ext/bStdXMLCalc.h>
#include <std_ext/bStdUserExt.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdVar::bStdVar(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		: bStdExt(elt,gapp,bndl){
	_sign=0;
	_root=NULL;
	_subclss=kXMLSubClassExtVar;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdVar::~bStdVar(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdVar::open(int* flags){
	(*flags)=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdVar::close(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::event(EventRef evt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::nsevent(void* nsevt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdVar::idle(void* prm){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::process(int msg, void* prm){
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			return(exec());	
			break;
		case kExtProcessCallGetData:
			p->sign=_sign;
			p->name=NULL/*_name*/;			
			p->content=_root;
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::test(void* prm){
//_bTrace_("bStdVar::test",true);
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::edit(void* prm){
	return(process(kExtProcessCallFromIntf,NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLFormulaElement* bStdVar::root(){
	return((bGenericXMLFormulaElement*)_root);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdVar::kind(){
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdVar::solve(bGenericGeoElement* o){
//bTrace			trc("bStdVar::solve",true);
bStdDBValue*	dbval=NULL;
bStdSolver		solver(_gapp);

	for(;;){
		if(!_root){
//_te_("_root==NULL");
			break;
		}
		solver.set_object(NULL);
		_root->init(&solver);	
		if(!_root->solve(&solver)){
//_te_("_root->solve==false");
			break;	
		}
		if(!solver.result(&dbval)){
//_te_("solver.result==false");
			break;
		}
		if(dbval){
			dbval=dbval->replicate();
		}
		_root->reset(&solver);	
		break;
	}
	solver.reset();

	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdVar::get_params(char* name, char* value){
_bTrace_("bStdVar::get_params",true);
	if(_root){
		_root->getclassname(name);
	}
	else{
_tm_("no root");
		strcpy(name,"");
	}
	strcpy(value,"");
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdVar::write(bStdDBValue* val){
	if(!val){
		WriteMessage(_gapp,"????");
		return;
	}
double			d;
bGenericUnit*	up=_gapp->distMgr()->get();
char			u[256],bf[256];
	val->get(&d);
	up->short_name(u);
	sprintf(bf,"%.*f %s",_gapp->document()->dist_pref_digits(),d,u);
	WriteMessage(_gapp,bf);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdVar::exec(){
bStdDBValue*	val=solve(NULL);
	write(val);
	delete val;
	return(true);
}
