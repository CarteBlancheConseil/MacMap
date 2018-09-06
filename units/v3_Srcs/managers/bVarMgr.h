//----------------------------------------------------------------------------
// File : bVarMgr.h
// Project : MacMap
// Purpose : Header file : Constants/Variables management class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2007 Carte Blanche Conseil.
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bVarMgr__
#define __bVarMgr__

//----------------------------------------------------------------------------

#include "bStdUserExtMgr.h"

//----------------------------------------------------------------------------

#define kUserVarName	"uvar"

//----------------------------------------------------------------------------

class bVarMgr : public bStdUserExtMgr{
public:		
	bVarMgr					(	);
	virtual ~bVarMgr		(	);

	virtual bool add		(	bGenericXMLBaseElement* root);
	virtual bool modify		(	int idx, 
								bGenericXMLBaseElement* root);
	
protected:

private:

};


//----------------------------------------------------------------------------

#endif
