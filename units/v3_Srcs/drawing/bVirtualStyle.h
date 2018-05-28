//----------------------------------------------------------------------------
// File : bVirtualStyle.h
// Project : MacMap
// Purpose : Header file : Virtual style class (i.e. styles for non objects : WMS, tiling, legends, titles, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2004 Carte Blanche Conseil.
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
// 28/01/2009 creation.
// 04/09/2014 elimination des appels avec geometrie QD.
//----------------------------------------------------------------------------

#ifndef __bVirtualStyle__
#define __bVirtualStyle__

//----------------------------------------------------------------------------

#include "bStyle.h"

//----------------------------------------------------------------------------

class bVirtualStyle : public bStyle{
public:
	bVirtualStyle 						(	bGenericLayersMgr* mgr); 
	virtual ~bVirtualStyle				(	);

	virtual void settype				(	const char *name);
	virtual void setselectable			(	bool sel);
	
// Accès noyau/externes		
	virtual void setbounds				(	ivx_rect* bounds);

	virtual void draw					(	bStdWait& wt);
	virtual void draw					(	bGenericGeoElement* o){};
	virtual void rect					(	bGenericGeoElement* o, 
											CGRect* bounds);

	virtual bGenericGraphicContext* 
	get_context_for_object				(	bGenericGeoElement* o,
											int pass,
											bool render);
	virtual bGenericGraphicContext* 
	get_context_for_class				(	int clss,
											int pass,
											bool render);
	virtual bArray* objsinrect			(	CGRect* cgr, 
											bool strict);
	virtual bArray* objsincircle		(	CGRect* cgr, 
											bool strict);
	virtual bArray* objsinpath			(	CGPoint* cgp, 
											int nb, 
											bool strict);
	virtual bArray* objsatpoint			(	CGPoint* cgp, 
											bool all);
	
	virtual bool load					(	);
	virtual bool is_virtual				(	);
																									
protected:
	virtual float dist					(	bScreenObj* scr, 
											
											CGPoint* cgp);
	
private:
};

//----------------------------------------------------------------------------

#endif
