//----------------------------------------------------------------------------
// File : bXMapReIndex.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, rebuilt geographic index
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
// 20/01/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapReIndex.h"
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapReIndex::bXMapReIndex(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("reindex");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapReIndex::~bXMapReIndex(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapReIndex::create(bGenericXMLBaseElement* elt){
	return(new bXMapReIndex(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapReIndex::open(int* flags){
    bStdXMap::open(flags);
    process(kExtProcessCallFromIntf,NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapReIndex::process(int msg, void* prm){
_bTrace_("bXMapReIndex::process",true);
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
bGenericType*	tp;
ivx_rect		vxr;
	for(int i=1;i<=_gapp->typesMgr()->count();i++){
_tm_("Type "+i+"...");
		tp=_gapp->typesMgr()->get(i);
		tp->iterator()->bounds(&vxr);		
_tm_("bounds were ("+vxr.left+";"+vxr.right+";"+vxr.top+";"+vxr.bottom+")");
		vxr.top=vxr.left=vxr.bottom=vxr.right=0;
		tp->iterator()->iterate(&vxr,get_rect);		
		if(	(vxr.top==0)	&&
			(vxr.bottom==0)	&&
			(vxr.left==0)	&&
			(vxr.right==0)	){
		}
		else{
			tp->iterator()->set_bounds(&vxr);
		}
		tp->fields()->h_write(1,kHDR_bounds_,&vxr);
_tm_("bounds set to ("+vxr.left+";"+vxr.right+";"+vxr.top+";"+vxr.bottom+")");
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapReIndex::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapReIndex::get_rect(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
ivx_rect*			vxr=(ivx_rect*)prm;
	if(	(vxr->top==0)		&&
		(vxr->bottom==0)	&&
		(vxr->left==0)		&&
		(vxr->right==0)		){
		geo->getBounds(vxr);
	}
	else{
ivx_rect			bnd;
		geo->getBounds(&bnd);
		ivr_union(vxr,&bnd,vxr);
	}
	return(0);
}

