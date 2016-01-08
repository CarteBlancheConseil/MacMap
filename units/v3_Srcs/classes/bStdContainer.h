//----------------------------------------------------------------------------
// File : bStdContainer.h
// Project : MacMap
// Purpose : Header file : Standard container base class for styles and views management
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
// 08/02/2005 creation.
// 29/01/2009 PARAMS OK
//----------------------------------------------------------------------------

#ifndef __bStdContainer__
#define __bStdContainer__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericElement.h>
#include <mox_intf/bGenericXMLBaseElement.h>

//----------------------------------------------------------------------------

class bStdContainer : public bGenericElement{
	public:		
		bStdContainer							(	const char* file_name);
		bStdContainer							(	const char* file_name, 
													bStdContainer& c);
		bStdContainer							(	const char* file_name, 
													bGenericXMLBaseElement* root);
		bStdContainer							(	const char* file_name, 
													bool dummy);
		virtual ~bStdContainer					(	);
		
		virtual int status						(	);
		
		virtual void get_name					(	char* name);
		virtual bool set_name					(	const char* name);
		virtual void file_name					(	char* name);
		virtual bGenericXMLBaseElement* root	(	);
		virtual bool dump						(	);
		virtual bool destroy					(	);
		
	protected:
		virtual void load						(	);
		virtual void unload						(	);
		bool set_root_name						(	const char* name);
		
		int						_status;
		char					_file_name[256];
		bGenericXMLBaseElement*	_root;
		
	private:
		
};

//----------------------------------------------------------------------------

#endif
