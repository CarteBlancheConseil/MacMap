//----------------------------------------------------------------------------
// File : bSelectSolver.h
// Project : MacMap
// Purpose : Header file : Class for selection solver (used database and geographic requests)
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
// 08/03/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bSelectSolver__
#define __bSelectSolver__

//----------------------------------------------------------------------------

#include <mox_intf/bStdSolver.h>
#include <mox_intf/bGenericXMLFormulaElement.h>
#include <mox_intf/bStdWait.h>

//----------------------------------------------------------------------------

enum{
	kSelNone,
	kSelReplace,
	kSelAdd,
	kSelRemove
};

enum{
	kSrcNone,
	kSrcSel,
	kSrcCnt,
	kSrcObj
};

enum{
	kAdd2HiddenNone,
	kAdd2Hidden,
	kAdd2HiddenInv
};

//----------------------------------------------------------------------------

class bSelectSolver : public bStdSolver{
public:
    bSelectSolver 								(	void* gapp,
                                                    bool wait);
    virtual ~bSelectSolver 						(	);
    
    virtual void set_select_kind				(	int k);
    virtual int get_select_kind					(	);

    virtual void set_use_masked					(	bool b);
    virtual bool get_use_masked					(	);

    virtual void set_next						(	UInt32 nxt);
    virtual UInt32 get_next						(	);

    virtual void set_add2contrast				(	bool b);
    virtual bool get_add2contrast				(	);

    virtual void set_add2hidden					(	int mode);
    virtual int get_add2hidden					(	);

    virtual void set_preprocess					(	);

    virtual int get_source_kind					(	);

    virtual bArray* get_source					(	);
    virtual bArray* get_processed				(	);
    
    virtual void valid_object					(	bGenericGeoElement* o);		
    virtual void inval_object					(	int idx);
    
    virtual bool exec							(	bGenericXMLBaseElement* elt);
    
protected:
    OSStatus obj_test							(	bGenericGeoElement* o, 
                                                    bool* good);
    static int obj_count						(	void* o, 
                                                    void* prm);
    static int obj_add							(	void* o, 
                                                    void* prm);
    static int obj_comp							(	const void* a, 
                                                    const void* b);
    static int obj_mask							(	void* o, 
                                                    void* prm);
    
private:
    bGenericXMLFormulaElement*	_root;
    bArray*						_objs;
    bArray*						_source;
    long						_ocount;
    int							_selkind;
    int							_srckind;
    bool						_use_masked;
    bool						_pre_process;
    bool						_wait;
    UInt32						_next;
    bool						_add2cnt;
    int							_add2hdn;
};

//----------------------------------------------------------------------------

#endif
