//----------------------------------------------------------------------------
// File : bViewContainer.h
// Project : MacMap
// Purpose : Header file : Views management classes
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
// 02/02/2005 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bViewContainer__
#define __bViewContainer__

//----------------------------------------------------------------------------

#include "bStdContainer.h"

//----------------------------------------------------------------------------

class bViewContainer : public bStdContainer{
public:
    bViewContainer          (	const char* file_name);
    bViewContainer          (	const char* file_name,
                                    bViewContainer& v);
    virtual ~bViewContainer (	);

    virtual bool set_name   (	const char* name);
    virtual bool dump       (	);
    virtual bool destroy    (	);
    
protected:
    virtual void load       (	);

private:
    void make               (	);
};

//----------------------------------------------------------------------------

#endif
