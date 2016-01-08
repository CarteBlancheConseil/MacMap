//----------------------------------------------------------------------------
// File : bXMapUngroup.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, split object with complex geometry into several objects
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
// 07/08/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapUngroup.h"
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>


// ---------------------------------------------------------------------------
// 
// ------------
typedef struct check_dist_prm{
	bArray&		res;
	double		dmax;
	i2dvertex	vx;
}check_dist_prm;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapUngroup	::bXMapUngroup(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("ungroup");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapUngroup::~bXMapUngroup(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapUngroup::create(bGenericXMLBaseElement* elt){
	return(new bXMapUngroup(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapUngroup::open(int* flags){	
	bStdXMap::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			return(ungroup());
			break;
		case kExtProcessCallWithParams:
		case kExtProcessCallWithXMLTree:
			if(test(prm)){
				return(ungroup(true,true));
			}
			break;
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::ungroup(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapUngroupMessageID,
				GetSignature(this));
	_gapp->layersMgr()->SetObjInvalidation(false);
	ungroup(true,false);
	_gapp->layersMgr()->SetObjInvalidation(true);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::ungroup(bool kill, bool silent){
bGenericGeoElement* o;
bArray				sel(*(_gapp->selMgr()->elements()));
bGenericType*		tp;
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapUngroupProgressMessageID,mess,getbundle(),1);
bProgressWait		wt("",mess,true,false,sel.count());

	FlushSelection(_gapp);
	FlushContrasts(_gapp);

	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			break;
		}	
		sel.get(i,&o);
		ungroup(o,silent);
		if(kill){
			tp=_gapp->typesMgr()->get(o->getType());
			tp->kill_object(o);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUngroup::ungroup(bGenericGeoElement* o, bool silent){
bGenericGeoElement* no;
ivertices*			vxs;
ivertices*			nvxs;
i2dvertex*			part;
int					n;
bGenericType*		tp;

	o->getVertices(&vxs);
	for(int i=0;i<ivs_n_parts(vxs);i++){
		part=ivs2_part(vxs,i,&n);
		if(n>0){
			tp=_gapp->typesMgr()->get(o->getType());
			tp->clone_object(o,&no);
			nvxs=ivs_new(_2D_VX,n,0);
			memmove(nvxs->vx.vx2,part,n*sizeof(i2dvertex));
			no->setVertices(nvxs);
			ivs_free(nvxs);
			o=no;
		}
	}
	return(true);
}

