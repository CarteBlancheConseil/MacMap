//----------------------------------------------------------------------------
// File : bToolSelect.h
// Project : MacMap
// Purpose : Header file : Select tool class
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
// 27/12/2004 creation.
//----------------------------------------------------------------------------
// 17/06/2011 CGGeometry OK.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolSelect__
#define __bToolSelect__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolNav.h>

//----------------------------------------------------------------------------

enum{
	kSelectDragRect			=1,
	kSelectDragCircle		=2,
	kSelectDragPoly			=3
};

enum{
	kSelectEditSign			='slct',
	kSelectAllObjectsID		=4,
	kSelectAllObjectsCmd	='pobj',
	kSelectAllTypesID		=5,
	kSelectAllTypesCmd		='ptyp',
	kSelectIncludeID		=7,
	kSelectIncludeCmd		='incl',
	kSelectAngleUseID		=8,
	kSelectAngleUseCmd		='uang',
	kSelectAngleValueID		=10,
	kSelectAngleValueCmd	='vang',
	kSelectAngleTextID		=9,
	kSelectAngleUnitID		=11,
	kSelectPTypesID			=12,
	kSelectDTypesID			=13,
	kSelectCheckSWPProperty	='chWP',
	kSelectCheckSBDProperty	='chBD',
	kSelectNameProperty		='name'
};

//----------------------------------------------------------------------------

class bToolSelect : public bStdToolNav{
public:		
	bToolSelect								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolSelect					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void activate					(	);
	virtual void deactivate					(	);
	virtual void pop						(	cocoaMenuPrm prm);
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void end_clic					(	);
	virtual void update						(	bool global);
	virtual void set_modifiers				(	int k);

	virtual bool set_key					(	int k);
	
	virtual void action						(	int count);
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);
	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray& arr);
	
protected:

private:
	virtual void rect_select				(	);
	virtual void circle_select				(	);
	virtual void polygon_select				(	);
	virtual void point_select				(	);
	virtual void report						(	);
	virtual void toggle						(	bArray* input, 
												bArray* handled);
	virtual void reset_handled				(	bArray* handled);

	virtual	void populate_types				(	WindowRef wd);
	static pascal OSStatus dtb_proc			(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue);
		
	CursorRef		_ctoggle;
	bool			_pt_all_obj;
	bool			_pt_all_typ;
	bool			_full_include;
	bool			_poly_angle;
	bArray			_types;
};

//----------------------------------------------------------------------------

#endif
