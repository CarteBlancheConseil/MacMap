//----------------------------------------------------------------------------
// File : bXMapJoinToNearest.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, join to nearest
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
// 12/07/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapJoinToNearest.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapJoinToNearest	::bXMapJoinToNearest(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapJoinFamily(elt,gapp,bndl){
	setclassname("jointonearest");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapJoinToNearest::~bXMapJoinToNearest(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapJoinToNearest::create(bGenericXMLBaseElement* elt){
	return(new bXMapJoinToNearest(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinToNearest::process(int msg, void* prm){
join_prm*	j=(join_prm*)prm;
	switch(msg){
		case kExtProcessCallWithParams:
			if(j->dmax<0){
				j->dmax=_dmax;
			}
			return(join(&j->vx,j->ref,j->dmax,&j->res,&j->cur,&j->d));
		default:
			return(bXMapJoinFamily::process(msg,prm));
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinToNearest::edit(void* prm){
bArray*             arr=_gapp->selMgr()->elements();
join_prm			prm1,prm2;
bGenericGeoElement	*gref,*geo;
ivertices			*vx;
bool                b1,b2;
long                toofar=0,alreadyjoined=0;

	arr->get(1,&gref);
    gref->getVertices(&prm1.ref);

bEventLog	log(_gapp,this);
    
    for(long i=2;i<=arr->count();i++){
        arr->get(i,&geo);
        geo->getVertices(&vx);
        
        prm1.dmax=_dmax;
        prm2=prm1;
        prm1.vx=vx->vx.vx2[0];
        prm2.vx=vx->vx.vx2[vx->nv-1];
        prm1.d=prm2.d=-1;

        b1=process(kExtProcessCallWithParams,&prm1);
        b2=process(kExtProcessCallWithParams,&prm2);
        if(!b2&&!b1){
            toofar++;
            continue;
        }
        if((prm1.d==0)||(prm2.d==0)){
            alreadyjoined++;
            continue;
        }
        geo->inval(kOBJ_Vertices_);
        if(b1&&b2){
            if(prm1.d<=prm2.d){
                vx->vx.vx2[0]=prm1.res;
            }
            else{
                vx->vx.vx2[vx->nv-1]=prm2.res;
            }
        }
        else if(b2&&!b1){
            vx->vx.vx2[vx->nv-1]=prm2.res;
        }
        else{
            vx->vx.vx2[0]=prm1.res;
        }
        geo->setVertices(vx);
    }
	log.close();
    info(toofar,alreadyjoined);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinToNearest::join(	i2dvertex* vx,
								ivertices* ref,
								double	dmax,
								i2dvertex* res,
								int* cur,
								double *d){
double		dst,r;
int			idx;
i2dvertex	vr;

	dst=Measure_i2d(_gapp,ivx_proj(vx,ref,&vr,&idx,&r));
	if(dst<=dmax){
		*res=vr;
		*cur=idx;
		*d=dst;
		return(true);
	}
	return(false);
}
