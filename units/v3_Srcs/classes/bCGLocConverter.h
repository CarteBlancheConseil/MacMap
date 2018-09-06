//----------------------------------------------------------------------------
// File : bCGLocConverter.h
// Project : MacMap
// Purpose : Header file : Coordinates conversion class
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
// 25/11/2004 creation.
// 04/09/2014 Conversion en géométrie CG et suppression des géométries QD
//----------------------------------------------------------------------------

#ifndef __bCGLocConverter__
#define __bCGLocConverter__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericLocConverter.h>
#include <MacMapSuite/vx_transform.h>

//----------------------------------------------------------------------------

class bCGLocConverter : public bGenericLocConverter{
public:		
	bCGLocConverter			(	);
	virtual ~bCGLocConverter(	);
	
// Point Conversion
	virtual bool convert	(	i2dvertex* lt,	
								i2dvertex* lf);
	virtual bool convert	(	i2dvertex* lt,
								d2dvertex* lf);
	virtual bool convert	(	i2dvertex* lt,	
								CGPoint* lf);
	
	virtual bool convert	(	d2dvertex* lt,	
								i2dvertex* lf);
	virtual bool convert	(	d2dvertex* lt,	
								d2dvertex* lf);
	virtual bool convert	(	d2dvertex* lt,	
								CGPoint* lf);

	virtual bool convert	(	CGPoint* lt,	
								i2dvertex* lf);
	virtual bool convert	(	CGPoint* lt,	
								d2dvertex* lf);
	virtual bool convert	(	CGPoint* lt,	
								CGPoint* lf);
	
// Lock Up
	virtual void set_lockup	(	dvertices* fich,	
								dvertices* click,	
								int method);
	virtual void transform	(	i2dvertex* fich,	
								i2dvertex* click);
	virtual void transform	(	d2dvertex* fich,	
								d2dvertex* click);
	virtual void transform	(	CGPoint* fich,
								CGPoint* click);

	virtual void init		(	ivx_rect* vxr);
	virtual void reset		(	ivx_rect* vxr);
	
protected:

private:
	virtual void dump_calp	(	SIMILPrm* CalP);// Debug

	double		_conv;
	ivx_rect	_port_bnds;
	CGRect		_port_rect;
	
	SIMILPrm	_CalP;
	int			_method;		
};

//----------------------------------------------------------------------------

#endif
