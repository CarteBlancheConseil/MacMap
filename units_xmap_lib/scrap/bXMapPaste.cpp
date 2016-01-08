//----------------------------------------------------------------------------
// File : bXMapPaste.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, paste MacMap objects
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
// 29/04/2015 Elimination ScrapManager.
//----------------------------------------------------------------------------

#include "bXMapPaste.h"
#include "MMPasteBoardData.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPaste	::bXMapPaste(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("paste");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPaste::~bXMapPaste(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPaste::create(bGenericXMLBaseElement* elt){
	return(new bXMapPaste(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaste::process(int msg, void* prm){
_bTrace_("bXMapPaste::process",true);
void*	data=NULL;
size_t  dtsz;
void*   nsdata=getPasteBoardData(&data,&dtsz);

    if((dtsz==0)||(data==NULL)){
        return true;
    }
    
void*				cur=data;
void*				last=(void*)((UInt32)data+dtsz);
int					idx,k;
tag*				tg;
bGenericGeoElement*	o=NULL;
bGenericType*		tp=NULL;
char				name[260];
UInt32				etsz=(sizeof(tag)-sizeof(void*));
char				mess[__MESSAGE_STRING_LENGTH_MAX__];
	get_localized_name(mess,_bndl);
bBarberWait			wt(mess,NULL,true,true);
bEventLog           log(_gapp,this);
   
    _gapp->layersMgr()->SetObjInvalidation(false);

	while(cur<last){
		if(!wt.set_progress(0)){
            _gapp->layersMgr()->SetObjInvalidation(true);
			return(true);
		}
		tg=(tag*)cur;
		if(tg->sign==kScrapMacMapTagType){
			memcpy(name,(void*)((UInt32)cur+etsz),tg->snsz);
			tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(name));
			if(!tp){
_te_("bad type name "+name);
				o=NULL;
			}
			else if(!tp->new_object(&o)){
_te_("new_object failed");
				o=NULL;
			}
		}
		else if(tg->sign==kScrapMacMapTagField){
			if(o){
				memcpy(name,(void*)((UInt32)cur+etsz),tg->snsz);
				idx=tp->fields()->get_index(name);
				if(idx!=0){
					tp->fields()->get_kind(idx,&k);
					switch(k){
						case _ivxs2:{
						case _ivxs3:
							ivertices*	vxs=(ivertices*)malloc(tg->cnsz);
							memcpy(vxs,(void*)((UInt32)cur+etsz+tg->snsz),tg->cnsz);
							vxs=ivs_unpack(vxs);
							o->setValue(idx,(void*)vxs);
							ivs_free(vxs);
							}break;
						case _dvxs2:
						case _dvxs3:{
							dvertices*	vxs=(dvertices*)malloc(tg->cnsz);
							memcpy(vxs,(void*)((UInt32)cur+etsz+tg->snsz),tg->cnsz);
							vxs=dvs_unpack(vxs);
							o->setValue(idx,(void*)vxs);
							dvs_free(vxs);
							}break;
						default:
							o->setValue(idx,(void*)((UInt32)cur+etsz+tg->snsz));
							break;
					}					
				}
				else{
_te_("bad field name "+name);
				}
			}
		}
		else{
_te_("bad tag sign");
			break;
		}
		cur=(void*)((UInt32)cur+etsz+tg->snsz+tg->cnsz);
	}
    _gapp->layersMgr()->SetObjInvalidation(true);
	log.close();
    cleanPasteBoardData(nsdata);
    
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaste::test(void* prm){
    return testPasteBoardData();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPaste::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

