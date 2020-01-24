//----------------------------------------------------------------------------
// File : bMacMapApp.h
// Project : MacMap
// Purpose : Header file : Application main class
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

#ifndef __bMacMapApp__
#define __bMacMapApp__

//----------------------------------------------------------------------------

//#include "mm_config.h"
#include <mox_intf/bGenericMacMapApp.h>
//#include <mox_intf/bGenericMapIntf.h>
//#include <mox_intf/bGenericStyleMgr.h>

//----------------------------------------------------------------------------

/*enum{
	kMacMapEventClass				='MapT',
	kAEMacMapSendCommand			='SenD',
	kAEMacMapSendCommandWithParam	='SenP'
};*/

//----------------------------------------------------------------------------

class bMacMapApp : public bGenericMacMapApp{
public:
	bMacMapApp	 											(	const char* app_name);
	virtual ~bMacMapApp  									(	);

	static bGenericMacMapApp* instance						(	const char* app_name);
    static bGenericMacMapApp* instance						(	){return(_appinst);};
	
    virtual bGenericExtLibMgr* extMgr						(	){return(_extmgr);};
    virtual bGenericClassesMgr* classMgr					(	){return(_clssmgr);};
	
    virtual bGenericUnitMgr* scaleMgr						(	){return(_scalemgr);};
    virtual bGenericUnitMgr* distMgr						(	){return(_distmgr);};
    virtual bGenericUnitMgr* angleMgr						(	){return(NULL);};
    virtual bGenericUnitMgr* timeMgr						(	){return(NULL);};
	
    virtual bGenericLayersMgr* layersMgr					(	){return(_layersmgr);};
    virtual bGenericTypesMgr* typesMgr						(	){return(_typesmgr);};
    virtual bGenericViewMgr* viewMgr						(	){return(_viewmgr);};
    virtual bGenericStyleMgr* styleMgr						(	){return(_stylemgr);};
	
    virtual bGenericObjListMgr* selMgr						(	){return(_selmgr);};
    virtual bGenericObjListMgr* cntMgr						(	){return(_cntmgr);};
		
    virtual bGenericLocConverter* locConverter				(	){return(_locConverter);};
					
    virtual bGenericDocument* document						(	){return(_doc);};
	virtual bGenericMapIntf* mapIntf						(	){return(_map_intf);};
	virtual bGenericStatusIntf* statusIntf					(	){return(_status_intf);};
	
	virtual bGenericEventMgr* eventMgr						(	){return(_eventmgr);};
	virtual bGenericMenuMgr* menuMgr						(	){return(_menumgr);};
	virtual bGenericPrintMgr* printMgr						(	){return(_printmgr);};

	virtual bGenericExtMgr* toolMgr							(	){return(_toolmgr);};
	virtual bGenericExtMgr* xmapMgr							(	){return(_xmapmgr);};
	virtual bGenericExtMgr* vdefMgr							(	){return(_vdefmgr);};
	virtual bGenericExtMgr* geogMgr							(	){return(_geogmgr);};
	virtual bGenericExtMgr* calcMgr							(	){return(_calcmgr);};
	virtual bGenericExtMgr* macroMgr						(	){return(_macromgr);};
	virtual bGenericExtMgr* scriptMgr						(	){return(_scriptmgr);};
	virtual bGenericExtMgr* xboxMgr							(	){return(_xboxmgr);};
	virtual bGenericExtMgr* varMgr							(	){return(_varmgr);};
			
// Hors Kit
    virtual OSStatus run                                    (    );
    virtual bool is_running                                 (    ){return _on_run;};

	virtual OSStatus open_doc								(	const char* path,
                                                                void* cc);
	virtual OSStatus close_doc								(	);
	virtual OSStatus create_doc								(	void* cc);

