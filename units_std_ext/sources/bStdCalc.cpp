//----------------------------------------------------------------------------
// File : bStdCalc.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for Calcs
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
// 29/11/2005 creation.
//----------------------------------------------------------------------------

#include "bStdCalc.h"
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <std_ext/bStdXMLCalc.h>
#include <std_ext/bStdUserExt.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdCalc::bStdCalc(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
		: bStdExt(elt,gapp,bndl){
	_sign=0;
	_root=NULL;
	_subclss=kXMLSubClassExtCalc;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdCalc::~bStdCalc(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCalc::open(int* flags){
	(*flags)=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCalc::close(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdCalc::event(EventRef evt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdCalc::nsevent(void* nsevt){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCalc::idle(void* prm){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdCalc::process(int msg, void* prm){
//_bTrace_("bStdCalc::process",true);
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallFromIntf:
			return(exec());	
			break;
		case kExtProcessCallGetData:
			p->sign=_sign;
			p->name=/*_cfname*/NULL/*_name*/;			
			p->content=_root;
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdCalc::test(void* prm){
//_bTrace_("bStdCalc::test",true);
/*bArray*	arr=_gapp->selMgr()->elements();
	if(arr->count()!=1){
		return(false);
	}*/
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdCalc::edit(void* prm){
//_bTrace_("bStdCalc::edit",true);
	return(process(kExtProcessCallFromIntf,NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericXMLFormulaElement* bStdCalc::root(){
	return((bGenericXMLFormulaElement*)_root);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdCalc::kind(){
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdCalc::solve(bGenericGeoElement* o){
//_bTrace_("bStdCalc::solve",true);
bStdDBValue*	dbval=NULL;
bStdSolver		solver(_gapp);

//_tm_("before "+bDoubleDBValue::count);

	for(;;){
		if(!_root){
//_te_("_root==NULL");
			break;
		}
		if(o){
bGenericType*	tp=_gapp->typesMgr()->get(o->getType());
			solver.set_type(tp);
		}
		solver.set_object(o);
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
	solver.set_object(NULL);
	solver.reset();

//_tm_("after "+bDoubleDBValue::count);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCalc::get_params(char* name, char* value){
//_bTrace_("bStdCalc::get_params",true);
	if(_root){
		_root->getclassname(name);
	}
	else{
//_tm_("no root");
		strcpy(name,"");
	}
	strcpy(value,"");
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdCalc::write(bStdDBValue* val){
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
bool bStdCalc::exec(){
//_bTrace_("bStdCalc::exec",true);
bGenericGeoElement*	o;
	if(!_gapp->selMgr()->elements()->get(1,&o)){
		return(false);
	}
bStdDBValue*	val=solve(o);
	if(val){
		write(val);
		delete val;
	}
	return(true);
}
