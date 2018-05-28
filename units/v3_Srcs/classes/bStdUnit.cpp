//----------------------------------------------------------------------------
// File : bStdUnit.cpp
// Project : MacMap
// Purpose : C++ source file : Standard class for units
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 24/11/2004 creation.
//----------------------------------------------------------------------------

#include "bStdUnit.h"
#include <mox_intf/mm_messages.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUnit::bStdUnit(){
	_id=NULL;
	_shrt_name=NULL;
	_long_name=NULL;
	_coef=1;
	_conv=1;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUnit::~bStdUnit(){
	if(_id){
		free(_id);
	}
	if(_shrt_name){
		free(_shrt_name);
	}
	if(_long_name){
		free(_long_name);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnit::name_id(char* name){
	strcpy(name,_id);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnit::long_name(char* name){
	strcpy(name,_long_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUnit::short_name(char* name){
	strcpy(name,_shrt_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bStdUnit::coef(){
	return(_coef);
}

// ---------------------------------------------------------------------------
// 
// -----------
double bStdUnit::conv(){
	return(_conv);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bStdUnit::set(const char* name, double coef){
	if(_id){
		free(_id);
	}
	if(_shrt_name){
		free(_shrt_name);
	}
	if(_long_name){
		free(_long_name);
	}

	_coef=coef;

	if(!name){
		_id=strdup("");
		_shrt_name=strdup("");
		_long_name=strdup("");
		return;
	}

	_id=strdup(name);
	
char msg[__MESSAGE_STRING_LENGTH_MAX__];
char nm[__MESSAGE_STRING_LENGTH_MAX__];
	
	sprintf(nm,"%sAB",name);
	
	message_string(name,msg,1);
	_long_name=strdup(msg);
	
	message_string(nm,msg,0);
	_shrt_name=strdup(msg);
}
