//----------------------------------------------------------------------------
// File : bXMapShortCuts.h
// Project : MacMap
// Purpose : Header file : XMap class, shortcuts management
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
// 15/03/2010 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapShortcuts__
#define __bXMapShortcuts__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapShortcutsSignature				='Shrt',
	kXMapShortcutsPopupFamilyID			=1,
	kXMapShortcutsPopupFamilyCmd		='PopG',
	kXMapShortcutsExternListID			=2,

	kXMapShortcutsModiShiftID			=3,
	kXMapShortcutsModiShiftCmd			='Shft',
	kXMapShortcutsModiCtrlID			=4,
	kXMapShortcutsModiCtrlCmd			='Ctrl',
	kXMapShortcutsModiAltID				=5,
	kXMapShortcutsModiAltCmd			='Alt_',
	kXMapShortcutsModiCmdID				=6,
	kXMapShortcutsModiCmdCmd			='Cmd_',
	kXMapShortcutsShortCarID			=7,
	kXMapShortcutsApplyModiID			=8,
	kXMapShortcutsApplyModiCmd			='Modi',
	kXMapShortcutsShortCarNumID			=9,
	
	kXMapShortcutsDBProperty			='text'
};

//----------------------------------------------------------------------------

class bXMapShortcuts : public bStdXMapWithModalIntf{
	public:		
		bXMapShortcuts							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapShortcuts					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool event						(	EventRef evt);
		virtual void idle						(	void* prm);

		virtual bool process					(	int msg, 
													void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		
		void scan_read_menu						(	MenuRef mnu);
		void scan_write_menu					(	MenuRef mnu);

		UInt32 scan_array						(	bArray& arr,
													UInt32 key,
													UInt32 modi,
													bool* inmenu);
		
		void populate_ext						(	);
		
		void family_action						(	);
		void ext_action							(	DataBrowserItemID item);
		
		void modify_ext							(	);

		void check_modify						(	);
		
		void set_modifiers						(	UInt32 modi);

		void read_p								(	);
		void write_p							(	);

		void read_p								(	const char* name,
													bArray& arr);
		void write_p							(	const char* name,
													bArray& arr);

		bGenericXMLBaseElement* make_p			(	bArray& arr);
		
		
		void add_ext							(	bArray& arr,
													bGenericExtMgr* mgr,
													UInt32 sgn);
		void modify_ext							(	bArray& arr,
													bGenericExtMgr* mgr,
													UInt32 sgn);
		void remove_ext							(	bArray& arr,
													bGenericExtMgr* mgr,
													UInt32 sgn);
	
	private:
		static ControlKeyFilterResult name_fltr	(	ControlRef c, 
													SInt16* key, 
													SInt16* car, 
													EventModifiers *m);												
		static OSStatus ctrl_evt_hdlr			(	EventHandlerCallRef hdlr,
													EventRef evt,
													void *up);
		static OSStatus shrt_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void shrt_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
	
		int		_fml;
		
		bool	_first;
		bArray	_xmap;
		bArray	_xbox;
		bArray	_macr;
		bArray	_scrp;
		
		UInt32	_curk;
		
		UInt32	_call;

};

//----------------------------------------------------------------------------

#endif
