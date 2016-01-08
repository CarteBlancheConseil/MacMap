//----------------------------------------------------------------------------
// File : bXMapUndo.h
// Project : MacMap
// Purpose : Header file : XMap class, undo
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
// 05/07/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapUndo__
#define __bXMapUndo__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <mox_intf/bStdWait.h>

//----------------------------------------------------------------------------

#define kXMapUndoMessageID			"Undo"
#define kXMapUndoProgressMessageID	"UndoProgress"

//----------------------------------------------------------------------------

class bXMapUndo : public bStdXMap{
	public:		
		bXMapUndo								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapUndo						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);
		
	protected:
		virtual void process_event				(	bGenericEvent* evt,
													bStdWait& wt); 

	private:
		void*	_buffer;
		int		_bsz;
		int		_tot;
};

//----------------------------------------------------------------------------

#endif
