//----------------------------------------------------------------------------
// File : bXMapDBStructure.h
// Project : MacMap
// Purpose : Header file : XMap class, database structure modification (adding, deleting, modifying fields, etc...)
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
// 28/04/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapDBStructure__
#define __bXMapDBStructure__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapDBStructureSignature			='FFmt',
	kXMapDBStructurePopupTypeID			=1,
	kXMapDBStructurePopupTypeCmd		='PopT',
	kXMapDBStructureFieldListID			=2,
	kXMapDBStructureFieldKindID			=3,
	kXMapDBStructureFieldKindCmd		='PopG',
	kXMapDBStructureFieldNameNameID		=4,
	kXMapDBStructureFieldNameID			=5,
	kXMapDBStructureFieldLenNameID		=6,
	kXMapDBStructureFieldLenID			=7,
	kXMapDBStructureFieldDefValNameID	=8,
	kXMapDBStructureFieldDefValID		=9,
	kXMapDBStructureFieldMaskID			=10,
	kXMapDBStructureFieldMaskCmd		='fMsk',
	kXMapDBStructureFieldDelLockID		=11,
	kXMapDBStructureFieldDelLockCmd		='fDel',
	kXMapDBStructureFieldWriteLockID	=12,
	kXMapDBStructureFieldWriteLockCmd	='fWrt',

	kXMapDBStructureRmvFieldID			=13,
	kXMapDBStructureRmvFieldCmd			='FDel',
	kXMapDBStructureAddFieldID			=14,
	kXMapDBStructureAddFieldCmd			='FAdd',
	kXMapDBStructureModFieldID			=15,
	kXMapDBStructureModFieldCmd			='FMod',
	kXMapDBStructureFieldInfoID			=16,
	
	kXMapDBStructureDBProperty			='text'
};

#define	kXMapDBStructureMessageID		"data structure"

typedef struct del_p{
	int					status;
	ControlRef 			c;
	bStdBaseAccessor*	flds;
	bGenericMacMapApp*	gapp;
}del_p;

//----------------------------------------------------------------------------

class bXMapDBStructure : public bStdXMapWithModalIntf{
	public:		
		bXMapDBStructure						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDBStructure				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													

		void populate_types						(	);
		void populate_fields					(	);
		
		void type_action						(	);
		void check_field						(	);
		void field_action						(	DataBrowserItemID item);
		void kind_action						(	);
		
		void add_field							(	);
		void modify_field						(	);
		void remove_field						(	);

		void check_del							(	);
		void check_add							(	);
		void check_modify						(	);
		
		void set_kind							(	int kind);
		void set_name							(	char* name);
		void set_title							(	char* msg_id);
		void set_len							(	int len);
		void set_prot							(	bool msk, 
													bool del, 
													bool wrt);
		
	private:
		static void delete_field				(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
		static void sel_test					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
		static ControlKeyFilterResult name_fltr	(	ControlRef c, 
													SInt16* key, 
													SInt16* car, 
													EventModifiers *m);												
		static OSStatus ctrl_evt_hdlr			(	EventHandlerCallRef hdlr,
													EventRef evt,
													void *up);
		static OSStatus field_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void field_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		
		bGenericType*			_tp;
		int						_fld;
		bool					_flg;
};

//----------------------------------------------------------------------------

#endif
