//----------------------------------------------------------------------------
// File : CWXMLTree.cpp
// Project : MacMap
// Purpose : C++ source file : C wrappers for MacMap XML trees creation, modification, access, etc...
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
// 21/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWXMLTree.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/xmldesc_utils.h>
#include <std_ext/bStdXMLBaseElement.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWArray CWXMLTree_AllocArrayForXMLData(){
	return(CWArray_Alloc(sizeof(xmlelt),0));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_AddDescriptorToArray(CWArray arr, 
									int indent, 
									CWCharData name,
									CWCharData value){
bArray*	barr=(bArray*)arr;
	add_cdesc((*barr),indent,name,value);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_FreeDescriptors(	CWArray arr){
bArray*	barr=(bArray*)arr;
	free_descs((*barr));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWXMLTree CWXMLTree_MakeFromDescriptors(CWCallback Callback,
										CWArray arr){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bArray*				barr=(bArray*)arr;
	return((CWXMLTree)gapp->classMgr()->ParseXMLDescriptors(barr));	
}
													
// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_Free(CWCallback Callback,
					CWXMLTree tree){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	gapp->classMgr()->ReleaseXMLInstance((bStdXMLBaseElement*)tree);
}
													
// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_GetNodeName(	CWXMLTree tree,
							CWCharData data){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	elt->getclassname(data);
}
													
// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_GetNodeValue(CWXMLTree tree,
							CWCharData data){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	elt->getvalue(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWXMLTree_SetNodeValue(CWXMLTree tree,
							CWCharData data){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	elt->setvalue(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWXMLTree_GetChildCount(CWXMLTree tree){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	return(elt->countelements());
}

// ---------------------------------------------------------------------------
// 
// ------------
CWXMLTree CWXMLTree_GetIndChild(CWXMLTree tree, int index){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	return((CWXMLTree)elt->getelement(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWXMLTree_AddChild(CWXMLTree tree, CWXMLTree child){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	return(elt->addelement((bStdXMLBaseElement*)child));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWXMLTree_RemoveChild(CWXMLTree tree, int index){
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	elt->rmvelement(index);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWXMLTree CWXMLTree_GetIndNodeWithName(	CWCallback Callback,
										CWXMLTree tree,
										int index,
										CWCharData name){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bStdXMLBaseElement*	elt=(bStdXMLBaseElement*)tree;
	return((CWXMLTree)gapp->classMgr()->NthElement(elt,index,name));
}

