//----------------------------------------------------------------------------
// File : bToolPrintArea.h
// Project : MacMap
// Purpose : Header file : Print area tool class
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
// 22/03/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bToolPrintArea__
#define __bToolPrintArea__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolPres.h>

//----------------------------------------------------------------------------

enum{
	kPrintAreaMenuCommandCenter		=1,
	kPrintAreaMenuCommandWindow		=2,
	kPrintAreaMenuCommandSelection	=3,
	kPrintAreaMenuCommandContrasts	=4
};

enum{
	kPrintAreaSignature				='tZIm',
	kPrintAreaXID					=1,
	kPrintAreaYID					=2,
	kPrintAreaPopKindID				=3,
	kPrintAreaPopKindCmd			='PopK',
	kPrintAreaWID					=4,
	kPrintAreaHID					=5,
	kPrintAreaPopSizeID				=6,
	kPrintAreaOrientationID			=7,
	kPrintAreaFrameCustomSizeID		=8,
	kPrintAreaFrameStandardSizeID	=9,
	kPrintAreaXUnitID				=10,
	kPrintAreaYUnitID				=11,
	kPrintAreaWUnitID				=12,
	kPrintAreaHUnitID				=13,
	kPrintAreaDrawAreaID			=14
};

enum{
	kPrintAreaPopKindMenuGeo		=1,
	kPrintAreaPopKindMenuPix		=2,
	kPrintAreaPopKindMenuCustom		=3,
	kPrintAreaPopKindMenuStandard	=4
};

enum{
	kPrintAreaDragModeSet			=1,
	kPrintAreaDragModeMove			=2,
	kPrintAreaDragModeBottomResize	=3,
	kPrintAreaDragModeRightResize	=4,
	kPrintAreaDragModeFullResize	=5
};

//----------------------------------------------------------------------------

class bToolPrintArea : public bStdToolPres{
public:		
	bToolPrintArea							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolPrintArea					(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void pop						(	cocoaMenuPrm prm);
	virtual bool event						(	EventRef evt);
	virtual void clic						(	i2dvertex loc, 
												int count);
	virtual void end_clic					(	);
	virtual void activate					(	);
	virtual void update						(	bool global);
	virtual void drag						(	CGPoint pt);	
	virtual void track_draw					(	);

	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);

	virtual bGenericXMLBaseElement* load	(	);

protected:
	
private:
	virtual void pop_kind_action			(	WindowRef wd);
	virtual void set_as_geo					(	WindowRef wd);
	virtual void set_as_ppsz				(	WindowRef wd);
	virtual void set_as_pix					(	WindowRef wd);
	virtual void puts						(	WindowRef wd);
	virtual void put_area					(	WindowRef wd);

	virtual void center_to_screen			(	);
	virtual void fit_to_screen				(	);
	virtual void fit_to_objs				(	bArray* arr);

	bGenericXMLBaseElement* make			(	bArray &arr);

	bool		_draw;
	int			_pprnat;
	int			_pprkind;
	int			_sizekind;
	ivx_rect	_vxr;
	int			_dragmode;
	bool		_startmove;
	CursorRef	_cmove;
	CursorRef	_cxyresize;
	CursorRef	_cxresize;
	CursorRef	_cyresize;
};

//----------------------------------------------------------------------------

class bToolPrintAreaParams  : public bGraphicParams {
public:
	bToolPrintAreaParams			(	bGenericXMLBaseElement* root, 
										bGenericMacMapApp* gapp,
										CFBundleRef bndl);
	
	virtual ~bToolPrintAreaParams	(	);
	
	virtual bool update				(	bArray& arr, 
										long indent);
	
	virtual void apply				(	CGContextRef ctx);	
	virtual void setDash			(	const float* dash,
										long ndash);
	
	virtual const CGFloat* dash		(	){return _dash;};
	
protected:
	virtual void parse				(	bGenericXMLBaseElement* root, 
										bGenericMacMapApp* gapp);
	
	long		_ndash;
	CGFloat*	_dash;
	
private:
	
};

//----------------------------------------------------------------------------

#endif
