 //----------------------------------------------------------------------------
// File : bLayersMgr.h
// Project : MacMap
// Purpose : Header file : Layers management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2003 Carte Blanche Conseil.
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
// 29/08/2014 remove QD, mapwd safe.
// 13/04/2017 merged with LayerAccessContext (removed).
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
	bLayersMgr                          (	);
	virtual ~bLayersMgr                 (	);
	
	virtual int load                    (	);
	virtual int unload                  (	);
	virtual int unload                  (	bool save);

	virtual void AddContext             (	bGenericGraphicContext* ctx);
	virtual void SwitchContext          (	int symbolicName,
                                            void* data);

	virtual void DrawLayers             (	void* wd,
                                            ivx_rect* bounds);

	virtual void StopDraw               (	);
	virtual void UpdateLayers           (	);//av
		
	virtual void DrawSelection          (	CGContextRef ctx,
                                            bGenericGeoElement* o);
	virtual void DrawContrastes         (	CGContextRef ctx,
                                            bGenericGeoElement* o);
	
	virtual void InvalElement           (	bGenericGeoElement* o);

	virtual void ViewChanged            (	);
	virtual void SetObjInvalidation     (	bool b);
	virtual void BackgroundChanged      (	);
    virtual bGenericGraphicContext*
        CurrentGraphicContext           (   ){return _ctx;};

    virtual bool breaked                (	){return !_drawing;};
    virtual bool drawing                (	){return _drawing;};

    
    
    virtual bGenericLayersMgr* clone    (   );
    virtual void cloneDelete            (   bGenericLayersMgr* mgr);

    
    
    virtual bool addlayer				(	void* elt);
    virtual bool addvirtuallayer		(	void* elt);
    
    virtual void settype                (	const char* name);
    virtual void setstyle				(	const char* name);
    virtual void setvisible				(	bool vis);
    virtual void setselectable			(	bool sel);
    
    virtual void setroot				(	void* elt);
    virtual void setarray				(	void* elt);
    virtual void setidentification		(	void* elt);
    
    virtual void flush					(	);
    virtual bool parse					(	int idx);
    virtual int count					(	);
    virtual bGenericStyle* get			(	int idx);
    
    virtual bool set_current			(	int idx);
    virtual int get_current				(	);
    virtual bool move					(	int idx,
                                            int offset);
    virtual bool remove					(	int idx);
    virtual bool add					(	int tidx,
                                            int sidx);
    virtual bool change					(	int idx,
                                            int newidx);

    
    
protected:
	virtual void GetBackground          (	float* color,
                                            int* spc);

    
    virtual void check_on_screen		(	);
    virtual int curview_load            (	);

    
	bGenericGraphicContext* _ctx;
	bool                    _breaked;
	bool                    _drawing;
	bArray                  _ctxs;
	
	bGraphicParams*         _selg;
	bGraphicParams*         _cntg;
    
    bArray*								_elts;
    int									_cursor;
    bGenericXMLBaseElement* 			_curview;
    bGenericXMLBaseElement* 			_array;
    bGenericXMLBaseElement*				_identifier;

private:	

};

//----------------------------------------------------------------------------

#endif
