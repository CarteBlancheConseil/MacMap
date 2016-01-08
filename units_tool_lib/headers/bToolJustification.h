//----------------------------------------------------------------------------
// File : bToolJustification.h
// Project : MacMap
// Purpose : Header file : Object modification (justification by field) tool class
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

#ifndef __bToolJustification__
#define __bToolJustification__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolPres.h>

//----------------------------------------------------------------------------

#define	kJustificationMessageID	"justification"

enum{
	kJustificationKindNone		=0,
	kJustificationKindOnPoint	=1,
	kJustificationKindOnPath	=2
};

enum{
	kJustificationMethodNone		=1,
	kJustificationMethodGlyphSpace	=2,
	kJustificationKindOnWordSpace	=3
};

enum{
	kJustificationUserModeStd		=0,
	kJustificationUserModeLeft		=1,
	kJustificationUserModeCenter	=2,
	kJustificationUserModeRight		=3,
	kJustificationUserModeSpace		=4
};

enum{
	kJustificationEditSign			='JstT',
	kJustificationButtonsID			=1
};

//----------------------------------------------------------------------------

class bToolJustification : public bStdToolPres{
public:		
	bToolJustification						(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolJustification				(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void set_modifiers				(	int k);

	virtual void track_draw					(	);
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);

	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray &arr);

protected:

	void put_value							(	int fidx,
												double val);
	double get_justification_on_path		(	CGPoint cgp);
	double get_justification_on_point		(	CGPoint cgp);
	double get_inter_on_path				(	CGPoint cgp);
	double get_inter_on_point				(	CGPoint cgp);


private:
	CursorRef		_cleft;
	CursorRef		_cright;
	CursorRef		_cmiddle;
	CursorRef		_cspace;

	int				_umode;
	int				_jkd;
	int				_jmd;
	
	int				_fjust;
	double			_ijust;
	int				_fspc;
	double			_ispc;
	
	double			_curjust;
	double			_compjust;

	double			_curspc;

	double			_txtwdt;
	double			_pthlen;

	bGenericStyle*	_styl;
	
	int				_clkidx;
	float*			_xpts;
	float*			_ypts;
	int				_npts;
	int*			_offs;
	int				_noffs;
};

//----------------------------------------------------------------------------

#endif
