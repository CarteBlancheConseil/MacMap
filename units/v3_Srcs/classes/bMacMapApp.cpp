//----------------------------------------------------------------------------
// File : bMacMapApp.cpp
// Project : MacMap
// Purpose : C++ source file : Application main class
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


#include "bMacMapApp.h"
#include "mm_config.h"
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
#include "bMacMapLayerAccessContext.h"

#include "bCGLocConverter.h"

#include "bMacMapDocument.h"
#include "CreateDoc.h"
#include "bDelayedAE.h"

#include <mox_intf/AppSignaturesUTF8.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/NSOpenSavePanelWrappers.h>
#include <mox_intf/bStdAlert.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/endian.h>
#include <mox_intf/bGenericXMLBaseElement.h>

#include <loader/loader.h>

#include <MacMapSuite/MacMapSuiteCPP.h>
#include <MacMapSuite/bStdDirectory.h>

#include <WebKit/WebKit.h>
#include <WebKit/HIWebView.h>
#include <WebKit/CarbonUtils.h>


// ---------------------------------------------------------------------------
// 
// ------------
OSStatus appRegisterHelpBook	(void);
OSStatus appGoToHelpAnchor		(CFStringRef an);

// ---------------------------------------------------------------------------
// Globale
// ------------
bGenericMacMapApp* bMacMapApp::_appinst=NULL;

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapApp	::bMacMapApp(const char* app_name)
			:_trc(app_name,false)
			,_dae(sizeof(bDelayedAE*)){

/**/
	(void)NSApplicationLoad();// Pour intŽgration de cocoa intf
/**/

	_nibs=NULL;
	_mainbundle=NULL;
	
	_status=noErr;
	
	if(nibinit()){
		crash(_bMacMapAppNibInitErr_);
		return;
	}

	appRegisterHelpBook();

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
	_anglemgr			=NULL;
	_timemgr			=NULL;
	
	_layersmgr			=new bLayersMgr();
	_viewmgr			=new bViewMgr();
	_stylemgr			=new bStyleMgr(NULL,&status);
	_typesmgr			=new bTypesMgr();
	_eventmgr			=new bMacMapEventMgr();
	_menumgr			=new bMenuMgr();
	_printmgr			=new bPrintMgr();

	_selmgr				=new bSelectionMgr();
	_cntmgr				=new bContrastesMgr();
	
	_layersaccessctx	=new bMacMapLayerAccessContext();
	
	_locConverter		=new bCGLocConverter();
	
	_map_intf			=NULL;// CrŽŽ ˆ run
	_status_intf		=NULL;// CrŽŽ ˆ run
				
	_doc				=NULL;// CrŽŽ ˆ load
	
	_on_run				=false;
	_in_timer			=false;
	
	_closing=false;
	_opening=false;
	
// Apple events handlers
	AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,AEOpenHandler,(long)this,false);
	AEInstallEventHandler(kAECoreSuite,kAEClose,AECloseHandler,(long)this,false);
	AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,AEQuitHandler,(long)this,false);

//	
	AEInstallEventHandler(kMacMapEventClass,kAEMacMapSendCommand,AECmdHandler,(long)this,false);
	AEInstallEventHandler(kMacMapEventClass,kAEMacMapSendCommandWithParam,AECmdParamHandler,(long)this,false);

