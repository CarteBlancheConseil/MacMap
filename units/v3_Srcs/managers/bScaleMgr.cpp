//----------------------------------------------------------------------------
// File : bScaleMgr.cpp
// Project : MacMap
// Purpose : C++ source file : Scale units manager class
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
// 08/01/2004 creation.
//----------------------------------------------------------------------------

#include "bMacMapApp.h"
#include "bScaleMgr.h"
#include "bScaleUnit.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>

// ---------------------------------------------------------------------------
// 
// ------------
#define _fname	"scales"
#define _fpath	"scales.xml"

// ---------------------------------------------------------------------------
// 
// ------------
int scale_comp(	const void* a,
				const void* b){
bScaleUnit*	sa=*((bScaleUnit**)a);
bScaleUnit*	sb=*((bScaleUnit**)b);
	if(sa->coef()-sb->coef()>0){
		return(1);
	}
	if(sb->coef()-sa->coef()>0){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bScaleMgr	::bScaleMgr()
			:bStdUnitMgr(){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bScaleMgr::~bScaleMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
bStdUnit* bScaleMgr::allocator(){
	return(new bScaleUnit());
}

// ---------------------------------------------------------------------------
// 
// -----------
int bScaleMgr::load(){
_bTrace_("bScaleMgr::load",_DEVELOPMENT_VERS_);
	return(pth_load(_fpath));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bScaleMgr::unload(){
_bTrace_("bScaleMgr::unload",_DEVELOPMENT_VERS_);
	return(pth_unload(_fpath,true));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bScaleMgr::set_current(int idx){
	if(bStdUnitMgr::set_current(idx)){
		bMacMapApp::instance()->mapIntf()->reset();
		WriteScale(_MMAPP_);
		return(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bScaleMgr::set(int idx, const char* name, double coef){
	bStdUnitMgr::set(idx,name,coef);
	if(idx==get_current()){	
		bMacMapApp::instance()->mapIntf()->reset();
		WriteScale(_MMAPP_);
	}
	sort(scale_comp);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bScaleMgr::add(const char* name, double coef){
	if(!bStdUnitMgr::add(name,coef)){
		return(false);
	}
	sort(scale_comp);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bScaleMgr::rmv(int idx){	
	if(count()==1){
		return(false);
	}	
	if(bStdUnitMgr::rmv(idx)){
		if((idx==count()+1)&&(idx==get_current())){
			set_current(idx-1);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bScaleMgr::make(){
	return(bStdUnitMgr::make(_fpath,_fname));
}
