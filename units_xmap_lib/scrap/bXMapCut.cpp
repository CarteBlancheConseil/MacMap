//----------------------------------------------------------------------------
// File : bXMapCut.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, cut MacMap objects
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
// 21/11/2006 Creation.
// 29/04/2015 Elimination ScrapManager, passage en subclasser de bXMapCopy
//----------------------------------------------------------------------------

#include "bXMapCut.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <MacMapSuite/bTrace.h>

// --------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCut	::bXMapCut(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bXMapCopy(elt,gapp,bndl){
	setclassname("cut");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCut::~bXMapCut(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapCut::create(bGenericXMLBaseElement* elt){
	return(new bXMapCut(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapCut::killObjects(bArray& arr){
_bTrace_("bXMapCut::killObjects",true);
bGenericGeoElement*	o;
bGenericType*		tp;
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
    get_localized_name(mess,_bndl);
bBarberWait			wt(mess,"",true);
bEventLog           log(_gapp,this);
    
    _gapp->layersMgr()->SetObjInvalidation(false);
    for(long i=1;i<=arr.count();i++){
        if(!wt.set_progress(0)){
            _gapp->layersMgr()->SetObjInvalidation(true);
            return;
        }
        arr.get(i,&o);
        tp=_gapp->typesMgr()->get(o->getType());
        tp->kill_object(o);
    }
    _gapp->layersMgr()->SetObjInvalidation(true);
 
    log.close();
}

