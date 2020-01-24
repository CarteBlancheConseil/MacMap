//----------------------------------------------------------------------------
// File : bXMapMgr.h
// Project : MacMap
// Purpose : Header file : XMaps management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2005-2018 Carte Blanche Conseil.
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
// 18/01/2005 creation.
// 09/11/2018 64Bits
//----------------------------------------------------------------------------

#ifndef __bXMapMgr__
#define __bXMapMgr__

//----------------------------------------------------------------------------

#include "bStdExtMgr.h"

//----------------------------------------------------------------------------

class bXMapMgr : public bStdExtMgr{
public:		
	bXMapMgr				(	);
	virtual ~bXMapMgr		(	);
	
	virtual void open		(	long idx);
	virtual void close		(	);
	
    virtual bool event		(	void* nsevt);
	virtual void idle		(	);
	
protected:

private:
	bArray			_evts;
	bArray			_idle;
};

//----------------------------------------------------------------------------

#endif
