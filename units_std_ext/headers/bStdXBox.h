//----------------------------------------------------------------------------
// File : bStdXBox.h
// Project : MacMap
// Purpose : Header file : Base class for window based XMapBoxes (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSXBox instead)
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
// 02/12/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdXBox__
#define __bStdXBox__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithIntf.h>

//----------------------------------------------------------------------------

class bStdXBox : public bStdXMapWithIntf{
public:		
	bStdXBox								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdXBox						(	);
	virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
	
	virtual void open						(	int* flags);
	virtual void close						(	);
	virtual bool event						(	EventRef evt);
	virtual bool nsevent					(	void* nsevt);
	virtual void idle						(	void* prm);
	virtual bool process					(	int msg, 
												void* prm);
	virtual bool test						(	void* prm);
	
	virtual bool wd_event					(	EventRef evt, 
												WindowRef wd);
	virtual void wd_init					(	WindowRef wd);
	
protected:
	virtual void wd_close					(	);

	virtual void read_p						(	);
	virtual void write_p					(	);
	virtual bGenericXMLBaseElement* make_p	(	);
	
	bool	_on;
	bool	_wd_open;
	UInt32	_sign;
	int		_h;
	int		_v;
	
private:		
	
};

//----------------------------------------------------------------------------

#endif
