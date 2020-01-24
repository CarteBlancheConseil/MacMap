//----------------------------------------------------------------------------
// File : bXMapPerforate.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, make hole in a polygon from another polygon, assing geometry to the first object then destroy second object
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
// 11/12/2008 creation.
//----------------------------------------------------------------------------

#include "bXMapPerforate.h"
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_lib.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPerforate	::bXMapPerforate(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("perforate");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPerforate::~bXMapPerforate(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPerforate::create(bGenericXMLBaseElement* elt){
	return(new bXMapPerforate(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapPerforate::open(int* flags){	
	bStdXMap::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPerforate::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			(void)perforate();
			break;
		case kExtProcessCallWithParams:
		case kExtProcessCallWithXMLTree:
			if(test(prm)){
				(void)perforate(false,true);
			}
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPerforate::test(void* prm){
	return(	(_gapp->selMgr()->count()>1)						&&
			(SelectionIsMonoGender(_gapp)==kBaseKindPolygon)	);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPerforate::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPerforate::perforate(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapPerforateMessageID,
				GetSignature(this));
	perforate(false,false);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPerforate::perforate(bool kill, bool silent){
_bTrace_("bXMapPerforate::perforate(bool kill, bool silent)",false);
bGenericGeoElement	*oa,*ob;
ivertices			*vsa,*vsb;
bArray				sel(*(_gapp->selMgr()->elements()));
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapPerforateProgressMessageID,mess,getbundle(),1);
bProgressWait		wt("",mess,false,sel.count());
bGenericType*		tp;
i2dvertex*			p;
int					i,j,nv;

	sel.get(1,&oa);
	oa->getVertices(&vsb);
	ivs2ivs(vsb,&vsa);
	p=ivs2_part(vsa,0,&nv);
	if(ivx2_clockwise(p,nv)==-1){
//_tm_("reverse vsa");
		ivs_reverse(vsa);
	}
//_tm_("vsb->no="+vsb->no+" vsa->no="+vsa->no);
	for(i=2;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		sel.get(i,&ob);
		ob->getVertices(&vsb);
		if(vsb->nv<4){
_te_("vsb->nv<4");
			continue;
		}
		if(ivs_n_parts(vsb)>1){
_te_("ivs_n_parts(vsb)>1");
			continue;
		}
		for(j=0;j<vsb->nv-1;j++){
			if(!ivx_in_ivs(&vsb->vx.vx2[j],vsa,0)){
				break;
			}
		}
		if(j<vsb->nv-1){
_te_("outside");
			continue;
		}
		p=ivs2_part(vsb,0,&nv);
		if(ivx2_clockwise(p,nv)==1){
//_tm_("reverse vsb");
			ivs_reverse(vsb);
		}
		vsa=ivs_group(vsa,vsb);
		if(!vsa){
_te_("ivs_group failed");
			return(false);
		}
		if(kill){
			tp=_gapp->typesMgr()->get(ob->getType());
			tp->kill_object(ob);
		}
	}
	oa->setVertices(vsa);
	ivs_free(vsa);
	return(true);
}
