//----------------------------------------------------------------------------
// File : bXMapDataFillRun.cpp
// Project : MacMap
// Purpose : C++ source file : Utility classes, for datafill XMap
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
// 17/03/2006 creation.
// 10/06/2016 Bug fix in bXMapDataFillRunCalc::apply : val was not deallocated.
//----------------------------------------------------------------------------

#include "bXMapDataFillRun.h"
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/bTrace.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapDataFillRun
// ---------------------------------------------------------------------------
// Constructeur
// ------------
int bXMapDataFillRun::_lid=0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFillRun::bXMapDataFillRun(char* text, int fld, int sz, int kind, int decs){
	_lid++;
	_id=_lid;
	_dst=fld;
	_value=malloc(sz);
	_kind=kind;
	_decs=decs;
	strncpy(_text,text,512);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFillRun::~bXMapDataFillRun(){
	if(_value){
		free(_value);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFillRun::apply(bGenericGeoElement* o){
	if(!_value){
		return(false);
	}
	return(o->setValue(_dst,_value));
}


// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapDataFillRunValue
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFillRunValue	::bXMapDataFillRunValue(	char* text, 
													int fld, int sz, int kind, int decs, 
													char* value)	
						:bXMapDataFillRun(text,fld,sz,kind,decs){
	_good=charToX(_kind,decs,value,_value);
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFillRunValue::~bXMapDataFillRunValue(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFillRunValue::apply(bGenericGeoElement* o){
	if(!_good){
		return(false);
	}
	return(bXMapDataFillRun::apply(o));
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapDataFillRunCalc
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFillRunCalc	::bXMapDataFillRunCalc(	char* text, 
												int fld, int sz, int kind, int decs, 
												bGenericCalc* clc)	
						:bXMapDataFillRun(text,fld,sz,kind,decs){
	_src=clc;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFillRunCalc::~bXMapDataFillRunCalc(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFillRunCalc::apply(bGenericGeoElement* o){
//_bTrace_("bXMapDataFillRunCalc::apply",false);
bStdDBValue*	val=_src->solve(o);
bool            ret=false;
    
    if(!val){
//_te_("val==NULL");
		return(false);
	}
	switch(_src->kind()){
		case _int:
		case _bool:{
//_tm_("_int/_bool");
int			x;
			val->get(&x);
			ret=o->setValue(_dst,x);
			}break;
		case _double:/*
		case _date:
		case _time:*/{
//_tm_("_double/_date/_time");
double		x;
			val->get(&x);
            ret=o->setValue(_dst,x);
			}break;
		case _date:
		case _time:
		case _char:{
//_tm_("_char");
char		x[_values_length_max_];
			val->get(x);
            ret=o->setValue(_dst,x);
			}break;
		default:
//_te_("bad kind");
			break;
	}
	delete val;
	return(ret);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapDataFillRunField
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFillRunField	::bXMapDataFillRunField(	char* text, 
													int fld, int sz, int kind, int decs, 
													int src, int srcsz, int srckind, int srcdecs)	
						:bXMapDataFillRun(text,fld,sz,kind,decs){
	_src=src;
	_srcvalue=malloc(srcsz);
	_srckind=srckind;
	_srcdecs=srcdecs;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFillRunField::~bXMapDataFillRunField(){
	if(_srcvalue){
		free(_srcvalue);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFillRunField::apply(bGenericGeoElement* o){
//_bTrace_("bXMapDataFillRunField::apply",false);
	if(!_srcvalue){
		return(false);
	}
	if(!o->getValue(_src,_srcvalue)){
		return(false);
	}
//_tm_("decimals:"+_decs);
	switch(_srckind){
		case _int:
		case _bool:
//_tm_("int");
			if(!intToX(_kind,0,*((int*)_srcvalue),_value)){
				return(false);
			}
			break;
		case _double:
//_tm_("double");
			if(!doubleToX(_kind,_srcdecs,*((double*)_srcvalue),_value)){
				return(false);
			}
			break;
		case _date:
//_tm_("date");
			if(!dateToX(_kind,0,*((double*)_srcvalue),_value)){
				return(false);
			}
			break;
		case _time:
//_tm_("time");
			if(!timeToX(_kind,0,*((double*)_srcvalue),_value)){
				return(false);
			}
			break;
		case _char:
//_tm_("char");
			if(!charToX(_kind,_decs,(char*)_srcvalue,_value)){
				return(false);
			}
			break;
		default:
			return(false);
			break;
	}
	return(bXMapDataFillRun::apply(o));
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark bXMapDataFillRunAskValue
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataFillRunAskValue	::bXMapDataFillRunAskValue(	char* text, 
														int fld, int sz, int kind, int decs,
														bGenericType* tp)	
							:bXMapDataFillRun(text,fld,sz,kind,decs){
char		value[1024];
	_good=GetAValue(tp,fld,text,value);
	if(_good){
		_good=charToX(_kind,decs,value,_value);
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataFillRunAskValue::~bXMapDataFillRunAskValue(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataFillRunAskValue::apply(bGenericGeoElement* o){
	if(!_good){
		return(false);
	}
	return(bXMapDataFillRun::apply(o));
}
