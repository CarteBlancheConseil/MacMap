//----------------------------------------------------------------------------
// File : bXMapDataJoin.h
// Project : MacMap
// Purpose : Header file : XMap class, data join from DBF, text, csv, ... file
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
// 29/06/2006 creation.
// 11/02/2015 NAV to NS transformation.
//----------------------------------------------------------------------------

#ifndef __bXMapDataJoin__
#define __bXMapDataJoin__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

#include <stdio.h>

//----------------------------------------------------------------------------

enum{
	kXMapDataJoinSignature			='FJon',
	kXMapDataJoinTypesID			=1,
	kXMapDataJoinTypesCmd			='Typs',
	kXMapDataJoinUniqueID			=2,
	kXMapDataJoinUseMaskedID		=3,
	kXMapDataJoinListID				=4,
	kXMapDataJoinActionID			=5,
	kXMapDataJoinActionCmd			='Act_',
	kXMapDataJoinFieldsID			=6,
	kXMapDataJoinFieldsCmd			='Flds',
	kXMapDataJoinColumnsProperty	='text'
};

enum{
	kXMapDataJoinNoErr,
	kXMapDataJoinUnsupportedFormatErr,
	kXMapDataJoinOpenErr,
	kXMapDataJoinKeyNotDefinedErr,
	kXMapDataJoinNoDataToFillErr,
	kXMapDataJoinNoObjectErr,
	kXMapDataJoinNotUniqueErr
};

#define	kXMapDataJoinMessageID		"DataJoin"
#define	kXMapDataJoinLineNumberID	"LineNumber"
#define	kXMapDataJoinErrorsOccursID	"ErrorsOccurs"
#define	kXMapDataJoinReadLogID		"ReadLog"
#define	kXMapDataJoinReadContinueID	"Continue"

//----------------------------------------------------------------------------

class bXMapDataJoin : public bStdXMapWithModalIntf{
	public:		
		bXMapDataJoin							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDataJoin					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
		virtual void open						(	int* flags);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);

	protected:
		static OSStatus col_handler				(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void col_notifier				(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		virtual void col_action					(	DataBrowserItemID item);
		
		virtual void type_action				(	);
		virtual void act_action					(	);
		virtual void field_action				(	);

		virtual void populate_act				(	);
		
		virtual bool test_search				(	int f);
		virtual bool test_create				(	int f);

		virtual void join						(	bool mode);
				
	private:
		
		int				_cur;
		
		bArray			_cols;
		int				_tpi;
		wtable			_tbl;
		char			_tpth[PATH_MAX];
		char			_tnam[FILENAME_MAX];
};

//----------------------------------------------------------------------------

#endif
