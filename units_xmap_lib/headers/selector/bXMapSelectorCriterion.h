//----------------------------------------------------------------------------
// File : bXMapSelectorCriterion.h
// Project : MacMap
// Purpose : Header file : XMap selector utility (selection criterion)
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

#ifndef __bXMapSelectorCriterion__
#define __bXMapSelectorCriterion__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/bGenericGeog.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#define kBeginWithElement		'obeg'
#define kContainElement			'ocnt'
#define kEndWithElement			'oend'
#define kEqualElement			'oeql'
#define kHigherOrEqualElement	'ohge'
#define kHigherElement			'ohgr'
#define kLowerOrEqualElement	'olwe'
#define kLowerElement			'olwr'
#define kNotEqualElement		'oneq'

#define kDBFieldElement			'vfld'

//----------------------------------------------------------------------------

class bXMapSelectorCriterion{
	public:
		bXMapSelectorCriterion				(	bGenericMacMapApp* gapp,
												int	id,
												int targetsign,
												char* targetvalue,
												int targetkind,
												int opsgn,
												char* val,
												bool* succed);
		virtual ~bXMapSelectorCriterion		(	);
		virtual int get_id					(	);
		virtual bool make					(	bArray* l, 
												int indent,
												CFBundleRef bndl);
		virtual CFStringRef	get_string		(	CFBundleRef bndl);
		
		static bool parse					(	bGenericXMLBaseElement* root, 
												void* prm,
												int indent);
	protected:
		
	private:
		static bGenericMacMapApp* _gapp;
		int		_id;
		int		_tgsign;
		char*	_tgname;
		char*	_tgvalue;			
		char*	_opname;
		char*	_valname;
		char*	_valvalue;		
};

//----------------------------------------------------------------------------

typedef struct parse_prm{
	bArray*					stack;
		
	bGenericType*			tp;
	int						selwhat;
	bool					use_mask;
	
	UInt32					next;
	bool					add2cnt;
	int						add2hdn;

	bGenericGeog*			geog;
		
	int						cid;
	int						targetsign;
	char					targetvalue[_values_length_max_];
	int						targetkind;
	int						opsgn;
	char					val[_values_length_max_];
	
}parse_prm;

//----------------------------------------------------------------------------

#endif
