//----------------------------------------------------------------------------
// File : CWGeometry.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap geometry utilities
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
// 15/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWGeometry.h"
#include <MacMapSuite/vx_lib.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWiGeometry_Alloc(	int ncoord,
								int npart){
	return((CWiGeometry)ivs_new(_2D_VX,ncoord,npart));
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWiGeometry_Free(CWiGeometry geom){
	ivs_free((ivertices*)geom);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_CoordCount(CWiGeometry geom){
ivertices*	vxs=(ivertices*)geom;
	return(vxs->nv);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_PartCount(CWiGeometry geom){
ivertices*	vxs=(ivertices*)geom;
	return(ivs_n_parts(vxs));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_CoordCountForPart(	CWiGeometry geom,
									int part){
ivertices*	vxs=(ivertices*)geom;
int			nvx;
/*i2dvertex*	p=*/(void)ivs2_part(vxs,part-1,&nvx);
	return(nvx);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_GetCoord(	CWiGeometry geom,
							int index,
							CWiCoord* coord){
ivertices*	vxs=(ivertices*)geom;
	if((index<1)||(index>vxs->nv)){
		return(0);
	}
	coord->h=vxs->vx.vx2[index-1].h;
	coord->v=vxs->vx.vx2[index-1].v;
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_SetCoord(	CWiGeometry geom,
							int cindex,
							CWiCoord coord){
ivertices*	vxs=(ivertices*)geom;
	if((cindex<1)||(cindex>vxs->nv)){
		return(0);
	}
	vxs->vx.vx2[cindex-1].h=coord.h;
	vxs->vx.vx2[cindex-1].v=coord.v;
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_GetCoordForPart(CWiGeometry geom,
								int pindex,
								int cindex,
								CWiCoord* coord){
ivertices*	vxs=(ivertices*)geom;
int			nvx;
i2dvertex*	p=ivs2_part(vxs,pindex-1,&nvx);
	if(p==NULL){
		return(0);		
	}
	if((cindex<1)||(cindex>nvx)){
		return(0);
	}
	coord->h=p[cindex-1].h;
	coord->v=p[cindex-1].v;	
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWiGeometry_SetCoordForPart(CWiGeometry geom,
								int pindex,
								int cindex,
								CWiCoord coord){
ivertices*	vxs=(ivertices*)geom;
int			nvx;
i2dvertex*	p=ivs2_part(vxs,pindex-1,&nvx);
	if(p==NULL){
		return(0);		
	}
	if((cindex<1)||(cindex>nvx)){
		return(0);
	}
	p[cindex-1].h=coord.h;
	p[cindex-1].v=coord.v;
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWiGeometry_AddCoord(	CWiGeometry geom,
									CWiCoord coord){
ivertices*	vxs=(ivertices*)geom;
i2dvertex	vx={coord.h,coord.v};
	return((CWiGeometry)ivx2_add(vxs,&vx));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWiGeometry_RemoveCoord(CWiGeometry geom,
									int index){
ivertices*	vxs=(ivertices*)geom;
	if((index<1)||(index>vxs->nv)){
		return(0);
	}
	return((CWiGeometry)ivx_remove(vxs,index-1));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWiGeometry_Cat(CWiGeometry geoma,
							CWiGeometry geomb){
	return(ivs_concat((ivertices*)geoma,(ivertices*)geomb));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiGeometry CWiGeometry_Group(	CWiGeometry geoma,
								CWiGeometry geomb){
	return(ivs_group((ivertices*)geoma,(ivertices*)geomb));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWiRect CWiGeometry_Bounds(CWiGeometry geom){
ivx_rect	vxr;
	ivs_bounds((ivertices*)geom,&vxr);
CWiRect		cwr={vxr.top,vxr.left,vxr.bottom,vxr.right};
	return(cwr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWiCoord2CWdCoord(	CWdCoord* dcoord,
						CWiCoord* icoord,
						CWdCoord* origin,
						double reso){
	vx_i2d((d2dvertex*)dcoord,(i2dvertex*)icoord,reso);
	dcoord->x+=origin->x;
	dcoord->y+=origin->y;
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWdCoord2CWiCoord(	CWiCoord* icoord,
						CWdCoord* dcoord,
						CWdCoord* origin,
						double reso){
CWdCoord buf=*dcoord;
	buf.x-=origin->x;
	buf.y-=origin->y;
	vx_d2i((i2dvertex*)icoord,(d2dvertex*)&buf,reso);
}

/*
// ---------------------------------------------------------------------------
// 
// ------------
CWdGeometry CWdGeometry_Alloc(	int ncoord,
								int npart){
	return((CWdGeometry)dvs_new(_2D_VX,ncoord,npart));
}
										
// ---------------------------------------------------------------------------
// 
// ------------
void CWdGeometry_Free(CWdGeometry geom){
	dvs_free((dvertices*)geom);
}
*/
