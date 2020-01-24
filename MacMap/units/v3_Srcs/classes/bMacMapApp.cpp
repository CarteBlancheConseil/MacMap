//----------------------------------------------------------------------------
// File : bMacMapApp.cpp
// Project : MacMap
// Purpose : C++ source file : Application main class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2003-2018 Carte Blanche Conseil.
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
// 20/11/2003 creation.
// 13/04/2017 LayerAccessContext removed.
// 15/11/2018 Updates for Cocoa.
//----------------------------------------------------------------------------


#include "bMacMapApp.h"
#include "bClassesMgr.h"
#include "bToolMgr.h"
#include "bXMapMgr.h"
#include "bvDefMgr.h"
#include "bGeogMgr.h"
#include "bCalcMgr.h"
#include "bMacroMgr.h"
#include "bScriptMgr.h"
#include "bXBoxMgr.h"
#include "bVarMgr.h"
#include "bScaleMgr.h"
#include "bDistUnitMgr.h"
#include "bLayersMgr.h"
#include "bViewMgr.h"
#include "bStyleMgr.h"
#include "bTypesMgr.h"
#include "bMacMapEventMgr.h"
#include "bMenuMgr.h"
#include "bPrintMgr.h"
#include "bSelectionMgr.h"
#include "bContrastesMgr.h"
#include "bCGLocConverter.h"
#include "bMacMapDocument.h"
#include "CreateDoc.h"
#include "bDelayedAE.h"
#include <loader/loader.h>
#include <MacMapSuite/MacMapSuiteCPP.h>

#include "MacMapDocument.h"

// ---------------------------------------------------------------------------
// 
// ------------
/*OSStatus appRegisterHelpBook	(void);
OSStatus appGoToHelpAnchor		(CFStringRef an);*/

