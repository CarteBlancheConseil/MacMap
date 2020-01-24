//----------------------------------------------------------------------------
// File : bCharDBValue.cpp
// Project : MacMap
// Purpose : C++ source file : Char values class
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

#include "bCharDBValue.h"
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCharDBValue::bCharDBValue():bStdDBValue(){
	val=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bCharDBValue::~bCharDBValue(){
	if(val){
		free(val);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bCharDBValue::replicate(){
bStdDBValue*	dbv=new bCharDBValue();
	dbv->put(val);
	return(dbv);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(char* value){
	if(val){
		strcpy(value,val);
	}
	else{
		value[0]=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(int* value){
	if(val){
		(*value)=atoi(val);
	}
	else{
		(*value)=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(double* value){
	if(val){
		(*value)=atof(val);
	}
	else{
		(*value)=NAN;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(bool* value){
	if(val){
		(*value)=atoi(val);
		if((*value)){
			(*value)=1;
		}
	}
	else{
		(*value)=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(void* value){
	get((char*)value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::get(time_t* value){
	if(val){
//#warning conversion à trouver
		(*value)=0;
	}
	else{
		(*value)=0;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(char* value){
	if(val){
		free(val);
	}
	val=strdup(value);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(int value){
char	buff[256];
	if(val){
		free(val);
	}
	sprintf(buff,"%d",value);
	val=strdup(buff);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(double value){
char	buff[256];
	if(val){
		free(val);
	}
	sprintf(buff,"%f",value);
	val=strdup(buff);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(bool value){
	if(val){
		free(val);
	}
	if(value){
		val=strdup("1");
	}
	else{
		val=strdup("0");
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(time_t value){
char	buff[256];
	if(val){
		free(val);
	}
	strcpy(buff,asctime(localtime(&value)));
	buff[strlen(buff)-1]=0;	
	val=strdup(buff);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCharDBValue::put(void* value){
	put((char*)value);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator == (bStdDBValue& gdbv){
/*char	value[1024];
	gdbv.get(value);
	return(!strcmp((char*)val,value));*/
char	avalue[1024],bvalue[1024];
	get(avalue);
	gdbv.get(bvalue);
	strupper(avalue);
	strupper(bvalue);
	return(!strcmp(avalue,bvalue));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator != (bStdDBValue& gdbv){
/*char	value[1024];
	gdbv.get(value);
	return(strcmp((char*)val,value));*/
char	avalue[1024],bvalue[1024];
	get(avalue);
	gdbv.get(bvalue);
	strupper(avalue);
	strupper(bvalue);
	return(strcmp(avalue,bvalue));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator < (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return(strcmp((char*)val,value)<0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator > (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return(strcmp((char*)val,value)>0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator <= (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return(strcmp((char*)val,value)<=0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator >= (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return(strcmp((char*)val,value)>=0);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator && (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return((strlen(val)>0)&&(strlen(value)>0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCharDBValue::operator || (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
	return((strlen(val)>0)||(strlen(value)>0));
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bCharDBValue::operator + (bStdDBValue& gdbv){
char	value[1024];
	gdbv.get(value);
char*	buf=new char[strlen(val)+strlen(value)+1];
	sprintf(buf,"%s%s",val,value);
bCharDBValue* dbval=new bCharDBValue();
	dbval->put(buf);
	delete[] buf;
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bCharDBValue::operator - (bStdDBValue& gdbv){
char			value[256];
bCharDBValue*	dbval=new bCharDBValue();
	gdbv.get(value);
	
size_t  k=strlen(value);
	if(k){
char*	buf=strdup(val);
char*	tmp=buf;
		while((tmp=strstr(tmp,value))){
			memmove(tmp,&tmp[k],(strlen(tmp)-k)+1);
		}
		dbval->put(buf);
		free(buf);
	}
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bCharDBValue::operator * (bStdDBValue& gdbv){
bCharDBValue* dbval=new bCharDBValue();
	dbval->put(val);
	return(dbval);
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdDBValue* bCharDBValue::operator / (bStdDBValue& gdbv){
bCharDBValue* dbval=new bCharDBValue();
	dbval->put(val);
	return(dbval);
}

