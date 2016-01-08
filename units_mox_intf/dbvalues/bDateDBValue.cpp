//----------------------------------------------------------------------------
// File : bDateDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Date values class
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

#include "bDateDBValue.h"
#include <MacMapSuite/db_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bDateDBValue::bDateDBValue():bStdDBValue(){
	val=0;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bDateDBValue::~bDateDBValue(){	
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDateDBValue::replicate(){
bStdDBValue*	dbv=new bDateDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(char* value){
	if(strlen(cval)==0){
struct tm*	ts=localtime(&val);
		(void)strftime(value,_kDateTimeLen,"%Y-%m-%d",ts);
	}
	else{
		strncpy(value,cval,_kDateTimeLen);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(int* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(double* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(bool* value){
	(*value)=(val==0)?0:1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(time_t* value){
	(*value)=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::get(void* value){
	*(double*)value=val;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(char* value){
	strncpy(cval,value,_kDateTimeLen);
	val=make_date(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(int value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(double value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(bool value){
	val=(value)?1:0;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(time_t value){
	val=value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bDateDBValue::put(void* value){
	val=*(double*)value;
	cval[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator == (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	return(	(ts1.tm_year==ts2.tm_year)	&&
			(ts1.tm_mon==ts2.tm_mon)	&&
			(ts1.tm_mday==ts2.tm_mday)	);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator != (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	return(	(ts1.tm_year!=ts2.tm_year)	||
			(ts1.tm_mon!=ts2.tm_mon)	||
			(ts1.tm_mday!=ts2.tm_mday)	);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator < (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	if(ts1.tm_year<ts2.tm_year){
		return(true);
	}
	if(ts1.tm_mon<ts2.tm_mon){
		return(true);
	}
	return(ts1.tm_mday<ts2.tm_mday);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator > (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	if(ts1.tm_year>ts2.tm_year){
		return(true);
	}
	if(ts1.tm_mon>ts2.tm_mon){
		return(true);
	}
	return(ts1.tm_mday>ts2.tm_mday);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator <= (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	if(ts2.tm_year>ts1.tm_year){
		return(false);
	}
	if(ts2.tm_mon>ts1.tm_mon){
		return(false);
	}
	return(ts1.tm_mday<=ts2.tm_mday);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator >= (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
struct tm	ts1=*gmtime(&val);
struct tm	ts2=*gmtime(&value);
	if(ts2.tm_year>ts1.tm_year){
		return(false);
	}
	if(ts2.tm_mon>ts1.tm_mon){
		return(false);
	}
	return(ts1.tm_mday>=ts2.tm_mday);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator && (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	return((val!=0)&&(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bDateDBValue::operator || (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	return((val!=0)||(value!=0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDateDBValue::operator + (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val+value;
bDateDBValue* dbval=new bDateDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDateDBValue::operator - (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val-value;
bDateDBValue* dbval=new bDateDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDateDBValue::operator * (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val*value;
bDateDBValue* dbval=new bDateDBValue();
	dbval->put(value);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bDateDBValue::operator / (bStdDBValue& gdbv){
time_t	value;
	gdbv.get(&value);
	value=val/value;
bDateDBValue* dbval=new bDateDBValue();
	dbval->put(value);
	return(dbval);
}

