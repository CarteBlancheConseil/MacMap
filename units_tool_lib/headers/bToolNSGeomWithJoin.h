//----------------------------------------------------------------------------
// File : bToolNSGeomWithJoin.h
// Project : MacMap
// Purpose : Header file : Subclass for geometry tools using join functionnalities
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005 Carte Blanche Conseil.
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
// 18/03/2014 Ready for CG geometry	& drawing 
// 28/03/2017 cocoa intf, and renamed from bToolGeomWithJoin to bToolNSGeomWithJoin
//----------------------------------------------------------------------------


#ifndef __bToolNSGeomWithJoin__
#define __bToolNSGeomWithJoin__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolGeom.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

class bToolNSGeomWithJoin : public bStdToolGeom{
public:		
	bToolNSGeomWithJoin						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolNSGeomWithJoin			(	);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
    
    virtual void update						(	bool global);
    virtual void track_obj					(	);
	virtual void track_join					(	);

	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);
	
    bArray& get_types                       (   ){return _tp_use;};
    
    bool get_use_nea                        (   ){return _use_nea;};
    bool get_use_dir                        (   ){return _use_dir;};
    bool get_use_ang                        (   ){return _use_ang;};
    bool get_use_end                        (   ){return _use_end;};
    
    void set_use_nea                        (   bool b){_use_nea=b;};
    void set_use_dir                        (   bool b){_use_dir=b;};
    void set_use_ang                        (   bool b){_use_ang=b;};
    void set_use_end                        (   bool b){_use_end=b;};
    
    char get_use_nea_key                    (   ){return _k_nea;};
    char get_use_dir_key                    (   ){return _k_dir;};
    char get_use_ang_key                    (   ){return _k_ang;};
    char get_use_end_key                    (   ){return _k_end;};

    void set_use_nea_key                    (   char k){_k_nea=k;};
    void set_use_dir_key                    (   char k){_k_dir=k;};
    void set_use_ang_key                    (   char k){_k_ang=k;};
    void set_use_end_key                    (   char k){_k_end=k;};

    bGenericExt* get_nea                    (   ){return _jnea;};
    bGenericExt* get_dir                    (   ){return _jdir;};
    bGenericExt* get_ang                    (   ){return _jang;};
    bGenericExt* get_end                    (   ){return _jend;};

    void putKeys                            (   );

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
    
    bArray			_tp_use;

private:
	static int proc							(	void* o,
												void* prm);
    long            _lsi;
    static bArray   _gwjs;
};

//----------------------------------------------------------------------------

typedef struct proc_prm{
	bToolNSGeomWithJoin*	t;
	i2dvertex               r;
	ivx_rect                b;
	double                  d;
}proc_prm;

//----------------------------------------------------------------------------

#endif
