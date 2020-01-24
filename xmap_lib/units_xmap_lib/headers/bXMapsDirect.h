//----------------------------------------------------------------------------
// File : bXMapsDirect.h
// Project : MacMap
// Purpose : Header file : XMap classes, sets object's direction flag (-1, 0, 1)
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
// 06/04/2007 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapsDirect__
#define __bXMapsDirect__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

#define kXMapsDirMessageID	"dir"

//----------------------------------------------------------------------------

class bXMapDirect : public bStdXMap{
	public:		
		bXMapDirect								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDirect					(	);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);
		virtual bool edit						(	void* prm);
		
	protected:
		int	_dir;
		
	private:
};

//----------------------------------------------------------------------------

class bXMapDir : public bXMapDirect{
	public:		
		bXMapDir								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapDir						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
	protected:
	
	private:
};

//----------------------------------------------------------------------------

class bXMapInv : public bXMapDirect{
	public:		
		bXMapInv								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapInv						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		virtual bool test						(	void* prm);

	protected:
	
	private:
};

//----------------------------------------------------------------------------

class bXMapNoDir : public bXMapDirect{
	public:		
		bXMapNoDir								(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapNoDir						(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
	protected:
	
	private:
};

//----------------------------------------------------------------------------

#endif
