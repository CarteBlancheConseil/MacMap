//----------------------------------------------------------------------------
// File : bXMapsScripting.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classes, scripting utilities
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
// 31/12/2008 creation.
//----------------------------------------------------------------------------

#include "bXMapsScripting.h"
#include <mox_intf/ext_utils.h>
#include <std_ext/bStdUserMacro.h>

#pragma mark -> bXMapScriptingDictionary
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapScriptingDictionary::bXMapScriptingDictionary(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						:bStdXMap(elt,gapp,bndl){
	setclassname("scriptingdictionary");
	_child[0]=new bXMapExecMacro(this,gapp,bndl);
	_child[1]=new bXMapRefreshScreen(this,gapp,bndl);
	_child[2]=new bXMapInitLog(this,gapp,bndl);
	_child[3]=new bXMapCloseLog(this,gapp,bndl);
	_child[4]=new bXMapSetLog(this,gapp,bndl);
	_child[5]=new bXMapHideStyle(this,gapp,bndl);
	_child[6]=new bXMapShowStyle(this,gapp,bndl);
	for(int i=0;i<kNbScriptDictionaryChilCount;i++){
		gapp->classMgr()->AddXMLClass(_child[i]);
	}	
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapScriptingDictionary::~bXMapScriptingDictionary(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapScriptingDictionary::create(bGenericXMLBaseElement* elt){
	return(new bXMapScriptingDictionary(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapScriptingDictionary::open(int* flags){
	bStdXMap::open(flags);
	for(int i=0;i<kNbScriptDictionaryChilCount;i++){
		_child[i]->open(flags);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapScriptingDictionary::close(){
	bStdXMap::close();
	for(int i=0;i<kNbScriptDictionaryChilCount;i++){
		_child[i]->close();
	}
}

#pragma mark -> bXMapExecMacro
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapExecMacro	::bXMapExecMacro(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("execmacro");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapExecMacro::~bXMapExecMacro(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapExecMacro::create(bGenericXMLBaseElement* elt){
	return(new bXMapExecMacro(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapExecMacro::process(int msg, void* prm){
	if(msg!=kExtProcessCallWithXMLTree){
		return(false);
	}	
char					name[_values_length_max_];
bGenericXMLBaseElement*	elt;
	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(name);
	exec(name);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapExecMacro::test(void* prm){
	return(_gapp->typesMgr()->count()>0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapExecMacro::exec(char* name){
int idx=RecoverMacro(_gapp,name);
	if(idx){
bStdUserMacro*	ext=(bStdUserMacro*)(void*)_gapp->macroMgr()->get(idx);
		if(ext){
			ext->process(kExtProcessCallFromIntf,NULL);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapRefreshScreen
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapRefreshScreen	::bXMapRefreshScreen(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("refreshscreen");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapRefreshScreen::~bXMapRefreshScreen(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapRefreshScreen::create(bGenericXMLBaseElement* elt){
	return(new bXMapRefreshScreen(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapRefreshScreen::process(int msg, void* prm){
	_gapp->layersMgr()->SetObjInvalidation(false);
	_gapp->layersMgr()->SetObjInvalidation(true);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapInitLog
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapInitLog	::bXMapInitLog(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("initlog");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapInitLog::~bXMapInitLog(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapInitLog::create(bGenericXMLBaseElement* elt){
	return(new bXMapInitLog(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapInitLog::process(int msg, void* prm){
bGenericXMLBaseElement*	elt;
char					mess[_values_length_max_]="script";
	elt=getelement(1);
	if(elt){
		elt->getvalue(mess);
	}
	_gapp->eventMgr()->init_log(_kNULLSign_,mess);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapCloseLog
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapCloseLog	::bXMapCloseLog(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
				:bStdXMap(elt,gapp,bndl){
	setclassname("closelog");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapCloseLog::~bXMapCloseLog(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapCloseLog::create(bGenericXMLBaseElement* elt){
	return(new bXMapCloseLog(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapCloseLog::process(int msg, void* prm){
	_gapp->eventMgr()->close_log();
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapSetLog
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapSetLog	::bXMapSetLog(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
			:bStdXMap(elt,gapp,bndl){
	setclassname("setlog");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapSetLog::~bXMapSetLog(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapSetLog::create(bGenericXMLBaseElement* elt){
	return(new bXMapSetLog(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapSetLog::process(int msg, void* prm){
	if(msg!=kExtProcessCallWithXMLTree){
		return(false);
	}	
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;
	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	if(atoi(val)){
		_gapp->eventMgr()->enable();
	}
	else{
		_gapp->eventMgr()->disable();
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapHideStyle
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapHideStyle	::bXMapHideStyle(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
	:bStdXMap(elt,gapp,bndl){
	setclassname("hidestyle");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapHideStyle::~bXMapHideStyle(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapHideStyle::create(bGenericXMLBaseElement* elt){
	return(new bXMapHideStyle(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapHideStyle::process(int msg, void* prm){
	if(msg!=kExtProcessCallWithXMLTree){
		return(false);
	}	
char					tpname[_values_length_max_];
char					stlname[_values_length_max_];
bGenericXMLBaseElement*	elt;
	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(tpname);
	elt=getelement(2);
	if(!elt){
		return(false);
	}
	elt->getvalue(stlname);

bGenericType*	tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(tpname));
bGenericStyle*	stl;
	for(long i=1;i<=_gapp->layersAccessCtx()->count();i++){
		stl=_gapp->layersAccessCtx()->get(i);
		if((stl->gettype()==tp)&&(strcmp(stlname,stl->getname())==0)){
			_gapp->layersAccessCtx()->set_current(i);
			_gapp->layersAccessCtx()->setvisible(false);
		}
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
#pragma mark -> bXMapShowStyle
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapShowStyle	::bXMapShowStyle(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
:bStdXMap(elt,gapp,bndl){
	setclassname("showstyle");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapShowStyle::~bXMapShowStyle(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapShowStyle::create(bGenericXMLBaseElement* elt){
	return(new bXMapShowStyle(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapShowStyle::process(int msg, void* prm){
	if(msg!=kExtProcessCallWithXMLTree){
		return(false);
	}	
char					tpname[_values_length_max_];
char					stlname[_values_length_max_];
bGenericXMLBaseElement*	elt;
	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(tpname);
	elt=getelement(2);
	if(!elt){
		return(false);
	}
	elt->getvalue(stlname);
	
bGenericType*	tp=_gapp->typesMgr()->get(_gapp->typesMgr()->index(tpname));
bGenericStyle*	stl;
	for(long i=1;i<=_gapp->layersAccessCtx()->count();i++){
		stl=_gapp->layersAccessCtx()->get(i);
		if((stl->gettype()==tp)&&(strcmp(stlname,stl->getname())==0)){
			_gapp->layersAccessCtx()->set_current(i);
			_gapp->layersAccessCtx()->setvisible(true);
		}
	}
	
	return(true);
}

