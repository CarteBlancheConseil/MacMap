//----------------------------------------------------------------------------
// File : bXMapGroup.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, group geometries of selected object in a complex geometry, then assing it to the first object and destroy others
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
// 09/12/2008 creation.
//----------------------------------------------------------------------------

#include "bXMapGroup.h"
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/ext_utils.h>
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
bXMapGroup	::bXMapGroup(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("group");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapGroup::~bXMapGroup(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapGroup::create(bGenericXMLBaseElement* elt){
	return(new bXMapGroup(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapGroup::open(int* flags){	
	bStdXMap::open(flags);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGroup::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			(void)group();
			break;
		case kExtProcessCallWithParams:
		case kExtProcessCallWithXMLTree:
			if(test(prm)){
				(void)group(true,true);
			}
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGroup::test(void* prm){
	return(	(_gapp->selMgr()->count()>1)					&&
			(SelectionIsMonoType(_gapp))					&&
			(SelectionIsMonoGender(_gapp)!=kBaseKindRaster)	);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGroup::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGroup::group(){
bEventLog	log(_gapp,this);
	group(true,false);
	log.close();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapGroup::group(bool kill, bool silent){
bGenericGeoElement	*oa,*ob;
ivertices			*vsa,*vsb;
bArray				sel(*(_gapp->selMgr()->elements()));
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapGroupProgressMessageID,mess,getbundle(),1);
bProgressWait		wt("",mess,false,sel.count());
bGenericType*		tp;

	sel.get(1,&oa);
	oa->getVertices(&vsb);
	ivs2ivs(vsb,&vsa);
	for(int i=2;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			break;
		}	
		sel.get(i,&ob);
		ob->getVertices(&vsb);
		vsa=ivs_group(vsa,vsb);
		if(!vsa){
			break;
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
