//----------------------------------------------------------------------------
// File : bXMapSelector.h
// Project : MacMap
// Purpose : Header file : XMap class, selector : selecting objects by database and/or geographic request
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
// 20/01/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapSelector__
#define __bXMapSelector__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericGeog.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

#define kXMapSelectorSignature			'find'
#define kXMapSelectorCreateMacroMsgID	"CreateMacro"

enum{	
	kXMapSelectorSelectSignature	='dbse',
	kXMapSelectorPopupTypeID		=3,
	kXMapSelectorPopupTypeCmd		='popt',
	kXMapSelectorPopupSourceID		=4,
	kXMapSelectorPopupSourceCmd		='popw',	
	kXMapSelectorPopupOperatorsID	=5,
	kXMapSelectorPopupOperatorCmd	='popo',
	kXMapSelectorFieldListID		=6,
	kXMapSelectorFieldListCmd		='flds',
	kXMapSelectorValueID			=7,
	kXMapSelectorValueCmd			='valu',
	kXMapSelectorAddCriterionID		=8,
	kXMapSelectorAddCriterionCmd	='add_',
	kXMapSelectorRmvCriterionID		=9,
	kXMapSelectorRmvCriterionCmd	='rmv_',
	kXMapSelectorCriterionListID	=10,
	kXMapSelectorCriterionListCmd	='reca',
	kXMapSelectorPopupConstID		=11,
	kXMapSelectorPopupConstCmd		='popc',
	kXMapSelectorOpenQuestionID		=12,
	kXMapSelectorOpenQuestionCmd	='OpQu',
	
	kXMapSelectorGeoOptsSignature	='optg',
	kXMapSelectorPopupGeogID		=11,
	kXMapSelectorPopupGeogCmd		='geog',
	kXMapSelectorGeogResetCmd		='grst',
	kXMapSelectorGeogViewID			=18,
	kXMapSelectorGeogViewCmd		='ggvw',
	
	kXMapSelectorSlctOptsSignature	='opts',
	kXMapSelectorSelKindID			=12,
	kXMapSelectorSelKindCmd			='what',
	kXMapSelectorUseMaskedID		=14,
	kXMapSelectorUseMaskedCmd		='umsk',
	
	kXMapSelectorThenContrastID		=16,
	kXMapSelectorThenContrastCmd	='TCnt',
	kXMapSelectorThenMaskID			=17,
	kXMapSelectorThenMaskCmd		='TMsk',
	kXMapSelectorMaskTheseID		=18,
	kXMapSelectorMaskTheseCmd		='MskW',
	kXMapSelectorNextMacroID		=20,
	kXMapSelectorNextMacroCmd		='PMcr',
	
	kXMapSelectorTabs				='tabs',
	kXMapSelectorTabsID				=15,
	kXMapSelectorTabsCmd			='tabs',

	kXMapSelectorGetANameSignature	='Name',
	kXMapSelectorGetANameLegID		=1,
	kXMapSelectorGetANameNameID		=2,

	kXMapSelectorViews				='view',
	kXMapSelectorViewSelectID		=1,
	kXMapSelectorViewRelationID		=2,
	kXMapSelectorViewOptionID		=3,

	kXMapSelectorStuffSignature		='stuf',
	kXMapSelectorOKID				=1,
	kXMapSelectorCancelID			=2,
	
	kXMapSelectorDBProperty			='text'
};

enum{
	kXMapSelectorDTSourceField		=1,
	kXMapSelectorDTSourceCalc		=2
};

enum{
	kXMapSelectorSelect				=1,
	kXMapSelectorCreateMacro		=2,
	kXMapSelectorEditMacro			=3
};

#define kXMapSelectorCreateMacroCall	-1

#define kXMapSelectorSaveMsgID			"save"

typedef struct deletePrm{
	ControlRef 			c;
	DataBrowserItemID	id;
}deletePrm;

//----------------------------------------------------------------------------

class bXMapSelector : public bStdXMapWithModalIntf{
	public:		
		bXMapSelector							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapSelector					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);// Possibilité de passer un XML à éditer, -1 pour une macro
		
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:													
		static pascal OSStatus dt_fields		(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		static void nt_fields					(	ControlRef browser, 
													DataBrowserItemID item, 
													DataBrowserItemNotification msg);
		static pascal OSStatus dt_conditions	(	ControlRef browser, 
													DataBrowserItemID itemID, 
													DataBrowserPropertyID property, 
													DataBrowserItemDataRef itemData, 
													Boolean changeValue);
		void populate_types						(	);
		void populate_constraints				(	);
		void populate_fields					(	);
		void populate_conditions				(	);
		
		void reset_fields						(	);
		void reset_conditions					(	);
		
		void tab_action							(	);
		void field_action						(	DataBrowserItemID item);
		void type_action						(	);
		void constraints_action					(	);
		void source_action						(	);
		void sel_kind_action					(	);
		void use_masked_action					(	);
		void then_contrast_action				(	);
		void then_mask_action					(	);
		void mask_these_action					(	);
		void next_macro_action					(	);

		void add_criterion						(	);
		static void delete_criterion			(	DataBrowserItemID item, 
													DataBrowserItemState state, 
													void *clientData);
		void remove_criterion					(	);
		
		void populate_geogs						(	);
		void populate_macros					(	);
		void reset_geog							(	);
		bool choose_geog						(	int signature, 
													EventRef evt, 
													WindowRef wd);

		bool make_criterion						(	);
		
		void set_alpha_visibility				(	bool vis);
		void set_geog_visibility				(	bool vis);
		void set_option_visibility				(	bool vis);

		void flush_criterions					(	);
		bGenericXMLBaseElement* make			(	char* macro);
		bool parse								(	bGenericXMLBaseElement* root);
		
		void save_formula						(	);

		bool get_a_name							(	);
	
		static OSStatus name_evt_hdlr			(	EventHandlerCallRef hdlr,
													EventRef evt,
													void *up);

	private:											
		bGenericType*			_tp;
		bArray					_critarr;
		int						_dtsrc;
		int						_field;
		int						_selwhat;
		
		UInt32					_next;
		bool					_add2cnt;
		int						_add2hdn;
		
		bool					_use_mask;
		bGenericGeog*			_geog;
		int						_cid;
		
		int						_act;		
		bGenericXMLBaseElement*	_root;
		UInt32					_fsgn;
		char					_name[FILENAME_MAX];
		int						_idx;
};

//----------------------------------------------------------------------------

#endif