bString	str("MacMap app inited\n");
_trc.msg(str);
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapApp::~bMacMapApp(){
	if(_nibs){
	}
	if(_mainbundle){
	}
	
	delete _cntmgr;
	delete _selmgr;
	delete _scalemgr;
	delete _typesmgr;
	delete _layersmgr;
	delete _eventmgr;

	delete _clssmgr;
	
	delete _layersaccessctx;
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
// Accs globale
// ------------
bGenericMacMapApp* bMacMapApp::instance(){
	return(_appinst);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtLibMgr* bMacMapApp::extMgr(){
	return(_extmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericClassesMgr* bMacMapApp::classMgr(){
	return(_clssmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::toolMgr(){
	return(_toolmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericLayersMgr* bMacMapApp::layersMgr(){
	return(_layersmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericUnitMgr* bMacMapApp::scaleMgr(){
	return(_scalemgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericUnitMgr* bMacMapApp::distMgr(){
	return(_distmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericUnitMgr* bMacMapApp::angleMgr(){
	return(_anglemgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericUnitMgr* bMacMapApp::timeMgr(){
	return(_timemgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericTypesMgr* bMacMapApp::typesMgr(){
	return(_typesmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericObjListMgr* bMacMapApp::selMgr(){
	return(_selmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericObjListMgr* bMacMapApp::cntMgr(){
	return(_cntmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericLayerAccessContext* bMacMapApp::layersAccessCtx(){
	return(_layersaccessctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericLocConverter* bMacMapApp::locConverter(){
	return(_locConverter);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericDocument* bMacMapApp::document(){
	return(_doc);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::xmapMgr(){
	return(_xmapmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::vdefMgr(){
	return(_vdefmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::geogMgr(){
	return(_geogmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::calcMgr(){
	return(_calcmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::macroMgr(){
	return(_macromgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::scriptMgr(){
	return(_scriptmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::xboxMgr(){
	return(_xboxmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericExtMgr* bMacMapApp::varMgr(){
	return(_varmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericViewMgr* bMacMapApp::viewMgr(){
	return(_viewmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericStyleMgr* bMacMapApp::styleMgr(){
	return(_stylemgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericEventMgr* bMacMapApp::eventMgr(){
	return(_eventmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericMenuMgr* bMacMapApp::menuMgr(){
	return(_menumgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericPrintMgr* bMacMapApp::printMgr(){
	return(_printmgr);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::open_doc(){
_bTrace_("bMacMapApp::open_doc()",true);
OSStatus		status=noErr;
char			path[PATH_MAX*3];
const char*		nsft[3]={kMacMapProjectExt1,kMacMapProjectExt2,kMacMapProjectExt3};
const char**	pnsft=nsft;
	
	_opening=true;
	
	for(;;){
		if(_doc){
			status=-1;
_te_("document already open");
			_opening=false;
			return(status);
		}
		
		if(NSGetFilePath(pnsft,
						 3,
						 cNSUTF8StringEncoding,
						 false,
                         NULL,
						 path)==cNSCancelButton){
			break;
		}

		_doc=new bMacMapDocument();
		if((status=_doc->load(path))){
_te_("_doc->load = "+status);
			break;
		}
		
		if(_map_intf){
			_map_intf->open();
		}
		
		break;
	}
	if(status&&_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
	}

	_opening=false;

	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::open_doc(const char* path){
_bTrace_("bMacMapApp::open_doc(const char*)",true);
OSStatus	status=noErr;
	
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
			_map_intf->open();
		}

		break;
	}
	if(status&&_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
	}
	
	_opening=false;
	
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

	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::create_doc(){
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
			_map_intf->open();
		}
		
		break;
	}
	if(status&&_doc){
		_doc->unload();
		delete (bMacMapDocument*)_doc;
		_doc=NULL;
	}

	_opening=false;

	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::nibinit(){		
_bTrace_("bMacMapApp::nibinit",true);
	_mainbundle=CFBundleGetMainBundle();
	if(_mainbundle!=NULL){
		_status=CreateNibReferenceWithCFBundle(_mainbundle,CFSTR(kMainNibFileName),&_nibs);
		if((_status==noErr)&&(_nibs==NULL)){
_te_(_status+" at CreateNibReferenceWithCFBundle()");
			_status=-2;
		}
		else{
		}
	}
	else{
		_status=-1;
_te_(_status+" at CFBundleGetMainBundle()");
	}
_tm_("nib inited");
	return(_status);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapApp::crash(int idx){
_bTrace_("bMacMapApp::crash",false);
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
	error_string(error_num(_bMacMapAppErr_,idx),msg);
_te_(idx+" : "+msg);
bAlertStop alrt(msg,"");
	exit(error_num(_bMacMapAppErr_,idx));
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::run(){
_bTrace_("bMacMapApp::run",true);
const EventTypeSpec	kAppEvents[]={	{kEventClassCommand,kEventCommandProcess},
										
									{kEventClassMacMap,kEventMacMapDataBase},
	
									{kEventClassMenu,kEventMenuBeginTracking},
									{kEventClassMenu,kEventMenuOpening},
									{kEventClassMenu,kEventMenuClosed},
									{kEventClassMenu,kEventMenuEnableItems} };
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
	
	if((status=InstallApplicationEventHandler(	appEvtHandler,
												GetEventTypeCount(kAppEvents),
												kAppEvents,
												this,
												NULL))){
_te_("InstallApplicationEventHandler="+status);										
		return(status);
	}
	
	_on_run=true;
	
	if((status=InstallEventLoopTimer(GetCurrentEventLoop(),0,0,timer_proc,this,&_tmr))){
_te_("InstallEventLoopTimer="+status);										
		return(status);
	}
	if(_status_intf){
		_status_intf->init();
	}
	    
	RunApplicationEventLoop();// Et c'est parti... youp la boum !
	
	_on_run=false;
	
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// ------------
IBNibRef bMacMapApp::appnibs(){
	return(_nibs);
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bMacMapApp::appEvtHandler(	EventHandlerCallRef hdlr, 
									EventRef evt, 
									void *up){
_bTrace_("bMacMapApp::appEvtHandler",true);
OSStatus	status=eventNotHandledErr;
UInt32		clss;
HICommand	cmd;
bMacMapApp*	app=(bMacMapApp*)up;
	
	clss=GetEventClass(evt);
	if(clss==kEventClassMacMap){
		app->dispatch_evt(evt);
		status=noErr;
	}
	else if(clss==kEventClassCommand){
_tm_("kEventClassCommand");
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandAbout:
                Loader_SplashAlloc(app);
                Loader_SplashRelease();
				status=noErr;
				break;
			case kHICommandQuit:
_tm_("kHICommandQuit");
				if((!app->_closing)&&(!app->_opening)){
					if(app->_doc!=NULL){
						if(app->_map_intf){
							app->_map_intf->close();
						}
						else{
							app->close_doc();
						}
					}
					app->_on_run=false;
					QuitApplicationEventLoop();
				}
				status=noErr;
				break;
			case kHICommandNew:
				if((!app->_closing)&&(!app->_opening)){
					app->create_doc();
				}
				break;
			case kHICommandOpen:
				if((!app->_closing)&&(!app->_opening)){
					app->open_doc();
				}
				status=noErr;
				break;
			case kHICommandClose:// Normalement on ne devrait plus arriver ici
_tw_("kHICommandClose");
				if((!app->_closing)&&(!app->_opening)){
					if(app->_map_intf){
						app->_map_intf->close();
					}
					else{
						app->close_doc();
					}
				}
				status=noErr;
				break;
			case kHICommandAppHelp:
				appGoToHelpAnchor(NULL);
				break;
			default:
				if(app->dispatch_cmd(cmd.commandID)){
					status=noErr;
				}
				break;
		}
	}
	else if(clss==kEventClassMenu){
//_tm_("kEventClassMenu");
		app->menuMgr()->track(evt);		
	}
	else if(clss==kEventClassKeyboard){
//_tm_("keyboard");
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapApp::timer_proc(	EventLoopTimerRef timer, 
								void *up){
_bTrace_("bMacMapApp::timer_proc",true);
bMacMapApp*		gapp=(bMacMapApp*)up;
OSStatus		status;
EventRef		evt;
EventTargetRef	tgt=GetEventDispatcherTarget();
int				i;
bDelayedAE*		dae;

	gapp->_in_timer=true;

	do{
	    status=ReceiveNextEvent(0,NULL,kEventDurationSecond,true,&evt);
		if(status==noErr){
			SendEventToEventTarget(evt,tgt);
	        ReleaseEvent(evt);
	    }
	    else if(status==eventLoopTimedOutErr){
//	       status=noErr;
	    }
		if(gapp->_doc){
	        IdleApp();
			if(gapp->_map_intf){
				gapp->_map_intf->idle();
			}
			gapp->_toolmgr->idle();
			gapp->_xmapmgr->idle();
			gapp->_vdefmgr->idle();
			gapp->_geogmgr->idle();
			gapp->_calcmgr->idle();
			gapp->_varmgr->idle();
			gapp->_macromgr->idle();
			gapp->_scriptmgr->idle();
			gapp->_xboxmgr->idle();
		}
		for(i=1;i<=gapp->_dae.count();i++){
			gapp->_dae.get(i,&dae);
			dae->exec();
			delete dae;
		}
		gapp->_dae.reset();
	}
	while(gapp->_on_run);
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

// ---------------------------------------------------------------------------
// 
// -----------
void bMacMapApp::dispatch_evt(EventRef evt){
	if(_toolmgr->event(evt)){
	}
	if(_xmapmgr->event(evt)){
	}
	if(_xboxmgr->event(evt)){
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
OSErr bMacMapApp::AEOpenHandler(	const AppleEvent* ae,
									AppleEvent* dmy,
									long prm){
_bTrace_("bMacMapApp::AEOpenHandler",true);
bMacMapApp*	gapp=(bMacMapApp*)prm;
	if(!gapp->_in_timer){
bDelayedAEOpenWithPath*	dae=new bDelayedAEOpenWithPath(ae,gapp);
		gapp->_dae.add(&dae);
		return(noErr);
	}
bDelayedAEOpenWithPath	dae(ae,gapp);
	return(dae.exec(dmy));
}

// ---------------------------------------------------------------------------
// 
// -----------
OSErr bMacMapApp::AECloseHandler(	const AppleEvent* ae,
									AppleEvent* dmy,
									long prm){
_bTrace_("bMacMapApp::AECloseHandler",true);
bMacMapApp*	gapp=(bMacMapApp*)prm;
	if(!gapp->_in_timer){
bDelayedAEClose*	dae=new bDelayedAEClose(ae,gapp);
		gapp->_dae.add(&dae);
		return(noErr);
	}
bDelayedAEClose	dae(ae,gapp);
	return(dae.exec(dmy));
}

// ---------------------------------------------------------------------------
// 
// -----------
OSErr bMacMapApp::AEQuitHandler(	const AppleEvent* ae,
									AppleEvent* dmy,
									long prm){
_bTrace_("bMacMapApp::AEQuitHandler",true);
bMacMapApp*	gapp=(bMacMapApp*)prm;
	if(!gapp->_in_timer){
bDelayedAEQuit*	dae=new bDelayedAEQuit(ae,gapp);
		gapp->_dae.add(&dae);
		return(noErr);
	}
bDelayedAEQuit	dae(ae,gapp);
	return(dae.exec(dmy));
}

// ---------------------------------------------------------------------------
// 
// -----------
OSErr bMacMapApp::AECmdHandler(	const AppleEvent* ae,
								AppleEvent* dmy,
								long prm){
_bTrace_("bMacMapApp::AECmdHandler",true);
bMacMapApp*	gapp=(bMacMapApp*)prm;
	if(!gapp->_in_timer){
bDelayedAECmd*	dae=new bDelayedAECmd(ae,gapp);
		gapp->_dae.add(&dae);
		return(noErr);
	}
bDelayedAECmd	dae(ae,gapp);
	return(dae.exec(dmy));
}

// ---------------------------------------------------------------------------
// 
// -----------
OSErr bMacMapApp::AECmdParamHandler(	const AppleEvent* ae,
										AppleEvent* dmy,
										long prm){
_bTrace_("bMacMapApp::AECmdParamHandler",true);
bMacMapApp*	gapp=(bMacMapApp*)prm;
	if(!gapp->_in_timer){
bDelayedAECmdParam*	dae=new bDelayedAECmdParam(ae,gapp);
		gapp->_dae.add(&dae);
		return(noErr);
	}
bDelayedAECmdParam	dae(ae,gapp);
	return(dae.exec(dmy));
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus appRegisterHelpBook(void){
_bTrace_("appRegisterHelpBook",false);
CFBundleRef	bndl=NULL;
CFURLRef	url=NULL;
FSRef		ref;
OSStatus	err=noErr;

    bndl=CFBundleGetMainBundle();
    if(bndl==NULL){
_te_("CFBundleGetMainBundle");
		return(fnfErr);
	}
	url=CFBundleCopyBundleURL(bndl);
	if(url==NULL){
_te_("CFBundleCopyBundleURL");
		return(fnfErr);
	}
    if(!CFURLGetFSRef(url,&ref)){
_te_("CFURLGetFSRef");
		CFRelease(url);
		return(fnfErr);
	}
	CFRelease(url);
    err=AHRegisterHelpBook(&ref);
	if(err){
_te_("AHRegisterHelpBook");
	}
    return(err);
 
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus appGoToHelpAnchor(CFStringRef an){
_bTrace_("appGoToHelpAnchor",false);
OSStatus	err;
CFBundleRef	bndl=NULL;
CFTypeRef	bkn=NULL;

	bndl=CFBundleGetMainBundle();
	if(bndl==NULL){
_te_("CFBundleGetMainBundle");
		return(fnfErr);
	}
	bkn=CFBundleGetValueForInfoDictionaryKey(bndl,CFSTR("CFBundleHelpBookName"));
	if(bndl==NULL){
_te_("CFBundleGetValueForInfoDictionaryKey");
		return(fnfErr);
	}
	if(CFGetTypeID(bkn)!=CFStringGetTypeID()){
_te_("CFGetTypeID");
		return(paramErr);
	}
	if(an!=NULL){
		err=AHLookupAnchor((CFStringRef)bkn,an);
	}
	else{
		err=AHGotoPage((CFStringRef)bkn,NULL,NULL);
	}
	if(err){
_te_("AHGotoPage");
	}
	return(err);
}
