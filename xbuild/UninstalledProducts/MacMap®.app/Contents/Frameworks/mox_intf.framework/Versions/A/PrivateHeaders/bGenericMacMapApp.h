//----------------------------------------------------------------------------
// File : bGenericMacMapApp.h
// Project : MacMap
// Purpose : Header file : Root class for MacMap main application class
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
// 13/04/2017 LayerAccessContext removed.
//----------------------------------------------------------------------------

#ifndef __bGenericMacMapApp__
#define __bGenericMacMapApp__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericExtLibMgr.h>
#include <mox_intf/bGenericClassesMgr.h>
#include <mox_intf/bGenericExtMgr.h>
#include <mox_intf/bGenericMenuMgr.h>
#include <mox_intf/bGenericUnitMgr.h>
#include <mox_intf/bGenericLayersMgr.h>
#include <mox_intf/bGenericViewMgr.h>
#include <mox_intf/bGenericTypesMgr.h>
#include <mox_intf/bGenericObjListMgr.h>
#include <mox_intf/bGenericLocConverter.h>
#include <mox_intf/bGenericDocument.h>
#include <mox_intf/bGenericMapIntf.h>
#include <mox_intf/bGenericStatusIntf.h>
#include <mox_intf/bGenericEventMgr.h>
#include <mox_intf/bGenericPrintMgr.h>
#include <mox_intf/AppSignatures.h>
#include <MacMapSuite/vx_lib.h>

//----------------------------------------------------------------------------

class bGenericMacMapApp : public bGenericElement{
public:
	virtual bGenericExtLibMgr* extMgr						(	)=0;
	virtual bGenericClassesMgr* classMgr					(	)=0;
	
	virtual bGenericUnitMgr* scaleMgr						(	)=0;
	virtual bGenericUnitMgr* distMgr						(	)=0;
	virtual bGenericUnitMgr* angleMgr						(	)=0;
	virtual bGenericUnitMgr* timeMgr						(	)=0;
	
	virtual bGenericLayersMgr* layersMgr					(	)=0;
	virtual bGenericTypesMgr* typesMgr						(	)=0;
	virtual bGenericViewMgr* viewMgr						(	)=0;
	
	virtual bGenericObjListMgr* selMgr						(	)=0;
	virtual bGenericObjListMgr* cntMgr						(	)=0;
		
	virtual bGenericLocConverter* locConverter				(	)=0;
			
	virtual bGenericDocument* document						(	)=0;
	virtual bGenericMapIntf* mapIntf						(	)=0;
	virtual bGenericStatusIntf* statusIntf					(	)=0;

	virtual bGenericEventMgr* eventMgr						(	)=0;
	virtual bGenericMenuMgr* menuMgr						(	)=0;
	virtual bGenericPrintMgr* printMgr						(	)=0;

	virtual bGenericExtMgr* toolMgr							(	)=0;
	virtual bGenericExtMgr* xmapMgr							(	)=0;		
	virtual bGenericExtMgr* vdefMgr							(	)=0;
	virtual bGenericExtMgr* geogMgr							(	)=0;
	virtual bGenericExtMgr* calcMgr							(	)=0;
	virtual bGenericExtMgr* macroMgr						(	)=0;
	virtual bGenericExtMgr* scriptMgr						(	)=0;
	virtual bGenericExtMgr* xboxMgr							(	)=0;
	virtual bGenericExtMgr* varMgr							(	)=0;

	virtual OSStatus close_doc								(	)=0;

protected:
	virtual ~bGenericMacMapApp								(	){};
};

//----------------------------------------------------------------------------

#endif
