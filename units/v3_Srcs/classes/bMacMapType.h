//----------------------------------------------------------------------------
// File : bMacMapType.h
// Project : MacMap
// Purpose : Header file : MacMap type class
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
// 13/01/2004 creation.
// 30/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bMacMapType__
#define __bMacMapType__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericGeoIterator.h>
#include <mox_intf/bGenericStyleMgr.h>
#include <MacMapSuite/bStdTable.h>
#include "bMacMapBaseAccessor.h"

//----------------------------------------------------------------------------

class bMacMapType : public bGenericType{
public:
    bMacMapType	 							(	int t, 
                                                int* status);
    bMacMapType	 							(	const char* name, 
                                                const char* path, 
                                                const char* data, 
                                                int hkind,
                                                int ttype,
                                                int* status);
    virtual ~bMacMapType  					(	);
    
    virtual bool load						(	);
    virtual void save						(	);
    
    virtual void pack						(	);
    virtual void name						(	char* name);
    virtual void set_name					(	const char* name);
    virtual double tid						(	);
    virtual int index						(	);
    virtual int kind						(	);
    virtual int nb_rec						(	);
    virtual int nb_live						(	);
    virtual int nb_killed					(	);

    virtual bool load_object				(	int o,	
                                                bool* b);
    virtual bool load_object				(	int o,	
                                                bGenericGeoElement** f);
    virtual bool new_object					(	bGenericGeoElement** f);
    virtual bool clone_object				(	bGenericGeoElement* fin, 
                                                bGenericGeoElement** fout);
    virtual int append						(	);
    virtual bool kill_object				(	bGenericGeoElement* f);
    virtual bool unkill_object				(	bGenericGeoElement* f);
            
    virtual int nb_sel						(	);
    virtual void inc_sel					(	int n);
    
    virtual int nb_cont						(	);
    virtual void inc_cont					(	int n);
    
    virtual void location					(	char* path);
    
    virtual bool is_lock					(	);
    virtual void set_lock					(	bool b);

    virtual bool is_remove					(	);
    virtual void set_remove					(	bool b);
    
    virtual bStdBaseAccessor* fields		(	);
    virtual bGenericGeoIterator* iterator	(	);
    virtual bGenericStyleMgr* styles		(	);
    
    ivx_rect* get_load_area                 (   ){return &_lda;};
    
protected:
    virtual void read_load_area             (   );
    virtual bool make_style					(	);

private:
// en base
    double					_id;
    int						_state;
    int						_kind;
    ivx_rect				_bounds;
    char					_name[256];
    double					_precs;
    double					_unit2m;
    int						_srid;
    
// recalculé
    int						_idx;
    int						_nbalive;
    int						_nbkilled;
    int						_nsel;
    int						_ncnt;
    bool					_rmv;
    
// Autre
    ivx_rect				_lda;
    bMacMapBaseAccessor		_bse;
    bGenericGeoIterator*	_iter;
    bGenericStyleMgr*		_styles;
};

//----------------------------------------------------------------------------

#endif
