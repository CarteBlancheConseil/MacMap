//----------------------------------------------------------------------------
// File : bGenericGeog.h
// Project : MacMap
// Purpose : Header file : Root class for geogs
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

#ifndef __bGenericGeog__
#define __bGenericGeog__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericExt.h>
#include <mox_intf/bGenericXMLFormulaElement.h>

//----------------------------------------------------------------------------

class bGenericGeog : public virtual bGenericExt {
public:		
	virtual bool edit						(	WindowRef wd, 
												Rect* r)=0;// rect à virer
	virtual void end_edit					(	)=0;
	virtual void hide						(	)=0;
	virtual void show						(	)=0;
	virtual void get_params					(	bArray* arr, 
												int indent)=0;
	virtual void put_params					(	bGenericXMLBaseElement* root)=0;
	virtual bGenericXMLFormulaElement* root	(	)=0;
};

//----------------------------------------------------------------------------

#endif
