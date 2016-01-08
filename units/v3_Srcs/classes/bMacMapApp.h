//----------------------------------------------------------------------------
// File : bMacMapApp.h
// Project : MacMap
// Purpose : Header file : Application main class
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
// 20/11/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bMacMapApp__
#define __bMacMapApp__

//----------------------------------------------------------------------------

#include "mm_config.h"
#include <mox_intf/bGenericMacMapApp.h>

#include <mox_intf/bGenericMapIntf.h>

#include <mox_intf/bGenericStyleMgr.h>

#include <mox_intf/mm_errors.h>
#include <mox_intf/mm_messages.h>
#include <MacMapSuite/bTrace.h>

//----------------------------------------------------------------------------

enum{
	kMacMapEventClass				='MapT',
	kAEMacMapSendCommand			='SenD',
	kAEMacMapSendCommandWithParam	='SenP'
};

//----------------------------------------------------------------------------

class bMacMapApp : public bGenericMacMapApp{
public:
	bMacMapApp	 											(	const char* app_name);
	virtual ~bMacMapApp  									(	);

	static bGenericMacMapApp* instance						(	const char* app_name);
	static bGenericMacMapApp* instance						(	);
	
	virtual bGenericExtLibMgr* extMgr						(	);
	virtual bGenericClassesMgr* classMgr					(	);
	
	virtual bGenericUnitMgr* scaleMgr						(	);
	virtual bGenericUnitMgr* distMgr						(	);
	virtual bGenericUnitMgr* angleMgr						(	);
	virtual bGenericUnitMgr* timeMgr						(	);
	
	virtual bGenericLayersMgr* layersMgr					(	);
	virtual bGenericTypesMgr* typesMgr						(	);
	virtual bGenericViewMgr* viewMgr						(	);
	virtual bGenericStyleMgr* styleMgr						(	);
	
	virtual bGenericObjListMgr* selMgr						(	);
	virtual bGenericObjListMgr* cntMgr						(	);
	
	virtual bGenericLayerAccessContext* layersAccessCtx		(	);
	
	virtual bGenericLocConverter* locConverter				(	);
					
	virtual bGenericDocument* document						(	);
	virtual bGenericMapIntf* mapIntf						(	){return _map_intf;};
	virtual bGenericStatusIntf* statusIntf					(	){return _status_intf;};
	
	virtual bGenericEventMgr* eventMgr						(	);
	virtual bGenericMenuMgr* menuMgr						(	);
	virtual bGenericPrintMgr* printMgr						(	);

	virtual bGenericExtMgr* toolMgr							(	);
	virtual bGenericExtMgr* xmapMgr							(	);		
	virtual bGenericExtMgr* vdefMgr							(	);	
	virtual bGenericExtMgr* geogMgr							(	);	
	virtual bGenericExtMgr* calcMgr							(	);
	virtual bGenericExtMgr* macroMgr						(	);
	virtual bGenericExtMgr* scriptMgr						(	);
	virtual bGenericExtMgr* xboxMgr							(	);
	virtual bGenericExtMgr* varMgr							(	);
			
// Hors Kit
	virtual OSStatus run									(	);
	virtual IBNibRef appnibs								(	);
	
	virtual OSStatus open_doc								(	);
	virtual OSStatus open_doc								(	const char* path);
	virtual OSStatus close_doc								(	);
	virtual OSStatus create_doc								(	);
	
protected:
	
	OSStatus nibinit										(	);
	void crash												(	int idx);
	
	static OSStatus appEvtHandler							(	EventHandlerCallRef hdlr, 
																EventRef evt, 
																void *up);
	static void	timer_proc									(	EventLoopTimerRef timer, 
																void *up);
																
	bool dispatch_cmd										(	UInt32 cmd);
	void dispatch_evt										(	EventRef evt);
	



	static OSErr AEOpenHandler								(	const AppleEvent* ae,
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
																long prm);

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
	bGenericUnitMgr*				_anglemgr;
	bGenericUnitMgr*				_timemgr;
	
	bGenericLayersMgr*				_layersmgr;
	bGenericViewMgr*				_viewmgr;
	bGenericStyleMgr*				_stylemgr;
	bGenericTypesMgr*				_typesmgr;
	
	bGenericEventMgr*				_eventmgr;
	bGenericMenuMgr*				_menumgr;
	bGenericPrintMgr*				_printmgr;

	bGenericObjListMgr*				_selmgr;
	bGenericObjListMgr*				_cntmgr;
	
	bGenericLayerAccessContext*		_layersaccessctx;
	
	bGenericLocConverter*			_locConverter;
	
	bGenericMapIntf*				_map_intf;
	bGenericStatusIntf*				_status_intf;
	
	bGenericDocument*				_doc;
					
	IBNibRef						_nibs;
	CFBundleRef 					_mainbundle;
			
	OSStatus						_status;

	bTrace							_trc;
	bArray							_dae;
	
	EventLoopTimerRef				_tmr;
	
	bool							_on_run;
	bool							_in_timer;
	bool							_closing;
	bool							_opening;
	
};

//----------------------------------------------------------------------------

#define _MMAPP_	bMacMapApp::instance()
#define _appnibs	((bMacMapApp*)bMacMapApp::instance())->appnibs()

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
