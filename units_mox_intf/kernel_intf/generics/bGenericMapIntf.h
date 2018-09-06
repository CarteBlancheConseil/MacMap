//----------------------------------------------------------------------------
// File : bGenericMapIntf.h
// Project : MacMap
// Purpose : Header file : Root class for main window management (or every kind of map interface)
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
// 30/11/2004 creation.
// 13/08/2014 Remplacement de genericwind et genericmapwind dans le cadre de
// la suppression des appels QuickDraw
//----------------------------------------------------------------------------

#ifndef __bGenericMapIntf__
#define __bGenericMapIntf__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <MacMapSuite/vx.h>
//
//----------------------------------------------------------------------------

class bGenericMapIntf : public bGenericElement{
public:		
	virtual ~bGenericMapIntf		(	){};

// Gestion fenêtre
	virtual OSStatus open			(	)=0;
	virtual void close				(	)=0;
				
	virtual CGRect bounds			(	)=0;
	virtual void setBounds			(	CGRect r)=0;
	
	virtual void inval				(	)=0;
	virtual void inval				(	CGRect r)=0;
	
	virtual void* ref				(	)=0;
	
	virtual bool active				(	)=0;

// Gestion MacMap
	virtual void draw				(	)=0;
	virtual void idle				(	)=0;
	virtual void updatePath			(	)=0;
	virtual CGLayerRef getPathLayer	(	)=0;
	
	virtual void screenCenter		(	i2dvertex* vx)=0;
	virtual void screenTopLeft		(	i2dvertex* vx)=0;
	virtual void screenBotLeft		(	i2dvertex* vx)=0;
	virtual void screenBounds		(	ivx_rect* vxr)=0;
	
	virtual void setScreenCenter	(	i2dvertex vx)=0;
	
	virtual void reset				(	)=0;
	virtual long drawCount			(	)=0;
	virtual void registerAction		(	)=0;

};

//----------------------------------------------------------------------------

#endif
