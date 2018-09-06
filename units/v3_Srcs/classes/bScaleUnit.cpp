//----------------------------------------------------------------------------
// File : bScaleUnit.cpp
// Project : MacMap
// Purpose : C++ source file : Scales class
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
// 25/11/2004 creation.
//----------------------------------------------------------------------------

#include "bScaleUnit.h"
#include "bMacMapApp.h"

// ---------------------------------------------------------------------------
// 
// ------------
double bScaleUnit::_trans=72.0/254.0;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bScaleUnit	::bScaleUnit()
			:bStdUnit(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bScaleUnit::~bScaleUnit(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScaleUnit::set(const char* name, double coef){	
	if(_id){
		free(_id);
	}
	if(_shrt_name){
		free(_shrt_name);
	}
	if(_long_name){
		free(_long_name);
	}
	_shrt_name=strdup("");
	_long_name=(name)?strdup(name):strdup("");
	_id=strdup(_long_name);
bGenericUnit*	u=map_doc->dist_resolution_unit();
double			res=map_doc->dist_resolution();
	_coef=coef;
	_conv=(10000.0*res*u->coef()*_trans)/coef;
}
