//----------------------------------------------------------------------------
// File : MMsPTree.cpp
// Project : MacMap
// Purpose : C++ source file : 2D tree for scatter plot graph search
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

#include "MMsPTree.h"
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// 
// ------------
/*void MMsPT_Init(MMsPTNode** root,
				d2dvertex* vx,
				StatObject*	so){
	*root=(MMsPTNode*)malloc(sizeof(MMsPTNode));
	(*root)->prev=NULL;
	(*root)->left=NULL;
	(*root)->right=NULL;
	(*root)->state=0;
	(*root)->so=so;
	(*root)->vx=*vx;
}

// ---------------------------------------------------------------------------
// 
// ------------
MMsPTNode* MMsPT_Insert(MMsPTNode* root, 
						d2dvertex* vx,
						StatObject*	so){
char		pop=0;
char		inf;
MMsPTNode*	nx=root;
MMsPTNode*	pr;

	do{
		pr=nx;
		inf=(pop)?(vx->x<nx->vx.x):(vx->y<nx->vx.y);
		nx=(inf)?nx->left:nx->right;
		pop=!pop;
	}
	while(nx!=NULL);
	MMsPT_Init(&nx,vx,so);
	if(inf){
		pr->left=nx;
	}
	else{
		pr->right=nx;
	}
	nx->prev=pr;
	return(nx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void MMsPT_SearchSlot(	MMsPTNode* root, 
						dvx_rect* vr, 
						bArray& arr){
char		sx1,sx2,sy1,sy2,c1,c2,pop=0;
int			state;
MMsPTNode*	nx=root;
	
	state=nx->state;
	do{
		sx1=(nx->vx.x>=vr->xmin);
		sx2=(nx->vx.x<=vr->xmax);
		sy1=(nx->vx.y>=vr->ymin);
		sy2=(nx->vx.y<=vr->ymax);
		c1=(pop)?sx1:sy1;
		c2=(pop)?sx2:sy2;
		if(nx->state==state){
			if(c1&&(nx->left!=NULL)&&(nx->left->state==state)){
				nx=nx->left;
			}
			else{
				nx->state++;
				if(nx->left!=NULL){
					nx->left->state=state;
				}
				if(sx1&&sx2&&sy1&&sy2){
					arr.add(&nx);
				}
				nx=(c2&&(nx->right!=NULL))?nx->right:nx->prev;
			}
		}			
		else{
			if(nx->right!=NULL){
				nx->right->state=state;
			}
			nx=nx->prev;
		}
		pop=!pop;
	}
	while(nx!=NULL);
	root->state=state;
}

// ---------------------------------------------------------------------------
// 
// ------------
void MMsPT_Free(MMsPTNode* root){
	if(root->left){
		MMsPT_Free(root->left);
	}
	if(root->right){
		MMsPT_Free(root->right);
	}
	free(root);
}*/