// ---------------------------------------------------------------------------
// Globale
// ------------
bGenericMacMapApp* bMacMapApp::_appinst=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapApp	::bMacMapApp(const char* app_name){
	_status=noErr;
	
int status;
	
    _extmgr				=Loader_ExtLibMgrAlloc(this);
                
	_clssmgr			=new bClassesMgr();
	_toolmgr			=new bToolMgr();
	_xmapmgr			=new bXMapMgr();
	_vdefmgr			=new bvDefMgr();
	_geogmgr			=new bGeogMgr();
	_calcmgr			=new bCalcMgr();
	_macromgr			=new bMacroMgr();
	_scriptmgr			=new bScriptMgr();
	_xboxmgr			=new bXBoxMgr();
	_varmgr				=new bVarMgr();
	
	_scalemgr			=new bScaleMgr();
	_distmgr			=new bDistUnitMgr();
//	_anglemgr			=NULL;
//	_timemgr			=NULL;
	
	_layersmgr			=new bLayersMgr();
	_viewmgr			=new bViewMgr();
	_stylemgr			=new bStyleMgr(NULL,&status);
	_typesmgr			=new bTypesMgr();
	_eventmgr			=new bMacMapEventMgr();
	_menumgr			=new bMenuMgr();
	_printmgr			=new bPrintMgr();

	_selmgr				=new bSelectionMgr();
	_cntmgr				=new bContrastesMgr();
		
	_locConverter		=new bCGLocConverter();
	
	_map_intf			=NULL;// CrŽŽ ˆ run
	_status_intf		=NULL;// CrŽŽ ˆ run
				
	_doc				=NULL;// CrŽŽ ˆ load
	
	_on_run				=false;
	_in_timer			=false;
	
	_closing            =false;
	_opening            =false;
                
    _tmppath            =NULL;
    _tmpcc              =NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapApp::~bMacMapApp(){
	delete _cntmgr;
	delete _selmgr;
	delete _scalemgr;
	delete _typesmgr;
	delete _layersmgr;
	delete _eventmgr;

	delete _clssmgr;
}

// ---------------------------------------------------------------------------
// Init globale
// ------------
bGenericMacMapApp* bMacMapApp::instance(const char* app_name){
	if(_appinst==NULL){
		_appinst=new bMacMapApp(app_name);
	}
	return(_appinst);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::open_doc(const char* path, void* cc){
_bTrace_("bMacMapApp::open_doc(const char*)",true);
OSStatus	status=noErr;
	
    if(!_on_run){
        if(_tmppath==NULL){
            _tmppath=strdup(path);
            _tmpcc=cc;
        }
        else{
            status=-1;
        }
        return status;
    }
    
	_opening=true;
	
	for(;;){
		if(_doc){
			status=-1;
_te_("document already open");
			_opening=false;
			return(status);
		}

		_doc=new bMacMapDocument();
		if((status=_doc->load(path))){
_te_("_doc->load = "+status);
			break;
		}

// Affichage de la fentre 
		if(_map_intf){
			_map_intf->open(cc);
		}

		break;
	}
    if(status&&_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
	}
	
	_opening=false;
    menuMgr()->check_menus();

	return(status);
}

// ---------------------------------------------------------------------------
// Attention, maintenant close_doc est appelŽ par map_intf
// Donc pas d'appel vers map_intf ˆ ce niveau
// ------------
OSStatus bMacMapApp::close_doc(){
_bTrace_("bMacMapApp::close_doc",true);
OSStatus	status=noErr;

	_closing=true;
	
	if(_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
    }
	else{
		status=-1;
_tm_("no document");
	}

	_closing=false;
    menuMgr()->check_menus();

	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::create_doc(void* cc){
_bTrace_("bMacMapApp::create_doc",true);
OSStatus	status=noErr;
char		path[PATH_MAX];
int			unit=10;
int			srid=-1;
double		reso=1;

	_opening=true;

	for(;;){
		if(_doc){
			status=-1;
_te_("document already open");
			_opening=false;
			return(status);
		}
		if((status=choose_path(path,&unit,&reso,&srid))){
_te_("choose_path = "+status);
			break;
		}
		_doc=new bMacMapDocument();
		if(!_doc->make(path,unit,reso,srid)){
_te_("_doc->make failed");
			status=-1;
			break;
		}
		if((status=_doc->load(path))){
_te_("_doc->load = "+status);
			break;
		}
// Affichage de la fentre 
		if(_map_intf){
			_map_intf->open(cc);
		}
		
		break;
	}
	if(status&&_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
	}

	_opening=false;
    menuMgr()->check_menus();
    
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::run(){
_bTrace_("bMacMapApp::run",true);
    
    if(_on_run==true){
        return noErr;
    }
OSStatus status;
    
	if((status=_extmgr->load())){
_te_("_extmgr->load()="+status);
		return(status);
	}
	
// On charge ici pour Žviter les pb au cas ou une externe voudrait Žcrire dans la minifiche
	if(_extmgr->get_component(kComponentStatusWd)){
		_status_intf=(bGenericStatusIntf*)_extmgr->get_component(kComponentStatusWd)->i_allocate(0,_MMAPP_,NULL);
	}	
	if(_extmgr->get_component(kComponentMapWd)){
		_map_intf=(bGenericMapIntf*)_extmgr->get_component(kComponentMapWd)->i_allocate(0,_MMAPP_,NULL);
	}	
	
	if((status=_clssmgr->load())){
_te_("_clssmgr->load()="+status);
		return(status);
	}
	if((status=_toolmgr->load())){
_te_("_toolmgr->load()="+status);
		return(status);
	}
	if((status=_xmapmgr->load())){
_te_("_xmapmgr->load()="+status);
		return(status);
	}
	if((status=_vdefmgr->load())){
_te_("_vdefmgr->load()="+status);
		return(status);
	}
	if((status=_geogmgr->load())){
_te_("_geogmgr->load()="+status);
		return(status);
	}
	if((status=_calcmgr->load())){
_te_("_calcmgr->load()="+status);
		return(status);
	}
	if((status=_varmgr->load())){
_te_("_varmgr->load()="+status);
		return(status);
	}
	if((status=_xboxmgr->load())){
_te_("_xboxmgr->load()="+status);
		return(status);
	}
	if((status=_macromgr->load())){
_te_("_xboxmgr->load()="+status);
		return(status);
	}
	if((status=_scriptmgr->load())){
_te_("_xboxmgr->load()="+status);
		return(status);
	}
    
    _on_run=true;
    
    if(_status_intf){
        _status_intf->init();
    }

    if(_tmppath!=NULL){
        if(open_doc(_tmppath,_tmpcc)){
_te_("open_doc "+_tmppath+" failed");
        }
        free(_tmppath);
        _tmppath=NULL;
        _tmpcc=NULL;
    }
    
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapApp::dispatch_cmd(UInt32 cmd){
_bTrace_("bMacMapApp::dispatch_cmd",true);
bGenericExt* ext=_xmapmgr->find(cmd);
	if(!ext){
		ext=_calcmgr->find(cmd);
	}
	if(!ext){
		ext=_varmgr->find(cmd);
	}
	if(!ext){
		ext=_macromgr->find(cmd);
	}
	if(!ext){
		ext=_scriptmgr->find(cmd);
	}
	if(!ext){
		ext=_xboxmgr->find(cmd);
	}
	if(!ext){
_tm_("command <"+(unsigned int*)&cmd+"> not available");
		return(false);
	}
	if(!_doc){
_tm_("no document");
		return(false);	
	}
	if(_closing){
_tm_("closing in progress");
		return(false);	
	}
	if(_opening){
_tm_("opening in progress");
		return(false);	
	}
	if(!ext->test(NULL)){
_tm_("ext <"+(unsigned int*)&cmd+"> can't be applied");
		return(false);
	}
    
char name[256];
    ext->getclassname(name);
_tm_(name);
    
	return(ext->edit(NULL));
}
