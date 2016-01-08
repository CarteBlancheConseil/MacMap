//----------------------------------------------------------------------------
// File : bXMapDataFill.h
// Project : MacMap
// Purpose : Header file : XMap class, fills objects data
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
// 16/03/2006 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapDataFill__
#define __bXMapDataFill__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapDataFillSignature		='FFil',
	kXMapDataFillFieldListID	=1,
	kXMapDataFillPopFillID		=2,
	kXMapDataFillPopFillCmd		='PopF',
	kXMapDataFillValueID		=3,
	kXMapDataFillPopConstID		=4,
	kXMapDataFillPopConstCmd	='PopC',
	kXMapDataFillPopValueID		=5,
	kXMapDataFillRunListID		=6,

	kXMapDataFillAddRunID		=7,
	kXMapDataFillAddRunCmd		='FAdd',
	kXMapDataFillModRunID		=8,
	kXMapDataFillModRunCmd		='FMod',
	kXMapDataFillRmvRunID		=9,
	kXMapDataFillRmvRunCmd		='FDel',
	
	kXMapDataFillDBProperty		='text'
};

#define	kXMapDataFillMessageID		"DataFill"
#define	kXMapDataFillMsgFillField	"FillField"
#define	kXMapDataFillMsgWithValue	"WithValue"
#define	kXMapDataFillMsgWithCalc	"WithCalc"
#define	kXMapDataFillMsgWithField	"WithField"

#define	kXMapDataFillErrorCode		-1

//----------------------------------------------------------------------------

class bXMapDataFill : public bStdXMapWithModalIntf{
	public:		
		bXMapDataFill							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDataFill					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													

		void populate_constraints				(	);
		void populate_fields					(	);
		void populate_popfields					(	);
		void populate_popcalcs					(	);
		
		void constraints_action					(	);
		void field_action						(	DataBrowserItemID item);
		void run_action							(	DataBrowserItemID item);
		void kind_action						(	);
		
		virtual void add_run					(	);
		virtual void add_run					(	char* field, 
													int mode, 
													char* value);
		void modify_run							(	);
		void remove_run							(	);

		void check_del							(	);
		void check_add							(	);
		void check_modify						(	);
		
	private:
		static void delete_run					(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);	
																							
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
		static OSStatus run_handler				(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void run_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		virtual void make_text					(	char* text);
		virtual void fill_data					(	);
		virtual void fill_data					(	bool silent);
		virtual void reset						(	);
		virtual bool parse_tree					(	);
		
		bGenericType*	_tp;
		int				_fld;
		int				_run;
		bArray			_runs;
};

//----------------------------------------------------------------------------

#endif
