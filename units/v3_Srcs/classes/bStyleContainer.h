//----------------------------------------------------------------------------
// File : bStyleContainer.h
// Project : MacMap
// Purpose : Header file : Styles management classes
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
// 07/02/2005 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bStyleContainer__
#define __bStyleContainer__

//----------------------------------------------------------------------------

#include "bStdContainer.h"
#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------

class bStdStyleContainer : public bStdContainer{
public:
    bStdStyleContainer          (	const char* file_name);
    bStdStyleContainer          (	const char* file_name,
                                    bStdStyleContainer& s);
    bStdStyleContainer          (	const char* file_name,
                                    bGenericXMLBaseElement* root);

    virtual ~bStdStyleContainer (	);

    virtual bool set_name       (	const char* name);
    virtual bool dump           (	);
    virtual bool destroy        (	);

    virtual void set_root       (	bGenericXMLBaseElement* root);
    virtual int signature       (	);
    virtual bool save           (	);
    virtual bool on_screen      (	);
    virtual void set_on_screen  (	bool b);
    virtual bool on_edit        (	);
    virtual void set_on_edit    (	bool b);
    
protected:

private:
    bool    _on_screen;
    bool    _on_edit;
};

//----------------------------------------------------------------------------

class bStyleContainer : public bStdStyleContainer{
public:
    bStyleContainer             (	const char* file_name,
                                    bGenericType* tp);
    bStyleContainer             (	const char* file_name,
                                    bStdStyleContainer& s);
    bStyleContainer             (	const char* file_name,
                                    bGenericXMLBaseElement* root,
                                    bGenericType* tp);

    virtual ~bStyleContainer    (	);
    
    virtual bool set_name       (	const char* name);
    virtual bool dump           (	);
    virtual bool destroy        (	);

protected:
    virtual void load           (	);

private:
    bGenericType*	_tp;
};

//----------------------------------------------------------------------------

class bVirtualStyleContainer : public bStdStyleContainer{
public:
    bVirtualStyleContainer          (	const char* file_name);
    bVirtualStyleContainer          (	const char* file_name,
                                        bStdStyleContainer& s);
    bVirtualStyleContainer          (	const char* file_name,
                                        bGenericXMLBaseElement* root);

    virtual ~bVirtualStyleContainer (	);

    virtual bool set_name           (	const char* name);
    virtual bool dump               (	);
    virtual bool destroy            (	);

protected:
    virtual void load               (	);

private:

};

//----------------------------------------------------------------------------

#endif
