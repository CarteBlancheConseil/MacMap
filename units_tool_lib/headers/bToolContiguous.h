//----------------------------------------------------------------------------
// File : bToolContiguous.h
// Project : MacMap
// Purpose : Header file : Contiguous tool class (Select a set of connected lines having same value in a field, usefull for roadmaps). 
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
// 07/04/2006 creation.
//----------------------------------------------------------------------------
// 19/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolContiguous__
#define __bToolContiguous__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolNav.h>

//----------------------------------------------------------------------------

enum{
	kContiguousEditSign				='Cont',
	kContiguousTypesID				=1,
	kContiguousTypesCmd				='popt',
	kContiguousFieldsID				=2,
	kContiguousFieldsCmd			='flds',
	kContiguousFieldsNameProperty	='name'
};

//----------------------------------------------------------------------------

class bToolContiguous : public bStdToolNav{
public:		
	bToolContiguous							(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolContiguous				(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	
	virtual void clic						(	CGPoint pt, int count);
	
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);

	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray& arr);
	virtual void save						(	);
	
protected:

private:
	virtual void line_propagate				(	bGenericGeoElement* o);
	virtual void populate_fields			(	WindowRef wd);
	virtual void flush_fields				(	WindowRef wd);

	virtual bGenericXMLBaseElement* make	(	int tidx);
	virtual void load_type					(	int tidx);

	static OSStatus dtb_proc				(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue);
	static void dtb_notifier				(	ControlRef browser, 
												DataBrowserItemID item, 
												DataBrowserItemNotification msg);
								
	int		_cur;
	bArray	_types;
};

//----------------------------------------------------------------------------

#endif
