//----------------------------------------------------------------------------
// File : bToolTextEdition.cpp
// Project : MacMap
// Purpose : C++ source file : Object modification (field edition) tool class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 13/06/2007 creation.
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#include "bToolTextEdition.h"
#include <mox_intf/bCoreTextInfo.h>
#include <mox_intf/xmldesc_utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/bEventLog.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bToolTextEdition::bToolTextEdition(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				: bStdToolPres(elt,gapp,bndl){
	setclassname("texteditiontool");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bToolTextEdition::~bToolTextEdition(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bToolTextEdition::create(bGenericXMLBaseElement* elt){
	return(new bToolTextEdition(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bToolTextEdition::clic(CGPoint pt, int count){
_bTrace_("bToolTextEdition::clic",true);
	bStdToolPres::clic(pt,count);
	
bArray*					ga;
bArray					runs(sizeof(bGenericXMLBaseElement*));
bGenericXMLBaseElement	*run,*gm,*elt;
bGenericGeoElement*		o=NULL;
int						i,n=_gapp->layersAccessCtx()->count();
CGPoint					p;
bGenericStyle*			_styl;
int						_fld;

	set_obj(NULL);
	get_clic(&p);
	for(i=n;i>0;i--){
		_styl=_gapp->layersAccessCtx()->get(i);
		if(!_styl){
			continue;
		}	
		ga=_styl->objsatpoint(&p,true);
		if(!ga){
			continue;
		}
		if(ga->count()<=0){
			delete ga;
			continue;
		}
		if(ga->count()>1){
			delete ga;
			break;
		}
		ga->get(1,&o);
		delete ga;
		break;
	}
	
	if(!o){
		_styl=NULL;
		return;
	}

bGenericUnit*	scl=_gapp->scaleMgr()->get();

	_styl->get_styleruns(runs,o,scl->coef(),1);

char			val[_values_length_max_];
bGenericType	*tp=_gapp->typesMgr()->get(o->getType());
	_fld=0;

	for(int i=1;i<=runs.count();i++){
		runs.get(i,&run);
		gm=_gapp->classMgr()->NthElement(run,1,"text");
		if(!gm){
			continue;
		}
		elt=_gapp->classMgr()->NthElement(gm,1,"field");
		if(!elt){
			continue;
		}
		elt->getvalue(val);
		_fld=tp->fields()->get_index(val);
		break;
	}

	if(!_fld){
_te_("no text field");
		_styl=NULL;
		return;
	}
	
	set_obj(o);

	o->getValue(_fld,val);
	if(GetAName(val,"",true)){
CGRect	bnds;
		_styl->rect(o,&bnds);
		_gapp->mapIntf()->inval(bnds);		
		
bEventLog	log(_gapp,
				getbundle(),
				kTextEditionMessageID,
				GetSignature(this));
		o->setValue(_fld,&val);	
		log.close();
		_styl->rect(o,&bnds);
		_gapp->mapIntf()->inval(bnds);		
	}
	
	set_obj(NULL);
}

