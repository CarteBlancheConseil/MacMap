//----------------------------------------------------------------------------
// File : bMacMapEventMgr.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap events management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 17/08/2005 creation.
//----------------------------------------------------------------------------

#include "bMacMapEventMgr.h"
#include "bObjectEvent.h"
#include "bTypeEvent.h"
#include "bFieldEvent.h"
#include "bUserExtEvent.h"
#include "bMacMapApp.h"
#include <MacMapSuite/bTrace.h>


#include "bMacMapModifiedGeoElement.h"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapEventMgr	::bMacMapEventMgr()
				:_evts(sizeof(bGenericEvent*))
				,_tmp(sizeof(bGenericEvent*)){
	_enabled=false;
	_logged=false;
	_evt=NULL;
	_sign=_kNULLSign_;
	_msg[0]=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMacMapEventMgr::~bMacMapEventMgr(){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::enable(){
_bTrace_("bMacMapEventMgr::enable",true);_tm_("");
	_enabled=true;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::disable(){
<<<<<<< HEAD
_bTrace_("bMacMapEventMgr::disable",true);_tm_("");
=======
_bTrace_("bMacMapEventMgr::disable",true);
>>>>>>> origin/master
    if(_enabled){
_tm_("enabled");
        if(_evt){
_tm_("delete current event");
            delete _evt;
        }
_tm_("delete temp events array :"+_tmp.count());
        for(long i=1;i<=_tmp.count();i++){
            _tmp.get(i,&_evt);
            delete _evt;
        }
        _tmp.reset();
_tm_("delete events array :"+_evts.count());
        for(long i=1;i<=_evts.count();i++){
            _evts.get(i,&_evt);
            delete _evt;
        }
        _evts.reset();
        _evt=NULL;
    }
    
    _evt=NULL;
    _enabled=false;
    _logged=false;
    _sign=_kNULLSign_;
    _msg[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEventMgr::state(){
	return(_enabled);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEventMgr::init_log(int creator, const char* msg){
_bTrace_("bMacMapEventMgr::init_log",true);
_tm_("logged by "+msg+" ("+(UInt32*)&creator+")");
	if(!_enabled){
_tm_("event manager disabled");
		return(-1);
	}
	if(_logged){
_tm_("event manager already logged");
		return(-2);
	}
    
// On ne reset l'existant qu'au close
// => permet de conserver les évènements en cas de session avortée
    
	_logged=true;
	_evt=NULL;
	_sign=creator;
	sprintf(_msg,"%s",msg);
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapEventMgr::close_log(){
_bTrace_("bMacMapEventMgr::close_log",true);
	if(!_enabled){
		return(-1);
	}
	if(!_logged){
		return(-1);
	}
	if(_evt){
		std_close();
	}
	_logged=false;

_tm_("reset "+_evts.count()+" events");
bMacMapEvent*	evt;
	for(long i=1;i<=_evts.count();i++){
		_evts.get(i,&evt);
        delete evt;
	}
	_evts.reset();
    
bool	b=_evts+_tmp;
#pragma unused(b)
	_tmp.reset();
	
	if(_evts.count()>0){
		send();
	}
    
_tm_("now "+_evts.count()+" events");
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::reset_log(){
_bTrace_("bMacMapEventMgr::reset_log",true);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	_logged=false;
	if(!_evt){
		return;
	}
	std_close();
	for(long i=1;i<=_tmp.count();i++){
		_tmp.get(i,&_evt);
		delete _evt;
	}
	_evt=NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
bArray* bMacMapEventMgr::events(){
	return(&_evts);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::create(field_info* fi){
//_bTrace_("MacMapEventMgr::create()",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
field_info	lfi;
	if((_evt)&&(!std_test(kEventKindDataStruct,kEventActionCreate))){
		std_close();
	}
	if((_evt)&&(_evt->elements()->count()>1)){
		_evt->elements()->get(1,&lfi);
		if(lfi.tp!=fi->tp){
			std_close();
		}
	}
	if(!_evt){
		_evt=new bFieldCreateEvent(_msg,_sign);
	}
	_evt->add(fi);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::create(bGenericGeoElement* o){
//_bTrace_("MacMapEventMgr::create(bGenericGeoElement* o)",false);
//_tm_("new object");
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindGeoElement,kEventActionCreate))){
		std_close();
	}
	if(!_evt){
		_evt=new bObjectCreateEvent(_msg,_sign);
	}
	_evt->add(&o);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::create(bGenericType* t){
//_bTrace_("MacMapEventMgr::create(bGenericType* t)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindTypeElement,kEventActionCreate))){
		std_close();
	}
	if(!_evt){
		_evt=new bTypeCreateEvent(_msg,_sign);
	}
	_evt->add(&t);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::create(bGenericExt* e){
//_bTrace_("MacMapEventMgr::create(bGenericExt* e)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindUserExt,kEventActionCreate))){
		std_close();
	}
	if(!_evt){
		_evt=new bUserExtCreateEvent(_msg,_sign);
	}
	_evt->add(&e);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::kill(field_info* fi){
//_bTrace_("MacMapEventMgr::kill(field_info* fi)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
field_info	lfi;
	if((_evt)&&(!std_test(kEventKindDataStruct,kEventActionDestroy))){
		std_close();
	}
	if((_evt)&&(_evt->elements()->count()>1)){
		_evt->elements()->get(1,&lfi);
		if(lfi.tp!=fi->tp){
			std_close();
		}
	}
	if(!_evt){
		_evt=new bFieldDeleteEvent(_msg,_sign);
	}
	_evt->add(fi);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::kill(bGenericGeoElement* o){
//_bTrace_("MacMapEventMgr::kill(bGenericGeoElement* o)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindGeoElement,kEventActionDestroy))){
		std_close();
	}
	if(!_evt){
		_evt=new bObjectDeleteEvent(_msg,_sign);
	}
	_evt->add(&o);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::kill(bGenericType* t){
//_bTrace_("MacMapEventMgr::kill(bGenericType* t)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindTypeElement,kEventActionDestroy))){
		std_close();
	}
	if(!_evt){
		_evt=new bTypeDeleteEvent(_msg,_sign);
	}
	_evt->add(&t);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::kill(bGenericExt* e){
//_bTrace_("MacMapEventMgr::kill(bGenericExt* e)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindUserExt,kEventActionDestroy))){
		std_close();
	}
	if(!_evt){
		_evt=new bUserExtDeleteEvent(_msg,_sign);
	}
	_evt->add(&e);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::modify(field_info* fi){
//_bTrace_("MacMapEventMgr::modify(field_info* fi)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
field_info	lfi;
	if((_evt)&&(!std_test(kEventKindDataStruct,kEventActionModify))){
		std_close();
	}
	if((_evt)&&(_evt->elements()->count()>1)){
		_evt->elements()->get(1,&lfi);
		if(lfi.tp!=fi->tp){
			std_close();
		}
	}
	if(!_evt){
		_evt=new bFieldModifyEvent(_msg,_sign);
	}
	_evt->add(fi);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::modify(bGenericGeoElement* o, int field){
//_bTrace_("MacMapEventMgr::modify(bGenericGeoElement* o)",false);
//_tm_("modify object");
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindGeoElement,kEventActionModify))){
//_tm_("closing event");
		std_close();
	}
	if(!_evt){
        _evt=new bObjectModifyEvent(_msg,_sign);
	}
	_evt->add(&o,field);}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::modify(bGenericType* t){
//_bTrace_("MacMapEventMgr::modify(bGenericType* t)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindTypeElement,kEventActionModify))){
		std_close();
	}
	if(!_evt){
		_evt=new bTypeModifyEvent(_msg,_sign);
	}
	_evt->add(&t);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::modify(bGenericExt* e){
//_bTrace_("MacMapEventMgr::modify(bGenericExt* e)",false);
	if(!_enabled){
		return;
	}
	if(!_logged){
		return;
	}
	if((_evt)&&(!std_test(kEventKindUserExt,kEventActionModify))){
		std_close();
	}
	if(!_evt){
		_evt=new bUserExtModifyEvent(_msg,_sign);
	}
	_evt->add(&e);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::send(){
EventRef	evt;
	
// Création d'un EventRef MacMap
	(void)CreateEvent(	kCFAllocatorDefault,
						kEventClassMacMap,
						kEventMacMapDataBase,
                        GetCurrentEventTime(),
						kEventAttributeUserEvent,
						&evt);
// Envoi de l'EventRef à l'appli
	(void)SendEventToApplication(evt);
	ReleaseEvent(evt);
// Pour forcer la fenêtre carto à activer le mécanisme undo/redo
	_MMAPP_->mapIntf()->registerAction();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapEventMgr::std_test(int kind, int action){
	if(_evt==NULL){
		return(false);
	}
	return((_evt->action()==action)&&(_evt->kind()==kind));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapEventMgr::std_close(){
	_evt->close();
	if(!_tmp.add(&_evt)){
		return;
	}
	_evt=NULL;
}

