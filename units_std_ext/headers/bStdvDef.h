//----------------------------------------------------------------------------
// File : bStdvDef.h
// Project : MacMap
// Purpose : Header file : Base class for vDefs (Warning : as it's an old carbon based model, consider it's DEPRECATED, use bStdNSPalettevDef instead)
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
// 10/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __bStdvDef__
#define __bStdvDef__

//----------------------------------------------------------------------------

#include <std_ext/bStdExt.h>
#include <mox_intf/bGenericvDef.h>

//----------------------------------------------------------------------------

#define	kStdvDefConfigSign	'vdef'
#define vdef_op_max	9
extern char * const vdef_op_list[];

//----------------------------------------------------------------------------

class bStdvDef : public virtual bGenericvDef, public bStdExt {
public:		
	bStdvDef								(	bGenericXMLBaseElement* elt, 
												bGenericMacMapApp* gapp, 
												CFBundleRef bndl);
	virtual ~bStdvDef						(	);
			
	virtual void open						(	int* flags);
	virtual void close						(	);
	virtual bool process					(	int msg, 
												void* prm);
	virtual bool test						(	void* prm);
	virtual bool edit						(	void* prm);
	
	virtual bool test						(	bGenericType* tp);
	virtual bool edit						(	bGenericXMLBaseElement** root, 
												bGenericType* tp);
	virtual void make						(	);
	virtual bool wd_event					(	EventRef evt, 
												WindowRef wd);
	virtual void wd_init					(	WindowRef wd);
			
	ControlRef get_control					(	OSType sign, 
												int id);

protected:
	bool					_run;
	WindowRef				_wd;
	EventHandlerUPP			_hdlr;
	bGenericXMLBaseElement* _root;
	bGenericType*			_tp;
	
	
	pascal static OSStatus wd_evt_hdlr		(	EventHandlerCallRef hdlr, 
												EventRef evt, 
												void *up);
	
private:
	

};

//----------------------------------------------------------------------------

#endif
