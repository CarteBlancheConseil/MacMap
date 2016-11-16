//----------------------------------------------------------------------------
// File : bXMapJoinOnDir.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, join in the direction
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

#include "bXMapJoinOnDir.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>

// ---------------------------------------------------------------------------
//
// ------------
static int ivx_on_seg(i2dvertex* vx,
                      i2dvertex* a,
                      i2dvertex* b){
    return( (   ((a->h<=vx->h)&&(vx->h<=b->h))	||
                ((a->h>=vx->h)&&(vx->h>=b->h))	)&&
            (	((a->v<=vx->v)&&(vx->v<=b->v))	||
                ((a->v>=vx->v)&&(vx->v>=b->v))	));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapJoinOnDir	::bXMapJoinOnDir(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bXMapJoinFamily(elt,gapp,bndl){
	setclassname("joinondir");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapJoinOnDir::~bXMapJoinOnDir(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapJoinOnDir::create(bGenericXMLBaseElement* elt){
	return(new bXMapJoinOnDir(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinOnDir::process(int msg, void* prm){
join_prm*	j=(join_prm*)prm;
	switch(msg){
		case kExtProcessCallWithParams:
			if(j->dmax<0){
				j->dmax=_dmax;
			}
			return(join(&j->vx,&j->prev,j->ref,j->dmax,&j->res,&j->cur,&j->d));
		default:
			return(bXMapJoinFamily::process(msg,prm));
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinOnDir::edit(void* prm){
bArray				arr(*(_gapp->selMgr()->elements()));
join_prm			prm1,prm2;
bGenericGeoElement	*gref,*geo;
ivertices			*vx;
bool                b1,b2;
long                toofar=0,alreadyjoined=0;

	arr.get(1,&gref);
    gref->getVertices(&prm1.ref);
	
bEventLog	log(_gapp,this);
   
    for(long i=2;i<=arr.count();i++){
        arr.get(i,&geo);
        geo->getVertices(&vx);
        geo->inval(kOBJ_Vertices_);
        
        prm1.dmax=_dmax;
        prm2=prm1;
        prm1.vx=vx->vx.vx2[0];
        prm1.prev=vx->vx.vx2[1];
        prm2.vx=vx->vx.vx2[vx->nv-1];
        prm2.prev=vx->vx.vx2[vx->nv-2];
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
                vx->vx.vx2[vx->nv-1]=prm1.res;
            }
            else{
                vx->vx.vx2[0]=prm2.res;
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
bool bXMapJoinOnDir::join(	i2dvertex* vx,
							i2dvertex* pvx,
							ivertices* ref,
							double	dmax,
							i2dvertex* res,
							int* cur,
							double*	d){
i2dvertex	tmp;
double		l=LONG_MAX;

	(*d)=l;
	for(int i=0;i<ref->nv-1;i++){
        ivx2_get_sect(pvx,vx,&ref->vx.vx2[i],&ref->vx.vx2[i+1],&tmp);
        if(ivx_on_seg(&tmp,&ref->vx.vx2[i],&ref->vx.vx2[i+1])){
            l=Measure_i2d(_gapp,ivx2_dist(vx,&tmp));
            if(l<(*d)){
                (*d)=l;
                (*res)=tmp;
                (*cur)=i+1;
            }
        }
    }
    return((*d)<dmax);
}
