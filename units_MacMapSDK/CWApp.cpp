//----------------------------------------------------------------------------
// File : CWApp.cpp
// Project : MacMap
// Purpose : C++ source file : bGenericMacMapApp C wrappers
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
// 19/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWApp.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/valconv.h>

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_WriteStatus(	CWCallback Callback,
						CWCharData data){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	WriteMessage(gapp,data);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWApp_GetSelection(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWArray)gapp->selMgr()->elements());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_AddToSelection(CWCallback Callback, CWArray objects){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->selMgr()->add((bArray*)objects);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_ResetSelection(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->selMgr()->flush();
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_GetSelectionCount(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->selMgr()->getState());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWApp_GetContrastes(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWArray)gapp->cntMgr()->elements());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_AddToContrasts(CWCallback Callback, CWArray objects){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->cntMgr()->add((bArray*)objects);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_ResetContrasts(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->cntMgr()->flush();
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_GetContrastsCount(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->cntMgr()->getState());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_CountScales(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWUnit CWApp_GetScale(CWCallback Callback, int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->get(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_SetScale(	CWCallback Callback, 
					int index,
					CWCharData name,
					double coef){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->scaleMgr()->set(index,name,coef);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_AddScale(	CWCallback Callback, 
					CWCharData name,
					double coef){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->add(name,coef));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_RemoveScale(	CWCallback Callback, 
						int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->rmv(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_GetCurrentScale(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->get_current());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_SetCurrentScale(	CWCallback Callback, 
							int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->scaleMgr()->set_current(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_CountDistUnits(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->distMgr()->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWUnit CWApp_GetDistUnit(CWCallback Callback, int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->distMgr()->get(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_GetCurrentDistUnit(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->distMgr()->get_current());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWApp_SetCurrentDistUnit(	CWCallback Callback, 
								int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return(gapp->distMgr()->set_current(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_InitEventLog(CWCallback Callback, 
						CWCharData message,
						int big_endian_creator){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->eventMgr()->init_log(big_endian_creator,message);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_CloseEventLog(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->eventMgr()->close_log();
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_ResetEventLog(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->eventMgr()->reset_log();
}

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWApp_GetEvents(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWArray*)gapp->eventMgr()->events());
}

// ---------------------------------------------------------------------------
// 
// ------------
//void CWApp_DrawInPort(CWCallback Callback, CGrafPtr port, CWiRect bounds){
//bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
//ivx_rect			back;
//
//	gapp->mapwd()->screen_bounds(&back);
//	gapp->locConverter()->reset((ivx_rect*)&bounds);
//	gapp->layersMgr()->DrawLayers(port,(ivx_rect*)&bounds);
//	gapp->locConverter()->reset(&back);
//}

// ---------------------------------------------------------------------------
// 
// ------------
void CWApp_SwitchContext(CWCallback Callback, int signature, CWPath path){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->layersMgr()->SwitchContext(signature,path);
}
