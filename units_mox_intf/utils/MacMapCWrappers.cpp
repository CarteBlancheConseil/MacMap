//----------------------------------------------------------------------------
// File : MacMapCWrappers.cpp
// Project : MacMap
// Purpose : C++ source file : C wrappers around MacMap
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
// 03/09/2014 creation.
//----------------------------------------------------------------------------

#include "MacMapCWrappers.h"
#include "bGenericMacMapApp.h"

#pragma mark -> StatusWindow
// ---------------------------------------------------------------------------
// 
// ------------
void WriteMessage(void* xapp, const char* msg){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	if(gapp->statusIntf()){
		gapp->statusIntf()->writeMessage(msg);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void WriteScale(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	if(gapp->statusIntf()){
char			msg[512];
bGenericUnit*	scl=gapp->scaleMgr()->get();
		if(scl){
			scl->long_name(msg);
			if(strlen(msg)==0){
				sprintf(msg,"1/%.0f",scl->coef());
			}
		}
		else{
			msg[0]=0;
		}		
		gapp->statusIntf()->writeScale(msg);
	}
}

#pragma mark -> Tools
// ---------------------------------------------------------------------------
// 
// ------------
void* CurTool(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	return gapp->toolMgr()->get();
}

#pragma mark -> Selection & Contrasts
// ---------------------------------------------------------------------------
// 
// ------------
void FlushSelection(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	gapp->selMgr()->flush();
}

// ---------------------------------------------------------------------------
// 
// ------------
void FlushContrasts(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	gapp->cntMgr()->flush();
}

#pragma mark -> Layer
// ---------------------------------------------------------------------------
// 
// ------------
long CountLayers(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	return gapp->layersAccessCtx()->count();
}

// ---------------------------------------------------------------------------
// 
// ------------
void* GetIndLayer (void* xapp,
				   long i){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
	return gapp->layersAccessCtx()->get(i);
}

// ---------------------------------------------------------------------------
// 
// -----------
void* CurLayer(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
long	x=gapp->layersAccessCtx()->get_current();
	if(!x){
		return(NULL);
	}
	return(gapp->layersAccessCtx()->get(x));
}

#pragma mark -> Types
// ---------------------------------------------------------------------------
// 
// -----------
void* CurType(void* xapp){
bGenericMacMapApp* gapp=(bGenericMacMapApp*)xapp;
bGenericStyle*	style=(bGenericStyle*)CurLayer(gapp);
	if(!style){
		return(NULL);
	}
	return(style->gettype());
}

