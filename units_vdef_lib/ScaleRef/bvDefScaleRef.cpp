//----------------------------------------------------------------------------
// File : bvDefScaleRef.cpp
// Project : MacMap
// Purpose : C++ source file : Scale reference vDef utility class
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
// 28/04/2010 creation.
//----------------------------------------------------------------------------

#include "bvDefScaleRef.h"
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/valconv.h>
#include <MacMapSuite/C_Utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/mm_messages.h>
#include <limits.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bvDefScaleRef	::bvDefScaleRef(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdvDef(elt,gapp,bndl){
	setclassname("scalerefvdef");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bvDefScaleRef::~bvDefScaleRef(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bvDefScaleRef::create(bGenericXMLBaseElement* elt){
	return(new bvDefScaleRef(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefScaleRef::open(int* flags){
	_scale=0;
	_idx=1;
	_set=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefScaleRef::close(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefScaleRef::process(int msg, void* prm){
_bTrace_("bvDefScaleRef::process",true);
bGenericXMLBaseElement* root=NULL;
	switch(msg){
		case kExtProcessCallFromIntf:
			_set=1;
			if(edit(&root,NULL)){
				_set=2;
				return(true);
			}
			else{
				_set=0;
				return(false);
			}
			break;
		case kExtProcessCallSetData:
			_set=0;
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefScaleRef::test(bGenericType* tp){
	return(false);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bvDefScaleRef::edit(bGenericXMLBaseElement** root, bGenericType* tp){
	if(_set==0){
		return(bStdvDef::edit(root,tp));
	}
	else if(_set==1){
		return(bStdvDef::edit(root,tp));
	}
	_root=(*root);
	write();
	(*root)=_root;
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefScaleRef::wd_event(EventRef evt, WindowRef wd){
bool			b=true;
UInt32			clss=GetEventClass(evt);	
HICommand		cmd;
ControlRef		c;
bGenericUnit*	u;

	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				write();
				break;
			case kHICommandCancel:
				break;
			case kvDefScaleRefPopupCmd:
				c=get_control(kvDefScaleRefViewSignature,kvDefScaleRefPopupID);
				_idx=GetControl32BitValue(c);
				if(_idx>2){
					u=_gapp->scaleMgr()->get(_idx-2);
					_scale=u->coef();
				}
				else{
					_scale=0;
				}
				break;
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bvDefScaleRef::wd_init(WindowRef wd){
	_idx=1;
	read();
	
ControlRef	c;
	if(_set==1){
		c=get_control(kvDefScaleRefViewSignature,kvDefScaleRefTypeNameLegendID);
		HideControl(c);
		c=get_control(kvDefScaleRefViewSignature,kvDefScaleRefTypeNameID);
		HideControl(c);
	}
	else{
		c=get_control(kvDefScaleRefViewSignature,kvDefScaleRefTypeNameID);
char		name[256];
		if(_tp){
			_tp->name(name);
		}
		else{
			message_string(kMsgVirtual,name,1);
		}
		SetTextControlValue(c,name);
	}
	
	c=get_control(kvDefScaleRefViewSignature,kvDefScaleRefPopupID);
	PopulatePopupControlWithScales(_gapp,c,_idx);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bvDefScaleRef::make(){
_bTrace_("bvDefScaleRef::make",true);
	if(_set>0){
		return;
	}
	
bArray	arr(sizeof(xmlelt));
char	val[256];
int		i,n;
	
	if(_tp){
		n=_tp->fields()->count_constraints(kOBJ_SubType_);
	}
	else{
		n=0;
	}
	add_cdesc(arr,0,"circlestyle","");
	
// Identification		
	add_cdesc(arr,1,"styleidentification","");
	add_cdesc(arr,2,"name","");
	add_cdesc(arr,2,"sign","SRef");
	add_cdesc(arr,2,"version","3.0.0");

// Globals
	add_cdesc(arr,1,"globals","");
	if(_tp){
		_tp->fields()->get_name(kOBJ_SubType_,val);
		add_cdesc(arr,2,"classfield",val);
	}
	add_ddesc(arr,2,"scaleref",_scale,0);
	add_ddesc(arr,2,"unitcoef",1,0);
	if(_tp){
		add_idesc(arr,2,"sort",2);
		add_idesc(arr,2,"passcount",1);
		add_cdesc(arr,2,"classboundsarray","");
		for(i=1;i<=n;i++){
			add_idesc(arr,3,"classbound",i);
			_tp->fields()->get_constraint(kOBJ_SubType_,i,val);
			add_cdesc(arr,3,"classname",val);
		}	
		add_idesc(arr,3,"classbound",i);
		add_cdesc(arr,3,"classname","");
	}
	else{
		add_idesc(arr,2,"sort",0);
		add_idesc(arr,2,"passcount",1);
	}
// Condition	
	add_cdesc(arr,1,"condition","");
// Nothing else, condition are optional.

// StyleRun
	add_cdesc(arr,1,"stylerun","");
	add_cdesc(arr,2,"validity","");
	if(_tp){
		add_idesc(arr,3,"classmin",1);
		add_idesc(arr,3,"classmax",2);
	}
	add_ddesc(arr,3,"scalemin",1,0);
	add_ddesc(arr,3,"scalemax",LONG_MAX,0);
	add_idesc(arr,3,"pass",1);
	add_cdesc(arr,2,"render","");
	add_cdesc(arr,3,"stroke","");
	add_ddesc(arr,4,"red",0.5,2);
	add_ddesc(arr,4,"green",0.5,2);
	add_ddesc(arr,4,"blue",0,2);
	add_ddesc(arr,4,"alpha",1,2);
	add_cdesc(arr,3,"fill","");
	add_ddesc(arr,4,"red",0.5,2);
	add_ddesc(arr,4,"green",0.5,2);
	add_ddesc(arr,4,"blue",0.5,2);
	add_ddesc(arr,4,"alpha",1,2);
	if(_tp){
		add_cdesc(arr,2,"stdgeometry","");
	}
	else{
		add_cdesc(arr,2,"screengeometry","");
	}
	add_idesc(arr,3,"centroid",1);
	add_ddesc(arr,3,"size",3,2);
	add_ddesc(arr,3,"width",1,2);
	_root=_gapp->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefScaleRef::read(){
_bTrace_("bvDefScaleRef::read",true);
	if(_set>0){
		return(false);
	}

bool	made=false;
	if(!_root){
		make();
		made=true;
	}

int						i;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
bGenericUnit*			u;

	elt=_gapp->classMgr()->NthElement(_root,1,"scaleref");
	if(elt){
		elt->getvalue(val);
		_scale=matof(val);
		for(i=_gapp->scaleMgr()->count();i>0;i--){
			u=_gapp->scaleMgr()->get(i);
			if(u->coef()==_scale){
				break;
			}
		}
		if(i==0){
			_idx=1;
		}
		else{
			_idx=i+2;
		}
	}
	else{
		_idx=1;
	}
	if(made){
		_gapp->classMgr()->ReleaseXMLInstance(_root);
		_root=NULL;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bvDefScaleRef::write(){
_bTrace_("bvDefScaleRef::write",true);
	if(_set==1){
		return(false);
	}

	if(!_root){
		make();
        if(!_root){
            return(false);
        }
	}
	
bMemFile	f;
char*		bf;
int			sz;
	
	_root->encode(0,&f);
	if(f.status()){
		_gapp->classMgr()->ReleaseXMLInstance(_root);
		return(false);
	}
	f.mount(&bf,&sz);
	bf=(char*)realloc(bf,sz+1);
	bf[sz]=0;
	
	_root=_gapp->classMgr()->ParseXMLData(bf,sz);
	
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
	
	elt=_gapp->classMgr()->NthElement(_root,1,"scaleref");
	if(elt){
		sprintf(val,"%f",_scale);
		elt->setvalue(val);
	}
		
	free(bf);

	return(true);
}

