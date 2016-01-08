//----------------------------------------------------------------------------
// File : bXMapToolMenuConfig.h
// Project : MacMap
// Purpose : Header file : XMap class, contextual tool menu configuration
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
// 21/09/2009 creation.
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------

#ifndef __bXMapToolMenuConfig__
#define __bXMapToolMenuConfig__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>

//----------------------------------------------------------------------------

enum{	
	kXMapToolMenuConfigSignature		='TlMC',
	kXMapToolMenuConfigLeftListID		=2,
	kXMapToolMenuConfigRightListID		=3,

	kXMapToolMenuConfigAddID			=4,
	kXMapToolMenuConfigAddCmd			='TAdd',
	kXMapToolMenuConfigInsertID			=5,
	kXMapToolMenuConfigInsertCmd		='TIns',
	kXMapToolMenuConfigAllID			=6,
	kXMapToolMenuConfigAllCmd			='TAll',
	kXMapToolMenuConfigRemoveID			=7,
	kXMapToolMenuConfigRemoveCmd		='TDel',
	
	kXMapToolMenuConfigDBProperty		='text'
};

//----------------------------------------------------------------------------

class bXMapToolMenuConfig : public bStdXMapWithModalIntf{
	public:		
		bXMapToolMenuConfig						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapToolMenuConfig			(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
    
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		void populate_left						(	);
		void populate_right						(	);
		void add_action							(	);
		void insert_action						(	);
		void addall_action						(	);
		void rmv_action							(	);
		void db_resets							(	);

		virtual void write_p					(	);
		virtual bGenericXMLBaseElement*	make_p	(	);

	private:
		static OSStatus tool_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void _add_tool					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
		static void _rmv_tool					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
};

//----------------------------------------------------------------------------

#endif
