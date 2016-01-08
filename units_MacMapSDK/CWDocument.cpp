//----------------------------------------------------------------------------
// File : CWDocument.cpp
// Project : MacMap
// Purpose : C++ source file : bGenericDocument C wrappers
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

#include "CWDocument.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bTrace.h>
#include <std_ext/bStdXMLBaseElement.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWDocument CWDocument_Get(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	return((CWDocument)gapp->document());
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWDocument_GetName(CWDocument document,
						CWFileName data){
bGenericDocument*	doc=(bGenericDocument*)document;
	doc->name(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWDocument_GetPath(CWDocument document,
						CWPath data){
bGenericDocument*	doc=(bGenericDocument*)document;
	doc->location(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_GetSRID(CWDocument document){
bGenericDocument*	doc=(bGenericDocument*)document;
	return(doc->srid());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_SetSRID(	CWDocument document,
						int data){
bGenericDocument*	doc=(bGenericDocument*)document;
	return(doc->set_srid(data));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWdCoord CWDocument_GetOrigin(CWDocument document){
_bTrace_("MacMapSDK::CWDocument_GetOrigin",false);
_tw_("proc not available");
bGenericDocument*	doc=(bGenericDocument*)document;
CWdCoord			crd={0,0};
	return(crd);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_SetOrigin(	CWDocument document,
							CWdCoord data){
_bTrace_("MacMapSDK::CWDocument_SetOrigin",false);
_tw_("proc not available");
bGenericDocument*	doc=(bGenericDocument*)document;
//	return(doc->set_origin((d2dvertex)data));
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_GetDistDigits(CWDocument document){
bGenericDocument*	doc=(bGenericDocument*)document;
	return(doc->dist_pref_digits());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_SetDistDigits(	CWDocument document,
								int data){
bGenericDocument*	doc=(bGenericDocument*)document;
	doc->set_dist_pref_digits(data);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
double CWDocument_GetDistResolution(CWDocument document){
bGenericDocument*	doc=(bGenericDocument*)document;
	return(doc->dist_resolution());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWUnit CWDocument_GetDistResolutionUnit(CWDocument document){
bGenericDocument*	doc=(bGenericDocument*)document;
	return((CWUnit)doc->dist_resolution_unit());
}
									
// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_GetParam(CWDocument document,
						int signature,
						CWFileName name,
						CWXMLTree* data){
bGenericDocument*	doc=(bGenericDocument*)document;
	return(doc->readTree((bGenericXMLBaseElement**)data,signature,name));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWDocument_SetParam(CWDocument document,
						int signature,
						CWFileName name,
						CWXMLTree data){
bGenericDocument*	doc=(bGenericDocument*)document;
	if(data==NULL){
		return(doc->removeTree(signature,name));
	}
	return(doc->writeTree((bStdXMLBaseElement*)data,signature,name));
}

