//----------------------------------------------------------------------------
// File : bXMapDelete.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, destroy selected objects
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
// 09/05/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapDelete.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/mm_messages.h>
#include <MacMapSuite/MacMapSuiteCPP.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDelete	::bXMapDelete(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("delete");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDelete::~bXMapDelete(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDelete::create(bGenericXMLBaseElement* elt){
	return(new bXMapDelete(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDelete::process(int msg, void* prm){
	if(	(msg!=kExtProcessCallFromIntf)		&&
		(msg!=kExtProcessCallWithXMLTree)	){
		return(false);
	}
	if(_gapp->selMgr()->count()>0){
bArray		arr(*(_gapp->selMgr()->elements()));
		_gapp->selMgr()->flush();

bGenericGeoElement*	o;
bGenericType*		tp;
bEventLog			log(_gapp,
						getbundle(),
						kXMapDeleteMessageID,
						GetSignature(this));
						
char	mess[__MESSAGE_STRING_LENGTH_MAX__];
		b_message_string(kXMapDeleteProgressMessageID,mess,getbundle(),1);
bProgressWait wt("",mess,true,arr.count());

		_gapp->layersMgr()->SetObjInvalidation(false);
		for(int i=arr.count();i>0;i--){
			if(!wt.set_progress(1+arr.count()-i)){
				break;
			}
			arr.get(i,&o);
			tp=_gapp->typesMgr()->get(o->getType());
			tp->kill_object(o);
		}
		_gapp->layersMgr()->SetObjInvalidation(true);
		log.close();
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDelete::test(void* prm){
	return(_gapp->selMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDelete::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}
