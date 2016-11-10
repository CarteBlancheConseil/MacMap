 //----------------------------------------------------------------------------
// File : bLayersMgr.h
// Project : MacMap
// Purpose : Header file : Layers management class
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
// 30/05/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bLayersMgr__
#define __bLayersMgr__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericLayersMgr.h>
#include <mox_intf/bGenericXMLLayerElement.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGraphicParams.h>

//----------------------------------------------------------------------------

class bLayersMgr : public bGenericLayersMgr{
public:
	bLayersMgr 								(	);
	virtual ~bLayersMgr  					(	);
	
	virtual int load						(	);
	virtual int unload						(	);
	virtual int unload						(	bool save);

	virtual void AddContext					(	bGenericGraphicContext* ctx);
	virtual void SwitchContext				(	int symbolicName,
												void* data);

	virtual void DrawLayers					(	void* wd,
												ivx_rect* bounds);

	virtual void StopDraw					(	);
	virtual void UpdateLayers				(	);//av
		
	virtual void DrawSelection				(	CGContextRef ctx,
												bGenericGeoElement* o);
	virtual void DrawContrastes				(	CGContextRef ctx,
												bGenericGeoElement* o);
	
	virtual void InvalElement				(	bGenericGeoElement* o);

	virtual void ViewChanged				(	);
	virtual void SetObjInvalidation			(	bool b);
	virtual void BackgroundChanged			(	);
    virtual bGenericGraphicContext*
        CurrentGraphicContext               (   ){return _ctx;};

    virtual bool breaked					(	){return !_drawing;};
    virtual bool drawing					(	){return _drawing;};

protected:
	virtual void GetBackground				(	float* color, 
												int* spc);

	bGenericGraphicContext*		_ctx;
	bool						_breaked;
	bool						_drawing;
	bArray						_ctxs;	
	
	bGraphicParams*				_selg;
	bGraphicParams*				_cntg;
	
private:	

};

//----------------------------------------------------------------------------

#endif
