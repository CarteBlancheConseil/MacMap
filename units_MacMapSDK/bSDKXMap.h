//----------------------------------------------------------------------------
// File : bSDKXMap.h
// Project : MacMap
// Purpose : Header file : standard MacMapSDK XMap
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
// 13/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __bSDKXMap__
#define __bSDKXMap__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include "CWXMapWrappers.h"

//----------------------------------------------------------------------------

class bSDKXMap : public bStdXMap{
public:		
    bSDKXMap								(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl,
                                                CWXMap_OpenPtr open,
                                                CWXMap_ClosePtr close,
                                                CWXMap_EventPtr event,
                                                CWXMap_IdlePtr idle,
                                                CWXMap_ProcessPtr process,
                                                CWXMap_TestPtr test,
                                                CWXMap_EditPtr edit,
                                                char* name);
    virtual ~bSDKXMap						(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
            
    virtual void open						(	int* flags);
    virtual void close						(	);
    virtual bool event						(	EventRef evt);
    virtual void idle						(	void* prm);
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool test						(	void* prm);
    virtual bool edit						(	void* prm);
    
protected:

private:
    CWXMap_OpenPtr		_open;
    CWXMap_ClosePtr		_close;
    CWXMap_EventPtr		_event;
    CWXMap_IdlePtr		_idle;
    CWXMap_ProcessPtr	_process;
    CWXMap_TestPtr		_test;
    CWXMap_EditPtr		_edit;
};

//----------------------------------------------------------------------------

#endif
