//----------------------------------------------------------------------------
// File : bGenericLayerAccessContext.h
// Project : MacMap
// Purpose : Header file : Root class for layer access context
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

#ifndef __bGenericLayerAccessContext__
#define __bGenericLayerAccessContext__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericContext.h>
#include <mox_intf/bGenericStyle.h>

//----------------------------------------------------------------------------

class bGenericLayerAccessContext : public bGenericContext{
	public:
		virtual ~bGenericLayerAccessContext			(	){};
		virtual int load							(	)=0;
		virtual int unload							(	)=0;
		
		virtual bool addlayer						(	void* elt)=0;
		virtual bool addvirtuallayer				(	void* elt)=0;
		
		virtual void settype						(	const char* name)=0;
		virtual void setstyle						(	const char* name)=0;
		virtual void setvisible						(	bool vis)=0;
		virtual void setselectable					(	bool sel)=0;
		
		virtual void setroot						(	void* elt)=0;
		virtual void setarray						(	void* elt)=0;
		virtual void setidentification				(	void* elt)=0;
		
		virtual void flush							(	)=0;
		virtual bool parse							(	int idx)=0;
		virtual int count							(	)=0;
		virtual bGenericStyle* get					(	int idx)=0;
		
		virtual bool set_current					(	int idx)=0;
		virtual int get_current						(	)=0;
		virtual bool move							(	int idx, 
														int offset)=0;
		virtual bool remove							(	int idx)=0;
		virtual bool add							(	int tidx, 
														int sidx)=0;
		virtual bool change							(	int idx, 
														int newidx)=0;

};

//----------------------------------------------------------------------------

#endif
