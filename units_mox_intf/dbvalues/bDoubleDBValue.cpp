//----------------------------------------------------------------------------
// File : bDoubleDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Double values class
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
// 04/12/2003 creation.
//----------------------------------------------------------------------------

#include "bDoubleDBValue.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>


// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDoubleDBValue::bDoubleDBValue():bStdDBValue(){
	val=NAN;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDoubleDBValue::~bDoubleDBValue(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDoubleDBValue::replicate(){
bStdDBValue*	dbv=new bDoubleDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(char* value){
	sprintf(value,"%f",val);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(int* value){
	(*value)=round(val);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(double* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(bool* value){
	(*value)=(val==0)?0:1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(time_t* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::get(void* value){
	*(double*)value=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(char* value){
	val=matof(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(int value){
	val=value;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(double value){
	val=value;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(bool value){
	val=(value)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(time_t value){
	val=value;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDoubleDBValue::put(void* value){
	val=*(double*)value;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator == (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return(val==value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator != (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return(val!=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator < (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return(val<value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator > (bStdDBValue& gdbv){
//_bTrace_("bDoubleDBValue::operator",false);
double	value;
	gdbv.get(&value);

//_tm_(val+">"+value+"="+(val>value));

	return(val>value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator <= (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return(val<=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator >= (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return(val>=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator && (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return((val!=0)&&(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDoubleDBValue::operator || (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	return((val!=0)||(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDoubleDBValue::operator + (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	value=val+value;
bDoubleDBValue* dbval=new bDoubleDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDoubleDBValue::operator - (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	value=val-value;
bDoubleDBValue* dbval=new bDoubleDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDoubleDBValue::operator * (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	value=val*value;
bDoubleDBValue* dbval=new bDoubleDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDoubleDBValue::operator / (bStdDBValue& gdbv){
double	value;
	gdbv.get(&value);
	value=val/value;
bDoubleDBValue* dbval=new bDoubleDBValue();
	dbval->put(value);
	return(dbval);
}

