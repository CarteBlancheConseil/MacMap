//----------------------------------------------------------------------------
// File : bToolZoom.h
// Project : MacMap
// Purpose : Header file : Zoom tool class
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
// 05/01/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bToolZoom__
#define __bToolZoom__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolNav.h>

//----------------------------------------------------------------------------

enum{
	kZoomEditSign				='zoom',
	kZoomScalesID				=3,
	kZoomScalesCmd				='scal',
	kZoomScalesValueProperty	='valu',
	kZoomScalesNameProperty		='name',
	kZoomRmvID					=4,
	kZoomRmvCmd					='rmv ',
	kZoomAddID					=6,
	kZoomAddCmd					='add '
};

typedef struct scale{
	int				idx;
	bool			modified;
	bool			created;
	bool			deleted;
	double			value;
	char			name[256];
	bGenericUnit*	u;
}scale;

//----------------------------------------------------------------------------

class bToolZoom : public bStdToolNav{
public:		
	bToolZoom								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolZoom						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void pop						(	cocoaMenuPrm prm);
	virtual void clic						(	CGPoint pt, 
												int count);
	virtual void update						(	bool global);
	virtual void set_modifiers				(	int k);

	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);
	
	virtual void pop_tools					(	CGPoint	location,
												WindowRef inWindow);

protected:

private:
	static pascal OSStatus dtb_proc			(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue);
	bool get_scale							(	int idx, 
												scale* scl);
	bool put_scale							(	int idx, 
												scale* scl);
	bool rmv_scale							(	int idx);
	bool add_scale							(	scale* scl);
	void add								(	WindowRef wd);
	void remove								(	WindowRef wd);
	static void _remove						(	DataBrowserItemID item, 
												DataBrowserItemState state, 
												void *clientData);
	void dump								(	);
	
	CursorRef		_cminus;
	bArray			_scales;
};

//----------------------------------------------------------------------------

#endif
