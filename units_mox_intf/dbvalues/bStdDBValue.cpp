//----------------------------------------------------------------------------
// File : bStdDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for values (used in solvers & formulas)
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

#include "bStdDBValue.h"
#include <MacMapSuite/bTrace.h>

//int bStdDBValue::count=0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdDBValue::bStdDBValue(){
//_bTrace_("bStdDBValue::bStdDBValue",false);
//    count++;
//_tm_((void*)this+" "+count);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdDBValue::~bStdDBValue(){	
//_bTrace_("bStdDBValue::~bStdDBValue",false);
//    count--;
//_tm_((void*)this+" "+count);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdDBValue::replicate(){
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(char* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(int* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(double* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(bool* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(time_t* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::get(void* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(char* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(int value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(double value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(bool value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(time_t value){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdDBValue::put(void* value){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator == (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator != (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator < (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator > (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator <= (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator >= (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator && (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdDBValue::operator || (bStdDBValue& gdbv){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdDBValue::operator + (bStdDBValue& gdbv){
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdDBValue::operator - (bStdDBValue& gdbv){
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdDBValue::operator * (bStdDBValue& gdbv){
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bStdDBValue::operator / (bStdDBValue& gdbv){
	return(NULL);
}
