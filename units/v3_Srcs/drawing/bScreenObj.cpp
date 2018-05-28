//----------------------------------------------------------------------------
// File : bScreenObj.cpp
// Project : MacMap
// Purpose : C++ source file : Screen object class (i.e. object appearance for a style)
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
// 06/02/2004 creation.
//----------------------------------------------------------------------------

#include "bMacMapApp.h"
#include "bScreenObj.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bScreenObj::bScreenObj(){
	_obj=NULL;
	_val=0;
	_clss=0;
	_xmin=0;
	_ymin=0;
	_xmax=0;
	_ymax=0;
	_hins=0;	
	_vins=0;
	_flg=false;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bScreenObj::~bScreenObj(){

}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setreference(bGenericGeoElement* o){
	_obj=o;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setvalue(double d){
	_val=d;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setclass(int k){
	_clss=k;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setbounds(	float xmin,
							float ymin,
							float xmax,
							float ymax){
	_xmin=xmin;
	_ymin=ymin;
	_xmax=xmax;
	_ymax=ymax;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setbounds(	float* x,
							float* y,
							int n,
							float hi,
							float vi,
							bool reset){
float	bh,bv;

	if(reset){
		_xmin=_xmax=x[0];
		_ymin=_ymax=y[0];
		_hins=hi;
		_vins=vi;
		bh=0;
		bv=0;
		if(n==1){
			_xmin-=_hins;
			_xmax+=_hins;
			_ymin-=_vins;
			_ymax+=_vins;
		}
	}
	else{
		_xmin=_xmax=x[0];
		_ymin=_ymax=y[0];
		_hins=hi;
		_vins=vi;
		bh=0;
		bv=0;
		if(n==1){
			_xmin-=_hins;
			_xmax+=_hins;
			_ymin-=_vins;
			_ymax+=_vins;
		}		
	}
	
	if(_hins<hi){
		_hins=hi;
	}
	if(_vins<vi){
		_vins=vi;
	}

	if(n==1){
		return;
	}

	for(int i=1;i<n;i++){
		if(_xmin>x[i]){
			_xmin=x[i];
		}
		else if(_xmax<x[i]){
			_xmax=x[i];
		}
		if(_ymin>y[i]){
			_ymin=y[i];
		}
		else if(_ymax<y[i]){
			_ymax=y[i];
		}
	}

	if(_hins>bh){
		_xmin-=(_hins-bh);
		_xmax+=(_hins-bh);
	}
	if(_vins>bv){
		_ymin-=(_vins-bv);
		_ymax+=(_vins-bv);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bScreenObj::getreference(){
	return(_obj);
}

// ---------------------------------------------------------------------------
// 
// ------------
double bScreenObj::getvalue(){
	return(_val);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bScreenObj::getclass(){
	return(_clss);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::getbounds(	float* xmin,
							float* ymin,
							float* xmax,
							float* ymax){
	(*xmin)=_xmin;
	(*ymin)=_ymin;
	(*xmax)=_xmax;
	(*ymax)=_ymax;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bScreenObj::visible(){
	return((_xmin!=0)||(_ymin!=0)||(_xmax!=0)||(_ymax!=0));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bScreenObj::setflag(bool flg){
	_flg=flg;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bScreenObj::flag(){
	return(_flg);
}

