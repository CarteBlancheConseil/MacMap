//----------------------------------------------------------------------------
// File : bXMapJoinFamily.cpp
// Project : MacMap
// Purpose : C++ source file : XMap base class for join utilities (geometry)
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
// 12/07/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapJoinFamily.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapJoinFamily	::bXMapJoinFamily(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapJoinFamily::~bXMapJoinFamily(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapJoinFamily::create(bGenericXMLBaseElement* elt){
	return(new bXMapJoinFamily(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapJoinFamily::open(int* flags){
	bStdXMap::open(flags);
	_dmax=0;
	read_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapJoinFamily::close(){
	bStdXMap::close();
	write_p();
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinFamily::process(int msg, void* prm){
_bTrace_("bXMapJoinFamily::process",false);
join_prm*	j=(join_prm*)prm;
	switch(msg){
		case kExtProcessCallWithParams:
			break;
		case kExtProcessCallWithXMLTree:
			break;
		case kExtProcessCallGetData:
			j->dmax=_dmax;
			break;
		case kExtProcessCallSetData:
_tm_("set data: "+j->dmax);
			_dmax=j->dmax;
			write_p();
			break;
		default:
			return(false);
	}
	return(false);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapJoinFamily::test(void* prm){
bArray* arr=_gapp->selMgr()->elements();
	return((arr->count()>=2));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapJoinFamily::read_p(){
_bTrace_("bXMapJoinFamily::read_p",false);
char	name[_names_length_max_];
	getclassname(name);
bGenericXMLBaseElement* root=NULL;
bGenericXMLBaseElement* elt=NULL;
bool					b=_gapp->document()->readTree(&root,kXMapJoinFamilyConfigSign,name);
char					val[_values_length_max_];
	
	if((!b)||(!root)){
_tw_("((!b)||(!root)) for "+name);
		return;
	}
	elt=_gapp->classMgr()->NthElement(root,1,"float");
	if(!elt){
_te_("dmax not found");
		return;
	}
	elt->getvalue(val);
	_dmax=Measure_i2d(_gapp,matof(val));
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapJoinFamily::write_p(){
bGenericXMLBaseElement* root=make_p();
	if(!root){
		return;
	}
char	name[_names_length_max_];
	getclassname(name);	

	if(!_gapp->document()->writeTree(root,kXMapJoinFamilyConfigSign,name)){
//		return;
	}
	_gapp->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericXMLBaseElement* bXMapJoinFamily::make_p(){
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
double	d=Measure_d2i(_gapp,_dmax);
	add_ddesc(arr,1,"float",d,_gapp->document()->dist_pref_digits());
bGenericXMLBaseElement*	root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapJoinFamily::info(long far, long joined){
    if(far+joined>0){
char	msgf[__MESSAGE_STRING_LENGTH_MAX__];
char	msgj[__MESSAGE_STRING_LENGTH_MAX__];
        b_message_string("too_far",msgf,getbundle(),1);
        b_message_string("already_joined",msgj,getbundle(),1);

char	msg[__MESSAGE_STRING_LENGTH_MAX__];
        
        if(far&&joined){
            sprintf(msg,"%s%ld\n%s%ld",msgf,far,msgj,joined);
        }
        else if(far){
            sprintf(msg,"%s%ld",msgf,far);
        }
        else if(joined){
            sprintf(msg,"%s%ld",msgj,joined);
        }
bAlertStop	alrt(msg,"",false,1);
    }
}

