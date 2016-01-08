//----------------------------------------------------------------------------// File : bObjListMgr.cpp// Project : MacMap// Purpose : C++ source file : Base class for object lists management (i.e. selection & contrats)// Author : Benoit Ogier, benoit.ogier@macmap.com//// Copyright (C) 1997-2015 Carte Blanche Conseil.//// This program is free software: you can redistribute it and/or modify// it under the terms of the GNU General Public License as published by// the Free Software Foundation, either version 3 of the License, or// (at your option) any later version.//// This program is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the// GNU General Public License for more details.//// You should have received a copy of the GNU General Public License// along with this program.  If not, see <http://www.gnu.org/licenses/>.//// See the LICENSE.txt file for more information.////----------------------------------------------------------------------------// //----------------------------------------------------------------------------// 22/01/2004 creation.//----------------------------------------------------------------------------#include "bMacMapApp.h"#include "bObjListMgr.h"// ---------------------------------------------------------------------------// Constructeur// ------------bObjListMgr::bObjListMgr(){	_elts=new bArray(sizeof(bGenericGeoElement*));	_state=0;	_serial=false;	_curs=0;}// ---------------------------------------------------------------------------// Destructeur// -----------bObjListMgr::~bObjListMgr(){	if(_elts){		delete _elts;	}}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::load(){		_state=0;	_serial=false;	_curs=0;	return(noErr);}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::unload(){	flush();	return(noErr);}// ---------------------------------------------------------------------------// // ------------int bObjListMgr::count(){	return(_elts->count());}// ---------------------------------------------------------------------------// // ------------int bObjListMgr::getState(){	return(_state);}// ---------------------------------------------------------------------------// // ------------void bObjListMgr::incState(){	_state++;}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::add(bGenericGeoElement* o){	if(!_elts->add(&o)){		return(-1);	}	if(!_serial){		_state++;		_curs=0;	}	return(noErr);}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::add(bArray* l){	if(!l){		return(noErr);	}	bArray* ln=(bArray*)l;		(*_elts)+(*ln);		_state++;	_curs=0;	return(noErr);}// ---------------------------------------------------------------------------// // -----------static int compare(const void *a, const void *b){bGenericGeoElement**	fa=(bGenericGeoElement**)a;bGenericGeoElement**	fb=(bGenericGeoElement**)b;	if((*fa)<(*fb)){		return(-1);	}	if((*fa)>(*fb)){		return(1);	}	return(0);}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::rmv(bGenericGeoElement* o){int	idx=_elts->search(&o,compare);	if(idx<=0){		return(-1);	}	if(!_elts->rmv(idx)){		return(-1);	}	if(!_serial){		_state++;		_curs=0;	}	return(noErr);}// ---------------------------------------------------------------------------// // -----------void bObjListMgr::flush(){	if(count()>0){		_elts->reset();		_state++;		_curs=0;	}}// ---------------------------------------------------------------------------// // -----------bArray* bObjListMgr::elements(){	return(_elts);}// ---------------------------------------------------------------------------// // -----------bGenericGeoElement* bObjListMgr::next(){	if(count()<=0){		return(NULL);	}	_curs++;	if(_curs>count()){		_curs=1;	}	bGenericGeoElement* o;	if(_elts->get(_curs,&o)){		return(o);	}	return(NULL);}// ---------------------------------------------------------------------------// // -----------bGenericGeoElement* bObjListMgr::prev(){	if(count()<=0){		return(NULL);	}	_curs--;	if(_curs<=0){		_curs=count();	}	bGenericGeoElement* o;	if(_elts->get(_curs,&o)){		return(o);	}	return(NULL);}// ---------------------------------------------------------------------------// // -----------int bObjListMgr::type_count(){int	n,i;	n=0;	for(i=1;i<=typesmgr->count();i++){		if(typesmgr->get(i)->nb_sel()>0){			n++;		}	}	return(n);}// ---------------------------------------------------------------------------// // -----------bool bObjListMgr::type_unique(){int	n,i;	n=0;	for(i=1;i<=typesmgr->count();i++){		if(typesmgr->get(i)->nb_sel()>0){			n++;			if(n>1){				return(false);			}		}	}	return(n==1);}