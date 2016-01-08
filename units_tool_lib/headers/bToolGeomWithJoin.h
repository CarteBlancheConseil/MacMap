//----------------------------------------------------------------------------
// File : bToolGeomWithJoin.h
// Project : MacMap
// Purpose : Header file : Subclass for geometry tools using join functionnalities
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
// 31/03/2005 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------


#ifndef __bToolGeomWithJoin__
#define __bToolGeomWithJoin__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolGeom.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

enum{
	kToolGeomWithJoinSign				='tGwJ',
	kToolGeomWithJoinUseNeaID			=5,
	kToolGeomWithJoinUseNeaCmd			='cour',
	kToolGeomWithJoinUseDirID			=6,
	kToolGeomWithJoinUseDirCmd			='dire',
	kToolGeomWithJoinUseAngID			=7,
	kToolGeomWithJoinUseAngCmd			='angu',
	kToolGeomWithJoinUseEndID			=8,
	kToolGeomWithJoinUseEndCmd			='extr',

	kToolGeomWithJoinKeyNeaID			=15,
	kToolGeomWithJoinKeyDirID			=16,
	kToolGeomWithJoinKeyAngID			=17,
	kToolGeomWithJoinKeyEndID			=18,
	
	kToolGeomWithJoinTypesID			=9,
	kToolGeomWithJoinTypesCmd			='typs',
	kToolGeomWithJoinTypesCheckProperty	='chck',
	kToolGeomWithJoinTypesNameProperty	='name'
};

//----------------------------------------------------------------------------

class bToolGeomWithJoin : public bStdToolGeom{
public:		
	bToolGeomWithJoin						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolGeomWithJoin				(	);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
    
    virtual void update						(	bool global);
    virtual void track_obj					(	);
	virtual void track_join					(	);
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);
	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);
	
protected:
	virtual void init_join					(	);
	virtual bool get_join					(	i2dvertex* res);
	virtual bool get_join					(	int tidx, 
												i2dvertex* res, 
												double* d);
	virtual bool get_join					(	bGenericGeoElement* o, 
												i2dvertex* res, 
												double* d);
	virtual double get_dmax					(	);


	bGenericExt*	_jnea;
	bGenericExt*	_jdir;
	bGenericExt*	_jang;
	bGenericExt*	_jend;
	
	bool			_use_nea;
	bool			_use_dir;
	bool			_use_ang;
	bool			_use_end;
	
	char			_k_nea;
	char			_k_dir;
	char			_k_ang;
	char			_k_end;
	
private:
	static int proc							(	void* o,
												void* prm);
	static pascal OSStatus dtb_proc			(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue);
	virtual void populate_types				(	WindowRef wd);
    void putKeys                            (   char k_nea,
                                                char k_dir,
                                                char k_ang,
                                                char k_end);
	bArray			_tp_use;
    long            _lsi;
    static bArray   _gwjs;
};

//----------------------------------------------------------------------------

typedef struct proc_prm{
	bToolGeomWithJoin*	t;
	i2dvertex			r;
	ivx_rect			b;
	double				d;
}proc_prm;

//----------------------------------------------------------------------------

#endif
