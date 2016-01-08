//----------------------------------------------------------------------------
// File : bXMapStringProcessing.h
// Project : MacMap
// Purpose : Header file : XMap class, string formating utility
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
// 28/05/2009 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapStringProcessing__
#define __bXMapStringProcessing__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapStringProcessingSignature		='PStr',
	kXMapStringProcessingLeftListID		=10,
	kXMapStringProcessingRightListID	=11,
	kXMapStringProcessingAddID			=12,
	kXMapStringProcessingAddCmd			='FAdd',
	kXMapStringProcessingInsertID		=13,
	kXMapStringProcessingInsertCmd		='FIns',
	kXMapStringProcessingRemoveID		=14,
	kXMapStringProcessingRemoveCmd		='FDel',
	kXMapStringProcessingSepID			=16,	
	kXMapStringProcessingDBProperty		='text',

	kXMapStringProcessingCaseID			=20,
	kXMapStringProcessingSearchID		=22,
	kXMapStringProcessingReplaceID		=24,
	kXMapStringProcessingFormatID		=26,
	
	kXMapStringProcessingTargetID		=3,

};

#define kXMapStringProcessingMessageID			"StringProcessing"
#define kXMapStringProcessingProcessMessageID	"Processing"
#define kXMapStringProcessingErrorsOccursID		"ErrorsOccurs"
#define kXMapStringProcessingReadLogID			"ReadLog"

//----------------------------------------------------------------------------

class bXMapStringProcessing : public bStdXMapWithModalIntf{
	public:		
		bXMapStringProcessing					(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapStringProcessing			(	);
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
		void add_action							(	);
		void insert_action						(	);
		void rmv_action							(	);

		void report								(	);

		bool process_str						(	);
		bool process_str						(	bool dummy);
		bool process_str						(	strprocess_prm* prm,
													bool silent);
	
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
															
		bArray			_arr;
		strprocess_prm	_prm;
		
};

//----------------------------------------------------------------------------

#endif
