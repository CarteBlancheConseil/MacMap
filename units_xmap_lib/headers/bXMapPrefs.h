//----------------------------------------------------------------------------
// File : bXMapPrefs.h
// Project : MacMap
// Purpose : Header file : XMap class, preferences
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
// 18/06/2007 creation. Les Prefs du 18 juin !
//----------------------------------------------------------------------------

#ifndef __bXMapPrefs__
#define __bXMapPrefs__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <MacMapSuite/wtable.h>

//----------------------------------------------------------------------------

enum{	
	kXMapPrefsSignature			='Prfs',
	kXMapPrefsPopupUnitID		=1,
	kXMapPrefsDigitsLegID		=2,
	kXMapPrefsDigitsID			=3,
	kXMapPrefsProjListID		=4,
	kXMapPrefsSRIDLegID			=5,
	kXMapPrefsSRIDID			=6,
	kXMapPrefsEPSGProperty		='Epsg',
	kXMapPrefsProjProperty		='Proj',
	kXMapPrefsBkgColorID		=8,
	kXMapPrefsBkgColorCmd		='Fill',
	kXMapPrefsSaveObjStateID	=9,
	kXMapPrefsOriginXStateID	=11,
	kXMapPrefsOriginYStateID	=13,
	kXMapPrefsResoInfoID		=15
};

//----------------------------------------------------------------------------

class bXMapPrefs : public bStdXMapWithModalIntf{
	public:		
		bXMapPrefs								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapPrefs						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
			
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													

		void populate_units						(	);
		void populate_projs						(	);
		
		void proj_action						(	DataBrowserItemID item);
				
	private:
		static OSStatus ctrl_evt_hdlr			(	EventHandlerCallRef hdlr,
													EventRef evt,
													void *up);
		static OSStatus proj_handler			(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void proj_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
											
		virtual void init_projs					(	);
		virtual void get_color					(	float* clr);
		virtual void put_color					(	float* clr);
		
		wtable		_tbl;
		bool		_flg;
		bool		_rpt;
		int			_lsrid;
};

//----------------------------------------------------------------------------

#endif
