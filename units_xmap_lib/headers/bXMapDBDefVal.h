//----------------------------------------------------------------------------
// File : bXMapDBDefVal.h
// Project : MacMap
// Purpose : Header file : XMap class, sets standard fields default values
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
// 16/03/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapDBDefVal__
#define __bXMapDBDefVal__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{	
	kXMapDBDefValSignature			='DefV',
	kXMapDBDefValPopupTypeID		=3,
	kXMapDBDefValPopupTypeCmd		='PopT',
	kXMapDBDefValPopupFieldID		=4,
	kXMapDBDefValPopupFieldCmd		='PopF',
	kXMapDBDefValValueLegID			=5,
	kXMapDBDefValValueTextID		=6,
	kXMapDBDefValValuePopupID		=7,

	kXMapDBDefValModifyID			=8,
	kXMapDBDefValModifyCmd			='CMod'
};

#define	kXMapDBDefValMessageID		"data defval"

//----------------------------------------------------------------------------

class bXMapDBDefVal : public bStdXMapWithModalIntf{
	public:		
		bXMapDBDefVal							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDBDefVal					(	);
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
		void populate_field						(	);
		
		void type_action						(	);		
		void modify								(	);

				
	private:		
		bGenericType*			_tp;
		int						_fidx;
};

//----------------------------------------------------------------------------

#endif
