//----------------------------------------------------------------------------
// File : bMacMapLayerAccessContext.h
// Project : MacMap
// Purpose : Header file : Layer context class (styles management in current view context)
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
// Classe de base pour dessin des objets
//----------------------------------------------------------------------------
// 20/05/2003 creation.
//----------------------------------------------------------------------------

#ifndef __bMacMapLayerAccessContext__
#define	__bMacMapLayerAccessContext__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericLayerAccessContext.h>
#include <mox_intf/bGenericXMLBaseElement.h>
#include <mox_intf/bGenericXMLLayerElement.h>
#include <MacMapSuite/bArray.h>
#include <mox_intf/bGenericStyle.h>

//----------------------------------------------------------------------------

class bMacMapLayerAccessContext : public bGenericLayerAccessContext{
	public:
		bMacMapLayerAccessContext 			(	);
		virtual ~bMacMapLayerAccessContext	(	);
				
		virtual int load					(	);
		virtual int unload					(	);
		
		virtual bool addlayer				(	void* elt);
		virtual bool addvirtuallayer		(	void* elt);
		
		virtual void settype                (	const char* name);
		virtual void setstyle				(	const char* name);
		virtual void setvisible				(	bool vis);
		virtual void setselectable			(	bool sel);
		
		virtual void setroot				(	void* elt);
		virtual void setarray				(	void* elt);
		virtual void setidentification		(	void* elt);
		
		virtual void flush					(	);
		virtual bool parse					(	int idx);
		virtual int count					(	);
		virtual bGenericStyle* get			(	int idx);

		virtual bool set_current			(	int idx);
		virtual int get_current				(	);
		virtual bool move					(	int idx,
                                                int offset);
		virtual bool remove					(	int idx);
		virtual bool add					(	int tidx,
                                                int sidx);
		virtual bool change					(	int idx,
                                                int newidx);

	protected:
		virtual void check_on_screen		(	);
	
	private:
		bArray*								_elts;
		int									_cursor;
		bGenericXMLBaseElement* 			_curview;
		bGenericXMLBaseElement* 			_array;
		bGenericXMLBaseElement*				_identifier;
};

//----------------------------------------------------------------------------

#endif
