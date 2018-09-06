//----------------------------------------------------------------------------
// File : CWMapWindow.cpp
// Project : MacMap
// Purpose : C++ source file : Map window (bGenericMapIntf) C wrappers
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

#include "CWMapWindow.h"
#include <MacMapSuite/vx.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <std_ext/bStdXMLBaseElement.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWMapWindow CWMapWindow_Get(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWMapWindow)gapp->mapIntf());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWMapWindow_GetBounds(CWMapWindow mapwd, CWiRect* data){
bGenericMapIntf*	wind=(bGenericMapIntf*)mapwd;
	wind->screenBounds((ivx_rect*)data);
}

// ---------------------------------------------------------------------------
// 
// ------------
//WindowRef CWMapWindow_GetWindowRef(CWMapWindow mapwd){
//bGenericMapIntf*	wind=(bGenericMapIntf*)mapwd;
//	return((WindowRef)wind->ref());
//}

// ---------------------------------------------------------------------------
// 
// ------------
void CWMapWindow_Inval(CWMapWindow mapwd){
bGenericMapIntf*	wind=(bGenericMapIntf*)mapwd;
	return(wind->inval());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWMapWindow_SetScreenCenter(CWMapWindow mapwd, CWiCoord data){
bGenericMapIntf*	wind=(bGenericMapIntf*)mapwd;
	wind->setScreenCenter(*(i2dvertex*)(&data));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiCoord CWMapWindow_Screen2Geom(CWCallback Callback, CGPoint cgp){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
CWiCoord			coord;
	gapp->locConverter()->convert((i2dvertex*)(&coord),&cgp);
	return(coord);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPoint CWMapWindow_Geom2Screen(CWCallback Callback, CWiCoord coord){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
CGPoint				cgp;
	gapp->locConverter()->convert(&cgp,(i2dvertex*)(&coord));
	return(cgp);
}


