//----------------------------------------------------------------------------
// File : bToolLineShift.h
// Project : MacMap
// Purpose : Header file : Object modification (line shift by field) tool class
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
// 19/06/2007 creation.
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolLineShift__
#define __bToolLineShift__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolPres.h>

//----------------------------------------------------------------------------

#define	kLineShiftMessageID	"lineshift"

enum{
	kLineShiftEditSign			='SmbR',
	kLineShiftAngleUseID		=1,
	kLineShiftAngleUseCmd		='UAng',
	kLineShiftAngleValueLegID	=2,
	kLineShiftAngleValueID		=3,
	kLineShiftAngleUnitID		=4
};

//----------------------------------------------------------------------------

class bToolLineShift : public bStdToolPres{
public:		
	bToolLineShift							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolLineShift					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void track_draw					(	);
	
protected:
	void put_value							(	int fidx,
												double val);
	double get_dist_to_path					(	CGPoint cgp);
	void reset								(	);
	
private:
	int				_fa;
	double			_ia;
	double			_cur;
	bGenericStyle*	_styl;
	float*			_xpts;
	float*			_ypts;
	int				_npts;
	int*			_offs;
	int				_noffs;
};

//----------------------------------------------------------------------------

#endif
