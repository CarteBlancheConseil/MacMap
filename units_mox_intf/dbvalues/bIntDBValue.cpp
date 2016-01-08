//----------------------------------------------------------------------------
// File : bIntDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Int values class
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

#include "bIntDBValue.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bIntDBValue::bIntDBValue():bStdDBValue(){
	val=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bIntDBValue::~bIntDBValue(){	
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bIntDBValue::replicate(){
bStdDBValue*	dbv=new bIntDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(char* value){
	sprintf(value,"%d",val);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(int* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(double* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(bool* value){
	(*value)=(val==0)?0:1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(time_t* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::get(void* value){
	*(int*)value=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(char* value){
	val=atoi(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(int value){
	val=value;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(double value){
	val=round(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(bool value){
	val=(value)?1:0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(time_t value){
	val=value;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bIntDBValue::put(void* value){
	val=*(int*)value;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator == (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val==value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator != (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val!=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator < (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val<value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator > (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val>value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator <= (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val<=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator >= (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return(val>=value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator && (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return((val!=0)&&(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bIntDBValue::operator || (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	return((val!=0)||(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bIntDBValue::operator + (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	value=val+value;
	bIntDBValue* dbval=new bIntDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bIntDBValue::operator - (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	value=val-value;
	bIntDBValue* dbval=new bIntDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bIntDBValue::operator * (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	value=val*value;
	bIntDBValue* dbval=new bIntDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bIntDBValue::operator / (bStdDBValue& gdbv){
	int	value;
	gdbv.get(&value);
	value=val/value;
	bIntDBValue* dbval=new bIntDBValue();
	dbval->put(value);
	return(dbval);
}

