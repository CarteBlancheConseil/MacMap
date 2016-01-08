//----------------------------------------------------------------------------
// File : bToolInfo.h
// Project : MacMap
// Purpose : Header file : Object info tooltip class (show a set of field when mouse is over objects)
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
// 07/01/2005 creation.
//----------------------------------------------------------------------------
// 18/03/2014 Ready for CG geometry	& drawing 
//----------------------------------------------------------------------------

#ifndef __bToolInfo__
#define __bToolInfo__

//----------------------------------------------------------------------------

#include <std_ext/bStdToolNav.h>

//----------------------------------------------------------------------------

enum{
	kInfoEditSign				='info',
	kInfoShowTypeID				=3,
	kInfoShowTypeCmd			='uset',
	kInfoTypesID				=4,
	kInfoTypesCmd				='popt',
	kInfoFieldsID				=5,
	kInfoFieldsCmd				='flds',
	kInfoFieldsCheckProperty	='chck',
	kInfoFieldsNameProperty		='name'
};

//----------------------------------------------------------------------------

class bToolInfo : public bStdToolNav{
public:		
	bToolInfo								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bToolInfo						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);

	virtual void clic						(	CGPoint pt, 
												int count);		
	virtual void update						(	bool global);
	virtual bool edit_event					(	EventRef evt, 
												WindowRef wd);
	virtual void edit_init					(	WindowRef wd);

	virtual bGenericXMLBaseElement* load	(	);
	virtual bGenericXMLBaseElement* make	(	bArray& arr);
	virtual void save						(	);

protected:

private:
	static pascal OSStatus dtb_proc			(	ControlRef browser, 
												DataBrowserItemID itemID, 
												DataBrowserPropertyID property, 
												DataBrowserItemDataRef itemData, 
												Boolean changeValue);
	void init_draw							(	);
	void draw								(	);
	void check_bounds						(	);
	bGenericXMLBaseElement* make			(	int tidx);
	virtual void load_type					(	int tidx);
	bGenericXMLBaseElement* find_type		(	bGenericXMLBaseElement* root, 
												double id);
	void flush_fields						(	WindowRef wd);
	void populate_fields					(	WindowRef wd);
	int get_index							(	int tpidx, 
												int fid);
		
	CursorRef		_cleft;
	bool			_cleft_on;
	CGFloat			_wd_width;
	CGFloat			_wd_height;
	bool			_show_type;
	CGFloat			_fnt_size;
	char			_fnt[256];
	bArray			_fields;
	int				_edit_type;

	int				_fnt_id;// A virer
};

//----------------------------------------------------------------------------

typedef struct fld{
	int		fid;
	int		idx;
	char	name[256];
	char	value[256];
}fld;

//----------------------------------------------------------------------------

class bToolInfoParams  : public bGraphicParams {
public:
	bToolInfoParams				(	bGenericXMLBaseElement* root, 
									bGenericMacMapApp* gapp,
									CFBundleRef bndl);
	
	virtual ~bToolInfoParams	(	);
	
	virtual bool update			(	bArray& arr, 
									long indent);
	
	virtual void applyFont		(	CGContextRef ctx);
	
	virtual void setFont		(	const char* fnt);
	virtual void setFontSize	(	float sz);
	virtual void setFontFill	(	const float* clr);
	virtual void setRadius		(	float rad);
	
	virtual const char* font	(	){return _fnt;};
	virtual CGFloat fontSize	(	){return _fnt_sz;};
	virtual CGFloat radius		(	){return _radius;};

protected:
	virtual void parse		(	bGenericXMLBaseElement* root, 
								bGenericMacMapApp* gapp);
	
	char	_fnt[256];
	CGFloat	_fnt_sz;
	CGFloat _fnt_fill[4];
	CGFloat _radius;

private:
	
};

//----------------------------------------------------------------------------

#endif
