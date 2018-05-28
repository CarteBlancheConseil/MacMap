//----------------------------------------------------------------------------
// File : bGenericLayersMgr.h
// Project : MacMap
// Purpose : Header file : Root class for layers manager
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

#ifndef __bGenericLayersMgr__
#define __bGenericLayersMgr__

//----------------------------------------------------------------------------

#include <MacMapSuite/bArray.h>
#include <mox_intf/bGenericMgr.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGenericStyle.h>

//----------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define kBackgroundSignature		'CkcB'
#else
#define kBackgroundSignature		'BckC'
#endif

#define kBackgroundConfigName		"color"

//----------------------------------------------------------------------------

typedef struct VectorData{
	char	path[PATH_MAX*3];
	CGRect	bounds;
}VectorData;

typedef struct BitmapData{
	void*			data;
	size_t			width;
	size_t			height;
	size_t			bitsPerComponent;
	size_t			bytesPerRow;
	CGColorSpaceRef colorspace;
	CGBitmapInfo	bitmapInfo;
	CGContextRef	outbm;
}BitmapData;

//----------------------------------------------------------------------------

class bGenericLayersMgr : public bGenericMgr{
public:
	virtual ~bGenericLayersMgr				(	){};
	virtual void AddContext					(	bGenericGraphicContext* ctx)=0;
	virtual void SwitchContext				(	int symbolicName,
												void* data)=0;		
	virtual void DrawLayers					(	void* wd,
												ivx_rect* bounds)=0;
	virtual void StopDraw					(	)=0;
	virtual void UpdateLayers				(	)=0;

	virtual void DrawSelection				(	CGContextRef ctx,
												bGenericGeoElement* o)=0;
	virtual void DrawContrastes				(	CGContextRef ctx,
												bGenericGeoElement* o)=0;

	virtual void InvalElement				(	bGenericGeoElement* o)=0;
	
	virtual void ViewChanged				(	)=0;
	virtual void SetObjInvalidation			(	bool b)=0;
	virtual void BackgroundChanged			(	)=0;
    virtual bGenericGraphicContext*
        CurrentGraphicContext               (   )=0;

    virtual bool breaked					(	)=0;
    virtual bool drawing					(	)=0;

    
    
    
    
    virtual bGenericLayersMgr* clone        (   )=0;
    virtual void cloneDelete                (   bGenericLayersMgr* mgr)=0;
    
    
    
    
    
    virtual bool addlayer						(	void* elt)=0;
    virtual bool addvirtuallayer				(	void* elt)=0;
    
    virtual void settype						(	const char* name)=0;
    virtual void setstyle						(	const char* name)=0;
    virtual void setvisible						(	bool vis)=0;
    virtual void setselectable					(	bool sel)=0;
    
    virtual void setroot						(	void* elt)=0;
    virtual void setarray						(	void* elt)=0;
    virtual void setidentification				(	void* elt)=0;
    
    virtual void flush							(	)=0;
    virtual bool parse							(	int idx)=0;
    virtual int count							(	)=0;
    virtual bGenericStyle* get					(	int idx)=0;
    
    virtual bool set_current					(	int idx)=0;
    virtual int get_current						(	)=0;
    virtual bool move							(	int idx,
                                                    int offset)=0;
    virtual bool remove							(	int idx)=0;
    virtual bool add							(	int tidx,
                                                    int sidx)=0;
    virtual bool change							(	int idx, 
                                                    int newidx)=0;

};

//----------------------------------------------------------------------------

#endif
