//----------------------------------------------------------------------------
// File : bXMapCalcEditor.h
// Project : MacMap
// Purpose : Header file : XMap class, MapCalculator editor
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapCalcEditor__
#define __bXMapCalcEditor__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <xmap_lib/bXMapCalcCell.h>

//----------------------------------------------------------------------------

#define kXMapCalcEditorCreateCalcMsgID	"CreateCalc"

enum{	
	kXMapCalcEditorSignature			='ClcE',
	kXMapCalcEditorPopupKindID			=100,
	kXMapCalcEditorPopupKindCmd			='PopK',
	kXMapCalcEditorAddID				=101,
	kXMapCalcEditorAddCmd				='Add_',
	kXMapCalcEditorPopupItemID			=103,
	kXMapCalcEditorPopupItemCmd			='PopF',
	kXMapCalcEditorListID				=104,
	kXMapCalcEditorListProperty			='text',
	kXMapCalcEditorClearID				=105,
	kXMapCalcEditorClearCmd				='Del_',
	kXMapCalcEditorViewID				=106,
	kXMapCalcEditorPopupReturnedKindID	=107,
	kXMapCalcEditorPopupReturnedKindCmd	='PpRK',
	kXMapCalcEditorNameID				=109
};

//----------------------------------------------------------------------------

class bXMapCalcEditor : public bStdXMapWithModalIntf{
	public:		
		bXMapCalcEditor							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapCalcEditor				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool process					(	int msg, void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);// Possibilité de passer un XML à éditer
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		static pascal OSStatus item_handler		(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void item_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		void populate_browser					(	);
		void kind_action						(	);
		void type_action						(	);
		void list_action						(	DataBrowserItemID item);
		void clear_action						(	);
		bool save_action						(	);

		bool parse								(	bGenericXMLBaseElement* root);

		void update_preview						(	);
		void draw								(	CGContextRef ctx,
													HIViewRef vw);
		
		static OSStatus view_event_proc			(	EventHandlerCallRef hdlr,
													EventRef evt,
													void* udt);
	
	private:
		int						_kind;
		int						_type;
		bXMapCalcCell*			_root;
		char					_name[FILENAME_MAX];
		UInt32					_fknd;
		UInt32					_fsgn;
		bool					_new;
		int						_idx;
};

//----------------------------------------------------------------------------

#endif
