//----------------------------------------------------------------------------
// File : bStdUserExt.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for user externs
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
// 05/12/2005 creation.
//----------------------------------------------------------------------------

#include "bStdUserExt.h"
#include <MacMapSuite/bTrace.h>
#include <mox_intf/endian.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bStdUserExt	::bStdUserExt(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			: bStdExt(elt,gapp,bndl){
	_name=NULL;
	_usign=_kNULLSign_;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bStdUserExt::~bStdUserExt(){
	if(_name){
		free(_name);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserExt::open(int* flags){
	bStdExt::open(flags);
	read();
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExt::process(int msg, void* prm){
user_ext_data* p=(user_ext_data*)prm;
	switch(msg){
		case kExtProcessCallSetData:
			break;
		case kExtProcessCallGetData:
			p->sign=_usign;
			p->name=_name;
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExt::test(void* prm){
    return(_gapp->document()!=NULL);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bStdUserExt::edit(void* prm){
	return(process(kExtProcessCallFromIntf,NULL));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bStdUserExt::read(){
_bTrace_("bStdUserExt::read",false);
char				buff[_values_length_max_];
bStdXMLBaseElement* elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,1,"sign");
	if(elt){
		elt->getvalue(buff);
		_usign=*((UInt32*)buff);
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&_usign,sizeof(UInt32));
	_tm_("_usign="+(UInt32*)&_usign);
	LBSwapWord(&_usign,sizeof(UInt32));
#else
	_tm_("_usign="+(UInt32*)&_usign);
#endif
	}
	else{
_te_("no sign");
	}
	elt=(bStdXMLBaseElement*)(void*)_gapp->classMgr()->NthElement(this,1,"name");
	if(elt){
		elt->getvalue(buff);
		_name=strdup(buff);
_tm_("_name="+_name);
	}
	else{
_te_("no name");
	}
}

