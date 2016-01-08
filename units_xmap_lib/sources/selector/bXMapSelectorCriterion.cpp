//----------------------------------------------------------------------------
// File : bXMapSelectorCriterion.cpp
// Project : MacMap
// Purpose : C++ source file : XMap selector utility (selection criterion)
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
// 20/01/2005 creation.
//----------------------------------------------------------------------------

#include "bXMapSelectorCriterion.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/bGenericCalc.h>
#include <mox_intf/bGenericGeog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/endian.h>
#include <std_ext/bStdUserExt.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
bGenericMacMapApp*	bXMapSelectorCriterion::_gapp=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSelectorCriterion::bXMapSelectorCriterion(	bGenericMacMapApp* gapp,
												int cid,
												int targetsign,
												char* targetvalue,
												int targetkind,
												int opsgn,
												char* val,
												bool* succed){
	if(_gapp==NULL){
		_gapp=gapp;
	}
	(*succed)=false;
	_id=cid;
	_tgsign=targetsign;
	_tgname=NULL;
	_tgvalue=NULL;			
	_opname=NULL;
	_valname=NULL;
	_valvalue=NULL;
	
char				name[256];
	
	if(_tgsign==kDBFieldElement){
		_tgname=strdup("dbfield");
		_tgvalue=strdup(targetvalue);
	}
	else{
char	sgn[10];
		if(_tgsign==0){
			strcpy(sgn,"");
		}
		else{
			sprintf(sgn,"%.4s",&_tgsign);
		}
		_tgname=strdup(targetvalue);
		_tgvalue=strdup(sgn);
	}

	switch(opsgn){
		case kBeginWithElement:
			sprintf(name,"beginwith");
			break;
		case kContainElement:
			sprintf(name,"contain");
			break;
		case kEndWithElement:
			sprintf(name,"endwith");
			break;
		case kEqualElement:
			sprintf(name,"equal");
			break;
		case kHigherOrEqualElement:
			sprintf(name,"higherorequal");
			break;
		case kHigherElement:
			sprintf(name,"higher");
			break;
		case kLowerOrEqualElement:
			sprintf(name,"lowerorequal");
			break;
		case kLowerElement:
			sprintf(name,"lower");
			break;
		case kNotEqualElement:
			sprintf(name,"notequal");
			break;
		default:
			sprintf(name,"equal");
			break;
	}
	_opname=strdup(name);
		
	switch(targetkind){
		case _char:
			(*succed)=true;
			sprintf(name,"dbchar");
			_valvalue=strdup(val);
			break;
		case _bool:
			(*succed)=true;
			sprintf(name,"dbbool");
			_valvalue=strdup(val);
			break;
		case _int:
			(*succed)=true;
			sprintf(name,"dbint");
			_valvalue=strdup(val);
			break;
		case _double:
			(*succed)=true;
			sprintf(name,"dbfloat");
			_valvalue=strdup(val);
			break;
		case _date:
			(*succed)=true;
			sprintf(name,"dbdate");
			_valvalue=strdup(val);
			break;
		case _time:
			(*succed)=true;
			sprintf(name,"dbtime");
			_valvalue=strdup(val);
			break;
		default:
			(*succed)=true;
			sprintf(name,"dbquestion");
			_valvalue=strdup("?");
			break;
	}
	if((*succed)){
		_valname=strdup(name);
	}
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSelectorCriterion::~bXMapSelectorCriterion(){	
	if(_tgname){
		free(_tgname);
	}
	if(_tgvalue){
		free(_tgvalue);
	}
	if(_opname){
		free(_opname);
	}
	if(_valname){
		free(_valname);
	}
	if(_valvalue){
		free(_valvalue);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bXMapSelectorCriterion::get_id(){
	return(_id);
}

// ---------------------------------------------------------------------------
// 
// -----------
CFStringRef bXMapSelectorCriterion::get_string(CFBundleRef bndl){
_bTrace_("bXMapSelectorCriterion::get_string",true);
CFStringRef	cfs;
char		knd[256]="????";
char		intfname[256]="????";
char		opname[256]="????";
int			i;

	if(_tgsign==kDBFieldElement){
		sprintf(intfname,"%s",_tgvalue);
		b_message_string("field",knd,bndl,0);
	}
	else{
		i=RecoverCalc(_gapp,_tgname);
_tm_("RecoverCalc "+_tgname+" returned "+i);
		if(i==0){	
			i=RecoverCalc(_gapp,_tgvalue);
_tm_("RecoverCalc "+_tgvalue+" returned "+i);
		}
		_gapp->calcMgr()->ext_name(i,intfname);
_tm_("ext mane for returned "+intfname+" for "+i);
		b_message_string("calc",knd,bndl,0);
	}
	b_message_string(_opname,opname,bndl,0);
	cfs=CFStringCreateWithFormat(kCFAllocatorDefault,NULL,CFSTR("%s %s %s %s"),knd,intfname,opname,_valvalue);
	return(cfs);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelectorCriterion::make(bArray* l, int indent, CFBundleRef bndl){
_bTrace_("bXMapSelectorCriterion::make",true);
_tm_(_opname);
	add_cdesc(*l,indent,_opname,"");
_tm_(_tgname+"->"+_tgvalue);
	add_cdesc(*l,indent+1,_tgname,_tgvalue);
	if(strcmp(_valname,"dbquestion")!=0){
_tm_(_valname+"->"+_valvalue);
		add_cdesc(*l,indent+1,_valname,_valvalue);
	}
	else{
char		buf1[_values_length_max_];
char		buf2[_values_length_max_];
CFStringRef	cfs=get_string(bndl);
		CFStringGetCString(cfs,buf1,_values_length_max_-1,kCFStringEncodingMacRoman);
		CFRelease(cfs);
		if(_tgsign==kDBFieldElement){
			sprintf(buf2,"%s:%s",_tgvalue,buf1);
		}
		else{
			if((_tgvalue)&&(strlen(_tgvalue)>0)){
				sprintf(buf2,"%s:%s",_tgvalue,buf1);
			}
			else{
				sprintf(buf2,"%s:%s",_tgname,buf1);
			}
		}
		add_cdesc(*l,indent+1,_valname,buf2);
_tm_(_valname+"->"+buf2);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapSelectorCriterion::parse(bGenericXMLBaseElement* root, void* prm, int indent){
_bTrace_("bXMapSelectorCriterion::parse",true);
bool					b=true;
parse_prm*				p=(parse_prm*)prm;
bXMapSelectorCriterion*	crit=NULL;
UInt32					knd=root->getsubclass();
char					name[_names_length_max_];
char					value[_values_length_max_];
int						idx;
bGenericCalc*			clc;
char					sgn[10];

	if(!_gapp){
		_gapp=(bGenericMacMapApp*)root->getapp();
	}
	root->getclassname(name);
	switch(knd){
		case kXMLSubClassFormula:
			if(!strcmp(name,"dbtype")){
				root->getvalue(value);
				p->tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(value));
				return(p->tp!=NULL);
			}
			else if(!strcmp(name,"dbselkind")){
				root->getvalue(value);
				p->selwhat=atoi(value);
				return(true);
			}
			else if(!strcmp(name,"dbusemasked")){
				root->getvalue(value);
				p->use_mask=atoi(value);
				return(true);
			}
			else if(!strcmp(name,"dbsetnext")){
				root->getvalue(value);
				p->next=(*(UInt32*)value);
//#ifdef __LITTLE_ENDIAN__
//				LBSwapWord(&p->next,sizeof(UInt32));
//#endif
				return(true);
			}
			else if(!strcmp(name,"dbaddtocontrast")){
				root->getvalue(value);
				p->add2cnt=atoi(value);
				return(true);
			}
			else if(!strcmp(name,"dbaddtomasked")){
				root->getvalue(value);
				p->add2hdn=atoi(value);
				return(true);
			}
			return(true);
			break;
		case kXMLSubClassCalc:
			if(!p->tp){
				return(false);
			}
			idx=RecoverCalc(_gapp,name);
_tm_("RecoverCalc "+name+" returned "+idx);
			if(idx==0){	
				root->getvalue(value);
				idx=RecoverCalc(_gapp,value);
_tm_("RecoverCalc "+value+" returned "+idx);
			}
			if(idx==0){	
				//
				return(true);
			}
			clc=(bGenericCalc*)(void*)_gapp->calcMgr()->get(idx);
			clc->get_params(p->targetvalue,sgn);
			if(strlen(sgn)>0){
				p->targetsign=*((int*)sgn);
			}
			else{
				p->targetsign=0;
			}
			p->targetkind=clc->kind();
			break;
		case kXMLSubClassField:
			if(!p->tp){
				return(false);
			}
			p->targetsign=kDBFieldElement;
			root->getvalue(p->targetvalue);
			idx=p->tp->fields()->get_index(p->targetvalue);
			if(idx==0){
				p->targetkind=_char;
			}
			else{
				p->tp->fields()->get_kind(idx,&p->targetkind);			
			}
			return(true);
			break;
		case kXMLSubClassConstant:
			if(strcmp(name,"dbquestion")!=0){
				root->getvalue(p->val);
			}
			else{
				sprintf(p->val,"?");
			}
			break;
		case kXMLSubClassOperator:
			if(	(!strcmp(name,"and"))	||
				(!strcmp(name,"or"))	||
				(!strcmp(name,"xor"))	){
				return(true);
			}
			
			if(	(p->targetkind!=0)		&&
				(p->targetvalue[0]!=0)	&&
				(p->opsgn!=0)			){
				crit=new bXMapSelectorCriterion(_gapp,
												p->cid,
												p->targetsign,
												p->targetvalue,
												p->targetkind,
												p->opsgn,
												p->val,
												&b);
				if(!b){
					return(b);
				}
				p->stack->add(&crit);
				p->cid++;
				p->targetsign=0;
				p->targetvalue[0]=0;
				p->targetkind=0;
				p->opsgn=0;
				p->val[0]=0;
			}
			
			if(!strcmp(name,"beginwith")){
				p->opsgn=kBeginWithElement;
			}
			else if(!strcmp(name,"contain")){
				p->opsgn=kContainElement;
			}
			else if(!strcmp(name,"endwith")){
				p->opsgn=kEndWithElement;
			}
			else if(!strcmp(name,"equal")){
				p->opsgn=kEqualElement;
			}
			else if(!strcmp(name,"higherorequal")){
				p->opsgn=kHigherOrEqualElement;
			}
			else if(!strcmp(name,"higher")){
				p->opsgn=kHigherElement;
			}
			else if(!strcmp(name,"lowerorequal")){
				p->opsgn=kLowerOrEqualElement;
			}
			else if(!strcmp(name,"lower")){
				p->opsgn=kLowerElement;
			}
			else if(!strcmp(name,"notequal")){
				p->opsgn=kNotEqualElement;
			}
			else{
				p->opsgn=kEqualElement;
			}
			break;
			
			
		case kXMLSubClassGeog:
			idx=RecoverGeog(_gapp,name);
			if(idx==0){	
				//
				return(true);
			}
			p->geog=(bGenericGeog*)(void*)_gapp->geogMgr()->get(idx);
			p->geog->put_params(root);
			break;
	}
	
	return(b);
}
