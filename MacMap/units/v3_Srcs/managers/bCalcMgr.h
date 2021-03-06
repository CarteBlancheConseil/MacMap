//----------------------------------------------------------------------------
// File : bCalcMgr.h
// Project : MacMap
// Purpose : Header file : Calcs management class
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
// 29/11/2005 creation.
// 09/11/2018 64Bits
//----------------------------------------------------------------------------

#ifndef __bCalcMgr__
#define __bCalcMgr__

//----------------------------------------------------------------------------

#include "bStdUserExtMgr.h"

//----------------------------------------------------------------------------

#define kUserCalcName	"ucalc"

//----------------------------------------------------------------------------

class bCalcMgr : public bStdUserExtMgr{
public:		
	bCalcMgr			(	);
	virtual ~bCalcMgr	(	);

	virtual bool add	(	bGenericXMLBaseElement* root);
	virtual bool modify	(	long idx,
                            bGenericXMLBaseElement* root);
	
protected:

private:

};


//----------------------------------------------------------------------------

#endif
