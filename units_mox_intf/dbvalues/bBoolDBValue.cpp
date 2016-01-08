//----------------------------------------------------------------------------
// File : bBoolDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Boolean values class
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

#include "bBoolDBValue.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBoolDBValue::bBoolDBValue():bStdDBValue(){
	val=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bBoolDBValue::~bBoolDBValue(){	
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bBoolDBValue::replicate(){
bStdDBValue*	dbv=new bBoolDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(char* value){
	if(val){
		sprintf(value,"1");
	}
	else{
		sprintf(value,"0");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(int* value){
	(*value)=(val)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(double* value){
	(*value)=(val)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(bool* value){
	(*value)=(val)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(time_t* value){
	(*value)=(val)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::get(void* value){
	(*(int*)value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(char* value){
	val=atof(value);
	if(val){
		val=1;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(int value){
	val=(value)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(double value){
	val=(value!=0)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(bool value){
	val=(value)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(time_t value){
	val=(value!=0)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bBoolDBValue::put(void* value){
	val=(*(int*)value!=0)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator == (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val==value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator != (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val==value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator < (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val<value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator > (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val>value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator <= (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val<=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator >= (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val>=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator && (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val&&value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bBoolDBValue::operator || (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	return(val||value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bBoolDBValue::operator + (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	value=val+value;
	bBoolDBValue* dbval=new bBoolDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bBoolDBValue::operator - (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	value=val-value;
	bBoolDBValue* dbval=new bBoolDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bBoolDBValue::operator * (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	value=val*value;
	bBoolDBValue* dbval=new bBoolDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bBoolDBValue::operator / (bStdDBValue& gdbv){
	bool	value;
	gdbv.get(&value);
	value=val/value;
	bBoolDBValue* dbval=new bBoolDBValue();
	dbval->put(value);
	return(dbval);
}
