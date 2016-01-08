//----------------------------------------------------------------------------
// File : bToolSymbRotation.h
// Project : MacMap
// Purpose : Header file : Object modification (symbols/text rotation by field) tool class
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
// 13/06/2007 creation.
//----------------------------------------------------------------------------
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolSymbRotation__
#define __bToolSymbRotation__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolPres.h>

//----------------------------------------------------------------------------

#define	kSymbRotationMessageID	"symbrotation"

enum{
	kSymbRotationEditSign			='SmbR',
	kSymbRotationAngleUseID			=1,
	kSymbRotationAngleUseCmd		='UAng',
	kSymbRotationAngleValueLegID	=2,
	kSymbRotationAngleValueID		=3,
	kSymbRotationAngleUnitID		=4
};

//----------------------------------------------------------------------------

class bToolSymbRotation : public bStdToolPres{
public:		
	bToolSymbRotation						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolSymbRotation				(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void track_draw					(	);
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);
	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);
		
protected:

private:
	int				_fa;
	double			_ia;
	double			_dst;
	bGenericStyle*	_styl;
};

//----------------------------------------------------------------------------

#endif
