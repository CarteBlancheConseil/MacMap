//----------------------------------------------------------------------------
// File : bXMapFExp.h
// Project : MacMap
// Purpose : Header file : XMap class, export format and projection
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
// 08/06/2007 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapFExp__
#define __bXMapFExp__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapFExpSignature		='FExp',
	kXMapFExpPopTypeID		=1,
	kXMapFExpPopTypeCmd		='PopT',
	kXMapFExpLeftListID		=2,
	kXMapFExpRightListID	=3,

	kXMapFExpAddID			=4,
	kXMapFExpAddCmd			='FAdd',
	kXMapFExpInsertID		=5,
	kXMapFExpInsertCmd		='FIns',
	kXMapFExpAllID			=6,
	kXMapFExpAllCmd			='FAll',
	kXMapFExpRemoveID		=7,
	kXMapFExpRemoveCmd		='FDel',
	
	kXMapFExpDBProperty		='text',

	kXMapFExpProjID			=8,
	kXMapFExpProjCmd		='Proj',
	kXMapFExpProjNameID		=9
};

#define	kXMapFExpMessageID		"fmtexp"
#define	kXMapFExpStdParamID		"std_format.xml"

//----------------------------------------------------------------------------

class bXMapFExp : public bStdXMapWithModalIntf{
	public:		
		bXMapFExp								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapFExp						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		void populate_left						(	);
		void populate_right						(	);
		void type_action						(	);
		void add_action							(	);
		void insert_action						(	);
		void addall_action						(	);
		void rmv_action							(	);
		void proj_action						(	);
		void db_resets							(	);

		void reset								(	bool dump);
		void report								(	);
		void get								(	int tp, 
													bArray* arr,
													int* srid);
		void set								(	int tp, 
													bArray* arr,
													int srid);
		virtual void read_p						(	);
		virtual void write_p					(	);
		virtual bGenericXMLBaseElement*	make_p	(	);
		virtual bool parse						(	);

	private:
		static OSStatus left_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void left_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		static OSStatus right_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void right_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		static void _add_field					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
		static void _rmv_field					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
															
		int		_tp;
		bArray	_arr;
		int		_srid;
};

//----------------------------------------------------------------------------

#endif
