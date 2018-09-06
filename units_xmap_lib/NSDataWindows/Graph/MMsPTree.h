//----------------------------------------------------------------------------
// File : MMsPTree.h
// Project : MacMap
// Purpose : Header file : 2D tree for scatter plot graph search
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
// 17/04/2008 creation.
//----------------------------------------------------------------------------

#ifndef __MMsPTree__
#define __MMsPTree__

//----------------------------------------------------------------------------

#include <Carbon/Carbon.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/vx.h>
#include "bXMapStatScatterPlotParam.h"

/*
//----------------------------------------------------------------------------

// Sructure des noeuds
typedef struct MMsPTNode MMsPTNode;
struct MMsPTNode{
	short				state;	// Etat
	MMsPTNode*			prev;	// Racine du noeud
	MMsPTNode*			left;	// Branche gauche
	MMsPTNode*			right;	// Branche droite
	StatObject*			so;		// Contenu
	d2dvertex			vx;		// Coordonnées
};

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

// Fonctions pour arbres 2D
// Création d'un noeud
void	MMsPT_Init			(	MMsPTNode** root, 
								d2dvertex* vx, 
								StatObject*	so);
// Positionnement du noeud "Root" dans l'arbre
MMsPTNode* 	MMsPT_Insert	(	MMsPTNode*	root, 
								d2dvertex* vx, 
								StatObject*	so);	
// Recherche par plage	
void	MMsPT_SearchSlot	(	MMsPTNode*	root, 
								dvx_rect* vr, 
								bArray& arr);
// Destruction
void	MMsPT_Free			(	MMsPTNode*	root);

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
*/
//----------------------------------------------------------------------------

#endif 
