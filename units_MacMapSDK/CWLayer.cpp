//----------------------------------------------------------------------------
// File : CWLayer.cpp
// Project : MacMap
// Purpose : C++ source file : Layers C wrappers & utilities
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
// 22/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWLayer.h"
#include <MacMapSuite/vx.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <std_ext/bStdXMLBaseElement.h>

// ---------------------------------------------------------------------------
// 
// ------------
int CWLayer_GetLayerCount(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->layersAccessCtx()->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWLayer CWLayer_Get(CWCallback Callback, int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWLayer)gapp->layersAccessCtx()->get(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWLayer CWLayer_GetCurrent(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWLayer)gapp->layersAccessCtx()->get(gapp->layersAccessCtx()->get_current()));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWLayer_SetCurrent(CWCallback Callback, int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->layersAccessCtx()->set_current(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWLayer_SetCurLayerVisibility(CWCallback Callback, int data){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->layersAccessCtx()->setvisible(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWLayer_SetCurLayerSelectability(CWCallback Callback, int data){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->layersAccessCtx()->setselectable(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWLayer_GetName(CWLayer layer, CWCharData data){
bGenericStyle*	styl=(bGenericStyle*)layer;
	strcpy(data,styl->getname());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWType CWLayer_GetType(CWLayer layer){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return((CWType)styl->gettype());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWXMLTree CWLayer_GetRoot(CWLayer layer){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return((CWXMLTree)styl->root());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWLayer_GetVisibility(CWLayer layer){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return(styl->visible());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWLayer_GetSelectability(CWLayer layer){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return(styl->selectable());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWLayer_GetCWObjectsAtPoint(CWLayer layer, CGPoint cgp, int all){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return((CWArray)styl->objsatpoint(&cgp,all));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWLayer_GetCWObjectsInRect(CWLayer layer, CGRect cgr, int strict){
bGenericStyle*	styl=(bGenericStyle*)layer;
	return((CWArray)styl->objsinrect(&cgr,strict));
}

