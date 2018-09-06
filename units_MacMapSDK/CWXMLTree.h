//----------------------------------------------------------------------------
// File : CWXMLTree.h
// Project : MacMap
// Purpose : Header file : C wrappers for MacMap XML trees creation, modification, access, etc...
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
// CWXMLTree manipulation. 
// CWXMLTrees must be used to store params (see in CWDocument.h and CWType.h)
// Every standard MacMap® params are defined as XML trees, including : 
// - macros, 
// - user calcs, 
// - user vars, 
// - scripts,
// - styles,
// - views,
// - ...
//----------------------------------------------------------------------------
// 21/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWXMLTree__
#define __CWXMLTree__

//----------------------------------------------------------------------------

#include "CWDefs.h"
#include "CWArray.h"

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// Tree Creation
//----------------------------------------------------------------------------

// Creating an CWArray sized for XML descriptors
// Returns a CWArray
CWArray		CWXMLTree_AllocArrayForXMLData		(	);					

// Adding a descriptor to a CWArray of XML descriptors
void		CWXMLTree_AddDescriptorToArray		(	CWArray arr,			// -> Reference array
													int indent,				// -> Indentation
													CWCharData name,		// -> Node name (Must be in MacMap®'s XML dictionnary)
													CWCharData value);		// -> Node value 

// Releasing descriptors contained in a CWArray 
void		CWXMLTree_FreeDescriptors			(	CWArray arr);			// -> Reference array

// Creating an XML tree from a CWArray of descriptors
// Returns a CWXMLTree if success, NULL if failed
CWXMLTree	CWXMLTree_MakeFromDescriptors		(	CWCallback Callback,	// -> Callback
													CWArray arr);			// -> Reference array

//----------------------------------------------------------------------------
// Tree releasing
//----------------------------------------------------------------------------

// Deleting a CWXMLTree
void		CWXMLTree_Free						(	CWCallback Callback,	// -> Callback
													CWXMLTree tree);		// -> Target CWXMLTree

//----------------------------------------------------------------------------
// Nodes Manipulation
//----------------------------------------------------------------------------

// Getting node's name
void		CWXMLTree_GetNodeName				(	CWXMLTree tree,			// -> Reference CWXMLTree
													CWCharData data);		// -> Name
													
// Getting node's value
void		CWXMLTree_GetNodeValue				(	CWXMLTree tree,			// -> Reference CWXMLTree
													CWCharData data);		// -> Value
													
// Setting node's value
void		CWXMLTree_SetNodeValue				(	CWXMLTree tree,			// -> Reference CWXMLTree
													CWCharData data);		// <- Value
													
// Getting node's child count
int			CWXMLTree_GetChildCount				(	CWXMLTree tree);		// -> Reference CWXMLTree

// Getting a node's child
// Returns a CWXMLTree if succeed, NULL if failed
CWXMLTree	CWXMLTree_GetIndChild				(	CWXMLTree tree,			// -> Reference CWXMLTree
													int index);				// -> Child index in [1..n]
													
// Adding a child to node
// Returns a 1 if succeed, 0 if failed
int			CWXMLTree_AddChild					(	CWXMLTree tree,			// -> Reference CWXMLTree
													CWXMLTree child);		// -> Child
													
// Removing a child from node
// Returns a 1 if succeed, 0 if failed
int			CWXMLTree_RemoveChild				(	CWXMLTree tree,			// -> Reference CWXMLTree
													int index);				// -> Child index in [1..n]
													
// Getting a node in a full tree with an index and a name
// Returns a CWXMLTree if succeed, NULL if failed
CWXMLTree	CWXMLTree_GetIndNodeWithName		(	CWCallback Callback,	// -> Callback
													CWXMLTree tree,			// -> Reference CWXMLTree
													int index,				// -> Node index in [1..n]
													CWCharData name);		// -> Node's name

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
