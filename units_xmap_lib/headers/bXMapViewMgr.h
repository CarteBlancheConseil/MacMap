//----------------------------------------------------------------------------
// File : bXMapViewMgr.h
// Project : MacMap
// Purpose : Header file : XMap class, views management
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
// 29/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapViewMgr__
#define __bXMapViewMgr__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapViewMgrSignature				='VMgr',
	kXMapViewMgrViewListID				=1,
	kXMapViewMgrViewListCmd				='List',
	kXMapViewMgrNewID					=2,
	kXMapViewMgrNewCmd					='VNew',
	kXMapViewMgrDuplicateID				=6,
	kXMapViewMgrDuplicateCmd			='VDup',
	kXMapViewMgrRenameID				=3,
	kXMapViewMgrRenameCmd				='VRen',
	kXMapViewMgrDeleteID				=5,
	kXMapViewMgrDeleteCmd				='VDel',
	kXMapViewMgrNameProperty			='data'
};

#define	kXMapViewMgrMessageID		"viewmgr"

//----------------------------------------------------------------------------

class bXMapViewMgr : public bStdXMapWithModalIntf{
	public:		
		bXMapViewMgr							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapViewMgr					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
    
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													

		void populate_views						(	);
		void reset_views						(	);
		
		void view_action						(	DataBrowserItemID item);
		
		void add_view							(	);
		void rename_view						(	);
		void duplicate_view						(	);
		void delete_view						(	);
		
		int id_to_idx							(	DataBrowserItemID itemID);
		void id_add								(	DataBrowserItemID itemID);
		void id_rmv								(	DataBrowserItemID itemID);
				
	private:
		static OSStatus view_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void view_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
											
		bArray		_arr;
};

//----------------------------------------------------------------------------

#endif
