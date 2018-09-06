//----------------------------------------------------------------------------
// File : CWXMap.cpp
// Project : MacMap
// Purpose : C++ source file : C wrappers for XMap developpement
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
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWXMap.h"
#include "bSDKXMap.h"

// ---------------------------------------------------------------------------
// 
// ------------
CWXMap		CWXMap_Alloc		(	CWCallback* Callback,
									CFBundleRef bndl,
									CWXMap_OpenPtr open,
									CWXMap_ClosePtr close,
									CWXMap_EventPtr event,
									CWXMap_IdlePtr idle,
									CWXMap_ProcessPtr process,
									CWXMap_TestPtr test,
									CWXMap_EditPtr edit,
									CWCharData name){
bSDKXMap*	xmap=new bSDKXMap(	NULL,
								(bGenericMacMapApp*)Callback,
								bndl,
								open,
								close,
								event,
								idle,
								process,
								test,
								edit,
								name);
	return((CWXMap)xmap);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWXMap_Free(CWXMap xmap){
	delete ((bSDKXMap*)xmap);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWCallback CWXMap_GetCallBack(CWXMap xmap){
	return(((bSDKXMap*)xmap)->getapp());
}

// ---------------------------------------------------------------------------
// 
// ------------
CFBundleRef CWXMap_GetBundle(CWXMap xmap){
	return(((bSDKXMap*)xmap)->getbundle());
}
