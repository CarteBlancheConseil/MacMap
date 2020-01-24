//----------------------------------------------------------------------------
// File : bBrowserSortValue.cpp
// Project : MacMap
// Purpose : C++ source file : Value classes for sort in browser context
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
// 5/10/2006 creation.
//----------------------------------------------------------------------------

#include "bBrowserSortValue.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBrowserSortValue::bBrowserSortValue(int ord){
	_ord=ord;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBrowserSortValue::~bBrowserSortValue(){
}

// ---------------------------------------------------------------------------
// 
// -----------
int bBrowserSortValue::compare(bBrowserSortValue* b){
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserSortValue::dump(){
	fprintf(stderr,"!!!!");
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBrowserDoubleSortValue	::bBrowserDoubleSortValue(int ord, double val)
						:bBrowserSortValue(ord){
	_val=val;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBrowserDoubleSortValue::~bBrowserDoubleSortValue(){
	
}

// ---------------------------------------------------------------------------
// 
// -----------
int bBrowserDoubleSortValue::compare(bBrowserSortValue* b){
double	d=_val-((bBrowserDoubleSortValue*)b)->_val;
//	fprintf(stderr,"%f->%f\n",_val,((bBrowserDoubleSortValue*)b)->_val);
	if(d>0){
		return(_ord);
	}
	if(d<0){
		return(-_ord);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserDoubleSortValue::dump(){
	fprintf(stderr,"%f",_val);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBrowserCharSortValue	::bBrowserCharSortValue(int ord, char* val)
						:bBrowserSortValue(ord){
	_val=strdup(val);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBrowserCharSortValue::~bBrowserCharSortValue(){
	if(_val){
		delete _val;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bBrowserCharSortValue::compare(bBrowserSortValue* b){
	if((_val)&&(!((bBrowserCharSortValue*)b)->_val)){
		return(_ord);
	}
	if((!_val)&&(((bBrowserCharSortValue*)b)->_val)){
		return(-_ord);
	}
	return(_ord*strcmp(_val,((bBrowserCharSortValue*)b)->_val));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserCharSortValue::dump(){
	fprintf(stderr,"%s",_val);
}

// ---------------------------------------------------------------------------
// StatVar
// ------------
int			bBrowserStackValue::_i=0;
int			bBrowserStackValue::_n=0;
int			bBrowserStackValue::_r=0;
int			bBrowserStackValue::_prg=0;
bStdWait*	bBrowserStackValue::_wt=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBrowserStackValue::bBrowserStackValue(int dim){
	_values=new bBrowserSortValue*[dim];
	_n=dim;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBrowserStackValue::~bBrowserStackValue(){
	for(_i=0;_i<_n;_i++){
		delete _values[_i];
	}
	delete _values;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserStackValue::put_ref(void* ref){	
	_ref=ref;
}

// ---------------------------------------------------------------------------
// 
// -----------
void* bBrowserStackValue::get_ref(){	
	return(_ref);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserStackValue::put(	int idx,
								bBrowserSortValue* val){	
	if((idx<0)||(idx>=_n)){
		return;
	}
	_values[idx]=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bBrowserStackValue::compare(const void* a,
								const void* b){	
bBrowserStackValue*	va=*(bBrowserStackValue**)a;
bBrowserStackValue*	vb=*(bBrowserStackValue**)b;

	_prg++;
	(void)_wt->set_progress(_prg);
	for(_i=0;_i<_n;_i++){
		_r=va->_values[_i]->compare(vb->_values[_i]);
		if(_r){
			return(_r);
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserStackValue::put_wt(bStdWait* wt){	
	_wt=wt;
	_prg=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBrowserStackValue::dump(){
	fprintf(stderr,"%d->",(long)_ref);
	for(_i=0;_i<_n;_i++){
		_values[_i]->dump();
		fprintf(stderr,";");
	}
	fprintf(stderr,"\n");
}

