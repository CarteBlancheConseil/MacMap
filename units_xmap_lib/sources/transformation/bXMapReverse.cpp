//----------------------------------------------------------------------------
// File : bXMapReverse.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, reverse vertices index in geometry
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
// 16/12/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapReverse.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapReverse	::bXMapReverse(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("reverse");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapReverse::~bXMapReverse(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapReverse::create(bGenericXMLBaseElement* elt){
	return(new bXMapReverse(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapReverse::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			reverse();
			break;
		case kExtProcessCallWithParams:
		case kExtProcessCallWithXMLTree:
			if(test(prm)){
				reverse(0);
			}
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapReverse::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapReverse::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapReverse::reverse(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapReverseMessageID,
				GetSignature(this));
	reverse(0);
	log.close();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapReverse::reverse(int dummy){
ivertices*			vxs;
bGenericGeoElement*	o;
bArray				sel(*(_gapp->selMgr()->elements()));
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	message_string(kMsgProgress,mess,1);
bProgressWait		wt("",mess,true,false,sel.count());

	_gapp->layersMgr()->SetObjInvalidation(false);
	for(int i=1;i<=sel.count();i++){
		if(!wt.set_progress(i)){
			break;
		}
		sel.get(i,&o);
		o->getVertices(&vxs);
		ivs_reverse(vxs);
		o->setVertices(vxs);
	}
	_gapp->layersMgr()->SetObjInvalidation(true);
}
