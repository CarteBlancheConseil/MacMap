//----------------------------------------------------------------------------
// File : bStdSolver.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for solvers (used for formulas execution)
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
// 08/03/2004 creation.
//----------------------------------------------------------------------------

#include <mox_intf/bStdSolver.h>
#include <mox_intf/bBoolDBValue.h>
#include <mox_intf/bCharDBValue.h>
#include <mox_intf/bDateDBValue.h>
#include <mox_intf/bDoubleDBValue.h>
#include <mox_intf/bIntDBValue.h>
#include <mox_intf/bTimeDBValue.h>
#include <mox_intf/bGenericMacMapApp.h>

#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------

#define _sapp ((bGenericMacMapApp*)_gapp)

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdSolver	::bStdSolver(void* gapp)
			:_elts(sizeof(bStdDBValue*)){
//_bTrace_("bStdSolver::bStdSolver",false);
//_tm_("%x",this);
	_tp=NULL;
	_obj=NULL;
	_gapp=gapp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdSolver::~bStdSolver(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bStdSolver::count(){
	return(_elts.count());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdSolver::add(bStdDBValue* elt){
	return(_elts.add(&elt));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdSolver::rmv(int idx){
	return(_elts.rmv(idx));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdSolver::get(int idx){
bStdDBValue* elt;
	if(!_elts.get(idx,&elt)){
		return(NULL);
	}
	return(elt);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdSolver::reset(){
// Il ne faut pas supprimer les dbvalues elles appartiennent aux calcs
/*bStdDBValue*	val;
	for(int i=1;i<=_elts.count();i++){
		_elts.get(i,&val);
		delete val;
	}*/
	_elts.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdSolver::set_type(bGenericType* tp){
	_tp=tp;
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericType* bStdSolver::get_type(){
	return(_tp);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdSolver::set_object(bGenericGeoElement* o){
	_obj=o;
}

// ---------------------------------------------------------------------------
// 
// -----------
bGenericGeoElement* bStdSolver::get_object(){
	return(_obj);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdSolver::result(bStdDBValue** elt){
	(*elt)=NULL;
	if(_elts.count()!=1){
		return(false);
	}
	return(_elts.get(1,elt));
}