    bool dispatch_cmd										(	UInt32 cmd);

protected:
/*	static OSErr AEOpenHandler								(	const AppleEvent* ae,
																AppleEvent* dmy,
																long prm);
	static OSErr AECloseHandler								(	const AppleEvent* ae,
																AppleEvent* dmy,
																long prm);
	static OSErr AEQuitHandler								(	const AppleEvent* ae,
																AppleEvent* dmy,
																long prm);
	static OSErr AECmdHandler								(	const AppleEvent* ae,
																AppleEvent* dmy,
																long prm);
	static OSErr AECmdParamHandler							(	const AppleEvent* ae,
																AppleEvent* dmy,
																long prm);*/

	static bGenericMacMapApp*		_appinst;
	
	bGenericExtLibMgr*				_extmgr;
    
	bGenericClassesMgr*				_clssmgr;
	bGenericExtMgr*					_toolmgr;
	bGenericExtMgr*					_xmapmgr;
	bGenericExtMgr*					_vdefmgr;
	bGenericExtMgr*					_geogmgr;
	bGenericExtMgr*					_calcmgr;
	bGenericExtMgr*					_macromgr;
	bGenericExtMgr*					_scriptmgr;
	bGenericExtMgr*					_xboxmgr;
	bGenericExtMgr*					_varmgr;
	
	bGenericUnitMgr*				_scalemgr;
	bGenericUnitMgr*				_distmgr;
//	bGenericUnitMgr*				_anglemgr;
//	bGenericUnitMgr*				_timemgr;
	
	bGenericLayersMgr*				_layersmgr;
	bGenericViewMgr*				_viewmgr;
	bGenericStyleMgr*				_stylemgr;
	bGenericTypesMgr*				_typesmgr;
	
	bGenericEventMgr*				_eventmgr;
	bGenericMenuMgr*				_menumgr;
	bGenericPrintMgr*				_printmgr;

	bGenericObjListMgr*				_selmgr;
	bGenericObjListMgr*				_cntmgr;
		
	bGenericLocConverter*			_locConverter;
	
	bGenericMapIntf*				_map_intf;
	bGenericStatusIntf*				_status_intf;
	
	bGenericDocument*				_doc;
    
	OSStatus						_status;

//	bArray							_dae;
	
	bool							_on_run;
	bool							_in_timer;
	bool							_closing;
	bool							_opening;
    
    char*                           _tmppath;
    void*                           _tmpcc;
};

//----------------------------------------------------------------------------

#define _MMAPP_     bMacMapApp::instance()

#define extmgr		bMacMapApp::instance()->extMgr()
#define clssmgr		bMacMapApp::instance()->classMgr()
#define toolmgr		bMacMapApp::instance()->toolMgr()
#define xmapmgr		bMacMapApp::instance()->xmapMgr()
#define vdefmgr		bMacMapApp::instance()->vdefMgr()
#define geogmgr		bMacMapApp::instance()->geogMgr()
#define calcmgr		bMacMapApp::instance()->calcMgr()
#define macromgr	bMacMapApp::instance()->macroMgr()
#define scriptmgr	bMacMapApp::instance()->scriptMgr()
#define xboxmgr		bMacMapApp::instance()->xboxMgr()
#define varmgr		bMacMapApp::instance()->varMgr()

#define scalemgr	bMacMapApp::instance()->scaleMgr()
#define distmgr		bMacMapApp::instance()->distMgr()
#define anglemgr	bMacMapApp::instance()->angleMgr()
#define timemgr		bMacMapApp::instance()->timeMgr()
#define layersmgr	bMacMapApp::instance()->layersMgr()
#define viewmgr		bMacMapApp::instance()->viewMgr()
#define typesmgr	bMacMapApp::instance()->typesMgr()
#define eventmgr	bMacMapApp::instance()->eventMgr()
#define menumgr		bMacMapApp::instance()->menuMgr()
#define printmgr	bMacMapApp::instance()->printMgr()
#define selmgr		bMacMapApp::instance()->selMgr()
#define cntmgr		bMacMapApp::instance()->cntMgr()

#define locconv		bMacMapApp::instance()->locConverter()
#define map_doc		bMacMapApp::instance()->document()

//----------------------------------------------------------------------------


#endif
