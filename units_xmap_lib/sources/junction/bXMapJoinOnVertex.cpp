//----------------------------------------------------------------------------
// File : bXMapJoinOnVertex.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, join on vertices
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

#include "bXMapJoinOnVertex.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bEventLog.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapJoinOnVertex	::bXMapJoinOnVertex(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapJoinFamily(elt,gapp,bndl){
	setclassname("joinonvertex");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapJoinOnVertex::~bXMapJoinOnVertex(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapJoinOnVertex::create(bGenericXMLBaseElement* elt){
	return(new bXMapJoinOnVertex(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinOnVertex::process(int msg, void* prm){
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
bool bXMapJoinOnVertex::edit(void* prm){
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
        geo->inval(kOBJ_Vertices_);
 
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
bool bXMapJoinOnVertex::join(	i2dvertex* vx,
								ivertices* ref,
								double	dmax,
								i2dvertex* res,
								int* cur,
								double *d){
bool	b=false;
double	l;
	
	(*d)=0x7FFFFFFF;
	(*d)*=2.0;
	for(int i=0;i<=ref->nv;i++){
		l=Measure_i2d(_gapp,sm_dst(vx,&ref->vx.vx2[i]));
		if((l<=dmax)&&((l<(*d))||(!b))){
			l=Measure_i2d(_gapp,ivx2_dist(vx,&ref->vx.vx2[i]));
			if((l<=dmax)&&((l<(*d))||(!b))){
				b=true;
				(*d)=l;
				(*res)=ref->vx.vx2[i];
				(*cur)=i;
				if((*cur)==ref->nv){
					(*cur)--;
				}
			}
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapJoinOnVertex::sm_dst(i2dvertex* a, i2dvertex* b){
int	dx=labs(b->h-a->h);
int	dy=labs(b->v-a->v);
	if(dx>dy){
		return(dx);
	}
	else{
		return(dy);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
/*pascal	static	Boolean	DoJoncAng(const	BigPoint	*theBP,LongSegsHdl	ref,long	max,BigPoint	*bp,long	*curSeg,double	*d)
{
	Boolean	found;
	long	i;
	double	l;
	BigPoint	bp1;
	
	Boolean	DoJoncAng__RESULT;

	(*d)=0x7FFFFFFF;
	(*d)=(*d)*2;
	found=false;
	for(i=1;i<=(**(ref)).nbPts;i++)
	{
		bp1=(**(ref)).bigPts[i-1];
		l=SmallDist(theBP,&bp1);
		if((l<=max)&&((l<(*d))||(!found)))
		{
			l=DistBigPoint(*theBP, bp1);
			if((l<=max)&&((l<(*d))||(!found)))
			{
				found=true;
				(*d)=l;
				(*bp)=bp1;
				(*curSeg)=i;
				if((*curSeg)==(**(ref)).nbPts)
					(*curSeg)=(*curSeg)-1;
				}
			}
		}
	DoJoncAng__RESULT=found;
	return(DoJoncAng__RESULT);
}
*/
