//----------------------------------------------------------------------------
// File : bGenericvDef.h
// Project : MacMap
// Purpose : Header file : Root class for vDefs
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
// 14/12/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bGenericvDef__
#define __bGenericvDef__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExt.h>
#include <mox_intf/bGenericType.h>

//----------------------------------------------------------------------------

#define _pm_coef_	(72.0/25.4)

//----------------------------------------------------------------------------

class bGenericvDef : public virtual bGenericExt {
public:
	virtual ~bGenericvDef		(	){};
	virtual bool edit			(	bGenericXMLBaseElement** root, 
									bGenericType* tp)=0;
};

//----------------------------------------------------------------------------

#endif
