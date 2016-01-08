//----------------------------------------------------------------------------
// File : bXMapUndo.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, undo
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
// 05/07/2006 creation.
//----------------------------------------------------------------------------

#include "bXMapUndo.h"
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/ext_utils.h>
#include <MacMapSuite/MacMapSuiteCPP.h>

// ---------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define kUndoSignature 'odnu'
#else
#define kUndoSignature kHICommandUndo
#endif

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapUndo::bXMapUndo(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("undo");
	_buffer=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapUndo::~bXMapUndo(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapUndo::create(bGenericXMLBaseElement* elt){
	return(new bXMapUndo(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapUndo::open(int* flags){
	bStdXMap::open(flags);
	_bsz=256;
	_buffer=malloc(_bsz);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapUndo::close(){
	bStdXMap::close();
	if(_buffer){
		free(_buffer);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUndo::process(int msg, void* prm){
	if(msg!=kExtProcessCallFromIntf){
		return(false);
	}
bEventLog	log(_gapp,
				getbundle(),
				kXMapUndoMessageID,
				GetSignature(this));

bGenericEvent*	evt;
bool			inval=false;
long			n=0;
	_tot=0;
	for(long i=1;i<=_gapp->eventMgr()->events()->count();i++){
		_gapp->eventMgr()->events()->get(i,&evt);
		if(	(evt->kind()==kEventKindGeoElement)	&&
			(evt->creator()!=kUndoSignature)	){
			n+=evt->elements()->count();
		}
	}
char	mess[__MESSAGE_STRING_LENGTH_MAX__];
	b_message_string(kXMapUndoProgressMessageID,mess,getbundle(),1);
bProgressWait wt("",mess,true,false,n);

	
	for(long i=_gapp->eventMgr()->events()->count();i>0;i--){
		_gapp->eventMgr()->events()->get(i,&evt);
		if(	(evt->kind()==kEventKindGeoElement)	&&
			(evt->creator()!=kUndoSignature)	){
			if(!inval){
				_gapp->layersMgr()->SetObjInvalidation(false);
				inval=true;
			}
			process_event(evt,wt);
		}
	}
	log.close();
	if(inval){
		_gapp->layersMgr()->SetObjInvalidation(true);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUndo::test(void* prm){
bGenericEvent*	evt;

	for(long i=1;i<=_gapp->eventMgr()->events()->count();i++){
		_gapp->eventMgr()->events()->get(i,&evt);
		if(	(evt->kind()==kEventKindGeoElement)	&&
//			(evt->creator()!=kUndoSignature)	){
			(!evt->is_undo())					){
			return(true);
		}
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapUndo::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapUndo::process_event(bGenericEvent* evt, bStdWait& wt){
bGenericGeoElement* elt;
bGenericGeoElement* ref;
bGenericType*		tp;
int					sz,bk;
long				i,j;
ivertices*			vxs;

	switch(evt->action()){
		case kEventActionCreate:
			for(i=evt->elements()->count();i>0;i--){
				if(!wt.set_progress(++_tot)){
				}
				evt->elements()->get(i,&elt);
				tp=_gapp->typesMgr()->get(elt->getType());
				tp->kill_object(elt);
			}
			break;
		case kEventActionModify:
			for(i=evt->elements()->count();i>0;i--){
				if(!wt.set_progress(++_tot)){
				}				
				evt->elements()->get(i,&elt);
				ref=elt->get_ref();
				if(ref){
					tp=_gapp->typesMgr()->get(elt->getType());
					for(j=kOBJ_Dir_+1;j<=tp->fields()->count();j++){
						tp->fields()->get_size(j,&sz);
						if(sz>_bsz){
							_bsz=sz;
							_buffer=realloc(_buffer,_bsz);
						}
						if(elt->getValue(j,_buffer)){
							ref->setValue(j,_buffer);
						}
					}
					elt->getVertices(&vxs);
					if(vxs){
						ref->setVertices(vxs);
					}
					elt->getName((char*)_buffer);
					if(((char*)_buffer)[0]!=-1){
						ref->setName((char*)_buffer);
					}
					bk=elt->getSubType();
					if(bk>0){
						ref->setSubType(bk);
					}
					bk=elt->getColor();
					if(bk>0){
						ref->setColor(bk);
					}
					bk=elt->getDirection();
					if((bk>=-1)&&(bk<=1)){
						ref->setDirection(bk);
					}
				}
			}
			break;
		case kEventActionDestroy:
			for(i=evt->elements()->count();i>0;i--){
				if(!wt.set_progress(++_tot)){
				}
				evt->elements()->get(i,&elt);
				tp=_gapp->typesMgr()->get(elt->getType());
				tp->unkill_object(elt);
			}
			break;
	}
}
