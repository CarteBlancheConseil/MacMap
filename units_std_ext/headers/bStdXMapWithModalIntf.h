//----------------------------------------------------------------------------
// File : bStdXMapWithModalIntf.h
// Project : MacMap
// Purpose : Header file : Base class for modal window based XMaps (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSAppModalWindowController instead)
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
// 19/01/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdXMapWithModalIntf__
#define __bStdXMapWithModalIntf__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithIntf.h>

//----------------------------------------------------------------------------

class bStdXMapWithModalIntf : public bStdXMapWithIntf{
public:		
	bStdXMapWithModalIntf				(	bGenericXMLBaseElement* elt, 
											bGenericMacMapApp* gapp, 
											CFBundleRef bndl);
	virtual ~bStdXMapWithModalIntf		(	);

	virtual bool edit					(	void* prm);
	
protected:
	static OSStatus wd_evt_hdlr			(	EventHandlerCallRef hdlr, 
											EventRef evt, 
											void *up);
	
private:		
	bool	_run;
};

//----------------------------------------------------------------------------

#endif
