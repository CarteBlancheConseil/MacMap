//----------------------------------------------------------------------------
// File : bTimeDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Time values class
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

#include "bTimeDBValue.h"
#include <MacMapSuite/db_utils.h>
#include <MacMapSuite/base_def.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bTimeDBValue::bTimeDBValue():bStdDBValue(){
	val=0;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bTimeDBValue::~bTimeDBValue(){	
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bTimeDBValue::replicate(){
bStdDBValue*	dbv=new bTimeDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(char* value){
	if(strlen(cval)==0){
struct tm*	ts=localtime(&val);
	(void)strftime(value,_kDateTimeLen,"%H:%M:%S",ts);	
	}
	else{
		strncpy(value,cval,_kDateTimeLen);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(int* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(double* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(bool* value){
	(*value)=(val==0)?0:1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(time_t* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::get(void* value){
	*(double*)value=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(char* value){
	strncpy(cval,value,_kDateTimeLen);
	val=make_time(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(int value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(double value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(bool value){
	val=(value)?1:0;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(time_t value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bTimeDBValue::put(void* value){
	val=*(double*)value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator == (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1==t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator != (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1!=t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator < (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1<t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator > (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1>t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator <= (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1<=t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator >= (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return(t1>=t2);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator && (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return((t1!=0)&&(t2!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bTimeDBValue::operator || (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
int	t1=(ts1.tm_hour*3600)+(ts1.tm_min*60)+ts1.tm_sec;
int	t2=(ts2.tm_hour*3600)+(ts2.tm_min*60)+ts2.tm_sec;
	return((t1!=0)||(t2!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bTimeDBValue::operator + (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val+value;
bTimeDBValue* dbval=new bTimeDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bTimeDBValue::operator - (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val-value;
bTimeDBValue* dbval=new bTimeDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bTimeDBValue::operator * (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val*value;
bTimeDBValue* dbval=new bTimeDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bTimeDBValue::operator / (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val/value;
bTimeDBValue* dbval=new bTimeDBValue();
	dbval->put(value);
	return(dbval);
}

