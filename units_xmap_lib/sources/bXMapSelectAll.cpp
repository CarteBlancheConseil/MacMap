//----------------------------------------------------------------------------
// File : bXMapSelectAll.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classes, select all objects
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
// 01/03/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapSelectAll.h"
#include <mox_intf/bStdWait.h>
#include <mox_intf/mm_messages.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct selall{
	int			n;
	bArray*		ft;
	bStdWait*	wt;
}selall;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSelectAll::bXMapSelectAll(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("selectall");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSelectAll::~bXMapSelectAll(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSelectAll::create(bGenericXMLBaseElement* elt){
	return(new bXMapSelectAll(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectAll::process(int msg, void* prm){
	if(	(msg==kExtProcessCallFromIntf)		||
		(msg==kExtProcessCallWithXMLTree)	){
int			typ;
selall		p;
char		mess[__MESSAGE_STRING_LENGTH_MAX__];
		get_localized_name(mess,_bndl);
bBarberWait	wt(mess,NULL,true,true);
	
		p.wt=&wt;
		p.n=0;
		for(typ=1;typ<=_gapp->typesMgr()->count();typ++){
			(void)_gapp->typesMgr()->get(typ)->iterator()->iterate(&p,count);
			if(!wt.get_progress()){
				return(true);
			}
		}
		if(p.n<=0){
			return(true);
		}
	
bArray		l(sizeof(bGenericGeoElement*),p.n);
		p.n=0;
		p.ft=&l;
		for(typ=1;typ<=_gapp->typesMgr()->count();typ++){
			(void)_gapp->typesMgr()->get(typ)->iterator()->iterate(&p,fill);
			if(!wt.get_progress()){
				return(true);
			}
		}
		_gapp->selMgr()->add(&l);
	}
	
	
	else if(	(msg==kExtProcessCallWithParams)	&&
				(prm!=NULL)							){
selall		p;
char		mess[__MESSAGE_STRING_LENGTH_MAX__];
		get_localized_name(mess,_bndl);
bBarberWait	wt(mess,NULL,true,true);

		p.wt=&wt;
		p.n=0;

bGenericType*	tp=(bGenericType*)prm;
		
		tp->iterator()->iterate(&p,count);
		if(!wt.get_progress()){
			return(true);
		}		
bArray			l(sizeof(bGenericGeoElement*),p.n);
		p.n=0;
		p.ft=&l;
		tp->iterator()->iterate(&p,fill);
		_gapp->selMgr()->add(&l);
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectAll::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSelectAll::edit(void* prm){
	return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapSelectAll::count(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
selall*				p=(selall*)prm;

	p->wt->set_progress(0);
	if(!p->wt->get_progress()){
		return(-1);
	}
	if((!geo->selected())&&(!geo->masked())){
		p->n++;
	}	
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bXMapSelectAll::fill(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
selall*				p=(selall*)prm;

	p->wt->set_progress(0);
	if(!p->wt->get_progress()){
		return(-1);
	}
	if((!geo->selected())&&(!geo->masked())){
		p->n++;
		p->ft->put(p->n,&geo);
	}	
	return(0);
}

