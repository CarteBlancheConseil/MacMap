//----------------------------------------------------------------------------
// File : bXMapDataImport.h
// Project : MacMap
// Purpose : Header file : XMap class, fills objects data from DBF, text, csv, ... file
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
// 25/10/2007 creation.
// 11/02/2015 NAV to NS transformation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapDataImport__
#define __bXMapDataImport__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

#include <stdio.h>

//----------------------------------------------------------------------------

enum{
	kXMapDataImportSignature		='FJon',
	kXMapDataImportTypesID			=1,
	kXMapDataImportTypesCmd			='Typs',
	kXMapDataImportUniqueID			=2,
	kXMapDataImportUseMaskedID		=3,
	kXMapDataImportListID			=4,
	kXMapDataImportActionID			=5,
	kXMapDataImportActionCmd		='Act_',
	kXMapDataImportFieldsID			=6,
	kXMapDataImportFieldsCmd		='Flds',
	kXMapDataImportColumnsProperty	='text'
};

enum{
	kXMapDataImportNoErr,
	kXMapDataImportUnsupportedFormatErr,
	kXMapDataImportOpenErr,
	kXMapDataImportKeyNotDefinedErr,
	kXMapDataImportNoDataToFillErr,
	kXMapDataImportNoObjectErr,
	kXMapDataImportNotUniqueErr
};

#define	kXMapDataImportMessageID		"DataImport"
#define	kXMapDataImportLineNumberID		"LineNumber"
#define	kXMapDataImportErrorsOccursID	"ErrorsOccurs"
#define	kXMapDataImportReadLogID		"ReadLog"
#define	kXMapDataImportReadContinueID	"Continue"

typedef	int	(*compProc)(const void*,const void*);

//----------------------------------------------------------------------------

class bXMapDataImport : public bStdXMapWithModalIntf{
	public:		
		bXMapDataImport							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDataImport					(	);
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
		
		virtual bool test_ignore				(	int f);
		virtual bool test_search				(	int f);
		virtual bool test_fill					(	int f);
		virtual bool test_create				(	int f);

		virtual void parse_tree					(	);
		virtual void join						(	);
		virtual void join						(	datajoin_prm* prm,
													bool mode);
		virtual int add_fields					(	datajoin_prm* prm);
				
	private:
		
		int				_cur;
		compProc		_comp;
		
		bArray			_cols;
		int				_tpi;
		wtable			_tbl;
		bool			_use_msk;
		bool			_unique;
		
		bStdFile*		_report;
};

//----------------------------------------------------------------------------

#endif
